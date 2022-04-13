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
  // // Integer std::vector that contains sites's coordinates
  std::vector<int> coordinates_;

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
      const std::shared_ptr<const Alphabet>& alphabet,
      std::vector< std::unique_ptr<Sequence> > vs):
    //AbstractSequenceContainer<SequenceType>(alphabet),
    VectorSequenceContainer<SequenceType>(alphabet, vs),
    VectorPositionedContainer<SiteType>(),
    coordinates_(),
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
  AlignedSequenceContainer(const std::shared_ptr<const Alphabet>& alphabet) :
    //AbstractSequenceContainer<SequenceType>(alphabet),
    VectorSequenceContainer<SequenceType>(alphabet),
    VectorPositionedContainer<SiteType>(),
    coordinates_(),
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
    coordinates_(asc.getSiteCoordinates()),
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
  AlignedSequenceContainer(const SequenceContainer<SequenceType, std::string>& sc) :
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

  AlignedSequenceContainer& operator=(const SiteContainer<SiteType>&  sc)
  {
    VectorSequenceContainer<SequenceType>::operator=(sc);

    // Initializing
    length_    = sc.getNumberOfSites();
    positions_ = sc.getSitePositions();
    VectorPositionedContainer<SiteType>::setSize(length_);

    return *this;
  }

  AlignedSequenceContainer& operator=(const SequenceContainer>sequenceType, std::string>& sc)
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
  virtual const SiteType& getSite(size_t sitePosition) const
  {
    if (VectorPositionedContainer<Site>::hasSiteWithPosition(sitePosition)) {
      return *VectorPositionedContainer<Site>::getObject(sitePosition);
    }

    if (sitePosition >= length_)
      throw IndexOutOfBoundsException("AlignedSequenceContainer::getSite", sitePosition, 0, getNumberOfSites());

    // Main loop: for all sequences
    size_t n = getNumberOfSequences();
    std::shared_ptr<SiteType> site(new SiteType(getAlphabet(), static_cast<int>(sitePosition + 1)));
    for (size_t j = 0; j < n; j++)
      site->addElement(getSequence(j)[sitePosition]);

    VectorPositionedContainer<SiteType>::addObject_(site, sitePosition, true);
    return *VectorPositionedContainer<SiteType>::getObject(sitePosition);
  }

//TODO: check that all sites derive from a CruxSymbolListSite object, and mention in the doc that a derived class should be used for the template to work
  const CruxSymbolListSite& getSymbolListSite(size_t sitePosition) const
  {
    return getSite(sitePosition);
  }

  
  void setSite(size_t sitePosition, const SiteType& site, bool checkPosition = true)
  {
    // New site's alphabet and site container's alphabet matching verification
    if (pos >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::setSite", pos, 0, getNumberOfSites());
    if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("AlignedSequenceContainer::setSite", getAlphabet(), site.getAlphabet());

    // Check size:
    if (site.size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::setSite, site does not have the appropriate length", &site);

    // Check position:
    if (checkPositions) {
      int position = site.getPosition();
      for (auto poss : positions_)
      {
        if (poss == position)
          throw SiteException("AlignedSequenceContainer::setSite: Site position already exists in container", &site);
      }
    }
  
    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); j++)
      getSequence_(j).setElement(pos, site[j]);

    positions_[pos] = site.getPosition();
  }

	  
  std::shared_ptr<Site> removeSite(size_t sitePosition)
  {
    if (pos >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::removeSite", pos, 0, getNumberOfSites());

    // Get old site
    getSite(pos); // Creates the site if it does not exist!

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); j++)
    {
      getSequence_(j).deleteElement(pos);
    }

    // Delete site's position
    positions_.erase(positions_.begin() + static_cast<ptrdiff_t>(pos));
    length_--;

    // Actualizes the 'sites' vector:
    return VectorPositionedContainer<SiteType>::removeObject(pos);
  }


  void deleteSite(size_t sitePosition)
  {
    if (pos >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::deleteSite", pos, 0, getNumberOfSites());

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j) {
      getSequence_(j).deleteElement(pos);
    }

    // Delete site's position
    positions_.erase(positions_.begin() + static_cast<ptrdiff_t>(pos));
    length_--;

    // Actualizes the 'sites' vector:
    VectorPositionedContainer<SiteType>::deleteObject(pos);
  }


  void deleteSites(size_t sitePosition, size_t length)
  {
    if (sitePosition + length > getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::deleteSites", sitePosition + length, 0, getNumberOfSites());

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j) {
      getSequence_(j).deleteElements(sitePosition, length);
    }

    // Delete site's sitePositionition
    positions_.erase(positions_.begin() + static_cast<ptrdiff_t>(sitePosition),
                     positions_.begin() + static_cast<ptrdiff_t>(sitePosition + length));
    length_ -= length;

    // Actualizes the 'sites' vector:
    VectorPositionedContainer<SiteType>::deleteObjects(sitePosition, length);
  }


  void addSite(const SiteType& site, bool checkPosition = true)
  {
    // New site's alphabet and site container's alphabet matching verification
    if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("AlignedSequenceContainer::addSite");

    // Check size:
    if (site.size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::addSite, site does not have the appropriate length", &site);

    // Check position:
    int position = site.getPosition();
    if (checkPositions)
    {
      // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < positions_.size(); ++i) {
        if (positions_[i] == position)
          throw SiteException("AlignedSequenceContainer::addSite: Site position already exists in container", &site);
      }
    }

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j)
      getSequence_(j).addElement(site[j]);

    length_++;
    positions_.push_back(position);

    // Actualizes the 'sites' vector:
    VectorPositionedContainer<SiteType>::appendObject(nullptr);
  }


  void addSite(const SiteType& site, int position, bool checkPosition = true)
  {
    // New site's alphabet and site container's alphabet matching verification
    if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("AlignedSequenceContainer::addSite");

    // Check size:
    if (site.size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::addSite, site does not have the appropriate length", &site);

    // Check position:
    if (checkPositions) {
      // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < positions_.size(); ++i)
      {
        if (positions_[i] == position)
          throw SiteException("AlignedSequenceContainer::addSite: Site position already exists in container", &site);
      }
    }

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j) {
      getSequence_(j).addElement(site[j]);
    }

    length_++;
    positions_.push_back(position);

    // Actualizes the 'sites' vector:
    VectorPositionedContainer<SiteType>::appendObject(nullptr);
  }

	  
  void addSite(const SiteType& site, size_t sitePosition, bool checkPosition = true)
  {
    if (siteIndex >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::addSite", siteIndex, 0, getNumberOfSites());

    // New site's alphabet and site container's alphabet matching verification
    if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("AlignedSequenceContainer::addSite", getAlphabet(), site.getAlphabet());

    // Check size:
    if (site.size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::addSite, site does not have the appropriate length", &site);

    // Check position:
    int position = site.getPosition();
    if (checkPositions) {
    // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < positions_.size(); ++i) {
        if (positions_[i] == position)
          throw SiteException("AlignedSequenceContainer::addSite: Site position already exists in container", &site);
      }
    }

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j)
      getSequence_(j).addElement(siteIndex, site[j]);

    length_++;
    positions_.insert(positions_.begin() + static_cast<ptrdiff_t>(siteIndex), position);

    // Actualizes the 'sites' vector:
    VectorPositionedContainer<SiteType>::insertObject(nullptr, siteIndex);
  }
  

  void addSite(const SiteType& site, size_t sitePosition, int position, bool checkPosition = true)
  {
    if (siteIndex >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::addSite", siteIndex, 0, getNumberOfSites() - 1);

    // New site's alphabet and site container's alphabet matching verification
    if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("AlignedSequenceContainer::addSite", getAlphabet(), site.getAlphabet());

    // Check size:
    if (site.size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::addSite, site does not have the appropriate length", &site);

    // Check position:
    if (checkPositions) {
      // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < positions_.size(); ++i) {
        if (positions_[i] == position)
          throw SiteException("AlignedSequenceContainer::addSite: Site position already exists in container", &site);
      }
    }

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j)
      getSequence_(j).addElement(siteIndex, site[j]);

    length_++;
    positions_.insert(positions_.begin() + static_cast<ptrdiff_t>(siteIndex), position);

    // Actualizes the 'sites' vector:
    VectorPositionedContainer<SiteType>::insertObject(0, siteIndex);
  }


  void clear()
  {
    length_ = 0;
    VectorSequenceContainer::clear();
    VectorPositionedContainer<SiteType>::clear();
  }

  
  AlignedSequenceContainer* createEmptyContainer() const
  {
    AlignedSequenceContainer* asc = new AlignedSequenceContainer(getAlphabet());
    asc->setComments(getComments());
    return asc;
  }


  size_t getNumberOfSites() const { return length_; }

  Vint getSitePositions() const { return positions_; }

  void setSitePositions(Vint vPositions)
  {
    if (vPositions.size() != getNumberOfSites())
      throw BadSizeException("AlignedSequenceContainer::setSitePositions bad size of positions vector", vPositions.size(), getNumberOfSites());

    for (size_t i = 0; i < vPositions.size(); ++i) {
      positions_[i] = vPositions[i];
    }
  }



  void reindexSites()
  {
    positions_.resize(length_);
    for (size_t i = 0; i < length_; ++i) {
      positions_[i] = static_cast<int>(i + 1); // starts with 1.
    }
  }

  /** @} */

  /**
   * @name Redefinition of VectorSequenceContainer methods, to check for sequence lengths.
   *
   * @{
   */
  void setSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequence, bool updateKey = false) override
  {
    // if container has only one sequence
    if (getNumberOfSequences() == 1)
      length_ = sequence->size();
    if (checkSize_(*sequence))
      VectorSequenceContainer::setSequence(sequenceKey, sequence, updateKey);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::setSequence", sequence.get());
  }

  
  void setSequence(size_t sequencePosition, std::uniue_ptr<SequenceType>& sequence, bool updateKey = false) override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::setSequence", sequencePosition, 0, getNumberOfSequences());
    // if container has only one sequence
    if (getNumberOfSequences() == 1)
      length_ = sequence->size();
    if (checkSize_(*sequence))
      VectorSequenceContainer::setSequence(sequencePosition, sequence, updateKey);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::setSequence", sequence.get());

    // Detroys all sites (but keep Site Container at same size)
    VectorPositionedContainer<SiteType>::nullify();
  }


  void addSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequence) override
  {
    // if container has only one sequence
    if (length_ == 0) {
      length_ = sequence.size();
      VectorPositionedContainer<SiteType>::setSize(length_);
      reindexSites();
    }
    if (checkSize_(sequence))
      VectorSequenceContainer::addSequence(sequenceKey, sequence);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::addSequence", sequence.get());
  }


  void addSequence(std::unique_ptr<SequenceType>& sequence) override
  {
    // if container has only one sequence
    if (length_ == 0) {
      length_ = sequence.size();
      VectorPositionedContainer<SiteType>::setSize(length_);
      reindexSites();
    }
    if (checkSize_(sequence))
      VectorSequenceContainer::addSequence(sequence);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::addSequence", sequence.get());
  }


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
