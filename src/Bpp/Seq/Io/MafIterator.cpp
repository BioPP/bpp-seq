//
// File: MafIterator.cpp
// Authors: Julien Dutheil
// Created: Tue Sep 07 2010
//

/*
Copyright or © or Copr. Bio++ Development Team, (2010)

This software is a computer program whose purpose is to provide classes
for sequences analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#include "MafIterator.h"
#include "../SequenceWithQuality.h"
#include "../SequenceWithAnnotationTools.h"
#include "../SequenceWalker.h"
#include "../Alphabet/AlphabetTools.h"
#include "../Container/VectorSiteContainer.h"
#include "../SiteTools.h"

using namespace bpp;

//From the STL:
#include <string>

using namespace std;

MafSequence* MafSequence::subSequence(unsigned int startAt, unsigned int length) const
{
  string subseq = toString().substr(startAt, length);
  unsigned int begin = begin_;
  if (hasCoordinates_) {
    for (unsigned int i = 0; i < startAt; ++i) {
      if (! getAlphabet()->isGap(operator[](i))) begin++;
    }
  }
  MafSequence* newSeq = new MafSequence(getName(), subseq, begin, strand_, srcSize_);
  vector<string> anno = getAnnotationTypes();
  for (size_t i = 0; i < anno.size(); ++i) {
    newSeq->addAnnotation(getAnnotation(anno[i]).getPartAnnotation(startAt, length));
  }
  return newSeq;
}

MafBlock* SequenceFilterMafIterator::nextBlock() throw (Exception)
{
  currentBlock_ = iterator_->nextBlock();
  while (currentBlock_) {
    map<string, unsigned int> counts;
    for (size_t i = currentBlock_->getNumberOfSequences(); i > 0; --i) {
      string species = currentBlock_->getSequence(i-1).getSpecies(); 
      if (!VectorTools::contains(species_, species)) {
        if (logstream_) {
          (*logstream_ << "SEQUENCE FILTER: remove sequence '" << species << "' from current block " << currentBlock_->getDescription() << ".").endLine();
        }
        currentBlock_->getAlignment().deleteSequence(i - 1);
      } else {
        counts[species]++;
      }
    }
    bool test = currentBlock_->getNumberOfSequences() == 0;
    //Avoid a memory leak:
    if (test) {
      if (logstream_) {
        (*logstream_ << "SEQUENCE FILTER: block " << currentBlock_->getDescription() << " is now empty. Try to get the next one.").endLine();
      }
      delete currentBlock_;
    } else {
      test = strict_ && (counts.size() != species_.size());
      if (test) {
        if (logstream_) {
          (*logstream_ << "SEQUENCE FILTER: block " << currentBlock_->getDescription() << " does not contain all species and will be ignored. Try to get the next one.").endLine();
        }
        delete currentBlock_;
      } else {
        if (rmDuplicates_) {
          test = false;
          map<string, unsigned int>::iterator it;
          for (it = counts.begin(); it != counts.end() && !(test = it->second > 1); it++) {}
          if (test) {
            if (logstream_) {
              (*logstream_ << "SEQUENCE FILTER: block " << currentBlock_->getDescription() << " has two sequences for species '" << it->first << "' and will be ignored. Try to get the next one.").endLine();
            }
            delete currentBlock_;
          } else {
            return currentBlock_;
          }
        } else {
          return currentBlock_;
        }
      }
    }

    //Look for the next block:
    currentBlock_ = iterator_->nextBlock();
  }
  
  return currentBlock_;
}

MafBlock* ChromosomeMafIterator::nextBlock() throw (Exception)
{
  currentBlock_ = iterator_->nextBlock();
  while (currentBlock_) {
    bool foundRef = false;
    string chr = "";
    for (size_t i = 0; i < currentBlock_->getNumberOfSequences() && !foundRef; ++i) {
      string species = currentBlock_->getSequence(i).getSpecies(); 
      if (species == ref_) {
        foundRef = true;
        chr = currentBlock_->getSequence(i).getChromosome();
      }
    }
    if (!foundRef) {
      if (logstream_) {
        (*logstream_ << "CHROMOSOME FILTER: block does not contain reference species and was removed.").endLine();
      }
      delete currentBlock_;
    } else if (chr != chr_) {
      if (logstream_) {
        (*logstream_ << "CHROMOSOME FILTER: reference species without queried chromosome was removed.").endLine();
      }
      delete currentBlock_;
    } else {
      return currentBlock_;
    }

    //Look for the next block:
    currentBlock_ = iterator_->nextBlock();
  }
  
  return currentBlock_;
}

MafBlock* DuplicateFilterMafIterator::nextBlock() throw (Exception)
{
  currentBlock_ = iterator_->nextBlock();
  while (currentBlock_) {
    bool foundRef = false;
    string chr = "";
    char strand = '+';
    unsigned int start = 0;
    unsigned int stop  = 0;
    for (size_t i = 0; i < currentBlock_->getNumberOfSequences() && !foundRef; ++i) {
      string species = currentBlock_->getSequence(i).getSpecies(); 
      if (species == ref_) {
        foundRef = true;
        chr    = currentBlock_->getSequence(i).getChromosome();
        strand = currentBlock_->getSequence(i).getStrand();
        start  = currentBlock_->getSequence(i).start();
        stop   = currentBlock_->getSequence(i).stop();
      }
    }
    if (!foundRef) {
      if (logstream_) {
        (*logstream_ << "DUPLICATE FILTER: block does not contain reference species and was removed.").endLine();
      }
      delete currentBlock_;
    } else {
      unsigned int occurrence = blocks_[chr][strand][start][stop]++;
      if (occurrence > 0) {
        if (logstream_) {
          (*logstream_ << "DUPLICATE FILTER: sequence in reference species was found in a previous block. New block was removed.").endLine();
        }
        delete currentBlock_;
      } else {
        return currentBlock_;
      }
    }

    //Look for the next block:
    currentBlock_ = iterator_->nextBlock();
  }
  
  return currentBlock_;
}

MafBlock* BlockMergerMafIterator::nextBlock() throw (Exception)
{
  if (!incomingBlock_) return 0;
  currentBlock_  = incomingBlock_;
  incomingBlock_ = iterator_->nextBlock();
  while (incomingBlock_) {
    unsigned int globalSpace = 0;
    for (unsigned int i = 0; i < species_.size(); ++i) {
      try {
        const MafSequence* seq1 = &currentBlock_->getSequenceForSpecies(species_[i]); 
        const MafSequence* seq2 = &incomingBlock_->getSequenceForSpecies(species_[i]);
        if (!seq1->hasCoordinates() || !seq2->hasCoordinates())
          throw Exception("BlockMergerMafIterator::nextBlock. Species '" + species_[i] + "' is missing coordinates in at least one block.");

        if (seq1->stop() >= seq2->start())
          return currentBlock_;
        unsigned int space = seq2->start() - seq1->stop() - 1;
        if (space > maxDist_)
          return currentBlock_;
        if (i == 0)
          globalSpace = space;
        else {
          if (space != globalSpace)
            return currentBlock_;
        }
        if (seq1->getChromosome() != seq2->getChromosome()
         || VectorTools::contains(ignoreChrs_, seq1->getChromosome())
         || VectorTools::contains(ignoreChrs_, seq2->getChromosome())
         || seq1->getStrand() != seq2->getStrand())
        {
          //There is a syntheny break in this sequence, so we do not merge the blocks.
          return currentBlock_;
        }
      } catch (SequenceNotFoundException& snfe) {
        //At least one block does not contain the sequence.
        //We don't merge the blocks:
        return currentBlock_;
      }
    }
    //We merge the two blocks:
    if (logstream_) {
      (*logstream_ << "BLOCK MERGER: merging two consecutive blocks.").endLine();
    }
    vector<string> sp1 = currentBlock_->getSpeciesList();
    vector<string> sp2 = incomingBlock_->getSpeciesList();
    vector<string> allSp = VectorTools::unique(VectorTools::vectorUnion(sp1, sp2));
    //We need to create a new MafBlock:
    MafBlock* mergedBlock = new MafBlock();
    //We average the score and pass values:
    unsigned int p1 = currentBlock_->getPass();
    unsigned int p2 = incomingBlock_->getPass();
    if (p1 == p2) mergedBlock->setPass(p1);
    double s1 = currentBlock_->getScore();
    double n1 = static_cast<double>(currentBlock_->getNumberOfSites());
    double s2 = incomingBlock_->getScore();
    double n2 = static_cast<double>(incomingBlock_->getNumberOfSites());
    mergedBlock->setScore((s1 * n1 + s2 * n2) / (n1 + n2));

    //Now fill the new block:
    for (size_t i = 0; i < allSp.size(); ++i) {
      auto_ptr<MafSequence> seq;
      try {
        seq.reset(new MafSequence(currentBlock_->getSequenceForSpecies(allSp[i])));

        //Check is there is a second sequence:
        try {
          auto_ptr<MafSequence> tmp(new MafSequence(incomingBlock_->getSequenceForSpecies(allSp[i])));
          string ref1 = seq->getDescription(), ref2 = tmp->getDescription();
          //Add spacer if needed:
          if (globalSpace > 0) {
            if (logstream_) {
              (*logstream_ << "BLOCK MERGER: a spacer of size " << globalSpace <<" is inserted in sequence for species " << allSp[i] << ".").endLine();
            }
            seq->append(vector<int>(globalSpace, AlphabetTools::DNA_ALPHABET.getUnknownCharacterCode()));
          }
          if (seq->getChromosome() != tmp->getChromosome()) {
            seq->setChromosome(seq->getChromosome() + "-" + tmp->getChromosome());
            seq->removeCoordinates();
          }
          if (seq->getStrand() != tmp->getStrand()) {
            seq->setStrand('?');
            seq->removeCoordinates();
          }
          if (seq->getName() != tmp->getName())
            tmp->setName(seq->getName()); //force name conversion to prevent exception in 'merge'.
          seq->merge(*tmp);
          seq->setSrcSize(0);
          if (logstream_) {
            (*logstream_ << "BLOCK MERGER: merging " << ref1 << " with " << ref2 << " into " << seq->getDescription()).endLine();
          }
        } catch (SequenceNotFoundException& snfe2) {
          //There was a first sequence, we just extend it:
          string ref1 = seq->getDescription();
          seq->setToSizeR(incomingBlock_->getNumberOfSites() + globalSpace);
          if (logstream_) {
            (*logstream_ << "BLOCK MERGER: extending " << ref1 << " with " << incomingBlock_->getNumberOfSites() << " gaps on the right.").endLine();
          }
        }
      } catch (SequenceNotFoundException& snfe1) {
        //There must be a second sequence then:
        seq.reset(new MafSequence(incomingBlock_->getSequenceForSpecies(allSp[i])));
        string ref2 = seq->getDescription();
        seq->setToSizeL(currentBlock_->getNumberOfSites() + globalSpace);
        if (logstream_) {
          (*logstream_ << "BLOCK MERGER: adding " << ref2 << " and extend it with " << currentBlock_->getNumberOfSites() << " gaps on the left.").endLine();
        }
      }
      mergedBlock->addSequence(*seq);
    }
    //Cleaning stuff:
    delete currentBlock_;
    delete incomingBlock_;
    currentBlock_ = mergedBlock;
    //We check if we can also merge the next block:
    incomingBlock_ = iterator_->nextBlock();
  }
  return currentBlock_;
}

MafBlock* FullGapFilterMafIterator::nextBlock() throw (Exception)
{
  MafBlock* block = iterator_->nextBlock();
  if (!block) return 0;

  //We create a copy of the ingroup alignement for better efficiency:
  VectorSiteContainer vsc(&AlphabetTools::DNA_ALPHABET);
  for (size_t i = 0; i < species_.size(); ++i) {
    vsc.addSequence(block->getSequence(i));
  }
  //Now check the positions that are only made of gaps:
  if (verbose_) {
    ApplicationTools::message->endLine();
    ApplicationTools::displayTask("Cleaning block for gap sites", true);
  }
  unsigned int n = block->getNumberOfSites();
  vector <unsigned int> start;
  vector <unsigned int> count;
  bool test = false;
  for(unsigned int i = 0; i < n; ++i) {
    const Site* site = &vsc.getSite(i);
    if (SiteTools::isGapOnly(*site)) {
      if (test) {
        count[count.size() - 1]++;
      } else {
        start.push_back(i);
        count.push_back(1);
        test = true;
      }
    } else {
      test = false;
    }
  }
  //Now remove blocks:
  unsigned int totalRemoved = 0;
  for(size_t i = start.size(); i > 0; --i) {
    if (verbose_)
      ApplicationTools::displayGauge(start.size() - i, start.size() - 1, '=');
    block->getAlignment().deleteSites(start[i - 1], count[i - 1]);
    totalRemoved += count[i - 1];
  }
  if (verbose_)
    ApplicationTools::displayTaskDone();
  
  //Correct coordinates:
  if (totalRemoved > 0) {
    for (unsigned int i = 0; i < block->getNumberOfSequences(); ++i) {
      const MafSequence* seq = &block->getSequence(i);
      if (!VectorTools::contains(species_, seq->getSpecies())) {
        block->removeCoordinatesFromSequence(i);
      }
    }
  }
  if (logstream_) {
    (*logstream_ << "FULL GAP CLEANER: " << totalRemoved << " positions have been removed.").endLine();
  }
  return block;
}

MafBlock* AlignmentFilterMafIterator::nextBlock() throw (Exception)
{
  if (blockBuffer_.size() == 0) {
    //Else there is no more block in the buffer, we need to parse more:
    do {
      MafBlock* block = iterator_->nextBlock();
      if (!block) return 0; //No more block.
    
      //Parse block.
      int gap = AlphabetTools::DNA_ALPHABET.getGapCharacterCode();
      int unk = AlphabetTools::DNA_ALPHABET.getUnknownCharacterCode();
      size_t nr = species_.size();
      vector< vector<int> > aln(nr);
      for (size_t i = 0; i < nr; ++i) {
        aln[i] = block->getSequenceForSpecies(species_[i]).getContent();
      }
      size_t nc = block->getNumberOfSites();
      //First we create a mask:
      vector<unsigned int> pos;
      vector<bool> col(nr);
      //Reset window:
      window_.clear();
      //Init window:
      size_t i;
      for (i = 0; i < windowSize_; ++i) {
        for (size_t j = 0; j < nr; ++j) {
          col[j] = (aln[j][i] == gap || aln[j][i] == unk);
        }
        window_.push_back(col);
      }
      //Slide window:
      if (verbose_) {
        ApplicationTools::message->endLine();
        ApplicationTools::displayTask("Sliding window for alignment filter", true);
      }
      while (i < nc) {
        if (verbose_)
          ApplicationTools::displayGauge(i - windowSize_, nc - windowSize_ - 1, '>');
        //Evaluate current window:
        unsigned int sum = 0;
        for (size_t u = 0; u < window_.size(); ++u)
          for (size_t v = 0; v < window_[u].size(); ++v)
            if (window_[u][v]) sum++;
        if (sum > maxGap_) {
          if (pos.size() == 0) {
            pos.push_back(i - windowSize_);
            pos.push_back(i);
          } else {
            if (i - windowSize_ < pos[pos.size() - 1]) {
              pos[pos.size() - 1] = i; //Windows are overlapping and we extend previous region
            } else { //This is a new region
              pos.push_back(i - windowSize_);
              pos.push_back(i);
            }
          }
        }
      
        //Move forward:
        for (unsigned int k = 0; k < step_; ++k) {
          for (size_t j = 0; j < nr; ++j) {
            col[j] = (aln[j][i] == gap || aln[j][i] == unk);
          }
          window_.push_back(col);
          window_.pop_front();
          ++i;
        }
      }

      //Evaluate last window:
      unsigned int sum = 0;
      for (size_t u = 0; u < window_.size(); ++u)
        for (size_t v = 0; v < window_[u].size(); ++v)
          if (window_[u][v]) sum++;
      if (sum > maxGap_) {
        if (pos.size() == 0) {
          pos.push_back(i - windowSize_);
          pos.push_back(i);
        } else {
          if (i - windowSize_ <= pos[pos.size() - 1]) {
            pos[pos.size()] = i; //Windows are overlapping and we extend previous region
          } else { //This is a new region
            pos.push_back(i - windowSize_);
            pos.push_back(i);
          }
        }
      } 
      if (verbose_)
        ApplicationTools::displayTaskDone();
    
      //Now we remove regions with two many gaps, using a sliding window:
      if (pos.size() == 0) {
        blockBuffer_.push_back(block);
        if (logstream_) {
          (*logstream_ << "ALN CLEANER: block " << block->getDescription() << " is clean and kept as is.").endLine();
        }
      } else if (pos.size() == 2 && pos.front() == 0 && pos.back() == block->getNumberOfSites()) {
        //Everything is removed:
        if (logstream_) {
          (*logstream_ << "ALN CLEANER: block " << block->getDescription() << " was entirely removed. Tried to get the next one.").endLine();
        }
      } else {
        if (logstream_) {
          (*logstream_ << "ALN CLEANER: block " << block->getDescription() << " with size "<< block->getNumberOfSites() << " will be split into " << (pos.size() / 2 + 1) << " blocks.").endLine();
        }
        if (verbose_) {
          ApplicationTools::message->endLine();
          ApplicationTools::displayTask("Spliting block", true);
        }
        for (i = 0; i < pos.size(); i+=2) {
          if (verbose_)
            ApplicationTools::displayGauge(i, pos.size() - 2, '=');
          if (logstream_) {
            (*logstream_ << "ALN CLEANER: removing region (" << pos[i] << ", " << pos[i+1] << ") from block " << block->getDescription() << ".").endLine();
          }
          if (pos[i] > 0) {
            MafBlock* newBlock = new MafBlock();
            newBlock->setScore(block->getScore());
            newBlock->setPass(block->getPass());
            for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
              MafSequence* subseq;
              if (i == 0) {
                subseq = block->getSequence(j).subSequence(0, pos[i]);
              } else {
                subseq = block->getSequence(j).subSequence(pos[i - 1], pos[i] - pos[i - 1]);
              }
              newBlock->addSequence(*subseq);
              delete subseq;
            }
            blockBuffer_.push_back(newBlock);
          }
        
          if (keepTrashedBlocks_) {
            MafBlock* outBlock = new MafBlock();
            outBlock->setScore(block->getScore());
            outBlock->setPass(block->getPass());
            for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
              MafSequence* outseq = block->getSequence(j).subSequence(pos[i], pos[i + 1] - pos[i]);
              outBlock->addSequence(*outseq);
              delete outseq;
            } 
            trashBuffer_.push_back(outBlock);
          }
        }
        //Add last block:
        if (pos[pos.size() - 1] < block->getNumberOfSites()) {
          MafBlock* newBlock = new MafBlock();
          newBlock->setScore(block->getScore());
          newBlock->setPass(block->getPass());
          for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
            MafSequence* subseq;
            subseq = block->getSequence(j).subSequence(pos[pos.size() - 1], block->getNumberOfSites() - pos[pos.size() - 1]);
            newBlock->addSequence(*subseq);
            delete subseq;
          }
          blockBuffer_.push_back(newBlock);
        }
        if (verbose_)
          ApplicationTools::displayTaskDone();

        delete block;
      }
    } while (blockBuffer_.size() == 0);
  }

  MafBlock* block = blockBuffer_.front();
  blockBuffer_.pop_front();
  return block;
}

MafBlock* MaskFilterMafIterator::nextBlock() throw (Exception)
{
  if (blockBuffer_.size() == 0) {
    do {
      //Else there is no more block in the buffer, we need parse more:
      MafBlock* block = iterator_->nextBlock();
      if (!block) return 0; //No more block.
    
      //Parse block.
      vector< vector<bool> > aln;
      for (size_t i = 0; i < species_.size(); ++i) {
        const MafSequence* seq = &block->getSequenceForSpecies(species_[i]);
        if (seq->hasAnnotation(SequenceMask::MASK)) {
          aln.push_back(dynamic_cast<const SequenceMask&>(seq->getAnnotation(SequenceMask::MASK)).getMask());
        }
      }
      size_t nr = aln.size();
      size_t nc = block->getNumberOfSites();
      //First we create a mask:
      vector<unsigned int> pos;
      vector<bool> col(nr);
      //Reset window:
      window_.clear();
      //Init window:
      size_t i;
      for (i = 0; i < windowSize_; ++i) {
        for (size_t j = 0; j < nr; ++j) {
          col[j] = aln[j][i];
        }
        window_.push_back(col);
      }
      //Slide window:
      if (verbose_) {
        ApplicationTools::message->endLine();
        ApplicationTools::displayTask("Sliding window for mask filter", true);
      }
      while (i < nc) {
        if (verbose_)
          ApplicationTools::displayGauge(i - windowSize_, nc - windowSize_ - 1, '>');
        //Evaluate current window:
        unsigned int sum = 0;
        for (size_t u = 0; u < window_.size(); ++u)
          for (size_t v = 0; v < window_[u].size(); ++v)
            if (window_[u][v]) sum++;
        if (sum > maxMasked_) {
          if (pos.size() == 0) {
            pos.push_back(i - windowSize_);
            pos.push_back(i);
          } else {
            if (i - windowSize_ <= pos[pos.size() - 1]) {
              pos[pos.size() - 1] = i; //Windows are overlapping and we extend previous region
            } else { //This is a new region
              pos.push_back(i - windowSize_);
              pos.push_back(i);
            }
          }
        }
      
        //Move forward:
        for (unsigned int k = 0; k < step_; ++k) {
          for (size_t j = 0; j < nr; ++j) {
            col[j] = aln[j][i];
          }  
          window_.push_back(col);
          window_.pop_front();
          ++i;
        }
      }

      //Evaluate last window:
      unsigned int sum = 0;
      for (size_t u = 0; u < window_.size(); ++u)
        for (size_t v = 0; v < window_[u].size(); ++v)
          if (window_[u][v]) sum++;
      if (sum > maxMasked_) {
        if (pos.size() == 0) {
          pos.push_back(i - windowSize_);
          pos.push_back(i);
        } else {
          if (i - windowSize_ < pos[pos.size() - 1]) {
            pos[pos.size()] = i; //Windows are overlapping and we extend previous region
          } else { //This is a new region
            pos.push_back(i - windowSize_);
            pos.push_back(i);
          }
        }
      }
      if (verbose_)
        ApplicationTools::displayTaskDone();
    
      //Now we remove regions with two many gaps, using a sliding window:
      if (pos.size() == 0) {
        blockBuffer_.push_back(block);
        if (logstream_) {
          (*logstream_ << "MASK CLEANER: block is clean and kept as is.").endLine();
        }
      } else if (pos.size() == 2 && pos.front() == 0 && pos.back() == block->getNumberOfSites()) {
        //Everything is removed:
        if (logstream_) {
          (*logstream_ << "MASK CLEANER: block was entirely removed. Tried to get the next one.").endLine();
        }
      } else {
        if (logstream_) {
          (*logstream_ << "MASK CLEANER: block with size "<< block->getNumberOfSites() << " will be split into " << (pos.size() / 2 + 1) << " blocks.").endLine();
        }
        if (verbose_) {
          ApplicationTools::message->endLine();
          ApplicationTools::displayTask("Spliting block", true);
        }
        for (i = 0; i < pos.size(); i+=2) {
          if (verbose_)
            ApplicationTools::displayGauge(i, pos.size() - 2, '=');
          if (logstream_) {
            (*logstream_ << "MASK CLEANER: removing region (" << pos[i] << ", " << pos[i+1] << ") from block.").endLine();
          }
          if (pos[i] > 0) {
            MafBlock* newBlock = new MafBlock();
            newBlock->setScore(block->getScore());
            newBlock->setPass(block->getPass());
            for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
              MafSequence* subseq;
              if (i == 0) {
                subseq = block->getSequence(j).subSequence(0, pos[i]);
              } else {
                subseq = block->getSequence(j).subSequence(pos[i - 1], pos[i] - pos[i - 1]);
              }
              newBlock->addSequence(*subseq);
              delete subseq;
            }
            blockBuffer_.push_back(newBlock);
          }
          
          if (keepTrashedBlocks_) {
            MafBlock* outBlock = new MafBlock();
            outBlock->setScore(block->getScore());
            outBlock->setPass(block->getPass());
            for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
              MafSequence* outseq = block->getSequence(j).subSequence(pos[i], pos[i + 1] - pos[i]);
              outBlock->addSequence(*outseq);
              delete outseq;
            } 
            trashBuffer_.push_back(outBlock);
          }
        }
        //Add last block:
        if (pos[pos.size() - 1] < block->getNumberOfSites()) {
          MafBlock* newBlock = new MafBlock();
          newBlock->setScore(block->getScore());
          newBlock->setPass(block->getPass());
          for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
            MafSequence* subseq;
            subseq = block->getSequence(j).subSequence(pos[pos.size() - 1], block->getNumberOfSites() - pos[pos.size() - 1]);
            newBlock->addSequence(*subseq);
            delete subseq;
          }
          blockBuffer_.push_back(newBlock);
        }
        if (verbose_)
          ApplicationTools::displayTaskDone();

        delete block;
      }  
    } while (blockBuffer_.size() == 0);
  }

  MafBlock* block = blockBuffer_.front();
  blockBuffer_.pop_front();
  return block;
}

MafBlock* QualityFilterMafIterator::nextBlock() throw (Exception)
{
  if (blockBuffer_.size() == 0) {
    do {
      //Else there is no more block in the buffer, we need parse more:
      MafBlock* block = iterator_->nextBlock();
      if (!block) return 0; //No more block.
    
      //Parse block.
      vector< vector<int> > aln;
      for (size_t i = 0; i < species_.size(); ++i) {
        const MafSequence* seq = &block->getSequenceForSpecies(species_[i]);
        if (seq->hasAnnotation(SequenceQuality::QUALITY_SCORE)) {
          aln.push_back(dynamic_cast<const SequenceQuality&>(seq->getAnnotation(SequenceQuality::QUALITY_SCORE)).getScores());
        }
      }
      if (aln.size() != species_.size()) {
        blockBuffer_.push_back(block);
        if (logstream_) {
          (*logstream_ << "QUAL CLEANER: block is missing quality score for at least one species and will therefore not be filtered.").endLine();
        }
        //NB here we could decide to discard the block instead!
      } else {
        size_t nr = aln.size();
        size_t nc = block->getNumberOfSites();
        //First we create a mask:
        vector<unsigned int> pos;
        vector<int> col(nr);
        //Reset window:
        window_.clear();
        //Init window:
        size_t i;
        for (i = 0; i < windowSize_; ++i) {
          for (size_t j = 0; j < nr; ++j) {
            col[j] = aln[j][i];
          }
          window_.push_back(col);
        }
        //Slide window:
        if (verbose_) {
          ApplicationTools::message->endLine();
          ApplicationTools::displayTask("Sliding window for quality filter", true);
        }
        while (i < nc) {
          if (verbose_)
            ApplicationTools::displayGauge(i - windowSize_, nc - windowSize_ - 1, '>');
          //Evaluate current window:
          double mean = 0;
          double n = static_cast<double>(aln.size() * windowSize_);
          for (size_t u = 0; u < window_.size(); ++u)
            for (size_t v = 0; v < window_[u].size(); ++v) {
              mean += (window_[u][v] > 0 ? static_cast<double>(window_[u][v]) : 0.);
              if (window_[u][v] == -1) n--;
            }
          if (n > 0 && (mean / n) < minQual_) {
            if (pos.size() == 0) {
              pos.push_back(i - windowSize_);
              pos.push_back(i);
            } else {
              if (i - windowSize_ <= pos[pos.size() - 1]) {
                pos[pos.size() - 1] = i; //Windows are overlapping and we extend previous region
              } else { //This is a new region
                pos.push_back(i - windowSize_);
                pos.push_back(i);
              }
            }
          }
      
          //Move forward:
          for (unsigned int k = 0; k < step_; ++k) {
            for (size_t j = 0; j < nr; ++j) {
              col[j] = aln[j][i];
            }  
            window_.push_back(col);
            window_.pop_front();
            ++i;
          }
        }

        //Evaluate last window:
        double mean = 0;
        double n = static_cast<double>(aln.size() * windowSize_);
        for (size_t u = 0; u < window_.size(); ++u)
          for (size_t v = 0; v < window_[u].size(); ++v) {
            mean += (window_[u][v] > 0 ? static_cast<double>(window_[u][v]) : 0.);
            if (window_[u][v] == -1) n--;
          }
        if (n > 0 && (mean / n) < minQual_) {
          if (pos.size() == 0) {
            pos.push_back(i - windowSize_);
            pos.push_back(i);
          } else {
            if (i - windowSize_ < pos[pos.size() - 1]) {
              pos[pos.size()] = i; //Windows are overlapping and we extend previous region
            } else { //This is a new region
              pos.push_back(i - windowSize_);
              pos.push_back(i);
            }
          }
        }
        if (verbose_)
          ApplicationTools::displayTaskDone();
    
        //Now we remove regions with two many gaps, using a sliding window:
        if (pos.size() == 0) {
          blockBuffer_.push_back(block);
          if (logstream_) {
            (*logstream_ << "QUAL CLEANER: block is clean and kept as is.").endLine();
          }
        } else if (pos.size() == 2 && pos.front() == 0 && pos.back() == block->getNumberOfSites()) {
          //Everything is removed:
          if (logstream_) {
            (*logstream_ << "QUAL CLEANER: block was entirely removed. Tried to get the next one.").endLine();
          }
        } else {
          if (logstream_) {
            (*logstream_ << "QUAL CLEANER: block with size "<< block->getNumberOfSites() << " will be split into " << (pos.size() / 2 + 1) << " blocks.").endLine();
          }
          if (verbose_) {
            ApplicationTools::message->endLine();
            ApplicationTools::displayTask("Spliting block", true);
          }
          for (i = 0; i < pos.size(); i+=2) {
            if (verbose_)
              ApplicationTools::displayGauge(i, pos.size() - 2, '=');
            if (logstream_) {
              (*logstream_ << "QUAL CLEANER: removing region (" << pos[i] << ", " << pos[i+1] << ") from block.").endLine();
            }
            if (pos[i] > 0) {
              MafBlock* newBlock = new MafBlock();
              newBlock->setScore(block->getScore());
              newBlock->setPass(block->getPass());
              for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
                MafSequence* subseq;
                if (i == 0) {
                  subseq = block->getSequence(j).subSequence(0, pos[i]);
                } else {
                  subseq = block->getSequence(j).subSequence(pos[i - 1], pos[i] - pos[i - 1]);
                }
                newBlock->addSequence(*subseq);
                delete subseq;
              }
              blockBuffer_.push_back(newBlock);
            }
           
            if (keepTrashedBlocks_) {
              MafBlock* outBlock = new MafBlock();
              outBlock->setScore(block->getScore());
              outBlock->setPass(block->getPass());
              for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
                MafSequence* outseq = block->getSequence(j).subSequence(pos[i], pos[i + 1] - pos[i]);
                outBlock->addSequence(*outseq);
                delete outseq;
              } 
              trashBuffer_.push_back(outBlock);
            }
          }
          //Add last block:
          if (pos[pos.size() - 1] < block->getNumberOfSites()) {
            MafBlock* newBlock = new MafBlock();
            newBlock->setScore(block->getScore());
            newBlock->setPass(block->getPass());
            for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
              MafSequence* subseq;
              subseq = block->getSequence(j).subSequence(pos[pos.size() - 1], block->getNumberOfSites() - pos[pos.size() - 1]);
              newBlock->addSequence(*subseq);
              delete subseq;
            }
            blockBuffer_.push_back(newBlock);
          }
          if (verbose_)
            ApplicationTools::displayTaskDone();

          delete block;
        }
      }
    } while (blockBuffer_.size() == 0);
  }

  MafBlock* block = blockBuffer_.front();
  blockBuffer_.pop_front();
  return block;
}

MafBlock* SequenceStatisticsMafIterator::nextBlock() throw (Exception)
{
  currentBlock_ = iterator_->nextBlock();
  if (currentBlock_) {
    for (vector<string>::iterator sp = species_.begin(); sp != species_.end(); ++sp) {
      if (sp > species_.begin())
        *output_ << "\t";
      if (currentBlock_->hasSequenceForSpecies(*sp)) {
        map<int, double> counts;
        const Sequence& seq = currentBlock_->getSequenceForSpecies(*sp);
        SequenceTools::getCounts(seq, counts, false);
        *output_ << counts[0] << "\t" << counts[1] << "\t" << counts[2] << "\t" << counts[3] << "\t" << counts[currentBlock_->getAlignment().getAlphabet()->getGapCharacterCode()];
        *output_ << "\t" << SequenceTools::getNumberOfSites(seq);
        *output_ << "\t" << SequenceTools::getNumberOfCompleteSites(seq);
        *output_ << "\t" << SequenceTools::getNumberOfUnresolvedSites(seq);
      } else {
        *output_ << "NA\tNA\tNA\tNA\tNA\tNA\tNA\tNA";
      }
    }
    output_->endLine();
  }
  return currentBlock_;
}

MafBlock* PairwiseSequenceStatisticsMafIterator::nextBlock() throw (Exception)
{
  currentBlock_ = iterator_->nextBlock();
  if (currentBlock_) {
    if (currentBlock_->hasSequenceForSpecies(species1_) && currentBlock_->hasSequenceForSpecies(species2_)) {
      *output_ << SequenceTools::getPercentIdentity(currentBlock_->getSequenceForSpecies(species1_), currentBlock_->getSequenceForSpecies(species2_), true);
    } else {
      *output_ << "NA";
    }
    output_->endLine();
  }
  return currentBlock_;
}

MafBlock* FeatureFilterMafIterator::nextBlock() throw (Exception)
{
  if (blockBuffer_.size() == 0) {
    //Unless there is no more block in the buffer, we need to parse more:
    do {
      MafBlock* block = iterator_->nextBlock();
      if (!block) return 0; //No more block.

      //Check if the block contains the reference species:
      if (!block->hasSequenceForSpecies(refSpecies_)) {
        if (logstream_) {
          (*logstream_ << "FEATURE FILTER: block " << block->getDescription() << " does not contain the reference species and was kept as is.").endLine(); 
        }
        return block;
      }

      //Get the feature ranges for this block:
      const MafSequence& refSeq = block->getSequenceForSpecies(refSpecies_);
      //first check if there is one (for now we assume that features refer to the chromosome or contig name, with implicit species):
      std::map<std::string, MultiRange<unsigned int> >::iterator mr = ranges_.find(refSeq.getChromosome());
      if (mr == ranges_.end()) {
        if (logstream_) {
          (*logstream_ << "FEATURE FILTER: block " << block->getDescription() << " does not contain any feature and was kept as is.").endLine(); 
        }
        return block;
      }
      //else
      MultiRange<unsigned int> mRange = mr->second;
      mRange.restrictTo(Range<unsigned int>(refSeq.start(), refSeq.stop()));
      if (mRange.isEmpty()) {
        if (logstream_) {
          (*logstream_ << "FEATURE FILTER: block " << block->getDescription() << " does not contain any feature and was kept as is.").endLine(); 
        }
        return block;
      }
      std::vector<unsigned int> tmp = mRange.getBounds(); 
      std::deque<unsigned int> refBounds(tmp.begin(), tmp.end()); 

      //Now extract corresponding alignments. We use the range to split the original block.
      //Only thing to watch out is the coordinates, refering to the ref species...
      //A good idea is then to convert those with respect to the given block:

      int gap = refSeq.getAlphabet()->getGapCharacterCode();
      long int refPos = refSeq.start() - 1;
      std::vector<size_t> pos;
      for (size_t alnPos = 0; alnPos < refSeq.size() && refBounds.size() > 0; ++alnPos) {
        if (refSeq[alnPos] != gap) {
          refPos++;
          //check if this position is a bound:
          if (refBounds.front() == static_cast<unsigned int>(refPos)) {
            pos.push_back(alnPos);
            refBounds.pop_front();
          }
        }
      }
      if (refBounds.size() > 0)
        throw Exception("FeatureFilterMafIterator::nextBlock(). An error occurred here, some coordinates are left... this is most likely a bug, please report!");

      //Next step is simply to split the black according to the translated coordinates:
      if (pos.size() == 2 && pos.front() == 0 && pos.back() == block->getNumberOfSites()) {
        //Everything is removed:
        if (logstream_) {
          (*logstream_ << "FEATURE FILTER: block " << block->getDescription() << " was entirely removed. Tried to get the next one.").endLine();
        }
      } else {
        if (logstream_) {
          (*logstream_ << "FEATURE FILTER: block " << block->getDescription() << " with size "<< block->getNumberOfSites() << " will be split into " << (pos.size() / 2 + 1) << " blocks.").endLine();
        }
        if (verbose_) {
          ApplicationTools::message->endLine();
          ApplicationTools::displayTask("Spliting block", true);
        }
        for (size_t i = 0; i < pos.size(); i+=2) {
          if (verbose_)
            ApplicationTools::displayGauge(i, pos.size() - 2, '=');
          if (logstream_) {
            (*logstream_ << "FEATURE FILTER: removing region (" << pos[i] << ", " << pos[i+1] << ") from block " << block->getDescription() << ".").endLine();
          }
          if (pos[i] > 0) {
            MafBlock* newBlock = new MafBlock();
            newBlock->setScore(block->getScore());
            newBlock->setPass(block->getPass());
            for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
              MafSequence* subseq;
              if (i == 0) {
                subseq = block->getSequence(j).subSequence(0, pos[i]);
              } else {
                subseq = block->getSequence(j).subSequence(pos[i - 1], pos[i] - pos[i - 1]);
              }
              newBlock->addSequence(*subseq);
              delete subseq;
            }
            blockBuffer_.push_back(newBlock);
          }
        
          if (keepTrashedBlocks_) {
            MafBlock* outBlock = new MafBlock();
            outBlock->setScore(block->getScore());
            outBlock->setPass(block->getPass());
            for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
              MafSequence* outseq = block->getSequence(j).subSequence(pos[i], pos[i + 1] - pos[i]);
              outBlock->addSequence(*outseq);
              delete outseq;
            } 
            trashBuffer_.push_back(outBlock);
          }
        }
        //Add last block:
        if (pos.back() < block->getNumberOfSites()) {
          MafBlock* newBlock = new MafBlock();
          newBlock->setScore(block->getScore());
          newBlock->setPass(block->getPass());
          for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
            MafSequence* subseq;
            subseq = block->getSequence(j).subSequence(pos[pos.size() - 1], block->getNumberOfSites() - pos[pos.size() - 1]);
            newBlock->addSequence(*subseq);
            delete subseq;
          }
          blockBuffer_.push_back(newBlock);
        }
        if (verbose_)
          ApplicationTools::displayTaskDone();

        delete block;
      }
    } while (blockBuffer_.size() == 0);
  }

  MafBlock* nxtBlock = blockBuffer_.front();
  blockBuffer_.pop_front();
  return nxtBlock;
}

MafBlock* FeatureExtractor::nextBlock() throw (Exception)
{
  if (blockBuffer_.size() == 0) {
    //Unless there is no more block in the buffer, we need to parse more:
    START:
    MafBlock* block = iterator_->nextBlock();
    if (!block) return 0; //No more block.

    //Check if the block contains the reference species:
    if (!block->hasSequenceForSpecies(refSpecies_))
      goto START;

    //Get the feature ranges for this block:
    const MafSequence& refSeq = block->getSequenceForSpecies(refSpecies_);
    //first check if there is one (for now we assume that features refer to the chromosome or contig name, with implicit species):
    std::map<std::string, RangeSet<unsigned int> >::iterator mr = ranges_.find(refSeq.getChromosome());
    if (mr == ranges_.end())
      goto START;
        
    RangeSet<unsigned int> ranges = mr->second;
    ranges.restrictTo(Range<unsigned int>(refSeq.start(), refSeq.stop()));
    if (ranges.isEmpty())
      goto START;

    //We will need to convert to alignment positions, using a sequence walker:
    SequenceWalker walker(refSeq);

    //Now creates all blocks for all ranges:
    if (verbose_) {
      ApplicationTools::message->endLine();
      ApplicationTools::displayTask("Extracting annotations", true);
    }
    if (logstream_) {
      (*logstream_ << "FEATURE EXTRACTOR: extracting " << ranges.getSet().size() << " features from block " << block->getDescription() << ".").endLine();
    }

    unsigned int i = 0;
    for (set< Range<unsigned int> >::iterator it = ranges.getSet().begin();
        it !=  ranges.getSet().end();
        ++it)
    {
      if (verbose_) {
        ApplicationTools::displayGauge(i++, ranges.getSet().size() - 1, '=');
      }
      MafBlock* newBlock = new MafBlock();
      newBlock->setScore(block->getScore());
      newBlock->setPass(block->getPass());
      for (unsigned int j = 0; j < block->getNumberOfSequences(); ++j) {
        MafSequence* subseq;
        unsigned int a = walker.getAlignmentPosition(it->begin() - refSeq.start());
        unsigned int b = walker.getAlignmentPosition(it->end() - refSeq.start() - 1);
        subseq = block->getSequence(j).subSequence(a, b - a + 1);
        newBlock->addSequence(*subseq);
        delete subseq;
      }
      blockBuffer_.push_back(newBlock);
    }
        
    if (verbose_)
      ApplicationTools::displayTaskDone();

    delete block;
  }

  MafBlock* nxtBlock = blockBuffer_.front();
  blockBuffer_.pop_front();
  return nxtBlock;
}


void OutputMafIterator::writeHeader(std::ostream& out) const
{
  out << "##maf version=1 program=Bio++" << endl << "#" << endl;
  //There are more options in the header that we may want to support...
}

void OutputMafIterator::writeBlock(std::ostream& out, const MafBlock& block) const
{
  out << "a";
  if (block.getScore() > -1.)
    out << " score=" << block.getScore();
  if (block.getPass() > 0)
    out << " pass=" << block.getPass();
  out << endl;
  
  //Now we write sequences. First need to count characters for aligning blocks:
  size_t mxcSrc = 0, mxcStart = 0, mxcSize = 0, mxcSrcSize = 0;
  for (unsigned int i = 0; i < block.getNumberOfSequences(); i++) {
    const MafSequence* seq = &block.getSequence(i);
    unsigned int start = 0; //Maybe we should output sthg else here?
    if (seq->hasCoordinates())
      start = seq->start();
    mxcSrc     = max(mxcSrc    , seq->getName().size());
    mxcStart   = max(mxcStart  , TextTools::toString(start).size());
    mxcSize    = max(mxcSize   , TextTools::toString(seq->getGenomicSize()).size());
    mxcSrcSize = max(mxcSrcSize, TextTools::toString(seq->getSrcSize()).size());
  }
  //Now print each sequence:
  for (unsigned int i = 0; i < block.getNumberOfSequences(); i++) {
    const MafSequence* seq = &block.getSequence(i);
    out << "s ";
    out << TextTools::resizeRight(seq->getName(), mxcSrc, ' ') << " ";
    unsigned int start = 0; //Maybe we should output sthg else here?
    if (seq->hasCoordinates())
      start = seq->start();
    out << TextTools::resizeLeft(TextTools::toString(start), mxcStart, ' ') << " ";
    out << TextTools::resizeLeft(TextTools::toString(seq->getGenomicSize()), mxcSize, ' ') << " ";
    out << seq->getStrand() << " ";
    out << TextTools::resizeLeft(TextTools::toString(seq->getSrcSize()), mxcSrcSize, ' ') << " ";
    //Shall we write the sequence as masked?
    string seqstr = seq->toString();
    if (mask_ && seq->hasAnnotation(SequenceMask::MASK)) {
      const SequenceMask* mask = &dynamic_cast<const SequenceMask&>(seq->getAnnotation(SequenceMask::MASK));
      for (unsigned int j = 0; j < seqstr.size(); ++j) {
        char c = ((*mask)[j] ? tolower(seqstr[j]) : seqstr[j]);
        out << c;
      }
    } else {
      out << seqstr;
    }
    out << endl;
    //Write quality scores if any:
    if (mask_ && seq->hasAnnotation(SequenceQuality::QUALITY_SCORE)) {
      const SequenceQuality* qual = &dynamic_cast<const SequenceQuality&>(seq->getAnnotation(SequenceQuality::QUALITY_SCORE));
      out << "q ";
      out << TextTools::resizeRight(seq->getName(), mxcSrc + mxcStart + mxcSize + mxcSrcSize + 5, ' ') << " ";
      string qualStr;
      for (unsigned int j = 0; j < seq->size(); ++j) {
        int s = (*qual)[j];
        if (s == -1) {
          qualStr += "-";
        } else if (s == -2) {
          qualStr += "?";
        } else if (s >=0 && s < 10) {
          qualStr += TextTools::toString(s);
        } else if (s == 10) {
          qualStr += "F";
        } else {
          throw Exception("MafAlignmentParser::writeBlock. Unsuported score value: " + TextTools::toString(s));
        }
      }
      out << qualStr << endl;
    }
  }
  out << endl;
}

