/*
 * File SiteContainer.h
 * Author : Julien Dutheil <julien.dutheil@ens-lyon.fr>
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Friday August 8 2003
*/
 
#ifndef _SITECONTAINER_H_
#define _SITECONTAINER_H_

#include "Site.h"
#include "OrderedSequenceContainer.h"
#include "SequenceContainerExceptions.h"

// From Utils:
#include <Utils/Exceptions.h>

// From NumCalc:
#include <NumCalc/VectorTools.h>

// From the STL:
#include <cstdlib>

using namespace std;

/**
 * @brief The SiteContainer interface.
 *
 * Container implementing the SiteContainer interface deal with <em>aligned</em> sequences.
 * This interface provides methods to retrieve, add or set sites in the alignment.
 * As for SequenceContainers, the maintenance of Sites is up to the container.
 * All site objects are cloned befored being added and retrieved.
 * All sites stored are deleted in the destructor of the container or after having called the deleteSite() method.
 */

class SiteContainer: public OrderedSequenceContainer
{
	public:
		virtual ~SiteContainer() {}

	public:
	
		/**
		 * @brief Get a site from the container.
		 *
		 * @param siteIndex The position of the site in the container.
		 * @return A site objet corresponding to site i in the alignment.
		 * @throw IndexOutOfBoundsException If the specified site does not exists.
		 */
		virtual const Site * getSite(unsigned int siteIndex) const throw (IndexOutOfBoundsException) = 0;

		/**
		 * @brief Set a site in the container.
		 *
		 * @param siteIndex     The position of the site in the container.
		 * @param site          The site to set.
		 * @param checkPosition Look if the position of the new site match a position attribute in the container.
		 * @throw Exception If the specified site does not exists or is not correct.
		 */
		virtual void setSite(unsigned int siteIndex, const Site & site, bool checkPosition) throw (Exception) = 0;

		/**
		 * @brief Remove a site from the container.
		 *
		 * The site is not deleted, a pointer toward it is returned.
		 *
		 * @param siteIndex The position of the site in the container.
		 * @return A pointer toward site i in the alignment.
		 * @throw IndexOutOfBoundsException If the specified site does not exists.
		 */
		virtual Site * removeSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) = 0;

		/**
		 * @brief Delete a site in the container.
		 *
		 * @param siteIndex The position of the site in the container.
		 * @throw IndexOutOfBoundsException If the specified site does not exists.
		 */
		virtual void deleteSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) = 0;

		/**
		 * @brief Get the number of sites in the container.
		 *
		 * @return The number of sites in the container.
		 */
		virtual unsigned int getNumberOfSites() const = 0;

		/**
		 * @brief Set all positions attributes.
		 */
		virtual void reindexSites() = 0;

		/**
		 * @brief Get all position attributes of sites.
		 *
		 * @return A vector with all site positions.
		 */
		virtual Vint getSitePositions() const = 0;
};

#endif	//_SITECONTAINER_H_
