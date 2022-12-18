//
// File: SiteContainer.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
//   Laurent Guéguen
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

//Aliases:
using SiteContainerInterface = TemplateSiteContainerInterface<Site, Sequence, std::string>;
using ProbabilisticSiteContainerInterface = TemplateSiteContainerInterface<ProbabilisticSite, ProbabilisticSequence, std::string>;

} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SITECONTAINER_H
