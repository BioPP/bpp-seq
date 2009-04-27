//
// File SiteTools.cpp
// Author : Julien Dutheil
//          Guillaume Deuchst
// Created on: Friday August 8 2003
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

#include "SiteTools.h"

// From Utils:
#include <Utils/MapTools.h>

// From NumCalc:
#include <NumCalc/NumTools.h>
#include <NumCalc/VectorTools.h>

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/******************************************************************************/

bool SiteTools::hasGap(const Site & site)
{
	// Main loop : for all characters in site
	for (unsigned int i = 0 ; i < site.size(); i++)
  {
		if (site[i] == -1) return true;
	}
	return false;
}

/******************************************************************************/

bool SiteTools::isGapOnly(const Site & site)
{
	// Main loop : for all characters in site
	for (unsigned int i = 0 ; i < site.size(); i++)
  {
		if(!site.getAlphabet()->isGap(site[i])) return false;
	}
	return true;
}

/******************************************************************************/

bool SiteTools::isGapOrUnresolvedOnly(const Site & site)
{
	// Main loop : for all characters in site
	for (unsigned int i = 0 ; i < site.size(); i++)
  {
		if(!site.getAlphabet()->isGap(site[i]) || !site.getAlphabet()->isUnresolved(site[i])) return false;
	}
	return true;
}

/******************************************************************************/

bool SiteTools::hasUnknown(const Site & site)
{
	// Main loop : for all characters in site
	for(unsigned int i = 0; i < site.size(); i++)
  {
		if(site[i] == site.getAlphabet()->getUnknownCharacterCode()) return true;
	}
	return false;
}

/******************************************************************************/

bool SiteTools::isComplete(const Site & site)
{
	// Main loop : for all characters in site
	for(unsigned int i = 0; i < site.size(); i++)
  {
		if(site[i] < 0 || site[i] >= (int)site.getAlphabet()->getSize()) return false;
	}
	return true;
}

/******************************************************************************/

bool SiteTools::areSitesIdentical(const Site & site1, const Site & site2)
{
	// Site's size and content checking
  if(site1.getAlphabet()->getAlphabetType() != site2.getAlphabet()->getAlphabetType()) return false;
	if(site1.size() != site2.size()) return false;
	else
  {
		for(unsigned int i = 0; i < site1.size(); i++)
    {
			if(site1[i] != site2[i]) return false;
		}
		return true;
	}
}

/******************************************************************************/

bool SiteTools::isConstant(const Site & site, bool ignoreUnknown) throw (EmptySiteException)
{
	// Empty site checking
	if (site.size() == 0) throw EmptySiteException("SiteTools::isConstant: Incorrect specified site", &site);

	// For all site's characters
  int gap = site.getAlphabet()->getGapCharacterCode();
  if (ignoreUnknown)
  {
	  int s = site[0];
    int unknown = site.getAlphabet()->getUnknownCharacterCode();
    unsigned int i = 0;
    while (i < site.size() && s == gap && s == unknown)
    {
      s = site[i];
      i++;
    }
    if(s == unknown || s == gap) throw EmptySiteException("SiteTools::isConstant: Site is only made of gaps or generic characters.");
	  while(i < site.size())
    {
      if (site[i] != s && site[i] != gap && site[i] != unknown) return false;
      i++;
    }
  } 
  else
  {
    int s = site[0];
    unsigned int i = 0;
    while  (i < site.size() && s == gap)
    {
      s = site[i];
      i++;
    }
    if (s == gap) throw EmptySiteException("SiteTools::isConstant: Site is only made of gaps.");
	  while (i < site.size())
    {
      if (site[i] != s && site[i] != gap) return false;
      i++;
    }
  }

	return true;
}

/******************************************************************************/

double SiteTools::variabilityShannon(const Site & site, bool resolveUnknown) throw (EmptySiteException)
{
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::variabilityShannon: Incorrect specified site", &site);
	map<int, double> p;
  getFrequencies(site, p, resolveUnknown);
  //We need to correct frequencies for gaps:
  double s;
  for(unsigned int i = 0; i < site.getAlphabet()->getSize(); i++)
  {
    double f = p[i];
	  if(f > 0) s += f * log(f);
  }
  return -s;
}

/******************************************************************************/

double SiteTools::mutualInformation(const Site & site1, const Site & site2, bool resolveUnknown) throw (DimensionException,EmptySiteException)
{
	// Empty site checking
	if(site1.size() == 0) throw EmptySiteException("SiteTools::mutualInformation: Incorrect specified site", &site1);
	if(site2.size() == 0) throw EmptySiteException("SiteTools::mutualInformation: Incorrect specified site", &site2);
  if(site1.size() != site2.size()) throw DimensionException("SiteTools::mutualInformation: sites must have the same size!", site1.size(), site2.size());
	vector<double> p1(site1.getAlphabet()->getSize());
  vector<double> p2(site2.getAlphabet()->getSize());
  map<int, map<int, double> > p12;
  getCounts(site1, site2, p12, resolveUnknown);
  double mi = 0, tot = 0, pxy;
  //We need to correct frequencies for gaps:
  for(unsigned int i = 0; i < site1.getAlphabet()->getSize(); i++)
  {
    for(unsigned int j = 0; j < site2.getAlphabet()->getSize(); j++)
    {
      pxy = p12[(int)i][(int)j];
      tot += pxy;
      p1[i] += pxy;
      p2[j] += pxy;
    }
  }
  for(unsigned int i = 0; i < site1.getAlphabet()->getSize(); i++) p1[i] /= tot;
  for(unsigned int j = 0; j < site2.getAlphabet()->getSize(); j++) p2[j] /= tot;
  for(unsigned int i = 0; i < site1.getAlphabet()->getSize(); i++)
  {
    for(unsigned int j = 0; j < site2.getAlphabet()->getSize(); j++)
    {
      pxy = p12[(int)i][(int)j] / tot;
      if(pxy > 0) mi += pxy * log(pxy/(p1[i]*p2[j]));
    }
  }
	return mi;
}

/******************************************************************************/

double SiteTools::variabilityFactorial(const Site & site) throw (EmptySiteException)
{
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::variabilityFactorial: Incorrect specified site", &site);
	map<int, unsigned int> p;
  getCounts(site, p);
	vector<unsigned int> c = MapTools::getValues(p);
	unsigned int s = VectorTools::sum(c);
  return std::log((double)NumTools::fact(s) / (double)VectorTools::sum(VectorTools::fact(c)));
}

/******************************************************************************/

double SiteTools::heterozygosity(const Site & site) throw (EmptySiteException)
{
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::heterozygosity: Incorrect specified site", &site);
	map<int, double> p;
  getFrequencies(site, p);
	vector<double> c = MapTools::getValues(p);
	double n = VectorTools::norm<double, double>(MapTools::getValues(p));
	return 1. - n*n;
}

/******************************************************************************/

unsigned int SiteTools::getNumberOfDistinctCharacters(const Site & site) throw (EmptySiteException)
{
  // Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::getNumberOfDistinctCharacters(): Incorrect specified site", &site);
	// For all site's characters
  if(SiteTools::isConstant(site)) return 1;
  map<int,unsigned int> counts;
  SymbolListTools::getCounts(site, counts);
  int s = 0;
  for(map<int, unsigned int>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if(it->second!=0) s++;
  }
  return s;
}

/******************************************************************************/

bool SiteTools::hasSingleton(const Site & site) throw (EmptySiteException)
{
  // Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::hasSingleton: Incorrect specified site", &site);
	// For all site's characters
  if(SiteTools::isConstant(site)) return false;
  map<int,unsigned int> counts;
  getCounts(site, counts);
	for(map<int, unsigned int>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if(it->second == 1) return true;
  }
  return false;
}

/******************************************************************************/

bool SiteTools::isParsimonyInformativeSite(const Site & site) throw (EmptySiteException)
{
  // Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::isParsimonyInformativeSite: Incorrect specified site", &site);
	// For all site's characters
  if(SiteTools::isConstant(site)) return false;
  map<int,unsigned int> counts;
  SymbolListTools::getCounts(site, counts);
  unsigned int npars = 0;
	for(map<int, unsigned int>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if(it->second > 1) npars++;
  }
  if(npars > 1) return true;
  return false;
}

/******************************************************************************/

bool SiteTools::isTriplet(const Site & site) throw (EmptySiteException)
{
	// Empty site checking
	if(site.size() == 0) throw EmptySiteException("SiteTools::isTriplet: Incorrect specified site", &site);
	// For all site's characters
  if(SiteTools::getNumberOfDistinctCharacters(site)>=3) return true;
  else return false;
}

/******************************************************************************/

