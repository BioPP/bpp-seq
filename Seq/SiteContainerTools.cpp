//
// File: SiteContainerTools.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Fri Dec 12 18:55:06 2003
//

#include "SiteContainerTools.h"
#include "VectorSiteContainer.h"
#include "SiteIterator.h"

// From the STL:
#include <vector>
#include <string>
#include <ctype.h>
using namespace std;

/******************************************************************************/

SiteContainer * SiteContainerTools::getSitesWithoutGaps(const SiteContainer & sites)
{
	vector<string> seqNames = sites.getSequencesNames();
	VectorSiteContainer * noGapCont = new VectorSiteContainer(seqNames.size(), sites.getAlphabet());
	noGapCont -> setSequencesNames(seqNames, false);
	NoGapSiteIterator ngsi(sites);
	while(ngsi.hasMoreSites()) {
		noGapCont -> addSite(* ngsi.nextSite());
	}
	return noGapCont;
}

/******************************************************************************/

SiteContainer * SiteContainerTools::getCompleteSites(const SiteContainer & sites)
{
	vector<string> seqNames = sites.getSequencesNames();
	VectorSiteContainer * noGapCont = new VectorSiteContainer(seqNames.size(), sites.getAlphabet());
	noGapCont -> setSequencesNames(seqNames, false);
	CompleteSiteIterator csi(sites);
	while(csi.hasMoreSites()) {
		noGapCont -> addSite(* csi.nextSite());
	}
	return noGapCont;
}

/******************************************************************************/

SiteContainer * SiteContainerTools::getSelectedSites(
    const SiteContainer & sequences,
    const SiteSelection & selection)
{
	vector<string> seqNames = sequences.getSequencesNames();
    VectorSiteContainer * sc = new VectorSiteContainer(seqNames.size(), sequences.getAlphabet());
	sc -> setSequencesNames(seqNames, false);
    for(unsigned int i = 0; i < selection.size(); i++) {
        sc -> addSite(* sequences.getSite(selection[i]), false);
        //We do not check names, we suppose that the container passed as an argument is correct.
        //WARNING: what if selection contains many times the same indice? ...
    }
    sc -> setGeneralComments(sequences.getGeneralComments());
    return sc;
}

/******************************************************************************/
