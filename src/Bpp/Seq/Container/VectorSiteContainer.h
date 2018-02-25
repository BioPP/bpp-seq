//
// File: VectorSiteContainer.h
// Created by: Julien Dutheil
// Created on: Mon Oct  6 11:50:40 2003
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

#ifndef _VECTORSITECONTAINER_H_
#define _VECTORSITECONTAINER_H_

#include "../Site.h"
#include "SiteContainer.h"
#include "AbstractValuesContainer.h"
#include "AlignedSequenceContainer.h"
#include "OrderedSequenceContainer.h"
#include <Bpp/Numeric/VectorTools.h>

// From the STL library:
#include <string>
#include <vector>
#include <iostream>

namespace bpp
{
/**
 * @brief The VectorSiteContainer class.
 *
 * Sites are stored in a std::vector of pointers.
 * Site access is hence in \f$O(1)\f$, and sequence access in \f$O(l)\f$, where
 * \f$l\f$ is the number of sites in the container.
 *
 * See AlignedSequenceContainer for an alternative implementation.
 *
 * @see Sequence, Site, AlignedSequenceContainer
 */
class VectorSiteContainer :
  public AbstractSequenceContainer,
  // This container implements the SequenceContainer interface
  // and use the AbstractSequenceContainer adapter.
  public virtual SiteContainer,        // This container is a SiteContainer.
  public virtual VectorPositionedContainer<Site>,
  public virtual VectorMappedContainer<Sequence> 
{
public:
  /**
   * @brief Build a new container from a set of sites.
   *
   * @param vs A std::vector of sites.
   * @param alpha The common alphabet for all sites.
   * @param checkPositions Check for the redundancy of site position tag. This may turn to be very time consuming!
   * @throw Exception If sites differ in size or in alphabet.
   */

  VectorSiteContainer(const std::vector<const CruxSymbolListSite*>& vs, const Alphabet* alpha, bool checkPositions = true);
  VectorSiteContainer(const std::vector<const Site*>& vs, const Alphabet* alpha, bool checkPositions = true);
  /**
   * @brief Build a new empty container with specified size.
   *
   * @param size Number of sequences in the container.
   * @param alpha The alphabet for this container.
   */
  VectorSiteContainer(size_t size, const Alphabet* alpha);

  /**
   * @brief Build a new empty container with specified sequence names.
   *
   * @param names Sequence names. This will set the number of sequences in the container.
   * @param alpha The alphabet for this container.
   */
  VectorSiteContainer(const std::vector<std::string>& names, const Alphabet* alpha);

  /**
   * @brief Build a new empty container.
   *
   * @param alpha The alphabet for this container.
   */
  VectorSiteContainer(const Alphabet* alpha);

  VectorSiteContainer(const VectorSiteContainer& vsc);
  VectorSiteContainer(const SiteContainer&  sc);
  VectorSiteContainer(const OrderedSequenceContainer& osc);
  VectorSiteContainer(const SequenceContainer&  sc);
  
  VectorSiteContainer(const AlignedValuesContainer& avc);

  VectorSiteContainer& operator=(const VectorSiteContainer& vsc);
  VectorSiteContainer& operator=(const SiteContainer&  sc);
  VectorSiteContainer& operator=(const OrderedSequenceContainer& osc);
  VectorSiteContainer& operator=(const SequenceContainer&  sc);

  virtual ~VectorSiteContainer() { clear(); }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  VectorSiteContainer* clone() const { return new VectorSiteContainer(*this); }
  /** @} */

  /**
   * @name The SiteContainer interface implementation:
   *
   * @{
   */

  const Site& getSite(size_t siteIndex) const
  {
    return *VectorPositionedContainer<Site>::getObject(siteIndex);
  }

  Site& getSite(size_t siteIndex)
  {
    return *VectorPositionedContainer<Site>::getObject(siteIndex);
  }

  void setSite(size_t siteIndex, const Site& site, bool checkPosition = true);

  std::shared_ptr<Site> deleteSite(size_t siteIndex)
  {
    return VectorPositionedContainer<Site>::deleteObject(siteIndex);
  }

  void        addSite(const Site& site,                                 bool checkPosition = true);
  void        addSite(const Site& site,                   int position, bool checkPosition = true);
  void        addSite(const Site& site, size_t siteIndex,               bool checkPosition = true);
  void        addSite(const Site& site, size_t siteIndex, int position, bool checkPosition = true);

  /** @} */

  /*
   * @name From AlignedValuesContainer interface
   *
   * @{
   */

  void deleteSites(size_t siteIndex, size_t length)
  {
    VectorPositionedContainer<Site>::deleteObjects(siteIndex, length);
  }

  size_t getNumberOfSites() const
  {
    return VectorPositionedContainer<Site>::getSize();
  }
  
  void reindexSites();

  Vint getSitePositions() const;

  void setSitePositions(Vint vPositions);

  
  /** @} */

  // These methods are implemented for this class:

  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */
  void setComments(size_t sequenceIndex, const Comments& comments);

  // Method to get a sequence object from sequence container
  const Sequence& getSequence(size_t sequenceIndex) const;
  const Sequence& getSequence(const std::string& name) const;
  
  bool hasSequence(const std::string& name) const
  {
    // Look for sequence name:
    return VectorMappedContainer<Sequence>::hasObject(name);
  }

/******************************************************************************/

  // Methods to get position of a sequence in sequence container from his name
  // This method is used by delete and remove methods
  size_t getSequencePosition(const std::string& name) const
  {
    // Look for sequence name:
    return VectorMappedContainer<Sequence>::getObjectPosition(name);
  }

  Sequence* removeSequence(size_t sequenceIndex);
  Sequence* removeSequence(const std::string& name);

  size_t getNumberOfSequences() const
  {
    return VectorMappedContainer<Sequence>::getNumberOfObjects();
  }

  std::vector<std::string> getSequencesNames() const
  {
    return VectorMappedContainer<Sequence>::getObjectsNames();
  }
  
  void setSequencesNames(const std::vector<std::string>& names, bool checkNames = true)
  {
    VectorMappedContainer<Sequence>::setObjectsNames(names);
  }
  
  void clear();

  VectorSiteContainer* createEmptyContainer() const;

  int& valueAt(const std::string& sequenceName, size_t elementIndex)
  {
    if (elementIndex >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(std::string, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[getSequencePosition(sequenceName)];
  }
  
  const int& valueAt(const std::string& sequenceName, size_t elementIndex) const
  {
    if (elementIndex >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(std::string, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[getSequencePosition(sequenceName)];
  }
  
  int& operator()(const std::string& sequenceName, size_t elementIndex)
  {
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[getSequencePosition(sequenceName)];
  }

  const int& operator()(const std::string& sequenceName, size_t elementIndex) const
  {
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[getSequencePosition(sequenceName)];
  }

  int& valueAt(size_t sequenceIndex, size_t elementIndex)
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(size_t, size_t).", sequenceIndex, 0, getNumberOfSequences() - 1);
    if (elementIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(size_t, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[sequenceIndex];
  }

  const int& valueAt(size_t sequenceIndex, size_t elementIndex) const
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(size_t, size_t).", sequenceIndex, 0, getNumberOfSequences() - 1);
    if (elementIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(size_t, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[sequenceIndex];
  }
  
  int& operator()(size_t sequenceIndex, size_t elementIndex)
  {
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[sequenceIndex];
  }
  const int& operator()(size_t sequenceIndex, size_t elementIndex) const
  {
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[sequenceIndex];
  }

  /**
   * @name SequencedValuesContainer methods.
   *
   * @{
   */
  
  double getStateValueAt(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    if (siteIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::getStateValueAt.", siteIndex, 0, getNumberOfSites() - 1);
    
    return getAlphabet()->isResolvedIn(valueAt(sequenceName, siteIndex),state)?1.:0.;
  }
  
  double operator()(size_t siteIndex, const std::string& sequenceName, int state) const{
    return getAlphabet()->isResolvedIn(valueAt(sequenceName, siteIndex),state)?1.:0.;
  }

  /*
   *
   * @}
   *
   */
  
  /**
   * @name OrderedValuesContainer methods.
   *
   * @{
   */

  /*
   * @brief get value of a state at a position
   * @param siteIndex  index of the site in the container
   * @param sequenceIndex index of the looked value in the site
   * @param state  state in the alphabet
   */
  
  double getStateValueAt(size_t siteIndex, size_t sequenceIndex, int state) const 
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::getStateValueAt.", sequenceIndex, 0, getNumberOfSequences() - 1);
    if (siteIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::getStateValueAt.", siteIndex, 0, getNumberOfSites() - 1);

    return getAlphabet()->isResolvedIn(valueAt(sequenceIndex, siteIndex),state)?1.:0.;
  }
  
  double operator()(size_t siteIndex, size_t sequenceIndex, int state) const 
  {
    return getAlphabet()->isResolvedIn(valueAt(sequenceIndex, siteIndex),state)?1.:0.;
  }

  /*
   * @}
   *
   */

  /** @} */

  void addSequence(const Sequence& sequence, bool checkName = true);

  void addSequence(const Sequence& sequence, size_t sequenceIndex, bool checkName = true);

  void setSequence(const std::string& name,    const Sequence& sequence, bool checkName);
  void setSequence(size_t sequenceIndex, const Sequence& sequence, bool checkName);

protected:
  // Create n void sites:
  void realloc(size_t n);
};
} // end of namespace bpp.

#endif  // _VECTORSITECONTAINER_H_

