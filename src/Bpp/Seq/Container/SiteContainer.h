// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_SITECONTAINER_H
#define BPP_SEQ_CONTAINER_SITECONTAINER_H

#include "AlignmentData.h"
#include "SequenceContainer.h"
#include "../Site.h"
#include "../ProbabilisticSite.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief The SiteContainer interface.
 *
 * Container implementing the SiteContainer interface deal with <em>aligned</em> sequences.
 * This interface provides methods to retrieve, add or set sites in the alignment.
 * As for SequenceContainers, the maintenance of Sites is up to the container.
 */
template<class SiteType, class SequenceType, class HashType>
class TemplateSiteContainerInterface :
  public virtual TemplateAlignmentDataInterface<HashType>,
  public virtual TemplateSequenceContainerInterface<SequenceType, HashType>
{
public:
  TemplateSiteContainerInterface() {}
  virtual ~TemplateSiteContainerInterface() {}

  TemplateSiteContainerInterface<SiteType, SequenceType, HashType>* clone() const override = 0;

  TemplateSiteContainerInterface<SiteType, SequenceType, HashType>* createEmptyContainer() const override = 0;

public:
  /**
   * @brief Get a site from the container.
   *
   * @param sitePosition The position of the site in the container.
   * @return A site objet corresponding to site i in the alignment.
   * @throw IndexOutOfBoundsException If the specified site does not exists.
   */
  virtual const SiteType& site(size_t sitePosition) const override = 0;

  /**
   * @brief Set a site in the container.
   *
   * @param sitePosition    The position of the site in the container.
   * @param site            The site to set.
   * @param checkCoordinate Look if the coordinate of the new site match a coordinate attribute in the container.
   * @throw Exception If the specified site does not exists or is not correct.
   */
  virtual void setSite(size_t sitePosition, std::unique_ptr<SiteType>& site, bool checkCoordinate = true) = 0;

  /**
   * @brief Add a site in the container.
   *
   * @param site            The site to add.
   * @param checkCoordinate Look if the coordinate of the new site match a coordinate attribute in the container.
   * @throw Exception If the specified site does not exists or is not correct.
   */
  virtual void addSite(std::unique_ptr<SiteType>& site, bool checkCoordinate) = 0;

  /**
   * @brief Add a site in the container.
   *
   * @param site            The site to add.
   * @param sitePosition    The coordinate where to insert the site.
   * @param checkCoordinate Look if the coordinate of the new site match a coordinate attribute in the container.
   * @throw Exception If the specified site does not exists or is not correct.
   */
  virtual void addSite(std::unique_ptr<SiteType>& site, size_t sitePosition, bool checkCoordinate) = 0;

  /**
   * @brief Remove a site from the container.
   *
   * The site is deleted (ie the container is shortened) and a pointer
   * toward it is returned.
   *
   * @param sitePosition The position of the site in the container.
   * @return A pointer toward the given site in the alignment.
   * @throw IndexOutOfBoundsException If the specified site does not exists.
   */
  virtual std::unique_ptr<SiteType> removeSite(size_t sitePosition) = 0;

  /**
   * @brief Delete a site from the container.
   *
   * The site is deleted (ie the container is shortened).
   *
   * @param sitePosition The position of the site in the container.
   * @throw IndexOutOfBoundsException If the specified site does not exists.
   */
  virtual void deleteSite(size_t sitePosition) = 0;

  /**
   * @brief Remove a continuous range of sites in the container.
   *
   * @param sitePosition The position of the first site in the container.
   * @param length The length of the region to delete, starting at pposition sitePosition.
   * @throw IndexOutOfBoundsException If the specified range is not valid.
   */
  virtual void deleteSites(size_t sitePosition, size_t length) override = 0;

  /**
   * @brief Get the number of aligned positions in the container.
   *
   * @return The number of sites in the container.
   */
  virtual size_t getNumberOfSites() const override = 0;

  /**
   * @brief Set all coordinate attributes.
   */
  virtual void reindexSites() override = 0;
};

// Aliases:
using SiteContainerInterface = TemplateSiteContainerInterface<Site, Sequence, std::string>;
using ProbabilisticSiteContainerInterface = TemplateSiteContainerInterface<ProbabilisticSite, ProbabilisticSequence, std::string>;
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SITECONTAINER_H
