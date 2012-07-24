//
// File: MafBlock.h
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

#ifndef _MAFBLOCK_H_
#define _MAFBLOCK_H_

#include "MafSequence.h"
#include "../../Container/AlignedSequenceContainer.h"

#include <Bpp/Clonable.h>

namespace bpp {

/**
 * @brief A synteny block data structure, the basic unit of a MAF alignement file.
 *
 * This class basically contains a AlignedSequenceContainer made of MafSequence objects.
 */
class MafBlock:
  public virtual Clonable
{
  private:
    double score_;
    unsigned int pass_;
    AlignedSequenceContainer alignment_;
    std::map<std::string, Clonable*> properties_;

  public:
    MafBlock() :
      score_(log(0)),
      pass_(0),
      alignment_(&AlphabetTools::DNA_ALPHABET),
      properties_()
    {}

    MafBlock(const MafBlock& block):
      score_(block.score_),
      pass_(block.pass_),
      alignment_(block.alignment_),
      properties_()
    {
      std::map<std::string, Clonable*>::const_iterator it;
      for (it = block.properties_.begin(); it != block.properties_.end(); ++it) {
        properties_[it->first] = it->second->clone();
      }
    }

    MafBlock& operator=(const MafBlock& block)
    {
      score_     = block.score_;
      pass_      = block.pass_;
      alignment_ = block.alignment_;
      deleteProperties_();
      std::map<std::string, Clonable*>::const_iterator it;
      for (it = block.properties_.begin(); it != block.properties_.end(); ++it) {
        properties_[it->first] = it->second->clone();
      }
      return *this;
    }

    MafBlock* clone() const { return new MafBlock(*this); }

    ~MafBlock() { deleteProperties_(); }

  public:
    void setScore(double score) { score_ = score; }
    void setPass(unsigned int pass) { pass_ = pass; }
    
    double getScore() const { return score_; }
    unsigned int getPass() const { return pass_; }

    AlignedSequenceContainer& getAlignment() { return alignment_; }
    const AlignedSequenceContainer& getAlignment() const { return alignment_; }

    unsigned int getNumberOfSequences() const { return alignment_.getNumberOfSequences(); }
    
    unsigned int getNumberOfSites() const { return alignment_.getNumberOfSites(); }

    void addSequence(const MafSequence& sequence) { alignment_.addSequence(sequence, false); }

    bool hasSequence(const std::string& name) const {
      return getAlignment().hasSequence(name);
    }

    const MafSequence& getSequence(const std::string& name) const throw (SequenceNotFoundException) {
      return dynamic_cast<const MafSequence&>(getAlignment().getSequence(name));
    }

    const MafSequence& getSequence(unsigned int i) const throw (IndexOutOfBoundsException) {
      return dynamic_cast<const MafSequence&>(getAlignment().getSequence(i));
    }

    bool hasSequenceForSpecies(const std::string& species) const {
      for (unsigned int i = 0; i < getNumberOfSequences(); ++i) {
        const MafSequence& seq = getSequence(i);
        if (seq.getSpecies() == species)
          return true;
      }
      return false;
    }

    //Return the first sequence with the species name.
    const MafSequence& getSequenceForSpecies(const std::string& species) const throw (SequenceNotFoundException) {
      for (unsigned int i = 0; i < getNumberOfSequences(); ++i) {
        const MafSequence& seq = getSequence(i);
        if (seq.getSpecies() == species)
          return seq;
      }
      throw SequenceNotFoundException("MafBlock::getSequenceForSpecies. No sequence with the given species name in this block.", species);
    }

    //Return all sequences with the species name.
    std::vector<const MafSequence*> getSequencesForSpecies(const std::string& species) const throw (SequenceNotFoundException) {
      std::vector<const MafSequence*> selection;
      for (unsigned int i = 0; i < getNumberOfSequences(); ++i) {
        const MafSequence* seq = &getSequence(i);
        if (seq->getSpecies() == species)
          selection.push_back(seq);
      }
      return selection;
    }

    /**
     * @return The species names for all sequencies in the container.
     */
    std::vector<std::string> getSpeciesList() const {
      std::vector<std::string> lst;
      for (unsigned int i = 0; i < getNumberOfSequences(); ++i) {
        lst.push_back(getSequence(i).getSpecies());
      }
      return lst;
    }

    void removeCoordinatesFromSequence(unsigned int i) throw (IndexOutOfBoundsException) {
      //This is a bit of a trick, but avoid useless recopies.
      //It is safe here because the AlignedSequenceContainer is fully encapsulated.
      //It would not work if a VectorSiteContainer was used.
      const_cast<MafSequence&>(getSequence(i)).removeCoordinates();
    }

    std::string getDescription() const {
      std::string desc;
      desc += TextTools::toString(getNumberOfSequences()) + "x" + TextTools::toString(getNumberOfSites());
      return desc;
    }

    /**
     * @return True or False, if data are associated to the given property.
     * @param property The name of the property to look for.
     */
    bool hasProperty(const std::string& property) const
    {
      std::map<std::string, Clonable*>::const_iterator it = properties_.find(property);
      return it != properties_.end();
    }

    /**
     * @brief Get the data associated to a query property.
     *
     * @param property The property to look for.
     * @return The data associated to the given property.
     * @throw Exception if no data is associated to the given property.
     */
    const Clonable& getProperty(const std::string& property) const throw (Exception)
    {
      std::map<std::string, Clonable*>::const_iterator it = properties_.find(property);
      if (it == properties_.end())
        throw Exception("MafBlock::getProperty. No data for property: " + property + " in block.");
      return *it->second;
    }

    /**
     * @brief Delete the data associated to a query property.
     *
     * @param property The property to look for.
     * @throw Exception if no data is associated to the given property.
     */
    void deleteProperty(const std::string& property) throw (Exception)
    {
      std::map<std::string, Clonable*>::iterator it = properties_.find(property);
      if (it == properties_.end())
        throw Exception("MafBlock::deleteProperty. No data for property: " + property + " in block.");
      delete it->second;
      properties_.erase(it);
    }

    /**
     * @brief Set the data associated to a query property.
     *
     * An existing data associated to this property will be deleted and replaced by the new one.
     * @param property The property to look for.
     * @throw Exception if the pointer toward the input data is NULL.
     */
    void setProperty(const std::string& property, Clonable* data) throw (Exception) 
    {
      if (!data)
        throw Exception("MafBlock::setProperty. Pointer to data is NULL.");
      if (hasProperty(property))
        deleteProperty(property);
      properties_[property] = data;
    }

  private:
    void deleteProperties_()
    {
      std::map<std::string, Clonable*>::const_iterator it;
      for (it = properties_.begin(); it != properties_.end(); ++it) {
        delete it->second;
      }
      properties_.clear();
    }
};

} // end of namespace bpp.

#endif //_MAFBLOCK_H_
