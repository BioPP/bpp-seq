//
// File SiteContainer.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#ifndef _SITECONTAINER_H_
#define _SITECONTAINER_H_

#include "../Site.h"
#include "OrderedSequenceContainer.h"
#include "AlignedValuesContainer.h"

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
 * All site objects are cloned befored being added and retrieved.
 */
  class SiteContainer :
    public virtual OrderedSequenceContainer,
    public virtual AlignedValuesContainer
  {
  public:
    SiteContainer() {}
    virtual ~SiteContainer() {}

    SiteContainer* clone() const = 0;

  public:
    /**
     * @brief Get a site from the container.
     *
     * @param siteIndex The position of the site in the container.
     * @return A site objet corresponding to site i in the alignment.
     * @throw IndexOutOfBoundsException If the specified site does not exists.
     */
  
    virtual const Site& getSite(size_t siteIndex) const = 0;
    virtual Site& getSite(size_t siteIndex) = 0;

    const CruxSymbolListSite& getSymbolListSite(size_t siteIndex) const
    {
      return getSite(siteIndex);
    }

    CruxSymbolListSite& getSymbolListSite(size_t siteIndex)
    {
      return getSite(siteIndex);
    }

    /**
     * @brief Set a site in the container.
     *
     * @param siteIndex     The position of the site in the container.
     * @param site          The site to set.
     * @param checkPosition Look if the position of the new site match a position attribute in the container.
     * @throw Exception If the specified site does not exists or is not correct.
     */
    virtual void setSite(size_t siteIndex, const Site& site, bool checkPosition) = 0;

    /**
     * @brief Add a site in the container.
     *
     * @param site          The site to add.
     * @param checkPosition Look if the position of the new site match a position attribute in the container.
     * @throw Exception If the specified site does not exists or is not correct.
     */
    virtual void addSite(const Site& site, bool checkPosition) = 0;

    /**
     * @brief Add a site in the container.
     *
     * @param site          The site to add.
     * @param position      The new position of the site, to superseed the one in 'site'.
     * @param checkPosition Look if the position of the new site match a position attribute in the container.
     * @throw Exception If the specified site does not exists or is not correct.
     */
    virtual void addSite(const Site& site, int position, bool checkPosition) = 0;

    /**
     * @brief Add a site in the container.
     *
     * @param site          The site to add.
     * @param siteIndex     The position where to insert the site.
     * @param checkPosition Look if the position of the new site match a position attribute in the container.
     * @throw Exception If the specified site does not exists or is not correct.
     */
    virtual void addSite(const Site& site, size_t siteIndex, bool checkPosition) = 0;

    /**
     * @brief Add a site in the container.
     *
     * @param site          The site to add.
     * @param siteIndex     The position where to insert the site.
     * @param position      The new position of the site, to superseed the one in 'site'.
     * @param checkPosition Look if the position of the new site match a position attribute in the container.
     * @throw Exception If the specified site does not exists or is not correct.
     */
    virtual void addSite(const Site& site, size_t siteIndex, int position, bool checkPosition) = 0;

    /**
     * @brief Remove a site from the container.
     *
     * The site is deleted (ie the container is shortened) and a pointer
     * toward it is returned.
     *
     * @param siteIndex The position of the site in the container.
     * @return A pointer toward site i in the alignment.
     * @throw IndexOutOfBoundsException If the specified site does not exists.
     */
  
    virtual std::shared_ptr<Site> deleteSite(size_t siteIndex) = 0;

  };
} // end of namespace bpp.

#endif  // _SITECONTAINER_H_

