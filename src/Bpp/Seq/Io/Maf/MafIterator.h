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

#include "MafBlock.h"
#include "MafStatistics.h"
#include "../Clustal.h"

//From the STL:
#include <iostream>
#include <string>
#include <deque>

namespace bpp {

//Forward declaration:
class IterationListener;

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

    virtual bool verbose() const = 0;
    
    virtual void verbose(bool yn) = 0;
    
    virtual void addIterationListener(IterationListener* listener) = 0;
    

};

/**
 * @brief Partial implementation of the MafIterator interface.
 *
 * This implements the listener parts.
 */
class AbstractMafIterator:
  public virtual MafIterator
{
  protected:
    std::vector<IterationListener*> iterationListeners_;
    bool started_;
    bool verbose_;

  public:
    AbstractMafIterator(): iterationListeners_(), started_(false), verbose_(true) {}
    
    virtual ~AbstractMafIterator() {}

  public:
    void addIterationListener(IterationListener* listener) {
      iterationListeners_.push_back(listener);
    }

    MafBlock* nextBlock() throw (Exception) {
      if (!started_) {
        fireIterationStartSignal_();
        started_ = true;
      }
      MafBlock* block = analyseCurrentBlock_();
      if (block)
        fireIterationMoveSignal_(*block);
      else
        fireIterationStopSignal_();
      return block;
    }
    
    bool verbose() const { return verbose_; }
    void verbose(bool yn) { verbose_ = yn; }

  protected:
    virtual MafBlock* analyseCurrentBlock_() = 0;
    virtual void fireIterationStartSignal_();
    virtual void fireIterationMoveSignal_(const MafBlock& currentBlock);
    virtual void fireIterationStopSignal_();

};

/**
 * @brief Interface to loop over removed blocks of a maf alignment.
 */
class MafTrashIterator
{
  public:
    virtual ~MafTrashIterator() {}

  public:
    /**
     * @brief Get the next available removed alignment block.
     *
     * @return A maf alignment block, or a null pointer if no more block is available.
     */
    virtual MafBlock* nextRemovedBlock() throw (Exception) = 0;
    
};


/**
 * @brief Helper class for developping filter for maf blocks.
 */
class AbstractFilterMafIterator:
  public AbstractMafIterator
{
  protected:
    MafIterator* iterator_;
    MafBlock* currentBlock_;
    OutputStream* logstream_;

  public:
    AbstractFilterMafIterator(MafIterator* iterator) :
      AbstractMafIterator(),
      iterator_(iterator), currentBlock_(0),
      logstream_(ApplicationTools::message) {}

  private:
    AbstractFilterMafIterator(const AbstractFilterMafIterator& it):
      AbstractMafIterator(it), 
      iterator_(it.iterator_), currentBlock_(0),
      logstream_(it.logstream_) {}

    AbstractFilterMafIterator& operator=(const AbstractFilterMafIterator& it) {
      AbstractMafIterator::operator=(it);
      currentBlock_ = 0;
      iterator_  = it.iterator_;
      logstream_ = it.logstream_;
      return *this;
    }

  public:
    void setLogStream(OutputStream* logstream) { logstream_ = logstream; }

};

/**
 * @brief Filter maf blocks to keep only the ones with a minimum number of sites.
 */
class BlockSizeMafIterator:
  public AbstractFilterMafIterator
{
  private:
    unsigned int minSize_;

  public:
    BlockSizeMafIterator(MafIterator* iterator, unsigned int minSize) :
      AbstractFilterMafIterator(iterator),
      minSize_(minSize)
    {}

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception) {
      bool test;
      do {
        currentBlock_ = iterator_->nextBlock();
        if (!currentBlock_) break;
        test = (currentBlock_->getNumberOfSites() < minSize_);
        if (test) {
          if (logstream_) {
            (*logstream_ << "BLOCK SIZE FILTER: block " << currentBlock_->getDescription() << " with size " << currentBlock_->getNumberOfSites() << " was discarded.").endLine();
          }
          delete currentBlock_;
          currentBlock_ = 0;
        }
      } while (test);
      return currentBlock_;
    }

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

  public:
    /**
     * @param iterator The input iterator.
     * @param species The list of species names to be retained.
     * @param strict If true, then block that do not contain all species will be discarded.
     * @param rmDuplicates If true, block that contain more than one instance for at least one species will be discarded.
     */
    SequenceFilterMafIterator(MafIterator* iterator, const std::vector<std::string>& species, bool strict = false, bool rmDuplicates = false) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      strict_(strict),
      rmDuplicates_(rmDuplicates)
    {}

  private:
    SequenceFilterMafIterator(const SequenceFilterMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      species_(iterator.species_),
      strict_(iterator.strict_),
      rmDuplicates_(iterator.rmDuplicates_)
    {}
    
    SequenceFilterMafIterator& operator=(const SequenceFilterMafIterator& iterator)
    {
      species_       = iterator.species_;
      strict_        = iterator.strict_;
      rmDuplicates_  = iterator.rmDuplicates_;
      return *this;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Filter maf blocks to keep only the block corresponding to one chromosome (of a reference sequence).
 */
class ChromosomeMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::string ref_;
    std::string chr_;

  public:
    /**
     * @param iterator The input iterator.
     * @param reference The reference species name.
     * @param chr the chromosome name to filter.
     */
    ChromosomeMafIterator(MafIterator* iterator, const std::string& reference, const std::string& chr) :
      AbstractFilterMafIterator(iterator),
      ref_(reference),
      chr_(chr)
    {}

  private:
    ChromosomeMafIterator(const ChromosomeMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      ref_(iterator.ref_),
      chr_(iterator.chr_)
    {}
    
    ChromosomeMafIterator& operator=(const ChromosomeMafIterator& iterator)
    {
      ref_ = iterator.ref_;
      chr_ = iterator.chr_;
      return *this;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Filter maf blocks to remove duplicated blocks, according to a reference sequence).
 */
class DuplicateFilterMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::string ref_;
    /**
     * Contains the list of 'seen' block, as [chr][strand][start][stop]
     */
    std::map< std::string, std::map< char, std::map< unsigned int, std::map< unsigned int, unsigned int > > > > blocks_;

  public:
    /**
     * @param iterator The input iterator.
     * @param reference The reference species name.
     */
    DuplicateFilterMafIterator(MafIterator* iterator, const std::string& reference) :
      AbstractFilterMafIterator(iterator),
      ref_(reference),
      blocks_()
    {}

  private:
    DuplicateFilterMafIterator(const DuplicateFilterMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      ref_(iterator.ref_),
      blocks_(iterator.blocks_)
    {}
    
    DuplicateFilterMafIterator& operator=(const DuplicateFilterMafIterator& iterator)
    {
      ref_    = iterator.ref_;
      blocks_ = iterator.blocks_;
      return *this;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Merge blocks if some of their sequences are contiguous.
 *
 * The user specifies the focus species. Sequences that are not in this set will
 * be automatically merged and their coordinates removed.
 * The scores, if any, will be averaged for the block, weighted by the corresponding block sizes.
 * the pass value will be removed if it is different for the two blocks.
 * It is possible to define a maximum distance for the merging. Setting a distance of zero implies that the blocks
 * have to be exactly contiguous. Alternatively, the appropriate number of 'N' will be inserted in all species.
 * All species however have to be distant of the exact same amount.
 */
class BlockMergerMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::vector<std::string> species_;
    MafBlock* incomingBlock_;
    std::vector<std::string> ignoreChrs_; //These chromsomes will never be merged (ex: 'Un').
    unsigned int maxDist_;

  public:
    BlockMergerMafIterator(MafIterator* iterator, const std::vector<std::string>& species, unsigned int maxDist = 0) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      incomingBlock_(0),
      ignoreChrs_(),
      maxDist_(maxDist)
    {
      incomingBlock_ = iterator->nextBlock();
    }

  private:
    BlockMergerMafIterator(const BlockMergerMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      species_(iterator.species_),
      incomingBlock_(iterator.incomingBlock_),
      ignoreChrs_(iterator.ignoreChrs_),
      maxDist_(iterator.maxDist_)
    {}
    
    BlockMergerMafIterator& operator=(const BlockMergerMafIterator& iterator)
    {
      species_       = iterator.species_;
      incomingBlock_ = iterator.incomingBlock_;
      ignoreChrs_    = iterator.ignoreChrs_;
      maxDist_       = iterator.maxDist_;
      return *this;
    }

  public:
    /**
     * brief Add a chromosome that should be ignored to the list.
     * @param chr The name of the chromosome to be ignored.
     */
    void ignoreChromosome(const std::string& chr) {
      ignoreChrs_.push_back(chr);
    }
  
  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Filter maf blocks to remove in each block the positions made only of gaps.
 *
 * The subset of species that should be examined is given as input. The coordinates of these
 * species will not be altered as only gap positions are removed. Other species however may be
 * altered as they might not have gap in the removed position. The coordinates for these species
 * will therefore be removed as they do not make sense anymore.
 */
class FullGapFilterMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::vector<std::string> species_;

  public:
    FullGapFilterMafIterator(MafIterator* iterator, const std::vector<std::string>& species) :
      AbstractFilterMafIterator(iterator),
      species_(species)
    {}

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Filter maf blocks to remove ambiguously aligned or non-informative regions.
 *
 * Regions with a too high proportion of gaps or unknown character in a set of species will be removed,
 * and blocks adjusted accordingly. 
 */
class AlignmentFilterMafIterator:
  public AbstractFilterMafIterator,
  public virtual MafTrashIterator
{
  private:
    std::vector<std::string> species_;
    unsigned int windowSize_;
    unsigned int step_;
    unsigned int maxGap_;
    std::deque<MafBlock*> blockBuffer_;
    std::deque<MafBlock*> trashBuffer_;
    std::deque< std::vector<bool> > window_;
    bool keepTrashedBlocks_;

  public:
    AlignmentFilterMafIterator(MafIterator* iterator, const std::vector<std::string>& species, unsigned int windowSize, unsigned int step, unsigned int maxGap, bool keepTrashedBlocks) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      windowSize_(windowSize),
      step_(step),
      maxGap_(maxGap),
      blockBuffer_(),
      trashBuffer_(),
      window_(species.size()),
      keepTrashedBlocks_(keepTrashedBlocks)
    {}

  public:
    MafBlock* nextRemovedBlock() throw (Exception) {
      if (trashBuffer_.size() == 0) return 0;
      MafBlock* block = trashBuffer_.front();
      trashBuffer_.pop_front();
      return block;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);
};

/**
 * @brief Filter maf blocks to remove ambiguously aligned or non-informative regions.
 *
 * This iterators offers a different algorithm than AlignmentFilterMafIterator.
 * It takes two parameters: g=maxGap and n=maxPos. windows with at more than n positions containing each of them more than g=maxPos gaps will be discarded.
 * In addition, consecutives patterns are only counted once.
 */
class AlignmentFilter2MafIterator:
  public AbstractFilterMafIterator,
  public virtual MafTrashIterator
{
  private:
    std::vector<std::string> species_;
    unsigned int windowSize_;
    unsigned int step_;
    unsigned int maxGap_;
    unsigned int maxPos_;
    std::deque<MafBlock*> blockBuffer_;
    std::deque<MafBlock*> trashBuffer_;
    std::deque< std::vector<bool> > window_;
    bool keepTrashedBlocks_;

  public:
    AlignmentFilter2MafIterator(MafIterator* iterator, const std::vector<std::string>& species, unsigned int windowSize, unsigned int step, unsigned int maxGap, unsigned int maxPos, bool keepTrashedBlocks) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      windowSize_(windowSize),
      step_(step),
      maxGap_(maxGap),
      maxPos_(maxPos),
      blockBuffer_(),
      trashBuffer_(),
      window_(species.size()),
      keepTrashedBlocks_(keepTrashedBlocks)
    {}

  public:
    MafBlock* nextRemovedBlock() throw (Exception) {
      if (trashBuffer_.size() == 0) return 0;
      MafBlock* block = trashBuffer_.front();
      trashBuffer_.pop_front();
      return block;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Filter maf blocks to remove regions with masked positions.
 *
 * Regions with a too high proportion of masked position in a set of species will be removed,
 * and blocks adjusted accordingly. 
 */
class MaskFilterMafIterator:
  public AbstractFilterMafIterator,
  public MafTrashIterator
{
  private:
    std::vector<std::string> species_;
    unsigned int windowSize_;
    unsigned int step_;
    unsigned int maxMasked_;
    std::deque<MafBlock*> blockBuffer_;
    std::deque<MafBlock*> trashBuffer_;
    std::deque< std::vector<bool> > window_;
    bool keepTrashedBlocks_;

  public:
    MaskFilterMafIterator(MafIterator* iterator, const std::vector<std::string>& species, unsigned int windowSize, unsigned int step, unsigned int maxMasked, bool keepTrashedBlocks) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      windowSize_(windowSize),
      step_(step),
      maxMasked_(maxMasked),
      blockBuffer_(),
      trashBuffer_(),
      window_(species.size()),
      keepTrashedBlocks_(keepTrashedBlocks)
    {}

  public:
    MafBlock* nextRemovedBlock() throw (Exception) {
      if (trashBuffer_.size() == 0) return 0;
      MafBlock* block = trashBuffer_.front();
      trashBuffer_.pop_front();
      return block;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Filter maf blocks to remove regions with low quality.
 *
 * Regions with a too low average quality in a set of species will be removed,
 * and blocks adjusted accordingly. 
 */
class QualityFilterMafIterator:
  public AbstractFilterMafIterator,
  public MafTrashIterator
{
  private:
    std::vector<std::string> species_;
    unsigned int windowSize_;
    unsigned int step_;
    unsigned int minQual_;
    std::deque<MafBlock*> blockBuffer_;
    std::deque<MafBlock*> trashBuffer_;
    std::deque< std::vector<int> > window_;
    bool keepTrashedBlocks_;

  public:
    QualityFilterMafIterator(MafIterator* iterator, const std::vector<std::string>& species, unsigned int windowSize, unsigned int step, unsigned int minQual, bool keepTrashedBlocks) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      windowSize_(windowSize),
      step_(step),
      minQual_(minQual),
      blockBuffer_(),
      trashBuffer_(),
      window_(species.size()),
      keepTrashedBlocks_(keepTrashedBlocks)
    {}

  public:
    MafBlock* nextRemovedBlock() throw (Exception) {
      if (trashBuffer_.size() == 0) return 0;
      MafBlock* block = trashBuffer_.front();
      trashBuffer_.pop_front();
      return block;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Remove from alignment all positions that fall within any feature from a list given as a SequenceFeatureSet object.
 *
 * Removed regions are outputed as a trash iterator.
 */
class FeatureFilterMafIterator:
  public AbstractFilterMafIterator,
  public MafTrashIterator
{
  private:
    std::string refSpecies_;
    std::deque<MafBlock*> blockBuffer_;
    std::deque<MafBlock*> trashBuffer_;
    bool keepTrashedBlocks_;
    std::map<std::string, MultiRange<unsigned int> > ranges_;

  public:
    FeatureFilterMafIterator(MafIterator* iterator, const std::string& refSpecies, const SequenceFeatureSet& features, bool keepTrashedBlocks) :
      AbstractFilterMafIterator(iterator),
      refSpecies_(refSpecies),
      blockBuffer_(),
      trashBuffer_(),
      keepTrashedBlocks_(keepTrashedBlocks),
      ranges_()
    {
      //Build ranges:
      std::set<std::string> seqIds = features.getSequences();
      for (std::set<std::string>::iterator it = seqIds.begin();
          it != seqIds.end();
          ++it) {
        {
          features.fillRangeCollectionForSequence(*it, ranges_[*it]);
        }
      }
    }

  public:
    MafBlock* nextRemovedBlock() throw (Exception) {
      if (trashBuffer_.size() == 0) return 0;
      MafBlock* block = trashBuffer_.front();
      trashBuffer_.pop_front();
      return block;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief Extract alignments corresponding to sequence features given as a vector of SequenceFeature objects.
 *
 * The resulting blocks will contain the specified annotated regions.
 * Note that this iterator is not the opposite of FeatureFilterMafIterator,
 * as overlapping features will all be extracted. This iterator may therefore results
 * in duplication of original data.
 */
class FeatureExtractor:
  public AbstractFilterMafIterator
{
  private:
    std::string refSpecies_;
    bool ignoreStrand_;
    std::deque<MafBlock*> blockBuffer_;
    std::map<std::string, RangeSet<unsigned int> > ranges_;

  public:
    /**
     * @brief Build a new FeatureExtractor iterator.
     *
     * @param iterator The input iterator
     * @param refSpecies The reference species for feature coordinates
     * @param features The set of features to extract
     * @param ignoreStrand If true, features will be extracted 'as is', without being reversed in case they ar eon the negative strand.
     */
    FeatureExtractor(MafIterator* iterator, const std::string& refSpecies, const SequenceFeatureSet& features, bool ignoreStrand = false) :
      AbstractFilterMafIterator(iterator),
      refSpecies_(refSpecies),
      ignoreStrand_(ignoreStrand),
      blockBuffer_(),
      ranges_()
    {
      //Build ranges:
      std::set<std::string> seqIds = features.getSequences();
      for (std::set<std::string>::iterator it = seqIds.begin();
          it != seqIds.end();
          ++it) {
        {
          features.fillRangeCollectionForSequence(*it, ranges_[*it]);
        }
      }
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

class TrashIteratorAdapter:
  public AbstractMafIterator
{
  private:
    MafTrashIterator* iterator_;

  public:
    TrashIteratorAdapter(MafTrashIterator* iterator) :
      iterator_(iterator) {}

  private:
    TrashIteratorAdapter(const TrashIteratorAdapter& iterator) :
      iterator_(iterator.iterator_) {}
    
    TrashIteratorAdapter& operator=(const TrashIteratorAdapter& iterator) {
      iterator_ = iterator.iterator_;
      return *this;
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception) {
      return iterator_->nextRemovedBlock();
    }
};

/**
 * @brief This iterator forward the iterator given as input after having printed its content to a file.
 */
class OutputMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::ostream* output_;
    bool mask_;

  public:
    OutputMafIterator(MafIterator* iterator, std::ostream* out, bool mask = true) :
      AbstractFilterMafIterator(iterator), output_(out), mask_(mask)
    {
      if (output_)
        writeHeader(*output_);
    }

  private:
    OutputMafIterator(const OutputMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      output_(iterator.output_),
      mask_(iterator.mask_)
    {}
    
    OutputMafIterator& operator=(const OutputMafIterator& iterator)
    {
      output_ = iterator.output_;
      mask_   = iterator.mask_;
      return *this;
    }


  public:
    MafBlock* analyseCurrentBlock_() throw (Exception) {
      currentBlock_ = iterator_->nextBlock();
      if (output_ && currentBlock_)
        writeBlock(*output_, *currentBlock_);
      return currentBlock_;
    }

  private:
    void writeHeader(std::ostream& out) const;
    void writeBlock(std::ostream& out, const MafBlock& block) const;
};

/**
 * @brief This iterator forward the iterator given as input after having printed its content to an alignment file.
 * The syntax for ENSMBL meta data is used. For now the output format is clustal.
 */
class OutputAlignmentMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::ostream* output_;
    bool mask_;
    Clustal writer_;

  public:
    OutputAlignmentMafIterator(MafIterator* iterator, std::ostream* out, bool mask = true) :
      AbstractFilterMafIterator(iterator), output_(out), mask_(mask), writer_()
    {}

  private:
    OutputAlignmentMafIterator(const OutputAlignmentMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      output_(iterator.output_),
      mask_(iterator.mask_),
      writer_(iterator.writer_)
    {}
    
    OutputAlignmentMafIterator& operator=(const OutputAlignmentMafIterator& iterator)
    {
      output_ = iterator.output_;
      mask_   = iterator.mask_;
      writer_ = iterator.writer_;
      return *this;
    }


  private:
    MafBlock* analyseCurrentBlock_() throw (Exception) {
      MafBlock* block = iterator_->nextBlock();
      if (output_ && block)
        writeBlock(*output_, *block);
      return block;
    }

    void writeBlock(std::ostream& out, const MafBlock& block) const;
};

/*
class SequenceStatisticsMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::vector<std::string> species_;
    OutputStream* output_;
    MafBlock* currentBlock_;

  public:
    SequenceStatisticsMafIterator(MafIterator* iterator, const std::vector<std::string>& species, OutputStream* output) :
      AbstractFilterMafIterator(iterator),
      species_(species),
      output_(output),
      currentBlock_(0)
    {
      for (std::vector<std::string>::iterator sp = species_.begin(); sp != species_.end(); ++sp) {
        if (sp > species_.begin())
          *output_ << "\t";
        (*output_ << *sp << ".A\t"
                  << *sp << ".C\t"
                  << *sp << ".G\t"
                  << *sp << ".T\t"
                  << *sp << ".gap\t"
                  << *sp << ".NbSites\t"
                  << *sp << ".NbComplete\t"
                  << *sp << ".NbUnresolved");
      }
      output_->endLine();
    }

  private:
    SequenceStatisticsMafIterator(const SequenceStatisticsMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      species_(iterator.species_),
      output_(iterator.output_),
      currentBlock_(0)
    {}
    
    SequenceStatisticsMafIterator& operator=(const SequenceStatisticsMafIterator& iterator)
    {
      species_       = iterator.species_;
      output_        = iterator.output_;
      currentBlock_  = 0;
      return *this;
    }

  public:
    MafBlock* nextBlock() throw (Exception);

};*/

/**
 * @brief Compute a series of sequence statistics for each block.
 *
 * Computed statistics are stored into a vector of double, which can be retrieved as well as statistics names.
 * Listeners can be set up to automatically analyse or write the output after iterations are over.
 *
 * The current implementation focuses on speed and memory efificiency, as it only stores in memory the current results of the statistics.
 * The only drawback of this, is that disk access might be high when writing the results,
 * although appropriate buffering should most likely circumvent the issue.
 * The code is easily extensible, however, to enable storage of all results into a matrix,
 * with writing only once at the end of iterations.
 */
class SequenceStatisticsMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::vector<MafStatistics*> statistics_;
    std::vector<double> results_;
    std::vector<std::string> names_;

  public:
    /**
     * @param iterator The input iterator.
     * @param species The list of species names for which statistics should be computed.
     * @param output The output stream where to store the results.
     */
    SequenceStatisticsMafIterator(MafIterator* iterator, const std::vector<MafStatistics*> statistics);

  private:
    SequenceStatisticsMafIterator(const SequenceStatisticsMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      statistics_(iterator.statistics_),
      results_(iterator.results_),
      names_(iterator.names_)
    {}
    
    SequenceStatisticsMafIterator& operator=(const SequenceStatisticsMafIterator& iterator)
    {
      statistics_ = iterator.statistics_;
      results_ = iterator.results_;
      names_ = iterator.names_;
      return *this;
    }

  public:
    const std::vector<double>& getResults() const { return results_; }
    const std::vector<std::string>& getResultsColumnNames() const { return names_; }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};


/*class PairwiseSequenceStatisticsMafIterator:
  public AbstractFilterMafIterator
{
  private:
    std::string species1_;
    std::string species2_;
    OutputStream* output_;
    MafBlock* currentBlock_;

  public:
    PairwiseSequenceStatisticsMafIterator(MafIterator* iterator, const std::string& species1, const std::string& species2, OutputStream* output) :
      AbstractFilterMafIterator(iterator),
      species1_(species1),
      species2_(species2),
      output_(output),
      currentBlock_(0)
    {
      (*output_ << "PercentId").endLine();
    }

  private:
    PairwiseSequenceStatisticsMafIterator(const PairwiseSequenceStatisticsMafIterator& iterator) :
      AbstractFilterMafIterator(0),
      species1_(iterator.species1_),
      species2_(iterator.species2_),
      output_(iterator.output_),
      currentBlock_(0)
    {}
    
    PairwiseSequenceStatisticsMafIterator& operator=(const PairwiseSequenceStatisticsMafIterator& iterator)
    {
      species1_     = iterator.species1_;
      species2_     = iterator.species2_;
      output_       = iterator.output_;
      currentBlock_ = 0;
      return *this;
    }

  public:
    MafBlock* nextBlock() throw (Exception);

};
*/

/**
 * @brief Splits block into windows of given sizes.
 */
class WindowSplitMafIterator:
  public AbstractFilterMafIterator
{
  private:
    unsigned int windowSize_;
    short align_;
    std::deque<MafBlock*> blockBuffer_;

  public:
    static const short RAGGED_LEFT;
    static const short RAGGED_RIGHT;
    static const short CENTER;
    static const short ADJUST;

  public:
    WindowSplitMafIterator(MafIterator* iterator, unsigned int windowSize, short splitOption = CENTER) throw (Exception):
      AbstractFilterMafIterator(iterator),
      windowSize_(windowSize), align_(splitOption), blockBuffer_()
    {
      if (splitOption != RAGGED_LEFT && splitOption != RAGGED_RIGHT
          && splitOption != CENTER && splitOption != ADJUST)
        throw Exception("WindowSplitMafIterator: unvalid split option: " + splitOption);
    }

  private:
    MafBlock* analyseCurrentBlock_() throw (Exception);

};

/**
 * @brief This special iterator synchronizes two adaptors.
 *
 * It takes as input a main iterator and a secondary one. The nextBlock method of the secondary iterator will be
 * called immediately after the one of the primary one. The resulting block of the main iterator will be forwarded,
 * while the one of the secondary iterator will be destroyed.
 */
class MafIteratorSynchronizer:
  public AbstractFilterMafIterator
{
  private:
    MafIterator* secondaryIterator_;

  public:
    MafIteratorSynchronizer(MafIterator* primaryIterator, MafIterator* secondaryIterator) :
      AbstractFilterMafIterator(primaryIterator), secondaryIterator_(secondaryIterator)
    {}

  private:
    MafIteratorSynchronizer(const MafIteratorSynchronizer& iterator) :
      AbstractFilterMafIterator(0),
      secondaryIterator_(iterator.secondaryIterator_)
    {}
    
    MafIteratorSynchronizer& operator=(const MafIteratorSynchronizer& iterator)
    {
      secondaryIterator_ = iterator.secondaryIterator_;
      return *this;
    }


  private:
    MafBlock* analyseCurrentBlock_() throw (Exception) {
      currentBlock_ = iterator_->nextBlock();
      MafBlock* secondBlock = secondaryIterator_->nextBlock();
      if (secondBlock)
        delete secondBlock;
      return currentBlock_;
    }

};


} // end of namespace bpp.

#endif //_MAFITERATOR_H_
