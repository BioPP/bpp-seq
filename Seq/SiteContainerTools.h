//
// File: SiteContainerTools.h
// Created by: Julien Dutheil
// Created on: Fri Dec 12 18:55:06 2003
//

#ifndef _SITECONTAINERTOOLS_H_
#define _SITECONTAINERTOOLS_H_

#include "SiteContainer.h"

typedef vector<unsigned int> SiteSelection;

/**
 * @brief Some util to deal with site containers.
 */
class SiteContainerTools
{
	public:
		SiteContainerTools() {}
		 ~SiteContainerTools() {}

	public:

		/**
		 * @brief Retrieves sites without gaps from SiteContainer.
		 *
		 * This function build a new SiteContainer instance with only sites without gaps.
		 * The container passed as input is not modified, all sites are copied.
		 *
		 * @param sites The container to analyse.
		 * @return A pointer toward a new SiteContainer with only sites with no gaps.
		 */
		static SiteContainer * getSitesWithoutGaps(const SiteContainer & sites);

		/**
		 * @brief Retrieves complete sites from SiteContainer.
		 *
		 * This function build a new SiteContainer instance with only complete sites,
		 * i.e. site with fully resolved states (no gap, no unknown caracters).
		 * The container passed as input is not modified, all sites are copied.
		 *
		 * @param sites The container to analyse.
		 * @return A pointer toward a new SiteContainer with only sites with no gaps.
		 */
		static SiteContainer * getCompleteSites(const SiteContainer & sites);

		/**
		 * @brief Create a new container with a specified set of sites.
		 *
		 * A new VectorSiteContainer is created with specified sites.
		 * The destruction of the container is up to the user.
		 * Sequences are specified by their indice, beginning at 0.
		 * No position verification is performed, based on the assumption that
		 * the container passed as an argument is a correct one.
		 * Redundant selection is not checked, so be careful with what you're doing!
		 *
		 * @param sequences The container from wich sequences are to be taken.
		 * @param selection The positions of all sites to retrieve.
		 * @return A new container with all selected sites.
		 */
		static SiteContainer * getSelectedSites(const SiteContainer & sequences, const SiteSelection & selection);


		/**
		 * @brief create the consensus sequence of the alignment
		 *
		 * gap can be taken into account or not (default option)
		 *
		 * @param sc a site container
		 * @param gapflag
		 * @param name the name of the sequence object that will be created. 
		 * @return A new Sequence object with the consensus sequence.
		 */

		static const Sequence * SiteContainerTools::getConsensus(const SiteContainer & sc,string name="consensus", bool gapflag = true);
};


#endif	//_SITECONTAINERTOOLS_H_
