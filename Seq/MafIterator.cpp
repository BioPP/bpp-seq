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
#include "SequenceWithQuality.h"

using namespace bpp;

//From the STL:
#include <string>

using namespace std;

MafBlock* SequenceFilterMafIterator::nextBlock() throw (Exception)
{
  currentBlock_ = iterator_->nextBlock();
  while (currentBlock_) {
    for (unsigned int i = currentBlock_->getNumberOfSequences(); i > 0; --i) {
      string name = currentBlock_->getAlignment().getName(i); 
      if (!VectorTools::contains(species_, name)) {
        if (logstream_) {
          (*logstream_ << "SEQUENCE FILTER: remove sequence '" << name << "' from current block.").endLine();
        }
        currentBlock_->getAlignment().deleteSequence(i);
      }
    }
    bool test = currentBlock_->getNumberOfSequences() == 0;
    //Avoid a memory leak:
    if (test)
      delete currentBlock_;
    else
      return currentBlock_;
    
    //Look for the next block:
    if (logstream_) {
      (*logstream_ << "SEQUENCE FILTER: block is now empty. Try to get the next one.").endLine();
    }
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
    for (unsigned int i = 0; i < species_.size(); ++i) {
      try {
        const MafSequence* seq1 = &currentBlock_->getSequence(species_[i]); 
        const MafSequence* seq2 = &incomingBlock_->getSequence(species_[i]);
        if (seq1->getChromosome() != seq2->getChromosome()
         || VectorTools::contains(ignoreChrs_, seq1->getChromosome())
         || VectorTools::contains(ignoreChrs_, seq2->getChromosome())
         || seq1->getStrand()     != seq2->getStrand()
         || seq1->start()         != seq2->stop() - 1)
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
    vector<string> names1 = currentBlock_->getAlignment().getSequencesNames();
    vector<string> names2 = incomingBlock_->getAlignment().getSequencesNames();
    vector<string> allNames = VectorTools::vectorUnion(names1, names2);
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
    for (unsigned int i = 0; i < allNames.size(); ++i) {
      auto_ptr<MafSequence> seq;
      try {
        seq.reset(new MafSequence(currentBlock_->getSequence(allNames[i])));
        unsigned int srcSize = seq->getSrcSize();

        //Check is there is a second sequence:
        try {
          const MafSequence* tmp = &incomingBlock_->getSequence(allNames[i]);
          string ref1 = seq->getDescription(), ref2 = tmp->getDescription();
          seq->merge(*tmp);
          srcSize += tmp->getSrcSize();
          seq->setSrcSize(srcSize);
          if (logstream_) {
            (*logstream_ << "BLOCK MERGER: merging " << ref1 << " with " << ref2 << " into " << seq->getDescription()).endLine();
          }
        } catch (SequenceNotFoundException& snfe2) {
          //There was a first sequence, we just extend it:
          string ref1 = seq->getDescription();
          seq->setToSizeR(incomingBlock_->getNumberOfSites());
          if (logstream_) {
            (*logstream_ << "BLOCK MERGER: extending " << ref1 << " with " << incomingBlock_->getNumberOfSites() << " gaps on the right.").endLine();
          }
        }
      } catch (SequenceNotFoundException& snfe1) {
        //There must be a second sequence then:
        seq.reset(new MafSequence(incomingBlock_->getSequence(allNames[i])));
        string ref2 = seq->getDescription();
        seq->setToSizeL(currentBlock_->getNumberOfSites());
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

MafBlock* GapFilterMafIterator::nextBlock() throw (Exception)
{
  return NULL;
}

MafBlock* QualityFilterMafIterator::nextBlock() throw (Exception)
{
  return NULL;
}

