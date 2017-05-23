//
// File: VectorIntSiteContainer.h
// Created by: Laurent Gueguen
// Created on: vendredi 31 mars 2017, à 15h 49
//

/*
   Copyright or © or Copr. CNRS, (November 17, 2004)

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

#ifndef _VECTOR_SYMBOLLIST_SITECONTAINER_H_
#define _VECTOR_SYMBOLLIST_SITECONTAINER_H_

#include "VectorPositionedContainer.h"
#include "VectorMappedContainer.h"
#include "AbstractSequenceContainer.h"

#include "../Site.h"
#include "../Sequence.h"

// From the STL :
#include <vector>

namespace bpp
{

/**
 * @brief The VectorIntSiteContainer class.
 *
 * Sites are stored in a std::vector of pointers.
 * Site access is hence in \f$O(1)\f$, and sequence
 * access in \f$O(l)\f$, where \f$l\f$ is the number of sites in the
 * container.
 *
 * This is a modified copy of VectorSiteContainer with the minimum
 * changes necessary for it to work with bppML and bppAncestor
 *
 * @see Sequence, Site, VectorSiteContainer
 */
  
class VectorIntSiteContainer :
    public VectorPositionedContainer<Site>,
    public VectorMappedContainer<Sequence>,
    public virtual AbstractSequenceContainer
{
public :

  /**
   * @brief Build a new emtpy container.
   *
   */
  
  VectorIntSiteContainer(const Alphabet * alpha);

  /**
   * @name The Clonable interface.
   *
   * @{
   */

  VectorIntSiteContainer(const VectorIntSiteContainer& vpsc) :
    AbstractSequenceContainer(vpsc),
    VectorPositionedContainer<Site>(vpsc),
    VectorMappedContainer<Sequence>(vpsc)
  {
  }

  VectorIntSiteContainer& operator=(const VectorIntSiteContainer& vpsc)
  {
    VectorPositionedContainer<Site>::operator=(vpsc);
    VectorMappedContainer<Sequence>::operator=(vpsc);
    AbstractSequenceContainer::operator=(vpsc);
    
    return *this;
  }
  
  VectorIntSiteContainer* clone() const { return new VectorIntSiteContainer(*this); }

  /**
   * @}
   */

  // class destructor
  virtual ~VectorIntSiteContainer() {}
  
 public :

  size_t getNumberOfSites() const { return VectorPositionedContainer<Site>::getSize(); }
  
  size_t getNumberOfSequences() const { return VectorMappedContainer<Sequence>::getSize(); }

  
  /*
   * @brief get Objects
   *
   * @{
   */
  
  const std::shared_ptr<Site> getSite(size_t i) const
  {
    if(i >= getNumberOfSites())
      throw IndexOutOfBoundsException("VectorIntSiteContainer::getSite.", i, 0, getNumberOfSites() - 1);

    return VectorPositionedContainer<Site>::getObject(i);
  }

  std::shared_ptr<Site> getSite(size_t i)
  {
    if(i >= getNumberOfSites())
      throw IndexOutOfBoundsException("VectorIntSiteContainer::getSite.", i, 0, getNumberOfSites() - 1);

    return VectorPositionedContainer<Site>::getObject(i);
  }

  const Sequence& getSequence(std::size_t i) const
  {
    return *VectorMappedContainer<Sequence>::getObject(i);
  }

  const Sequence& getSequence(const std::string& name) const
  {
    return *VectorMappedContainer<Sequence>::getObject(name);
  }

  Sequence& getSequence(const std::string& name)
  {
    return *VectorMappedContainer<Sequence>::getObject(name);
  }

  Sequence& getSequence(std::size_t i)
  {
    return *VectorMappedContainer<Sequence>::getObject(i);
  }

  void setSequence(const std::string& name, const Sequence& sequence, bool checkName) throw (Exception)
  {
    if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", getAlphabet(), sequence.getAlphabet());

    VectorMappedContainer<Sequence>::addObject(std::shared_ptr<Sequence>(sequence.clone()), getObjectPosition(sequence.getName()), sequence.getName(), checkName);
  }
  
  bool hasSequence(const std::string& name) const
  {
    return hasObject(name);
  }

  Sequence* removeSequence(const std::string& name)
  {
    return VectorMappedContainer<Sequence>::removeObject(name).get();
  }

  Sequence* removeSequence(size_t sequenceIndex)
  {
    return VectorMappedContainer<Sequence>::removeObject(sequenceIndex).get();
  }

  void setComments(size_t sequenceIndex, const Comments& comments)
  {
    getSequence(sequenceIndex).setComments(comments);
  }    

  size_t getSequencePosition(const std::string& name) const throw (SequenceNotFoundException)
  {
    return getObjectPosition(name);
  }
    
  void  setSequence(size_t sequenceIndex, const Sequence& sequence, bool checkName = true) throw (Exception)
  {
    if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", getAlphabet(), sequence.getAlphabet());

    VectorMappedContainer<Sequence>::addObject(std::shared_ptr<Sequence>(sequence.clone()), sequenceIndex, sequence.getName(), checkName);
  }

  std::vector<std::string> getSequencesNames() const
  {
    return getObjectsNames();
  }

  void setSequencesNames(const std::vector<std::string> & names, bool checkNames) throw (Exception)
  {
    for (size_t i=0; i<getNumberOfSequences(); i++)
      getSequence(i).setName(names[i]);
    
    setObjectsNames(names);
  }

  SequenceContainer* createEmptyContainer() const
  {
    return new VectorIntSiteContainer(getAlphabet());
  }
  

  /*
   * @}
   *
   */

  /*
   * @brief Add elements
   *
   */
  
  void addSite(std::shared_ptr<Site> site, bool checkPosition = false);

  void addSequence(const Sequence& sequence, bool checkName = true);

  void addSequence(std::shared_ptr<Sequence> sequence, bool checkName = true);

  void clear();

  void reindexSites();

  /*
   *
   * Members Access
   */

      
  int& valueAt(const std::string& sequenceName, size_t elementIndex) throw (SequenceNotFoundException, IndexOutOfBoundsException)
  {
    return getSequence(sequenceName)[elementIndex];
  }

  const int& valueAt(const std::string& sequenceName, size_t elementIndex) const throw (SequenceNotFoundException, IndexOutOfBoundsException)
  {
    return getSequence(sequenceName)[elementIndex]; 
  }

  int& operator()(const std::string& sequenceName, size_t elementIndex)
  {
    return getSequence(sequenceName)[elementIndex];
  }

  const int& operator()(const std::string& sequenceName, size_t elementIndex) const
  {
    return getSequence(sequenceName)[elementIndex]; 
  }
    
  int& valueAt(size_t sequenceIndex, size_t elementIndex) throw (IndexOutOfBoundsException)
  {
    return getSequence(sequenceIndex)[elementIndex];
  }

  const int& valueAt(size_t sequenceIndex, size_t elementIndex) const throw (IndexOutOfBoundsException)
  {
    return getSequence(sequenceIndex)[elementIndex];
  }
    
  int& operator()(size_t sequenceIndex, size_t elementIndex)
  {
    return getSequence(sequenceIndex)[elementIndex];
  }
  
  const int& operator()(size_t sequenceIndex, size_t elementIndex) const
  {
    return getSequence(sequenceIndex)[elementIndex];
  } 

};

} // end of namespace bpp

#endif  // _VECTOR_SYMBOLLIST_SITECONTAINER_H_
