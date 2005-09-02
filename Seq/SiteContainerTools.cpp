//
// File: SiteContainerTools.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Fri Dec 12 18:55:06 2003
//

#include "SiteContainerTools.h"
#include "VectorSiteContainer.h"
#include "SiteIterator.h"
#include "SiteTools.h"

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

const Sequence * SiteContainerTools::getConsensus(const SiteContainer & sc, bool gapflag){
	Vint consensus;
	SimpleSiteIterator ssi(sc);
	const Site * site;
	while(ssi.hasMoreSites()){
		site=ssi.nextSite();
		map<int,double> freq = SiteTools::getFrequencies(*site);
                int n = freq.size();
		double max = 0;
		int cons = 0;
		if(gapflag){
			for(map<int,double>::iterator it=freq.begin(); it!=freq.end();it++){
				if(it->second>max && it->first!=-1){
					max = it->second;
					cons = it->first;
				}
			}
		}
		else {
			for(map<int,double>::iterator it=freq.begin(); it!=freq.end();it++){
				if(it->second>max){
					max = it->second;
					cons = it->first;
				}
			}
		}
		consensus.push_back(cons);
	}
	const Sequence * seqConsensus = new Sequence("consensus",consensus,sc.getSequence(0)->getAlphabet());
        return seqConsensus;
}

/******************************************************************************/