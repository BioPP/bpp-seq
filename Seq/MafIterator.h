//
// File: MafIterator.h
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

#ifndef _MAFITERATOR_H_
#define _MAFITERATOR_H_

#include "SequenceWithAnnotation.h"
#include "SequenceTools.h"
#include "AlphabetTools.h"
#include "AlignedSequenceContainer.h"

//From the STL:
#include <iostream>
#include <string>

namespace bpp {

/**
 * @brief A sequence class which is used to store data from MAF files.
 * 
 * It extends the SequenceWithAnnotation class to store MAF-specific features,
 * like the chromosome position. The sequence is its own listener,
 * and recomputes its "genomic" site by using the SequenceTools::getNumberOfSites
 * function when a content modification is performed.
 * Tags like begin and stop, hovever, have to be set by hand.
 *
 * A MAF sequence is necessarily a DNA sequence.
 */
class MafSequence:
  public SequenceWithAnnotation
{
  private:
    unsigned int begin_;
    std::string  species_;
    std::string  chromosome_;
    char         strand_;
    unsigned int size_;
    unsigned int srcSize_;

  public:
    MafSequence():
      SequenceWithAnnotation(&AlphabetTools::DNA_ALPHABET), begin_(0), species_(""), chromosome_(""), strand_(0), size_(0), srcSize_(0)
    {
      size_ = 0;
    }

    MafSequence(const std::string& name, const std::string& sequence):
      SequenceWithAnnotation(name, sequence, &AlphabetTools::DNA_ALPHABET), begin_(0), species_(""), chromosome_(""), strand_(0), size_(0), srcSize_(0)
    {
      size_ = SequenceTools::getNumberOfSites(*this);
      size_t pos = name.find(".");
      if (pos != std::string::npos) {
        chromosome_ = name.substr(pos + 1);
        species_    = name.substr(0, pos);
      }
    }

    MafSequence(const std::string& name, const std::string& sequence, unsigned int begin, char strand, unsigned int srcSize) :
      SequenceWithAnnotation(name, sequence, &AlphabetTools::DNA_ALPHABET), begin_(begin), species_(""), chromosome_(""), strand_(strand), size_(0), srcSize_(srcSize)
    {
      size_ = SequenceTools::getNumberOfSites(*this);
      size_t pos = name.find(".");
      if (pos != std::string::npos) {
        chromosome_ = name.substr(pos + 1);
        species_    = name.substr(0, pos);
      }
    }

    MafSequence* clone() const { return new MafSequence(*this); }

    ~MafSequence() {}

  public:
    unsigned int start() const { return begin_; }
    unsigned int stop() const { return begin_ + size_ - 1; }
    const std::string& getSpecies() const { return species_; }
    const std::string& getChromosome() const { return chromosome_; }
    char getStrand() const { return strand_; }
    unsigned int getGenomicSize() const { return size_; }
    unsigned int getSrcSize() const { return srcSize_; }
    void setStart(unsigned int begin) { begin_ = begin; }
    void setChromosome(const std::string& chr) { chromosome_ = chr; }
    void setStrand(char s) { strand_ = s; }
    void setSrcSize(unsigned int srcSize) { srcSize_ = srcSize; }
  
    std::string getDescription() const { return getName() + strand_ + ":" + TextTools::toString(start()) + "-" + TextTools::toString(stop()); }
  
  private:
    void beforeSequenceChanged(const SymbolListEditionEvent& event) {}
    void afterSequenceChanged(const SymbolListEditionEvent& event) { size_ = SequenceTools::getNumberOfSites(*this); }
    void beforeSequenceInserted(const SymbolListInsertionEvent& event) {}
    void afterSequenceInserted(const SymbolListInsertionEvent& event) { size_ = SequenceTools::getNumberOfSites(*this); }
    void beforeSequenceDeleted(const SymbolListDeletionEvent& event) {}
    void afterSequenceDeleted(const SymbolListDeletionEvent& event) { size_ = SequenceTools::getNumberOfSites(*this); }
    void beforeSequenceSubstituted(const SymbolListSubstitutionEvent& event) {}
    void afterSequenceSubstituted(const SymbolListSubstitutionEvent& event) {}
};

/**
 * @brief A synteny block data structure, the basic unit of a MAF alignement file.
 *
 * This class basically contains a AlignedSequenceContainer made of MafSequence objects.
 */
class MafBlock
{
  private:
    double score_;
    unsigned int pass_;
    AlignedSequenceContainer alignment_;

  public:
    MafBlock() :
      score_(-1),
      pass_(0),
      alignment_(&AlphabetTools::DNA_ALPHABET)
    {}

  public:
    void setScore(double score) { score_ = score; }
    void setPass(unsigned int pass) { pass_ = pass; }
    
    double getScore() const { return score_; }
    unsigned int getPass() const { return pass_; }

    AlignedSequenceContainer& getAlignment() { return alignment_; }
    const AlignedSequenceContainer& getAlignment() const { return alignment_; }

    void addSequence(const MafSequence& sequence) { alignment_.addSequence(sequence, false); }

    const MafSequence& getSequence(const std::string& name) const throw (SequenceNotFoundException) {
      return dynamic_cast<const MafSequence&>(getAlignment().getSequence(name));
    }

    const MafSequence& getSequence(unsigned int i) const throw (IndexOutOfBoundsException) {
      return dynamic_cast<const MafSequence&>(getAlignment().getSequence(i));
    }

    unsigned int getNumberOfSequences() const { return alignment_.getNumberOfSequences(); }
    
    unsigned int getNumberOfSites() const { return alignment_.getNumberOfSites(); }

};


/**
 * @brief Interface to loop over maf alignment blocks.
 */
class MafIterator
{
  public:
    virtual ~MafIterator() {}

  public:
    /**
     * @brief Get the next available alignment block.
     *
     * @return A maf alignment block, or a null pointer if no more block is available.
     */
    virtual MafBlock* nextBlock() throw (Exception) = 0;
    
};

/**
 * @brief Helper class for developping filter for maf blocks.
 */
class AbstractFilterMafIterator:
  public MafIterator
{
  protected:
    MafIterator* iterator_;
    OutputStream* logstream_;

  public:
    AbstractFilterMafIterator(MafIterator* iterator) :
      iterator_(iterator), logstream_(ApplicationTools::message) {}

  private:
    AbstractFilterMafIterator(const AbstractFilterMafIterator& it): iterator_(it.iterator_), logstream_(it.logstream_) {}
    AbstractFilterMafIterator& operator=(const AbstractFilterMafIterator& it) {
      iterator_  = it.iterator_;
      logstream_ = it.logstream_;
      return *this;
    }

  public:
    void setLogStream(OutputStream* logstream) { logstream_ = logstream; }

};


/**
 * @brief Filter maf blocks to keep a subset of sequences, given their name.
 *
 * Blocks that are empty after the filtering are removed.
 */
class SequenceFilterMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::vector<std::string> species_;
    bool strict_;
    bool rmDuplicates_;
    MafBlock* currentBlock_;

  public:
    /**
     * @param strict If true, then block that do not contain all species will be discarded.
     * @param rmDuplicates If true, block that contain more than one instance for at least one species will be discarded.
     */
    SequenceFilterMafIterator(MafIterator* iterator, const std::vector<std::string>& species, bool strict = false, bool rmDuplicates = false) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      strict_(strict),
      rmDuplicates_(rmDuplicates),
      currentBlock_(0)
    {}

  private:
    SequenceFilterMafIterator(const SequenceFilterMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      species_(iterator.species_),
      strict_(iterator.strict_),
      rmDuplicates_(iterator.rmDuplicates_),
      currentBlock_(0)
    {}
    
    SequenceFilterMafIterator& operator=(const SequenceFilterMafIterator& iterator)
    {
      species_       = iterator.species_;
      strict_        = iterator.strict_;
      rmDuplicates_  = iterator.rmDuplicates_;
      currentBlock_  = 0;
      return *this;
    }

  public:
    MafBlock* nextBlock() throw (Exception);

};

/**
 * @brief Merge blocks if some of their sequences are contiguous.
 *
 * The user specifies the focus species. Sequences that are not in this set will
 * be merged without testing, and their genomic coordinates removed.
 * The scores and pass values, if any, will be averaged for the block, weighted by the corresponding block sizes.
 */
class BlockMergerMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::vector<std::string> species_;
    MafBlock* incomingBlock_;
    MafBlock* currentBlock_;
    std::vector<std::string> ignoreChrs_; //These chromsomes will never be merged (ex: 'Un').

  public:
    BlockMergerMafIterator(MafIterator* iterator, const std::vector<std::string>& species) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      incomingBlock_(0),
      currentBlock_(0),
      ignoreChrs_()
    {
      incomingBlock_ = iterator->nextBlock();
    }

  private:
    BlockMergerMafIterator(const BlockMergerMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      species_(iterator.species_),
      incomingBlock_(iterator.incomingBlock_),
      currentBlock_(iterator.currentBlock_),
      ignoreChrs_(iterator.ignoreChrs_)
    {}
    
    BlockMergerMafIterator& operator=(const BlockMergerMafIterator& iterator)
    {
      species_       = iterator.species_;
      incomingBlock_ = iterator.incomingBlock_;
      currentBlock_  = iterator.currentBlock_;
      ignoreChrs_    = iterator.ignoreChrs_;
      return *this;
    }

  public:
    MafBlock* nextBlock() throw (Exception);

    /**
     * brief Add a chromosome that should be ignored to the list.
     * @param chr The name of the chromosome to be ignored.
     */
    void ignoreChromosome(const std::string& chr) {
      ignoreChrs_.push_back(chr);
    }
};


/**
 * @brief Filter maf blocks to remove ambiguously aligned regions.
 *
 * Regions with a too high proportion of gaps in a set of species will be removed,
 * and blocks adjusted accordingly. 
 */
class GapFilterMafIterator:
  public AbstractFilterMafIterator
{
  public:
    GapFilterMafIterator(MafIterator* iterator) :
      AbstractFilterMafIterator(iterator)
    {}

  public:
    MafBlock* nextBlock() throw (Exception);

};

/**
 * @brief Filter maf blocks to remove regions with low quality scores.
 *
 * Regions with a too poor quality in a set of species will be removed,
 * and blocks adjusted accordingly. 
 */
class QualityFilterMafIterator:
  public AbstractFilterMafIterator
{
  public:
    QualityFilterMafIterator(MafIterator* iterator) :
      AbstractFilterMafIterator(iterator)
    {}

  public:
    MafBlock* nextBlock() throw (Exception);

};



} // end of namespace bpp.

#endif //_MAFITERATOR_H_
