/*
 * File SiteTools.cpp
 * Author : Julien Dutheil <julien.dutheil@ens-lyon.fr>
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Friday August 8 2003
*/

#include "SiteTools.h"

// From Utils:
#include <Utils/MapTools.h>

// From NumCalc:
#include <NumCalc/NumTools.h>
using namespace NumTools;
#include <NumCalc/VectorTools.h>
using namespace VectorStatTools;
using namespace VectorFunctions;

// From the STL:
#include <cmath>

// Method for know if a site contains gap(s) or not
bool SiteTools::hasGap(const Site & site) {
	// Main loop : for all characters in site
	for (unsigned int i = 0 ; i < site.size() ; i++) {
		if (site[i] == -1) return true;
	}
	return false;
}

// Method for know if a site contains undefined character or not
bool SiteTools::hasUnknown(const Site & site) {
	// Main loop : for all characters in site
	for(unsigned int i = 0; i < site.size(); i++) {
		if(site[i] >= (int)site.getAlphabet() -> getNumberOfTypes()) return true;
	}
	return false;
}

// Method fot know if a site is complete or not (doesn't contains gap or undefined character)
bool SiteTools::isComplete(const Site & site) {
	// Main loop : for all characters in site
	for(unsigned int i = 0; i < site.size(); i++) {
		if(site[i] < 0 || site[i] >= (int)site.getAlphabet() -> getSize()) return false;
	}
	return true;
}

/* This method is quite the same as operator ==,
 * excepted that it just look at the content of the site
 * whatever the position.
*/
bool SiteTools::areSitesIdentical(const Site & site1, const Site & site2) {
	// Site's size and content checking

	if(site1.size() != site2.size()) return false;
	else {
		for(unsigned int i = 0; i < site1.size(); i++) {
			if(site1[i] != site2[i]) return false;
		}
		return true;
	}
}

// Method for know if a site is constant or not (contain one character type only)
bool SiteTools::isConstant(const Site & site) throw (EmptySiteException) {
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::isConstant: Incorrect specified site", &site);

	// For all site's characters
	int s = site[0];
	for (unsigned int i = 1; i < site.size(); i++) if (site[i] != s) return false;

	return true;
}

/******************************************************************************/

double SiteTools::variabilityShannon(const Site & site) throw (EmptySiteException)
{
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::variabilityShannon: Incorrect specified site", &site);
	map<int, double> p = getFrequencies(site);
	return shannon(MapTools::getValues(p));
}

/******************************************************************************/

double SiteTools::variabilityFactorial(const Site & site) throw (EmptySiteException)
{
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::variabilityFactorial: Incorrect specified site", &site);
	map<int, unsigned int> p = getCounts(site);
	vector<unsigned int> c = MapTools::getValues(p);
	unsigned int s = sum(c);
	return std::log((double)fact(s) / (double)sum(fact(c)));
}

/******************************************************************************/

unsigned int SiteTools::getNumberOfDistinctCharacters(const Site & site) throw (EmptySiteException)
{
    // Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::getNumberOfDistinctCharacters(): Incorrect specified site", &site);
	// For all site's characters
        if (SiteTools::isConstant(site)) return 1;
        map<int,unsigned int> count = SymbolListTools::getCounts(site);
        int S = 0;
        for(map<int, unsigned int>::iterator it=count.begin(); it!=count.end(); it++){
            if(it->second!=0) S++;
        }
        return S;
}

/******************************************************************************/

bool SiteTools::isParsimonyInformativeSite(const Site & site) throw (EmptySiteException)
{
    // Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::isSingleton: Incorrect specified site", &site);
	// For all site's characters
    if(SiteTools::isConstant(site)) return false;
    map<int,unsigned int> count = SymbolListTools::getCounts(site);
    unsigned int npars = 0;
	for(map<int, unsigned int>::iterator it = count.begin(); it != count.end(); it++){
        if(it -> second > 1) npars++;
    }
    if(npars > 1) return true;
    return false;
}

/******************************************************************************/
