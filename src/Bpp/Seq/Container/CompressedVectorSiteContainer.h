// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_COMPRESSEDVECTORSITECONTAINER_H
#define BPP_SEQ_CONTAINER_COMPRESSEDVECTORSITECONTAINER_H

#include <Bpp/Numeric/VectorTools.h>

#include "../Site.h"
#include "AbstractSequenceContainer.h"
#include "AlignedSequenceContainer.h"
#include "SiteContainer.h"

// From the STL library:
#include <string>
#include <vector>
#include <iostream>

namespace bpp
{
/**
 * @brief A low memory, yet restricted, version of the VectorSiteContainer class.
 *
 * This implementation is very similar to VectorSiteContainer, but identical sites
 * are stored only once, which significantly reduces memory usage in the case of
 * containers where the number of sites is large compared to the number of sequences.
 * site access is as fast as in the standard VectorSiteContainer class, but site
 * addition takes more time, as the new site must be first compared to the existing set.
 * A major restriction of this container is that you can't add or remove sequences.
 * The number of sequences is fixed after the first site has been added.
 *
 * @warning Since the data is compressed, the sites given as input are modified. The
 * major practical consequence is that the 'position' attribute of sites will be lost.
 * Instead, the position will correspond to the position in the compressed container.
 * In addition, this container may lead to unexpected behavior if used with derived
 * classes of Site.
 *
 * @see Sequence, Site, VectorSiteContainer
 */
class CompressedVectorSiteContainer :
  public AbstractTemplateSequenceContainer<Sequence, std::string>,
  public virtual TemplateSiteContainerInterface<Site, Sequence, std::string>
{
protected:
  VectorPositionedContainer<Site> siteContainer_;
  VectorMappedContainer<Sequence> sequenceContainer_;
  std::vector<std::string> sequenceNames_;
  std::vector<Comments> sequenceComments_;
  std::vector<size_t> index_;   // For all sites, give the actual position in the set.

public:
  /**
   * @brief Build a new container from a set of sites.
   *
   * @param vs A std::vector of sites.
   * @param alphabet The common alphabet for all sites.
   * @throw Exception If sites differ in size or in alphabet.
   */
  CompressedVectorSiteContainer(
      std::vector<std::unique_ptr<Site>>& vs,
      std::shared_ptr<const Alphabet>& alphabet);

  /**
   * @brief Build a new empty container with specified size.
   *
   * @param size Number of sequences in the container.
   * @param alphabet The alphabet for this container.
   */
  CompressedVectorSiteContainer(
      size_t size,
      std::shared_ptr<const Alphabet>& alphabet);

  /**
   * @brief Build a new empty container with specified sequence names.
   *
   * @param names Sequence names. This will set the number of sequences in the container.
   * @param alphabet The alphabet for this container.
   */
  CompressedVectorSiteContainer(
      const std::vector<std::string>& names,
      std::shared_ptr<const Alphabet>& alphabet);

  /**
   * @brief Build a new empty container.
   *
   * @param alphabet The alphabet for this container.
   */
  CompressedVectorSiteContainer(std::shared_ptr<const Alphabet>& alphabet);

  CompressedVectorSiteContainer(const CompressedVectorSiteContainer& vsc);
  CompressedVectorSiteContainer(const SiteContainerInterface& sc);

  CompressedVectorSiteContainer& operator=(const CompressedVectorSiteContainer& vsc);
  CompressedVectorSiteContainer& operator=(const SiteContainerInterface& sc);

  virtual ~CompressedVectorSiteContainer()
  {
    clear();
  }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  CompressedVectorSiteContainer* clone() const override
  {
    return new CompressedVectorSiteContainer(*this);
  }
  /** @} */

  /**
   * @name The SiteContainer interface implementation:
   *
   * @{
   */
  const Site& site(size_t sitePosition) const override
  {
    return *siteContainer_.getObject(index_[sitePosition]);
  }

  void setSite(size_t sitePosition, std::unique_ptr<Site>& site, bool checkCoordinate = true) override;

  std::unique_ptr<Site> removeSite(size_t sitePosition) override;

  void deleteSite(size_t sitePosition) override;

  void addSite(std::unique_ptr<Site>& site, bool checkCoordinate = false) override;

  void addSite(std::unique_ptr<Site>& site, size_t sitePosition, bool checkCoordinate = false) override;

  void deleteSites(size_t sitePosition, size_t length) override;

  size_t getNumberOfSites() const override
  {
    return index_.size();
  }

  void reindexSites() override;

  Vint getSiteCoordinates() const override;

  void setSiteCoordinates(const Vint& coordinates) override;


  /** @} */

  /**
   * @return The number of site instances
   *
   **/
  size_t getNumberOfUniqueSites() const
  {
    return siteContainer_.getSize();
  }


  // These methods are implemented for this class:

  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */
  const Sequence& sequence(size_t sequenceIndex) const override;

  const Sequence& sequence(const std::string& sequenceKey) const override
  {
    // Look for sequence name:
    size_t pos = getSequencePosition(sequenceKey);
    return sequence(pos);
  }

  bool hasSequence(const std::string& name) const override
  {
    // Look for sequence key:
    return sequenceContainer_.hasObject(name);
  }

  size_t getSequencePosition(const std::string& sequenceKey) const override
  {
    // Look for sequence key:
    return sequenceContainer_.getObjectPosition(sequenceKey);
  }

  std::unique_ptr<Sequence> removeSequence(size_t sequencePosition) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::removeSequence.");
  }

  std::unique_ptr<Sequence> removeSequence(const std::string& sequenceKey) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::removeSequence.");
  }

  void deleteSequence(size_t sequencePosition) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::deleteSequence.");
  }

  void deleteSequence(const std::string& sequenceKey) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::deleteSequence.");
  }

  size_t getNumberOfSequences() const override
  {
    return sequenceContainer_.getNumberOfObjects();
  }

  std::vector<std::string> getSequenceKeys() const override
  {
    return sequenceContainer_.getObjectNames();
  }

  void setSequenceKeys(const std::vector<std::string>& sequenceKeys) override
  {
    sequenceContainer_.setObjectNames(sequenceKeys);
  }

  const std::string& sequenceKey(size_t sequencePosition) const override
  {
    return sequenceContainer_.getObjectName(sequencePosition);
  }

  std::vector<std::string> getSequenceNames() const override
  {
    return sequenceNames_;
  }

  void setSequenceNames(const std::vector<std::string>& names, bool updateKeys) override
  {
    if (names.size() != getNumberOfSequences())
      throw DimensionException("CompressedVectorSiteContainer::setSequenceNames : bad number of names", names.size(), getNumberOfSequences());
    sequenceContainer_.clear();
    sequenceNames_ = names;
    if (updateKeys)
    {
      setSequenceKeys(names);
    }
  }

  std::vector<Comments> getSequenceComments() const override
  {
    return sequenceComments_;
  }

  void clear() override
  {
    siteContainer_.clear();
    sequenceContainer_.clear();
    sequenceNames_.clear();
    sequenceComments_.clear();
    index_.clear();
  }

  CompressedVectorSiteContainer* createEmptyContainer() const override
  {
    auto alphaP = getAlphabet();
    CompressedVectorSiteContainer* vsc = new CompressedVectorSiteContainer(alphaP);
    vsc->setComments(getComments());
    return vsc;
  }

  /** @} */

  void setSequence(size_t sequencePosition, std::unique_ptr<Sequence>& sequence, const std::string& sequenceKey) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::setSequence.");
  }

  void setSequence(size_t sequencePosition, std::unique_ptr<Sequence>& sequence) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::setSequence.");
  }

  void setSequence(const std::string& sequenceKey, std::unique_ptr<Sequence>& sequence) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::setSequence.");
  }

  void addSequence(const std::string& sequenceKey, std::unique_ptr<Sequence>& sequence) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::addSequence.");
  }

  void insertSequence(size_t sequencePosition, std::unique_ptr<Sequence>& sequence, const std::string& sequenceKey) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::insertSequence.");
  }

  const int& valueAt(const std::string& sequenceKey, size_t sitePosition) const override
  {
    return site(sitePosition)[getSequencePosition(sequenceKey)];
  }

  int& valueAt(const std::string& sequenceKey, size_t sitePosition) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::valueAt (non const).");
  }

  const int& valueAt(size_t sequencePosition, size_t sitePosition) const override
  {
    return site(sitePosition)[sequencePosition];
  }

  int& valueAt(size_t sequencePosition, size_t sitePosition) override
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::valueAt (non const).");
  }

  /**
   * @name SequenceData methods.
   *
   * @{
   */
  double getStateValueAt(size_t sitePosition, const std::string& sequenceKey, int state) const override
  {
    return site(sitePosition).getStateValueAt(getSequencePosition(sequenceKey), state);
  }

  double operator()(size_t sitePosition, const std::string& sequenceKey, int state) const override
  {
    return site(sitePosition).getStateValueAt(getSequencePosition(sequenceKey), state);
  }

  double getStateValueAt(size_t sitePosition, size_t sequencePosition, int state) const override
  {
    return site(sitePosition).getStateValueAt(sequencePosition, state);
  }

  double operator()(size_t sitePosition, size_t sequencePosition, int state) const override
  {
    return site(sitePosition).getStateValueAt(sequencePosition, state);
  }

  /** @} */

protected:
  /**
   * Get a non-const reference to a site in the container.
   * It is a convenient short-cut for use within the class only, as this can potentially mess up the data.
   *
   * @param sitePosition the index of the site to retrieve.
   * @return A reference to the selected site.
   */
  Site& getSite_(size_t sitePosition)
  {
    return *siteContainer_.getObject(index_[sitePosition]);
  }

  /**
   * @return The position of the site in the compressed set. If the site is not found,
   * this will return the number of sites in the compressed set.
   */
  size_t getSiteIndex_(const Site& site);
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_COMPRESSEDVECTORSITECONTAINER_H
