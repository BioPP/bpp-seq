//
// File: SiteContainerTools.cpp
// Created by: Julien Dutheil
//             Sylvain Glémin
// Created on: Fri Dec 12 18:55:06 2003
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

const Sequence * SiteContainerTools::getConsensus(const SiteContainer & sc, string name, bool gapflag)
{
	Vint consensus;
	SimpleSiteIterator ssi(sc);
	const Site * site;
	while(ssi.hasMoreSites()){
		site=ssi.nextSite();
		map<int,double> freq = SiteTools::getFrequencies(*site);
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
	const Sequence * seqConsensus = new Sequence(name,consensus,sc.getSequence(0)->getAlphabet());
        return seqConsensus;
}

/******************************************************************************/

