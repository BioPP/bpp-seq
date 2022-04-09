//
// File: AlignedSequenceContainer.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
//   Laurent Guéguen
// Last modified: 2003-08-22 00:00:00
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

#ifndef BPP_SEQ_CONTAINER_ALIGNEDSEQUENCECONTAINER_H
#define BPP_SEQ_CONTAINER_ALIGNEDSEQUENCECONTAINER_H

#include <Bpp/Exceptions.h>

#include "../Site.h"
#include "OrderedSequenceContainer.h"
#include "SiteContainer.h"
#include "VectorPositionedContainer.h"
#include "VectorSequenceContainer.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief The AlignedSequencesContainer class.
 *
 * This class inherits from the VectorSequenceContainer and add site access.
 * Sequence addition methods are re-defined to check for sequence lengths.
 * Sequence access is in \f$O(1)\f$, and site access in \f$O(n)\f$, where
 * \f$n\f$ is the number of sequences in the container.
 *
 * Sites are built on the fly when needed, otherwise they are only nullptr
 *
 * See VectorSiteContainer for an alternative implementation.
 *
 * @see VectorSequenceContainer, Sequence, Site, VectorSiteContainer
 */
template<class SequenceType, class SiteType>
class AlignedSequenceContainer :
  public VectorSequenceContainer<SequenceType>,
  public virtual SiteContainer<SequenceType, std::string, SiteType>,
  private VectorPositionedContainer<SiteType>
{
private:
  // // Integer std::vector that contains sites's positions
  std::vector<int> positions_;

  size_t length_; // Number of sites for verifications before sequence's insertion in sequence container

public:

  /**
   * @brief Build a container with pointers to sequence objects.
   *
   * The keys of the map are set to the names of the sequences.
   *
   * @param alphabet The alphabet of the container.
   * @param vs       A vector of smart pointers toward sequence objects.
   */
  AlignedSequenceContainer(
      const std::shared_ptr<const Alphabet> alphabet,
      std::vector< std::unique_ptr<Sequence> > vs):
    //AbstractSequenceContainer<SequenceType>(alphabet),
    VectorSequenceContainer<SequenceType>(alphabet, vs),
    VectorPositionedContainer<SiteType>(),
    positions_(),
    length_(0)
  {
    if (vs.size() == 0)
      return;
  
    length_ = vs[0]->size();
    for (const auto& seq: vs) {
      if (!checkSize_(*seq)) {
        throw BadSizeException("AlignedSequenceContainer: sequences of different sizes in aligned construction", length_, seq->size());
      }
    }
    VectorPositionedContainer<SiteType>::setSize(length_);
    reindexSites();
  }
  
  /**
   * @brief Build a new empty container with the specified alphabet.
   *
   * @param alphabet The alphabet of the container.
   */
  AlignedSequenceContainer(const std::shared_ptr<const Alphabet> alphabet) :
    //AbstractSequenceContainer<SequenceType>(alphabet),
    VectorSequenceContainer<SequenceType>(alphabet),
    VectorPositionedContainer<SiteType>(),
    positions_(),
    length_(0)
  {
    reindexSites();
  }

  /**
   * @brief Copy constructor.
   *
   * @param asc The container to copy.
   */
  AlignedSequenceContainer(const AlignedSequenceContainer& asc) :
    //AbstractSequenceContainer<SequenceType>(asc),
    VectorSequenceContainer<SequenceType>(asc),
    VectorPositionedContainer<SiteType>(asc),
    positions_(asc.getSitePositions()),
    length_(asc.getNumberOfSites())
  {}

  /**
   * @brief Convert any SiteContainer object into a AlignedSequenceContainer object.
   *
   * @param sc The container to copy.
   */
  AlignedSequenceContainer(const SiteContainer& sc) :
    //AbstractSequenceContainer<SequenceType>(sc),
    VectorSequenceContainer<SequenceType>(sc),
    VectorPositionedContainer<SiteType>(sc.getNumberOfSites()), //@Laurent: shouldn't the content (sites) be copied here too? 
    positions_(sc.getSitePositions()),
    length_(sc.getNumberOfSites())
  {}

  /**
   * @brief Try to coerce a SequenceContainer object into an AlignedSequenceContainer object.
   *
   * Sequences in osc will be considered alligned, and have the same number of sites.
   *
   * @param sc The ordered container to coerce.
   * @throw SequenceNotAlignedException If sequences in sc do not have the same length.
   */
  AlignedSequenceContainer(const SequenceContainer& sc) :
    //AbstractSequenceContainer<SequenceType>(sc.getAlphabet()),
    VectorSequenceContainer<SequenceType>(sc.getAlphabet()),
    VectorPositionedContainer<SiteType>(),
    // We can't call the copy constructor because we want to use the overloaded addSequence method.
    positions_(),
    length_()
  {
    // Initializing
    for (size_t i = 0; i < osc.getNumberOfSequences(); i++) {
      addSequence(sc.getSequence(i), true);
    }
    if (sc.getNumberOfSequences() > 0) {
      length_ = getSequence(0).size();
    } else {
      length_ = 0;
    }
    reindexSites();
    VectorPositionedContainer<SiteType>::setSize(length_);
    setComments(osc.getComments());
  }

  AlignedSequenceContainer& operator=(const AlignedSequenceContainer& asc)
  {
    VectorSequenceContainer<SequenceType>::operator=(asc);
    VectorPositionedContainer<SiteType>::operator=(asc);

    // Initializing
    length_    = asc.getNumberOfSites();
    positions_ = asc.getSitePositions();

    return *this;
  }

  AlignedSequenceContainer& operator=(const SiteContainer&  sc)
  {
    VectorSequenceContainer<SequenceType>::operator=(sc);

    // Initializing
    length_    = sc.getNumberOfSites();
    positions_ = sc.getSitePositions();
    VectorPositionedContainer<SiteType>::setSize(length_);

    return *this;
  }

  AlignedSequenceContainer& operator=(const SequenceContainer& sc)
  {
    VectorSequenceContainer<SequenceType>::operator=(sc);

    // Initializing
    length_ = 0;
    reindexSites();
    VectorPositionedContainer<SiteType>::setSize(length_);

    return *this;
  }

  virtual ~AlignedSequenceContainer() {}

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  AlignedSequenceContainer* clone() const { return new AlignedSequenceContainer(*this); }
  /** @} */

  /**
   * @name The SiteContainer interface implementation:
   *
   * @{
   */
  virtual const Site& getSite(size_t siteIndex) const
  {
    if (VectorPositionedContainer<Site>::hasSiteWithPosition(siteIndex)) {
      return *VectorPositionedContainer<Site>::getObject(siteIndex);
    }

    if (siteIndex >= length_)
      throw IndexOutOfBoundsException("AlignedSequenceContainer::getSite", siteIndex, 0, getNumberOfSites());

    // Main loop: for all sequences
    size_t n = getNumberOfSequences();
    std::shared_ptr<SiteType> site(new SiteType(getAlphabet(), static_cast<int>(siteIndex + 1)));
    for (size_t j = 0; j < n; j++)
      site->addElement(getSequence(j)[siteIndex]);

    VectorPositionedContainer<SiteType>::addObject_(site, siteIndex, true);
    return *VectorPositionedContainer<SiteType>::getObject(siteIndex);
  }

//TODO: check that all sites derive from a CruxSymbolListSite object, and mention in the doc that a derived class should be used for the template to work
  const CruxSymbolListSite& getSymbolListSite(size_t siteIndex) const
  {
    return getSite(siteIndex);
  }

  void setSite(size_t siteIndex, const SiteType& site, bool checkPosition = true);
  std::shared_ptr<Site> removeSite(size_t siteIndex);
  void deleteSite(size_t siteIndex);
  void addSite(const Site& site, bool checkPosition = true);
  void addSite(const Site& site, int position, bool checkPosition = true);
  void addSite(const Site& site, size_t siteIndex, bool checkPosition = true);
  void addSite(const Site& site, size_t siteIndex, int position, bool checkPosition = true);

  void clear();

  AlignedSequenceContainer* createEmptyContainer() const;

  /** @} */

  /**
   * @name From AlignedValuesContainer interface
   *
   * @{
   */

  void deleteSites(size_t siteIndex, size_t length);

  size_t getNumberOfSites() const { return length_; }
  Vint getSitePositions() const { return positions_; }
  void setSitePositions(Vint vPositions);

  void reindexSites();

  /** @} */

  /**
   * @name Redefinition of VectorSequenceContainer methods, to check for sequence lengths.
   *
   * @{
   */
  void setSequence(const std::string& name, const Sequence& sequence, bool checkName = true);
  void setSequence(size_t sequenceIndex, const Sequence& sequence, bool checkName = true);

  void addSequence(const Sequence& sequence, bool checkName = true);
  void addSequence(const Sequence& sequence, size_t sequenceIndex, bool checkName = true);

/** @} */

protected:
  /**
   * @brief Check sequence's size before insertion in sequence container.
   *
   * @param sequence The sequence to check.
   * @return True if sequence length = number of sites in container.
   */
  bool checkSize_(const Sequence& sequence) { return sequence.size() == length_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_ALIGNEDSEQUENCECONTAINER_H
