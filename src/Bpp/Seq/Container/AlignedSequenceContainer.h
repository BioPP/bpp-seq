// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_ALIGNEDSEQUENCECONTAINER_H
#define BPP_SEQ_CONTAINER_ALIGNEDSEQUENCECONTAINER_H

#include <Bpp/Exceptions.h>

#include "../Site.h"
#include "../SiteTools.h"
#include "../ProbabilisticSite.h"
#include "../Sequence.h"
#include "../ProbabilisticSequence.h"
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
class TemplateAlignedSequenceContainer :
  public TemplateVectorSequenceContainer<SequenceType>,
  public virtual TemplateSiteContainerInterface<SiteType, SequenceType, std::string>
{
private:
  VectorPositionedContainer<SiteType> siteVector_;

  // std::vector that contains the site coordinates
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
  TemplateAlignedSequenceContainer(
      std::shared_ptr<const Alphabet> alphabet,
      std::vector< std::unique_ptr<Sequence> > vs):
    TemplateVectorSequenceContainer<SequenceType>(alphabet, vs),
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
    siteVector_.setSize(length_);
    reindexSites();
  }
  
  /**
   * @brief Build a new empty container with the specified alphabet.
   *
   * @param alphabet The alphabet of the container.
   */
  TemplateAlignedSequenceContainer(std::shared_ptr<const Alphabet> alphabet) :
    //AbstractSequenceContainer<SequenceType>(alphabet),
    TemplateVectorSequenceContainer<SequenceType>(alphabet),
    siteVector_(),
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
  TemplateAlignedSequenceContainer(const TemplateAlignedSequenceContainer<SequenceType, SiteType>& asc) :
    TemplateVectorSequenceContainer<SequenceType>(asc),
    siteVector_(asc.getNumberOfSites()), //Cache is not copied
    coordinates_(asc.getSiteCoordinates()),
    length_(asc.getNumberOfSites())
  {}

  /**
   * @brief Convert any SiteContainer object into a AlignedSequenceContainer object.
   *
   * @param sc The container to copy.
   */
  TemplateAlignedSequenceContainer(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sc) :
    TemplateVectorSequenceContainer<SequenceType>(sc),
    siteVector_(sc.getNumberOfSites()), //Cache is not copied
    coordinates_(sc.getSiteCoordinates()),
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
  TemplateAlignedSequenceContainer(const TemplateSequenceContainerInterface<SequenceType, std::string>& sc) :
    TemplateVectorSequenceContainer<SequenceType>(sc.getAlphabet()),
    siteVector_(),
    coordinates_(),
    length_()
  {
    // Initializing
    for (size_t i = 0; i < sc.getNumberOfSequences(); ++i) {
      addSequence(sc.getSequence(i), true);
    }
    if (sc.getNumberOfSequences() > 0) {
      length_ = sequence(0).size();
    } else {
      length_ = 0;
    }
    reindexSites();
    siteVector_.setSize(length_);
    setComments(sc.getComments());
  }

  TemplateAlignedSequenceContainer& operator=(const TemplateAlignedSequenceContainer<SequenceType, SiteType>& asc)
  {
    TemplateVectorSequenceContainer<SequenceType>::operator=(asc);

    // Initializing
    length_      = asc.getNumberOfSites();
    coordinates_ = asc.getSiteCoordinates();
    siteVector_.setSize(length_); // Reset the cache
    siteVector_.nullify();

    return *this;
  }

  TemplateAlignedSequenceContainer<SequenceType, SiteType>& operator=(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>&  sc)
  {
    TemplateVectorSequenceContainer<SequenceType>::operator=(sc);

    // Initializing
    length_      = sc.getNumberOfSites();
    coordinates_ = sc.getSiteCoordinates();
    siteVector_.setSize(length_); // Reset the cache
    siteVector_.nullify();

    return *this;
  }

  TemplateAlignedSequenceContainer& operator=(const TemplateSequenceContainerInterface<SequenceType, std::string>& sc)
  {
    TemplateVectorSequenceContainer<SequenceType>::operator=(sc);

    // Initializing
    length_ = sc.getNumberOfSequences() == 0 ? 0 : sc.sequence(0).getSize();
    reindexSites();
    siteVector_.setSize(length_); // Reset the cache
    siteVector_.nullify();

    return *this;
  }

  virtual ~TemplateAlignedSequenceContainer() {}

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  TemplateAlignedSequenceContainer<SequenceType, SiteType>* clone() const override
  { 
    return new TemplateAlignedSequenceContainer<SequenceType, SiteType>(*this);
  }
  /** @} */

  /**
   * @name The SiteContainer interface implementation:
   *
   * @{
   */
  const SiteType& site(size_t sitePosition) const override
  {
    if (siteVector_.hasObjectWithPosition(sitePosition)) {
      return *siteVector_.getObject(sitePosition);
    }

    if (sitePosition >= length_)
      throw IndexOutOfBoundsException("AlignedSequenceContainer::getSite", sitePosition, 0, getNumberOfSites());

    // Main loop: for all sequences
    size_t n = getNumberOfSequences();
    auto alphaPtr = getAlphabet();
    auto site = std::shared_ptr<SiteType>(
		    new SiteType(alphaPtr, coordinates_[sitePosition]),
		    SwitchDeleter<SiteType>());
    
    for (size_t j = 0; j < n; j++)
      site->addElement(sequence(j)[sitePosition]);

    siteVector_.addObject_(site, sitePosition, true);
    return *siteVector_.getObject(sitePosition);
  }

  void setSite(size_t sitePosition, std::unique_ptr<SiteType>& site, bool checkCoordinate = true) override
  {
    // New site's alphabet and site container's alphabet matching verification
    if (sitePosition >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::setSite", sitePosition, 0, getNumberOfSites());
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("AlignedSequenceContainer::setSite", getAlphabet(), site->getAlphabet());

    // Check size:
    if (site->size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::setSite, site does not have the appropriate length", site.get());

    // Check coordinate:
    if (checkCoordinate) {
      int coordinate = site->getCoordinate();
      // For all coordinates in vector: throw exception if position already exists
      for (size_t i = 0; i < getNumberOfSites(); ++i) {
        if (i != sitePosition && coordinates_[i] == coordinate)
          throw SiteException("AlignedSequenceContainer::setSite: Site position already exists in container", site.get());
      }
    }

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j) {
      sequence_(j).setElement(sitePosition, (*site)[j]);
    }
    
    // Reset site buffer for this position:
    siteVector_.addObject(nullptr, sitePosition);

    coordinates_[sitePosition] = site->getCoordinate();
  }

	  
  std::unique_ptr<SiteType> removeSite(size_t sitePosition) override
  {
    if (sitePosition >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::removeSite", sitePosition, 0, getNumberOfSites());

    // Get old site
    site(sitePosition); // Creates the site if it does not exist!

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j)
    {
      sequence_(j).deleteElement(sitePosition);
    }

    // Delete site's position
    coordinates_.erase(coordinates_.begin() + static_cast<ptrdiff_t>(sitePosition));
    length_--;

    // Actualizes the 'sites' vector:
    auto sitePtr = siteVector_.removeObject(sitePosition);
    std::get_deleter< SwitchDeleter<SiteType> >(sitePtr)->off();
    return std::unique_ptr<SiteType>(sitePtr.get());
  }


  void deleteSite(size_t sitePosition) override
  {
    if (sitePosition >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::deleteSite", sitePosition, 0, getNumberOfSites());

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j) {
      sequence_(j).deleteElement(sitePosition);
    }

    // Delete site's position
    coordinates_.erase(coordinates_.begin() + static_cast<ptrdiff_t>(sitePosition));
    length_--;

    // Actualizes the 'sites' vector:
    siteVector_.deleteObject(sitePosition);
  }


  void deleteSites(size_t sitePosition, size_t length) override
  {
    if (sitePosition + length > getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::deleteSites", sitePosition + length, 0, getNumberOfSites());

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j) {
      sequence_(j).deleteElements(sitePosition, length);
    }

    // Delete site's sitePositionition
    coordinates_.erase(coordinates_.begin() + static_cast<ptrdiff_t>(sitePosition),
                       coordinates_.begin() + static_cast<ptrdiff_t>(sitePosition + length));
    length_ -= length;

    // Actualizes the 'sites' vector:
    siteVector_.deleteObjects(sitePosition, length);
  }


  void addSite(std::unique_ptr<SiteType>& site, bool checkCoordinate = true) override
  {
    // New site's alphabet and site container's alphabet matching verification
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("AlignedSequenceContainer::addSite", site->getAlphabet(), getAlphabet());

    // Check size:
    if (site->size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::addSite, site does not have the appropriate length", site.get());

    // Check position:
    int coordinate = site->getCoordinate();
    if (checkCoordinate)
    {
      // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < coordinates_.size(); ++i) {
        if (coordinates_[i] == coordinate)
          throw SiteException("AlignedSequenceContainer::addSite: Site coordinate already exists in container", site.get());
      }
    }

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j)
      sequence_(j).addElement((*site)[j]);

    length_++;
    coordinates_.push_back(coordinate);

    // Actualizes the 'sites' vector:
    siteVector_.appendObject(nullptr);
  }
	  
  void addSite(std::unique_ptr<SiteType>& site, size_t sitePosition, bool checkCoordinate = true) override
  {
    if (sitePosition >= getNumberOfSites())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::addSite", sitePosition, 0, getNumberOfSites());

    // New site's alphabet and site container's alphabet matching verification
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("AlignedSequenceContainer::addSite", getAlphabet(), site->getAlphabet());

    // Check size:
    if (site->size() != getNumberOfSequences())
      throw SiteException("AlignedSequenceContainer::addSite, site does not have the appropriate length", site.get());

    // Check position:
    int coordinate = site->getCoordinate();
    if (checkCoordinate) {
    // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < coordinates_.size(); ++i) {
        if (coordinates_[i] == coordinate)
          throw SiteException("AlignedSequenceContainer::addSite: Site coordinate already exists in container", site.get());
      }
    }

    // For all sequences
    for (size_t j = 0; j < getNumberOfSequences(); ++j)
      sequence_(j).addElement(sitePosition, (*site)[j]);

    length_++;
    coordinates_.insert(coordinates_.begin() + static_cast<ptrdiff_t>(sitePosition), coordinate);

    // Actualizes the 'sites' vector:
    siteVector_.insertObject(nullptr, sitePosition);
  }
  
  void clear() override
  {
    length_ = 0;
    TemplateVectorSequenceContainer<SequenceType>::clear();
    siteVector_.clear();
  }

  
  TemplateAlignedSequenceContainer* createEmptyContainer() const override
  {
    auto alphaPtr = getAlphabet();
    auto asc = new TemplateAlignedSequenceContainer(alphaPtr);
    asc->setComments(getComments());
    return asc;
  }


  size_t getNumberOfSites() const override { return length_; }

  Vint getSiteCoordinates() const override { return coordinates_; }

  void setSiteCoordinates(const Vint& coordinates) override
  {
    if (coordinates.size() != getNumberOfSites())
      throw BadSizeException("AlignedSequenceContainer::setSiteCoordinates bad size of positions vector", coordinates.size(), getNumberOfSites());

    for (size_t i = 0; i < coordinates.size(); ++i) {
      coordinates_[i] = coordinates[i];
    }
  }



  void reindexSites() override
  {
    coordinates_.resize(length_);
    for (size_t i = 0; i < length_; ++i) {
      coordinates_[i] = static_cast<int>(i + 1); // starts with 1.
    }
  }

  /** @} */

  /**
   * @name Redefinition of VectorSequenceContainer methods, to check for sequence lengths.
   *
   * @{
   */
  void setSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequencePtr) override
  {
    // if container has only one sequence
    if (getNumberOfSequences() == 1)
      length_ = sequencePtr->size();
    if (checkSize_(*sequencePtr))
      TemplateVectorSequenceContainer<SequenceType>::setSequence(sequenceKey, sequencePtr);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::setSequence", sequencePtr.get());
  }

  
  void addSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequencePtr) override
  {
    // if container has only one sequence
    if (length_ == 0) {
      length_ = sequencePtr->size();
      siteVector_.setSize(length_);
      reindexSites();
    }
    if (checkSize_(*sequencePtr))
      TemplateVectorSequenceContainer<SequenceType>::addSequence(sequenceKey, sequencePtr);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::addSequence", sequencePtr.get());
  }


  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr) override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::setSequence", sequencePosition, 0, getNumberOfSequences());
    // if container has only one sequence
    if (getNumberOfSequences() == 1)
      length_ = sequencePtr->size();
    if (checkSize_(*sequencePtr))
      TemplateVectorSequenceContainer<SequenceType>::setSequence(sequencePosition, sequencePtr);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::setSequence", sequencePtr.get());

    // Detroys all sites (but keep Site Container at same size)
    siteVector_.nullify();
  }


  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr, const std::string& sequenceKey) override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::setSequence", sequencePosition, 0, getNumberOfSequences());
    // if container has only one sequence
    if (getNumberOfSequences() == 1)
      length_ = sequencePtr->size();
    if (checkSize_(*sequencePtr))
      TemplateVectorSequenceContainer<SequenceType>::setSequence(sequencePosition, sequencePtr, sequenceKey);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::setSequence", sequencePtr.get());

    // Detroys all sites (but keep Site Container at same size)
    siteVector_.nullify();
  }


  void insertSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr, const std::string& sequenceKey) override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("AlignedSequenceContainer::insertSequence", sequencePosition, 0, getNumberOfSequences());
    // if container has only one sequence
    if (getNumberOfSequences() == 1)
      length_ = sequencePtr->size();
    if (checkSize_(*sequencePtr))
      TemplateVectorSequenceContainer<SequenceType>::insertSequence(sequencePosition, sequencePtr, sequenceKey);
    else
      throw SequenceNotAlignedException("AlignedSequenceContainer::insertSequence", sequencePtr.get());

    // Detroys all sites (but keep Site Container at same size)
    siteVector_.nullify();
  }

/** @} */
  
  //Needed because of the template class
  using TemplateVectorSequenceContainer<SequenceType>::getAlphabet;
  using TemplateVectorSequenceContainer<SequenceType>::sequence;
  using TemplateVectorSequenceContainer<SequenceType>::sequence_;
  using TemplateVectorSequenceContainer<SequenceType>::valueAt;
  using TemplateVectorSequenceContainer<SequenceType>::getNumberOfSequences;
  using TemplateVectorSequenceContainer<SequenceType>::getComments;
  

  typename SequenceType::ElementType& valueAt(const std::string& sequenceKey, size_t sitePosition) override
  {
    // Reset site buffer for this position:
    siteVector_.addObject(nullptr, sitePosition);
    return sequence_(sequenceKey)[sitePosition];
  }

  typename SequenceType::ElementType& valueAt(const size_t sequencePosition, size_t sitePosition) override
  {
    // Reset site buffer for this position:
    siteVector_.addObject(nullptr, sitePosition);
    return sequence_(sequencePosition)[sitePosition];
  }

protected:
  /**
   * @brief Check sequence's size before insertion in sequence container.
   *
   * @param sequence The sequence to check.
   * @return True if sequence length = number of sites in container.
   */
  bool checkSize_(const Sequence& sequenceRef) { return sequenceRef.size() == length_; }
};

//Aliases:
using AlignedSequenceContainer = TemplateAlignedSequenceContainer<Sequence, Site>;
using ProbabilisticAlignedSequenceContainer = TemplateAlignedSequenceContainer<ProbabilisticSequence, ProbabilisticSite>;

} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_ALIGNEDSEQUENCECONTAINER_H
