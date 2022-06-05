//
// File: VectorSiteContainer.h
// Authors:
//   Julien Dutheil
// Created: 2003-10-06 11:50:40
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_CONTAINER_VECTORSITECONTAINER_H
#define BPP_SEQ_CONTAINER_VECTORSITECONTAINER_H

#include <Bpp/Numeric/VectorTools.h>

#include "SequenceContainer.h"
#include "AbstractSequenceContainer.h"
#include "SiteContainer.h"
#include "VectorPositionedContainer.h"
#include "VectorMappedContainer.h"

// From the STL library:
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <memory>

namespace bpp
{
/**
 * @brief The VectorSiteContainer class.
 *
 * Sites are stored in a std::vector of pointers.
 * Site access is hence in \f$O(1)\f$, and sequence access in \f$O(l)\f$, where
 * \f$l\f$ is the number of sites in the container.
 *
 * Sequences are built & stored on the fly, with a cache for time
 * efficiency.
 *
 * See VectorSequenceContainer for an alternative implementation.
 *
 * @see Sequence, Site, AlignedSequenceContainer
 *
 */
template<class SiteType, class SequenceType>  
class VectorSiteContainer :
  public AbstractSequenceContainer<SequenceType, std::string>,
  public virtual SiteContainer<SiteType, SequenceType, std::string>,
  private VectorPositionedContainer<SiteType>,
  private VectorMappedContainer<SequenceType>
{
protected:
  std::vector<std::string> sequenceNames_;
  std::vector< std::shared_ptr<Comments> > sequenceComments_;

public:

  /**
   * @brief Build a new container from a set of sites.
   *
   * @param vs A std::vector of sites.
   * @param alphabet The common alphabet for all sites.
   * @param checkPositions Check for the redundancy of site position tag. This may turn to be very time consuming!
   * @throw Exception If sites differ in size or in alphabet.
   */
  VectorSiteContainer(
      const std::vector<std::unique_ptr<SiteType> >& vs,
      std::shared_ptr<const Alphabet>& alphabet,
      bool checkPositions = true) :
    VectorPositionedContainer<SiteType>(),
    VectorMappedContainer<SequenceType>(),
    AbstractSequenceContainer<SequenceType>(alphabet),
    sequenceNames_(),
    sequenceComments_()
  {
    if (vs.size() == 0)
      throw Exception("VectorSiteContainer::VectorSiteContainer. Empty site set.");

    size_t nbSeq = vs[0]->size();
    for (size_t i = 0; i < nbSeq; ++i) {
      sequenceNames_.push_back("Seq_" + TextTools::toString(i));
      sequenceComments_.push_back(nullptr);
      VectorMappedContainer<SequenceType>::appendObject(nullptr, "Seq_" + TextTools::toString(i));
    }

    for (size_t i = 0; i < vs.size(); ++i)
      addSite(vs[i], checkPositions); // This may throw an exception if position argument already exists or is size is not valid.
  }



  /**
   * @brief Build a new empty container with specified size.
   *
   * @param size     Number of sequences in the container.
   * @param alphabet The alphabet for this container.
   */
  VectorSiteContainer(
      size_t size,
      std::shared_ptr<const Alphabet>& alphabet) :
    VectorPositionedContainer<SiteType>(),
    VectorMappedContainer<SequenceType>(),
    AbstractSequenceContainer<SequenceType>(alphabet),
    sequenceNames_(),
    sequenceComments_()
  {
    for (size_t i = 0; i < size; ++i) {
      sequenceNames_.push_back("Seq_" + TextTools::toString(i));
      sequenceComments_.push_back(nullptr);
      VectorMappedContainer<SequenceType>::appendObject(nullptr, "Seq_" + TextTools::toString(i));
    }
  }



  /**
   * @brief Build a new empty container with specified sequence keys.
   *
   * @param sequenceKeys Sequence keys. This will set the number of sequences in the container.
   * @param alphabet The alphabet for this container.
   */
  VectorSiteContainer(
      const std::vector<std::string>& sequenceKeys,
      std::shared_ptr<const Alphabet>& alphabet) :
    AbstractSequenceContainer<SequenceType>(alphabet),
    VectorPositionedContainer<SiteType>(),
    VectorMappedContainer<SequenceType>(),
    sequenceNames_(),
    sequenceComments_()
  {
    unsigned int i = 0;
    for (auto key : sequenceKeys) {
      ++i;
      sequenceNames_.push_back("Seq_" + TextTools::toString(i));
      sequenceComments_.push_back(nullptr);
      VectorMappedContainer<SequenceType>::appendObject(nullptr, key);
    }
  }  


  /**
   * @brief Build a new empty container.
   *
   * @param alphabet The alphabet for this container.
   */
  VectorSiteContainer(std::shared_ptr<const Alphabet>& alphabet) :
    AbstractSequenceContainer<SequenceType>(alphabet),
    VectorPositionedContainer<SiteType>(),
    VectorMappedContainer<SequenceType>()
  {}


  VectorSiteContainer(const VectorSiteContainer<SequenceType, SiteType>& vsc) :
    AbstractSequenceContainer<SequenceType>(vsc),
    VectorPositionedContainer<SiteType>(),
    VectorMappedContainer<SequenceType>(vsc),
    sequenceNames_(),
    sequenceComments_()
  {
    for (size_t i = 0; i < vsc.getNumberOfSites(); ++i)
      addSite(vsc.getSite(i), false); // We assume that positions are already correct.
  } 


  VectorSiteContainer(const SiteContainer<SiteType, SequenceType, std::string>&  sc) :
    AbstractSequenceContainer<SequenceType>(sc),
    VectorPositionedContainer<SiteType>(),
    VectorMappedContainer<SequenceType>(),
    sequenceNames_(),
    sequenceComments_()
  {
    for (size_t i = 0; i < sc.getNumberOfSites(); ++i) {
      std::unique_ptr<SiteType> sitePtr(sc.getSite(i).clone());
      addSite(sitePtr, false); // We assume that positions are already correct.
    }

    unsigned int i = 0;
    for (auto sequenceKey : sc.getSequenceKeys()) {
      ++i;
      sequenceNames_.push_back("Seq_" + TextTools::toString(i));
      sequenceComments_.push_back(nullptr);
      VectorMappedContainer<SequenceType>::appendObject(nullptr, sequenceKey);
    }
  }

  VectorSiteContainer(const SequenceContainer<SequenceType, std::string>& sc) :
    AbstractSequenceContainer<SequenceType>(sc),
    VectorPositionedContainer<SiteType>(),
    VectorMappedContainer<SequenceType>()
  {
    for (auto sequenceKey: sc.getSequenceKeys())
      addSequence(sc.getSequence(sequenceKey), false);

    reindexSites();
  }

  VectorSiteContainer& operator=(const VectorSiteContainer<SiteType, SequenceType>& vsc)
  {
    clear();
    AbstractSequenceContainer<SequenceType>::operator=(vsc);
    VectorMappedContainer<SequenceType>::operator=(vsc);
    sequenceNames_ = vsc.sequenceNames_;
    for (size_t i = 0; i < vsc.getNumberOfSequences(); ++i)
      sequenceComments_.push_back(std::shared_ptr<Comments>(vsc.getSequence(i).getComment()->clone())); 
  
    for (size_t i = 0; i < vsc.getNumberOfSites(); ++i)
      addSite(vsc.getSite(i), false); // We assume that positions are already correct.

    return *this;
  }


  VectorSiteContainer& operator=(const SiteContainer<SiteType, SequenceType, std::string>&  sc)
  {
    clear();
    AbstractSequenceContainer<SequenceType>::operator=(sc);
    sequenceNames_ = sc.sequenceNames_;
    for (size_t i = 0; i < sc.getNumberOfSequences(); ++i)
      sequenceComments_.push_back(std::shared_ptr<Comments>(sc.getSequence(i).getComment()->clone())); 
  
    for (size_t i = 0; i < sc.getNumberOfSites(); ++i)
      addSite(sc.getSite(i), false); // We assume that positions are already correct.

    for (auto sequenceKey : sc.getSequenceKeys())
      VectorMappedContainer<Sequence>::appendObject(nullptr, sequenceKey);
  
    return *this;
  }


  VectorSiteContainer& operator=(const SequenceContainer<SequenceType, std::string>&  sc)
  {
    clear();
    AbstractSequenceContainer<SequenceType>::operator=(sc);

    for (auto sequenceKey: sc.getSequenceKeys())
      addSequence(sc.getSequence(sequenceKey), false);

    reindexSites();

    return *this;
  }


  virtual ~VectorSiteContainer() { clear(); }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  VectorSiteContainer* clone() const override { return new VectorSiteContainer(*this); }
  /** @} */

  /**
   * @name The SiteContainer interface implementation:
   *
   * @{
   */
  const SiteType& getSite(size_t sitePosition) const override
  {
    return *VectorPositionedContainer<SiteType>::getObject(sitePosition);
  }

  SiteType& getSite(size_t sitePosition)
  {
    return *VectorPositionedContainer<SiteType>::getObject(sitePosition);
  }

  void setSite(size_t sitePosition, std::unique_ptr<SiteType>& site, bool checkCoordinate = true) override
  {
    if (sitePosition >= getNumberOfSites())
      throw IndexOutOfBoundsException("VectorSiteContainer::setSite.", sitePosition, 0, getNumberOfSites() - 1);

    // Check size:
    if (site->size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::setSite. Site does not have the appropriate length", std::move(site));

    // New site's alphabet and site container's alphabet matching verification
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::setSite", getAlphabet(), site->getAlphabet());

    // Check coordinate:
    if (checkCoordinate) {
      int coordinate = site->getCoordinate();
      // For all coordinates in vector: throw exception if position already exists
      for (size_t i = 0; i < getNumberOfSites(); ++i) {
        if (i != sitePosition && getSite(i).getCoordinate() == coordinate)
          throw SiteException("VectorSiteContainer::setSite: Site position already exists in container", std::move(site));
      }
    }

    std::shared_ptr<SiteType> sitePtr(site.release(), SwitchDeleter<SiteType>());
    VectorPositionedContainer<SiteType>::addObject(sitePtr, sitePosition, false);

    // Clean Sequence Container cache
    VectorMappedContainer<SequenceType>::nullify();
  }

  std::unique_ptr<SiteType> removeSite(size_t sitePosition) override
  {
    auto sitePtr = VectorPositionedContainer<SiteType>::removeObject(sitePosition);
    std::get_deleter< SwitchDeleter<SiteType> >(sitePtr)->off();
    return std::unique_ptr<SiteType>(sitePtr.get());
  }

  void deleteSite(size_t sitePosition) override
  {
    VectorPositionedContainer<SiteType>::deleteObject(sitePosition);
  }



  void addSite(std::unique_ptr<SiteType>& site, bool checkCoordinate = true) override
  {
    // Check size:
    if (getNumberOfSequences() != 0 && (site->size() != getNumberOfSequences()))
      throw SiteException("VectorSiteContainer::addSite. Site does not have the appropriate length", std::move(site));

    // New site's alphabet and site container's alphabet matching verification
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::addSite", getAlphabet(), site->getAlphabet());

    // Check coordinate:
    if (checkCoordinate) {
      int coordinate = site->getCoordinate();
      // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < getNumberOfSites(); ++i) {
        if (getSite(i).getCoordinate() == coordinate)
          throw SiteException("VectorSiteContainer::addSite(site, bool): Site position already exists in container", std::move(site));
      }
    }

    std::shared_ptr<SiteType> sitePtr(site.release(), SwitchDeleter<SiteType>());
    VectorPositionedContainer<SiteType>::appendObject(sitePtr);

    if (getNumberOfSequences() == 0) {
      for (size_t i = 0; i < site->size(); ++i) 
        VectorMappedContainer<SequenceType>::appendObject(nullptr, "Seq_" + TextTools::toString(i));
    } else {
      VectorMappedContainer<SequenceType>::nullify();
    }
  }


  void addSite(std::unique_ptr<SiteType>& site, size_t sitePosition, bool checkCoordinate = true) override
  {
    if (sitePosition >= getNumberOfSites())
      throw IndexOutOfBoundsException("VectorSiteContainer::addSite", sitePosition, 0, getNumberOfSites() - 1);

    // Check size:
    if (site->size() != getNumberOfSequences())
      throw SiteException("VectorSiteContainer::addSite. Site does not have the appropriate length", std::move(site));

    // New site's alphabet and site container's alphabet matching verification
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::addSite", getAlphabet(), site->getAlphabet());

    // Check coordinate:
    if (checkCoordinate) {
      int coordinate = site->getCoordinate();
      // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < getNumberOfSites(); i++) {
        if (i != sitePosition && getSite(i).getCoordinate() == coordinate)
          throw SiteException("VectorSiteContainer::addSite. Site coordinate already exists in container", std::move(site));
      }
    }

    std::shared_ptr<SiteType> sitePtr(site.release(), SwitchDeleter<SiteType>());
    VectorPositionedContainer<SiteType>::insertObject(sitePtr, sitePosition);

    if (getNumberOfSequences() == 0) {
      for (size_t i = 0; i < site->size(); i++)
        VectorMappedContainer<SequenceType>::appendObject(nullptr, "Seq_" + TextTools::toString(i));
    } else {
      VectorMappedContainer<SequenceType>::nullify();
    }
  }


  void deleteSites(size_t sitePosition, size_t length) override
  {
    VectorPositionedContainer<SiteType>::deleteObjects(sitePosition, length);
  }

  size_t getNumberOfSites() const override
  {
    return VectorPositionedContainer<SiteType>::getSize();
  }

  void reindexSites() override
  {
    for (size_t i = 0; i < getNumberOfSites(); ++i) {
      getSite(i).setCoordinate(static_cast<int>(i) + 1);
    }
  }

  Vint getSiteCoordinates() const override
  {
    Vint coordinates(getNumberOfSites());
    for (size_t i = 0; i < getNumberOfSites(); ++i) {
      coordinates[i] = getSite(i).getCoordinate();
    }
    return coordinates;
  }

  void setSiteCoordinates(const Vint& vCoordinates) override
  {
    if (vCoordinates.size() != getNumberOfSites())
      throw BadSizeException("VectorSiteContainer::setSiteCoordinates bad size of coordinates vector", vCoordinates.size(), getNumberOfSites());

    for (size_t i = 0; i < getNumberOfSites(); ++i) {
      getSite(i).setCoordinate(vCoordinates[i]);
    }
  }

  /** @} */


  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */

  bool hasSequence(const std::string& sequenceKey) const override
  {
    // Look for sequence key:
    return VectorMappedContainer<SequenceType>::hasObject(sequenceKey);
  }

  size_t getSequencePosition(const std::string& sequenceKey) const override
  {
    // Look for sequence key:
    return VectorMappedContainer<SequenceType>::getObjectPosition(sequenceKey);
  }

  const SequenceType& getSequence(const std::string& sequenceKey) const override
  {
    size_t pos = getSequencePosition(sequenceKey);
    return getSequence(pos);
  }

  const SequenceType& getSequence(size_t sequencePosition) const override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("VectorSiteContainer::getSequence.", sequencePosition, 0, getNumberOfSequences() - 1);

    // If Sequence already exsits
    auto name = VectorMappedContainer<SequenceType>::getObjectName(sequencePosition);
    if (!isAvailableName(name))
      return *VectorMappedContainer<SequenceType>::getObject(sequencePosition);

    // Main loop : for all sites
    size_t n = getNumberOfSites();
    std::vector<int> sequence(n);
    for (size_t j = 0; j < n; ++j)
      sequence[j] = getSite(j)[sequencePosition];

    std::shared_ptr<SequenceType> ns(new SequenceType(
          sequenceNames_[sequencePosition],
          sequence,
          sequenceComments_[sequencePosition],
          getAlphabet()));

    VectorMappedContainer<SequenceType>::addObject_(ns, sequencePosition, getSequenceKey(sequencePosition), false);

    return *ns;
  }

 
  std::unique_ptr<SequenceType> removeSequence(size_t sequencePosition) override
  {
    getSequence(sequencePosition); // this creates the sequence if it does not exist.

    for (size_t i = 0; i < getNumberOfSites(); ++i)
      getSite(i).deleteElement(sequencePosition);

    auto d = static_cast<std::vector<std::string>::difference_type>(sequencePosition);
    sequenceNames_.erase(std::next(sequenceNames_.begin(), d));
    sequenceComments_.erase(std::next(sequenceComments_.begin(), d));
  
    auto seq = VectorMappedContainer<SequenceType>::removeObject(sequencePosition);
    std::get_deleter< SwitchDeleter<SequenceType> >(seq)->off();
    std::unique_ptr<SequenceType> seq2(seq.get());
    return seq2;
  }

  std::unique_ptr<SequenceType> removeSequence(const std::string& sequenceKey) override
  {
    // Look for sequence key:
    size_t sequencePosition = getSequencePosition(sequenceKey);
    SequenceType* seq = removeSequence(sequencePosition).release();
    return std::unique_ptr<SequenceType>(seq);
  }

  void deleteSequence(size_t sequencePosition) override
  {
    for (size_t i = 0; i < getNumberOfSites(); ++i)
      getSite(i).deleteElement(sequencePosition);

    auto posN = static_cast<std::vector<std::string>::difference_type>(sequencePosition);
    sequenceNames_.erase(sequenceNames_.begin() + posN);
    auto posC = static_cast<std::vector<Comments>::difference_type>(sequencePosition);
    sequenceComments_.erase(sequenceComments_.begin() + posC);
  
    VectorMappedContainer<SequenceType>::deleteObject(sequencePosition);
  }

  void deleteSequence(const std::string& sequenceKey) override
  {
    // Look for sequence key:
    size_t sequencePosition = getSequencePosition(sequenceKey);
    deleteSequence(sequencePosition);
  }

  size_t getNumberOfSequences() const override
  {
    return VectorMappedContainer<SequenceType>::getNumberOfObjects();
  }

  std::vector<std::string> getSequenceKeys() const override
  {
    return VectorMappedContainer<SequenceType>::getObjectNames();
  }

  void setSequenceKeys(const std::vector<std::string>& sequenceKeys) override
  {
    VectorMappedContainer<SequenceType>::setObjectNames(sequenceKeys);
  }

  const std::string& getSequenceKey(size_t sequencePosition) const override
  { 
    return VectorMappedContainer<SequenceType>::getObjectName(sequencePosition);
  }

  std::vector<std::string> getSequenceNames() const override
  {
    return sequenceNames_;
  }

  void setSequenceNames(const std::vector<std::string>& names, bool updateKeys) override
  {
    if (names.size() != getNumberOfSequences())
      throw DimensionException("VectorSiteContainer::setSequenceNames : bad number of names", names.size(), getNumberOfSequences());
    VectorMappedContainer<SequenceType>::clear();
    sequenceNames_ = names;
    if (updateKeys) {
      setSequenceKeys(names);
    }
  }

  void clear() override
  {
    VectorPositionedContainer<SiteType>::clear();
    VectorMappedContainer<SequenceType>::clear();
    sequenceNames_.clear();
    sequenceComments_.clear();
  }

  VectorSiteContainer* createEmptyContainer() const override
  {
    auto alphaP = getAlphabet();
    VectorSiteContainer* vsc = new VectorSiteContainer<SiteType, SequenceType>(alphaP);
    vsc->setComments(getComments());
    return vsc;
  }

  int& valueAt(const std::string& sequenceKey, size_t elementIndex)
  {
    if (elementIndex >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(std::string, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<SiteType>::getObject(elementIndex))[getSequencePosition(sequenceKey)];
  }

  const int& valueAt(const std::string& sequenceKey, size_t elementIndex) const override
  {
    if (elementIndex >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(std::string, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<SiteType>::getObject(elementIndex))[getSequencePosition(sequenceKey)];
  }

  int& operator()(const std::string& sequenceKey, size_t elementIndex)
  {
    return (*VectorPositionedContainer<SiteType>::getObject(elementIndex))[getSequencePosition(sequenceKey)];
  }

  const int& operator()(const std::string& sequenceKey, size_t elementIndex) const override
  {
    return (*VectorPositionedContainer<SiteType>::getObject(elementIndex))[getSequencePosition(sequenceKey)];
  }

  int& valueAt(size_t sequencePosition, size_t elementIndex)
  {
    if (sequencePosition >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(size_t, size_t).", sequencePosition, 0, getNumberOfSequences() - 1);
    if (elementIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(size_t, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<SiteType>::getObject(elementIndex))[sequencePosition];
  }

  const int& valueAt(size_t sequencePosition, size_t elementIndex) const override
  {
    if (sequencePosition >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(size_t, size_t).", sequencePosition, 0, getNumberOfSequences() - 1);
    if (elementIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::valueAt(size_t, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<SiteType>::getObject(elementIndex))[sequencePosition];
  }

  int& operator()(size_t sequencePosition, size_t elementIndex)
  {
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[sequencePosition];
  }
  const int& operator()(size_t sequencePosition, size_t elementIndex) const override
  {
    return (*VectorPositionedContainer<Site>::getObject(elementIndex))[sequencePosition];
  }

  double getStateValueAt(size_t sitePosition, const std::string& sequenceKey, int state) const override
  {
    if (sitePosition  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::getStateValueAt.", sitePosition, 0, getNumberOfSites() - 1);

    return getAlphabet()->isResolvedIn(valueAt(sequenceKey, sitePosition), state) ? 1. : 0.;
  }

  double operator()(size_t sitePosition, const std::string& sequenceKey, int state) const override
  {
    return getAlphabet()->isResolvedIn(valueAt(sequenceKey, sitePosition), state) ? 1. : 0.;
  }

  /*
   * @brief get value of a state at a position
   * @param sitePosition  index of the site in the container
   * @param sequencePosition index of the looked value in the site
   * @param state  state in the alphabet
   */
  double getStateValueAt(size_t sitePosition, size_t sequencePosition, int state) const
  {
    if (sequencePosition >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::getStateValueAt.", sequencePosition, 0, getNumberOfSequences() - 1);
    if (sitePosition  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::getStateValueAt.", sitePosition, 0, getNumberOfSites() - 1);

    return getAlphabet()->isResolvedIn(valueAt(sequencePosition, sitePosition), state) ? 1. : 0.;
  }

  double operator()(size_t sitePosition, size_t sequencePosition, int state) const
  {
    return getAlphabet()->isResolvedIn(valueAt(sequencePosition, sitePosition), state) ? 1. : 0.;
  }


  void setSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequence) override
  {
    size_t pos = getSequencePosition(sequenceKey);
    setSequence(pos, sequence);
  }


  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequence) override
  {
    setSequence(sequencePosition, sequence, VectorMappedContainer<SequenceType>::getObjectName(sequencePosition));
  }


  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequence, const std::string& sequenceKey) override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("VectorSiteContainer::setSequence.", sequencePosition, 0, getNumberOfSequences() - 1);

    if (sequence->size() != getNumberOfSites())
      throw SequenceNotAlignedException("VectorSiteContainer::setSequence", sequence.get());

    // New sequence's alphabet and site container's alphabet matching verification
   if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::setSequence", getAlphabet(), sequence->getAlphabet());

    // Update elements at each site:
    for (size_t i = 0; i < getNumberOfSites(); i++)
      getSite(i).addElement(sequencePosition, sequence->getValue(i));

    VectorMappedContainer<SequenceType>::addObject(std::move(sequence), sequencePosition, sequenceKey);
  }


  void addSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequence)override
  {
    // If the container has no sequence, we set the size to the size of this sequence:
    if (getNumberOfSequences() == 0)
      realloc(sequence->size());

    // New sequence's alphabet and site container's alphabet matching verification
    if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::addSequence", getAlphabet(), sequence->getAlphabet());

    if (sequence->size() != getNumberOfSites())
      throw SequenceException("VectorSiteContainer::addSequence. Sequence has not the appropriate length: " + TextTools::toString(sequence->size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".", sequence.get());

    if (VectorMappedContainer<SequenceType>::hasObject(sequenceKey))
      throw SequenceException("VectorSiteContainer::addSequence. Name already exists in container.", sequence.get());

    // Update elements at each site:
    for (size_t i = 0; i < getNumberOfSites(); ++i)
      getSite(i).addElement(sequence->getValue(i));

    // Add sequence
    VectorMappedContainer<SequenceType>::appendObject(sequence, sequenceKey);
  }



  void insertSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequence, const std::string& sequenceKey) override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("VectorSiteContainer::insertSequence.", sequencePosition, 0, getNumberOfSequences() - 1);

    if (sequence->size() != getNumberOfSites())
      throw SequenceNotAlignedException("VectorSiteContainer::insertSequence", sequence.get());

    // New sequence's alphabet and site container's alphabet matching verification
   if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::insertSequence", getAlphabet(), sequence->getAlphabet());

    //if (checkNames && VectorMappedContainer<Sequence>::hasObject(sequenceKey))
    //  throw SequenceException("VectorSiteContainer::insertSequence. Name already exists in container.", &sequence);

    // Update elements at each site:
    for (size_t i = 0; i < getNumberOfSites(); ++i)
      getSite(i).addElement(sequencePosition, sequence->getValue(i));

    VectorMappedContainer<SequenceType>::insertObject(std::move(sequence), sequencePosition, sequenceKey);
  }

  //Needed because of the template class
  using AbstractSequenceContainer<SequenceType>::getAlphabet;
  using AbstractSequenceContainer<SequenceType>::getComments;
  /** @} */

protected:
  // Create n void sites:
  void realloc(size_t n)
  {
    clear();
    SiteType s(getAlphabet());

    for (size_t i = 0; i < n; ++i)
      addSite(std::unique_ptr<SiteType>(s.clone()), false);

    reindexSites();
  }

};

//Aliases:
using BasicVectorSiteContainer = VectorSiteContainer<Site, BasicSequence>;
using ProbabilisticVectorSiteContainer = VectorSiteContainer<ProbabilisticSite, ProbabilisticSequence>;

} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_VECTORSITECONTAINER_H
