//
// File: MafSequence.h
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

#ifndef _MAFSEQUENCE_H_
#define _MAFSEQUENCE_H_

#include "../../SequenceWithAnnotation.h"
#include "../../Alphabet/AlphabetTools.h"
#include "../../Feature/SequenceFeature.h"
#include "../../SequenceTools.h"

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
    bool         hasCoordinates_;
    unsigned int begin_;
    std::string  species_;
    std::string  chromosome_;
    char         strand_;
    unsigned int size_;
    unsigned int srcSize_;

  public:
    MafSequence():
      SequenceWithAnnotation(&AlphabetTools::DNA_ALPHABET), hasCoordinates_(false), begin_(0), species_(""), chromosome_(""), strand_(0), size_(0), srcSize_(0)
    {
      size_ = 0;
    }

    MafSequence(const std::string& name, const std::string& sequence):
      SequenceWithAnnotation(name, sequence, &AlphabetTools::DNA_ALPHABET), hasCoordinates_(false), begin_(0), species_(""), chromosome_(""), strand_(0), size_(0), srcSize_(0)
    {
      size_ = SequenceTools::getNumberOfSites(*this);
      size_t pos = name.find(".");
      if (pos != std::string::npos) {
        chromosome_ = name.substr(pos + 1);
        species_    = name.substr(0, pos);
      }
    }

    MafSequence(const std::string& name, const std::string& sequence, unsigned int begin, char strand, unsigned int srcSize) :
      SequenceWithAnnotation(name, sequence, &AlphabetTools::DNA_ALPHABET), hasCoordinates_(true), begin_(begin), species_(""), chromosome_(""), strand_(strand), size_(0), srcSize_(srcSize)
    {
      size_ = SequenceTools::getNumberOfSites(*this);
      setName(name);
    }

    MafSequence* clone() const { return new MafSequence(*this); }

    ~MafSequence() {}

  public:
    bool hasCoordinates() const { return hasCoordinates_; }

    void removeCoordinates() { hasCoordinates_ = false; begin_ = 0; }

    unsigned int start() const throw (Exception) { 
      if (hasCoordinates_) return begin_;
      else throw Exception("MafSequence::start(). Sequence " + getName() + " does not have coordinates.");
    }

    unsigned int stop() const { 
      if (hasCoordinates_) return begin_ + size_ - 1;
      else throw Exception("MafSequence::stop(). Sequence " + getName() + " does not have coordinates.");
    }

    Range<unsigned int> getRange() const {
      if (hasCoordinates_) return Range<unsigned int>(start(), stop());
      else throw Exception("MafSequence::getRange(). Sequence " + getName() + " does not have coordinates.");
    }

    void setName(const std::string& name) {
      size_t pos = name.find(".");
      if (pos != std::string::npos) {
        chromosome_ = name.substr(pos + 1);
        species_    = name.substr(0, pos);
      }
      SequenceWithAnnotation::setName(name);
    }

    const std::string& getSpecies() const { return species_; }
    
    const std::string& getChromosome() const { return chromosome_; }
    
    char getStrand() const { return strand_; }
    
    unsigned int getGenomicSize() const { return size_; }
    
    unsigned int getSrcSize() const { return srcSize_; }
    
    void setStart(unsigned int begin) { begin_ = begin; hasCoordinates_ = true; }
    
    void setChromosome(const std::string& chr) {
      chromosome_ = chr;
      SequenceWithAnnotation::setName(species_ + "." + chromosome_);
    }
    
    void setSpecies(const std::string& species) {
      species_ = species;
      SequenceWithAnnotation::setName(species_ + "." + chromosome_);
    }
    
    void setStrand(char s) { strand_ = s; }
    
    void setSrcSize(unsigned int srcSize) { srcSize_ = srcSize; }
  
    std::string getDescription() const { return getName() + strand_ + ":" + (hasCoordinates_ ? TextTools::toString(start()) + "-" + TextTools::toString(stop()) : "?-?"); }
  
    /**
     * @brief Extract a sub-sequence.
     *
     * @return A subsequence.
     * @param startAt Begining of sub-sequence.
     * @param length  the length of the sub-sequence.
     */
    MafSequence* subSequence(unsigned int startAt, unsigned int length) const;
    
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

} // end of namespace bpp.

#endif //_MAFSEQUENCE_H_
