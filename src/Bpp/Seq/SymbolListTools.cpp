//
// File: SymbolListTools.cpp
// Created by: Julien Dutheil
// Created on: Wed Apr 9 2004
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

#include "SymbolListTools.h"
#include "Alphabet/AlphabetTools.h"
#include <Bpp/Numeric/Random/RandomTools.h>
#include <Bpp/Utils/MapTools.h>
#include <Bpp/Numeric/NumTools.h>
#include <Bpp/Numeric/VectorTools.h>

//From the STL:
#include <algorithm>

using namespace std;

using namespace bpp;

/******************************************************************************/

bool SymbolListTools::hasGap(const IntCoreSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (site.getAlphabet()->isGap(site[i]))
      return true;
  }
  return false;
}

bool SymbolListTools::hasGap(const ProbabilisticSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (VectorTools::sum(site[i]) <= NumConstants::TINY())
      return true;
  }
  return false;
}

/******************************************************************************/

bool SymbolListTools::isGapOnly(const IntCoreSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (!site.getAlphabet()->isGap(site[i]))
      return false;
  }
  return true;
}

bool SymbolListTools::isGapOnly(const ProbabilisticSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (VectorTools::sum(site[i]) > NumConstants::TINY())
      return false;
  }
  return true;
}

/******************************************************************************/

bool SymbolListTools::isGapOrUnresolvedOnly(const IntCoreSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (!site.getAlphabet()->isGap(site[i]) && !site.getAlphabet()->isUnresolved(site[i]))
      return false;
  }
  return true;
}

bool SymbolListTools::isGapOrUnresolvedOnly(const ProbabilisticSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    double ss = VectorTools::sum(site[i]);
    if (ss>NumConstants::TINY() && ss<1.)
      return false;
  }
  return true;
}

/******************************************************************************/

bool SymbolListTools::hasUnknown(const IntCoreSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (site[i] == site.getAlphabet()->getUnknownCharacterCode())
      return true;
  }
  return false;
}

bool SymbolListTools::hasUnknown(const ProbabilisticSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    double ss = VectorTools::sum(site[i]);
    if (ss>1.)
      return true;
  }
  return false;
}

/******************************************************************************/

bool SymbolListTools::isComplete(const IntCoreSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (site.getAlphabet()->isGap(site[i]) || site.getAlphabet()->isUnresolved(site[i]))
      return false;
  }
  return true;
}

bool SymbolListTools::isComplete(const ProbabilisticSymbolList& site)
{
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    double ss = VectorTools::sum(site[i]);
    if (ss<NumConstants::TINY() || ss>1.)
      return false;
  }
  return true;
}

/******************************************************************************/

size_t SymbolListTools::numberOfGaps(const IntCoreSymbolList& site)
{
  size_t n=0;
  
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (site.getAlphabet()->isGap(site[i])) 
      n++;
  }
  return n;
}

size_t SymbolListTools::numberOfGaps(const ProbabilisticSymbolList& site)
{
  size_t n=0;
  
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (VectorTools::sum(site[i]) < NumConstants::TINY())
      n++;
  }
  return n;
}

/******************************************************************************/

size_t SymbolListTools::numberOfUnresolved(const IntCoreSymbolList& site)
{
  size_t n=0;
  
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (site.getAlphabet()->isUnresolved(site[i])) 
      n++;
  }
  return n;
}

size_t SymbolListTools::numberOfUnresolved(const ProbabilisticSymbolList& site)
{
  size_t n=0;
  
  // Main loop : for all characters in site
  for (size_t i = 0; i < site.size(); i++)
  {
    if (VectorTools::sum(site[i]) > 1.)
      n++;
  }
  return n;
}

/******************************************************************************/


bool SymbolListTools::areSymbolListsIdentical(const IntCoreSymbolList& site1, const IntCoreSymbolList& site2)
{
  // IntCoreSymbolList's size and content checking
  if (site1.getAlphabet()->getAlphabetType() != site2.getAlphabet()->getAlphabetType())
    return false;
  if (site1.size() != site2.size())
    return false;
  else
  {
    for (size_t i = 0; i < site1.size(); i++)
    {
      if (site1[i] != site2[i])
        return false;
    }
    return true;
  }
}

bool SymbolListTools::areSymbolListsIdentical(const ProbabilisticSymbolList& site1, const ProbabilisticSymbolList& site2)
{
  // IntCoreSymbolList's size and content checking
  if (site1.getAlphabet()->getAlphabetType() != site2.getAlphabet()->getAlphabetType())
    return false;
  if (site1.size() != site2.size())
    return false;
  else
  {
    for (size_t i = 0; i < site1.size(); i++)
    {
      if (site1[i] != site2[i])
        return false;
    }
    return true;
  }
}

/******************************************************************************/

bool SymbolListTools::isConstant(const IntCoreSymbolList& site, bool ignoreUnknown, bool unresolvedRaisesException)
{
  // Emspty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::isConstant: Incorrect specified site, size must be > 0");

  // For all site's characters
  int gap = site.getAlphabet()->getGapCharacterCode();
  if (ignoreUnknown)
  {
    int s = site[0];
    int unknown = site.getAlphabet()->getUnknownCharacterCode();
    size_t i = 0;
    while (i < site.size() && (s == gap || s == unknown))
    {
      s = site[i];
      i++;
    }
    if (s == unknown || s == gap)
    {
      if (unresolvedRaisesException)
        throw Exception("SymbolListTools::isConstant: IntCoreSymbolList is only made of gaps or generic characters.");
      else
        return false;
    }
    while (i < site.size())
    {
      if (site[i] != s && site[i] != gap && site[i] != unknown)
        return false;
      i++;
    }
  }
  else
  {
    int s = site[0];
    size_t i = 0;
    while  (i < site.size() && s == gap)
    {
      s = site[i];
      i++;
    }
    if (s == gap)
    {
      if (unresolvedRaisesException)
        throw Exception("SymbolListTools::isConstant: IntCoreSymbolList is only made of gaps.");
      else
        return false;
    }
    while (i < site.size())
    {
      if (site[i] != s && site[i] != gap)
        return false;
      i++;
    }
  }

  return true;
}

bool SymbolListTools::isConstant(const ProbabilisticSymbolList& site, bool unresolvedRaisesException)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::isConstant: Incorrect specified site, size must be > 0");

  // For all site's characters
  Vdouble s = site[0];
  double ss=VectorTools::sum(s);
  size_t i = 0;
  while (i < site.size() && (ss < NumConstants::TINY()))
  {
    s = site[i];
    ss=VectorTools::sum(s);
    
    i++;
  }
  if (ss<NumConstants::TINY())
  {
    if (unresolvedRaisesException)
      throw Exception("SymbolListTools::isConstant: IntCoreSymbolList is only made of gaps.");
    else
      return false;
  }
  while (i < site.size())
  {
    if (site[i] != s && VectorTools::sum(site[i])<NumConstants::TINY())
      return false;
    i++;
  }

  return true;
}


/**************************************************************/
/*********** IntCoreSymbolList ******************/
/**************************************************************/

void SymbolListTools::getCounts(const IntCoreSymbolList& list, map<int, double>& counts, bool resolveUnknowns)
{
  if (!resolveUnknowns)
  {
    for (size_t i = 0; i < list.size(); ++i)
      counts[list[i]]++;
  }
  else
  {
    for (size_t i = 0; i < list.size(); ++i)
    {
      vector<int> alias = list.getAlphabet()->getAlias(list[i]);
      double n = static_cast<double>(alias.size());
      for (size_t j = 0; j < alias.size(); j++) counts[alias[j]] += 1./n ;
    }
  }
}

void SymbolListTools::getCounts(const IntCoreSymbolList& list1, const IntCoreSymbolList& list2,  map< int, map<int, double> >& counts, bool resolveUnknowns)
{
  if (list1.size() != list2.size()) throw DimensionException("SymbolListTools::getCounts: the two sites must have the same size.", list1.size(), list2.size());
  if (!resolveUnknowns)
  {
    for (size_t i = 0; i < list1.size(); i++)
      counts[list1[i]][list2[i]]++;
  }
  else
  {
    for (size_t i = 0; i < list1.size(); i++)
    {
      vector<int> alias1 = list1.getAlphabet()->getAlias(list1[i]);
      vector<int> alias2 = list2.getAlphabet()->getAlias(list2[i]);
      double n1 = (double)alias1.size();
      double n2 = (double)alias2.size();
      for (size_t j = 0; j < alias1.size(); j++)
        for (size_t k = 0; k < alias2.size(); k++)
          counts[alias1[j]][alias2[k]] += 1./(n1*n2) ;
    }
  }
}

void SymbolListTools::getFrequencies(const CruxSymbolList& list, map<int, double>& frequencies, bool resolveUnknowns)
{
  double n = (double)list.size();
  map<int, double> counts;

  getCounts(list, counts, resolveUnknowns);
  
  for (auto it : counts)
  {
    frequencies[it.first] = it.second / n;
  }
}

void SymbolListTools::getFrequencies(const CruxSymbolList& list1, const CruxSymbolList& list2, map<int, map<int, double> >& frequencies, bool resolveUnknowns)
{
  double n2 = (double)list1.size() * (double)list1.size();
  map<int, map<int, double> > counts;
  getCounts(list1, list2, counts, resolveUnknowns);

  for (auto it1 : counts)
    for (auto it2 : it1.second)
    {
      frequencies[it1.first][it2.first] = it2.second / n2;
    }
}

double SymbolListTools::getGCContent(const IntCoreSymbolList& list, bool ignoreUnresolved, bool ignoreGap)
{
  const Alphabet * alphabet = list.getAlphabet();
  if (!AlphabetTools::isNucleicAlphabet(alphabet))
    throw AlphabetException("SymbolListTools::getGCContent. Method only works on nucleotides.", alphabet);
  double gc = 0;
  double total = 0;
  for (size_t i = 0; i < list.size(); i++) {
    int state = list.getValue(i);
    if (state > -1) { // not a gap
      if (state == 1 || state == 2) { // G or C
        gc++;
        total++;
      } else if (state == 0 || state == 3) { // A, T or U
        total++;
      } else { // Unresolved character
        if (!ignoreUnresolved) {
          total++;
          switch(state) {
          case(7): gc++; break;// G or C
          case(4): gc+=0.5; break;// A or C
          case(5): gc+=0.5; break;// A or G
          case(6): gc+=0.5; break;// C or T
          case(9): gc+=0.5; break;// G or T
          case(10): gc+=2./3.; break;// A or C or G
          case(11): gc+=1./3.; break;// A or C or T
          case(12): gc+=1./3.; break;// A or G or T
          case(13): gc+=2./3.; break;// C or G or T
          case(14): gc+=0.5; break;// A or C or G or T
          }
        }
      }
    } else {
      if (!ignoreGap) total++;
    }
  }
  return total != 0 ? gc/total : 0;
}

size_t SymbolListTools::getNumberOfDistinctPositions(const IntCoreSymbolList& l1, const IntCoreSymbolList& l2)
{
  if (l1.getAlphabet()->getAlphabetType() != l2.getAlphabet()->getAlphabetType()) throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
  size_t n = min(l1.size(), l2.size());
  size_t count = 0;
  for (size_t i = 0; i < n; i++) {
    if (l1[i] != l2[i]) count++;
  }
  return count;
}

size_t SymbolListTools::getNumberOfPositionsWithoutGap(const IntCoreSymbolList& l1, const IntCoreSymbolList& l2)
{
  if (l1.getAlphabet() -> getAlphabetType() != l2.getAlphabet() -> getAlphabetType()) throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
  size_t n = min(l1.size(), l2.size());
  size_t count = 0;
  for (size_t i = 0; i < n; i++) {
    if (l1[i] != -1 && l2[i] != -1) count++;
  }
  return count;
}

void SymbolListTools::changeGapsToUnknownCharacters(IntCoreSymbolList& l)
{
  int unknownCode = l.getAlphabet()->getUnknownCharacterCode();
  for (size_t i = 0; i < l.size(); i++)
  {
    if (l.getAlphabet()->isGap(l[i])) l[i] = unknownCode;
  }
}

void SymbolListTools::changeUnresolvedCharactersToGaps(IntCoreSymbolList& l)
{
  int gapCode = l.getAlphabet()->getGapCharacterCode();
  for (size_t i = 0; i < l.size(); i++)
  {
    if (l.getAlphabet()->isUnresolved(l[i])) l[i] = gapCode;
  }
}


/**************************************************************/
/*********** ProbabilisticSymbolList ******************/
/**************************************************************/


void SymbolListTools::getCounts(const ProbabilisticSymbolList& list, map<int, double>& counts, bool resolveUnknowns)
{
  if (!resolveUnknowns)
  {
    getCounts(list, counts);
  }
  else
  {
    for (size_t i = 0; i < list.size(); ++i)
    {
      const std::vector<double>& c=list[i];
      double s=VectorTools::sum(c);

      if (s!=0)
        for (size_t j=0; j < c.size(); j++)
          counts[(int)j]+=c.at(j)/s;
    }
  }
}


void SymbolListTools::getCounts(const ProbabilisticSymbolList& list1, const ProbabilisticSymbolList& list2,  map< int, map<int, double> >& counts, bool resolveUnknowns)
{
  if (!resolveUnknowns)
  {
    getCounts(list1, list2, counts);for (size_t i = 0; i < list1.size(); i++);
  }
  else
  {
    if(list1.size() != list2.size()) throw DimensionException("SymbolListTools::getCounts: the two sites must have the same size.", list1.size(), list2.size());
    for(size_t i = 0; i < list1.size(); i++)
    {
      const std::vector<double>& c1(list1[i]), &c2(list2[i]);;
      double s12=VectorTools::sum(c1)*VectorTools::sum(c2);
      if ((s12!=0))
        for (size_t j=0; j < c1.size(); j++)
          for (size_t k=0; k < c2.size(); k++)
            counts[(int)j][(int)k]+=c1.at(j)*c2.at(k)/s12;
    }
  }
}

double SymbolListTools::getGCContent(const ProbabilisticSymbolList& list, bool ignoreUnresolved, bool ignoreGap)
{
  const Alphabet * alphabet = list.getAlphabet();
  if (!AlphabetTools::isNucleicAlphabet(alphabet))
    throw AlphabetException("SymbolListTools::getGCContent. Method only works on nucleotides.", alphabet);
  double gc = 0;
  double total = 0;
  for (size_t i = 0; i < list.size(); i++) {
    const Vdouble& state = list.getValue(i);
    double ss=VectorTools::sum(state);
    if (ss!=0) { // not a gap
      if (ss<1)
      {
        total++;
        gc+= state.at(1) + state.at(2);
      }
      else if (!ignoreUnresolved)
      {
        total++;
        gc+= (state.at(1) + state.at(2))/ss;
      }
    }else {
        if (!ignoreGap) total++;
    }
  }
  
  return total != 0 ? gc/total : 0;
}

size_t SymbolListTools::getNumberOfDistinctPositions(const ProbabilisticSymbolList& l1, const ProbabilisticSymbolList& l2)
{
  if (l1.getAlphabet()->getAlphabetType() != l2.getAlphabet()->getAlphabetType()) throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
  
  size_t n = min(l1.size(), l2.size());
  size_t count = 0;
  for (size_t i = 0; i < n; i++) {
    if (l1[i] != l2[i])
      count++;
  }
  return count;
}

size_t SymbolListTools::getNumberOfPositionsWithoutGap(const ProbabilisticSymbolList& l1, const ProbabilisticSymbolList& l2)
{
  if (l1.getAlphabet() -> getAlphabetType() != l2.getAlphabet() -> getAlphabetType()) throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
  size_t n = min(l1.size(), l2.size());
  size_t count = 0;
  for (size_t i = 0; i < n; i++) {
    if (VectorTools::sum(l1[i]) > NumConstants::TINY() && VectorTools::sum(l2[i]) > NumConstants::TINY())
      count++;
  }
  return count;
}

void SymbolListTools::changeGapsToUnknownCharacters(ProbabilisticSymbolList& l)
{
  for (size_t i = 0; i < l.size(); i++)
  {
    if (VectorTools::sum(l[i])<NumConstants::TINY())
      VectorTools::fill(l[i],1.);
  }
}

void SymbolListTools::changeUnresolvedCharactersToGaps(ProbabilisticSymbolList& l)
{
  for (size_t i = 0; i < l.size(); i++)
  {
    if (VectorTools::sum(l[i])>1.)
      VectorTools::fill(l[i],0.);
  }
}

double SymbolListTools::variabilityShannon(const CruxSymbolList& site, bool resolveUnknown)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::variabilityShannon: Incorrect specified site, size must be > 0.");
  
  map<int, double> p;
  getFrequencies(site, p, resolveUnknown);
  // We need to correct frequencies for gaps:
  double s = 0.;
  for (int i = 0; i < static_cast<int>(site.getAlphabet()->getSize()); i++)
  {
    double f = p[i];
    if (f > 0)
      s += f * log(f);
  }
  return -s;
}

/******************************************************************************/



double SymbolListTools::mutualInformation(const CruxSymbolList& site1, const CruxSymbolList& site2, bool resolveUnknown)
{
  // Empty site checking
  if (site1.size() == 0)
    throw Exception("SymbolListTools::mutualInformation: Incorrect specified site, size must be > 0");
  if (site2.size() == 0)
    throw Exception("SymbolListTools::mutualInformation: Incorrect specified site, size must be > 0");
  if (site1.size() != site2.size())
    throw DimensionException("SymbolListTools::mutualInformation: sites must have the same size!", site1.size(), site2.size());
  vector<double> p1(site1.getAlphabet()->getSize());
  vector<double> p2(site2.getAlphabet()->getSize());
  map<int, map<int, double> > p12;
  getCounts(site1, site2, p12, resolveUnknown);
  double mi = 0, tot = 0, pxy;
  // We need to correct frequencies for gaps:
  for (size_t i = 0; i < site1.getAlphabet()->getSize(); i++)
  {
    for (size_t j = 0; j < site2.getAlphabet()->getSize(); j++)
    {
      pxy = p12[static_cast<int>(i)][static_cast<int>(j)];
      tot += pxy;
      p1[i] += pxy;
      p2[j] += pxy;
    }
  }
  for (size_t i = 0; i < site1.getAlphabet()->getSize(); i++)
  {
    p1[i] /= tot;
  }
  for (size_t j = 0; j < site2.getAlphabet()->getSize(); j++)
  {
    p2[j] /= tot;
  }
  for (size_t i = 0; i < site1.getAlphabet()->getSize(); i++)
  {
    for (size_t j = 0; j < site2.getAlphabet()->getSize(); j++)
    {
      pxy = p12[static_cast<int>(i)][static_cast<int>(j)] / tot;
      if (pxy > 0)
        mi += pxy * log(pxy / (p1[i] * p2[j]));
    }
  }
  return mi;
}

/******************************************************************************/

double SymbolListTools::jointEntropy(const CruxSymbolList& site1, const CruxSymbolList& site2, bool resolveUnknown)
{
  // Empty site checking
  if (site1.size() == 0)
    throw Exception("SymbolListTools::jointEntropy: Incorrect specified site, size must be > 0");
  if (site2.size() == 0)
    throw Exception("SymbolListTools::jointEntropy: Incorrect specified site, size must be > 0");
  if (site1.size() != site2.size())
    throw DimensionException("SymbolListTools::jointEntropy: sites must have the same size!", site1.size(), site2.size());
  map<int, map<int, double> > p12;
  getCounts(site1, site2, p12, resolveUnknown);
  double tot = 0, pxy, h = 0;
  // We need to correct frequencies for gaps:
  for (size_t i = 0; i < site1.getAlphabet()->getSize(); i++)
  {
    for (size_t j = 0; j < site2.getAlphabet()->getSize(); j++)
    {
      pxy = p12[static_cast<int>(i)][static_cast<int>(j)];
      tot += pxy;
    }
  }
  for (size_t i = 0; i < site1.getAlphabet()->getSize(); i++)
  {
    for (size_t j = 0; j < site2.getAlphabet()->getSize(); j++)
    {
      pxy = p12[static_cast<int>(i)][static_cast<int>(j)] / tot;
      if (pxy > 0)
        h += pxy * log(pxy);
    }
  }
  return -h;
}

/******************************************************************************/

double SymbolListTools::variabilityFactorial(const IntCoreSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::variabilityFactorial: Incorrect specified site, size must be > 0");
  map<int, size_t> p;
  getCounts(site, p);
  vector<size_t> c = MapTools::getValues(p);
  size_t s = VectorTools::sum(c);
  long double l = static_cast<long double>(NumTools::fact(s)) / static_cast<long double>(VectorTools::sum(VectorTools::fact(c)));
  return (static_cast<double>(std::log(l)));
}

/******************************************************************************/

double SymbolListTools::heterozygosity(const CruxSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::heterozygosity: Incorrect specified site, size must be > 0");
  map<int, double> p;
  getFrequencies(site, p);
  vector<double> c = MapTools::getValues(p);
  double n = VectorTools::norm<double, double>(MapTools::getValues(p));
  return 1. - n * n;
}

/******************************************************************************/

size_t SymbolListTools::getNumberOfDistinctCharacters(const IntCoreSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::getNumberOfDistinctCharacters(): Incorrect specified site, size must be > 0");
  // For all site's characters
  if (SymbolListTools::isConstant(site))
    return 1;
  map<int, size_t> counts;
  SymbolListTools::getCounts(site, counts);
  size_t s = 0;
  for (map<int, size_t>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if (it->second != 0)
      s++;
  }
  return s;
}

/******************************************************************************/

size_t SymbolListTools::getMajorAlleleFrequency(const IntCoreSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::getMajorAlleleFrequency(): Incorrect specified site, size must be > 0");
  // For all site's characters
  if (SymbolListTools::isConstant(site))
    return site.size();
  map<int, size_t> counts;
  getCounts(site, counts);
  size_t s = 0;
  for (map<int, size_t>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if (it->second != 0)
      if (it->second > s)
        s = it->second;
  }
  return s;
}

/******************************************************************************/

int SymbolListTools::getMajorAllele(const CruxSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::getMajorAllele(): Incorrect specified site, size must be > 0");
  // For all site's characters
  if (dynamic_cast<const IntCoreSymbolList*>(&site) && SymbolListTools::isConstant(site))
    return (dynamic_cast<const IntCoreSymbolList&>(site))[0];
  
  map<int, double> counts;
  SymbolListTools::getCounts(site, counts);
  double s = 0;
  int ma = -100;
  for (auto it : counts)
  {
    if (it.second != 0)
      if (it.second > s) {
        s = it.second;
        ma = it.first;
      }
  }
  return ma;
}

/******************************************************************************/

size_t SymbolListTools::getMinorAlleleFrequency(const IntCoreSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::getMinorAlleleFrequency(): Incorrect specified site, size must be > 0.");
  // For all site's characters
  if (SymbolListTools::isConstant(site))
    return site.size();
  map<int, size_t> counts;
  SymbolListTools::getCounts(site, counts);
  size_t s = site.size();
  for (auto it : counts)
  {
    if (it.second != 0)
      if (it.second < s)
        s = it.second;
  }
  return s;
}

/******************************************************************************/

int SymbolListTools::getMinorAllele(const CruxSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::getMinorAllele(): Incorrect specified site, size must be > 0.");
  // For all site's characters
  if (dynamic_cast<const IntCoreSymbolList*>(&site) && SymbolListTools::isConstant(site))
    return (dynamic_cast<const IntCoreSymbolList&>(site))[0];
  map<int, double> counts;
  SymbolListTools::getCounts(site, counts);
  double s = (double)site.size();
  int ma = -100;
  for (auto it : counts)
  {
    if (it.second != 0)
      if (it.second < s) {
        s = it.second;
        ma = it.first;
      }
  }
  return ma;
}

/******************************************************************************/

bool SymbolListTools::hasSingleton(const IntCoreSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::hasSingleton: Incorrect specified site, size must be > 0");
  // For all site's characters
  if (SymbolListTools::isConstant(site))
    return false;
  map<int, size_t> counts;
  getCounts(site, counts);
  for (map<int, size_t>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if (it->second == 1)
      return true;
  }
  return false;
}

/******************************************************************************/

bool SymbolListTools::isParsimonyInformativeSite(const IntCoreSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::isParsimonyInformativeSite: Incorrect specified site, size must be > 0");
  // For all site's characters
  if (SymbolListTools::isConstant(site, false, false))
    return false;
  map<int, size_t> counts;
  SymbolListTools::getCounts(site, counts);
  size_t npars = 0;
  for (map<int, size_t>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if (it->second > 1)
      npars++;
  }
  if (npars > 1)
    return true;
  return false;
}

/******************************************************************************/

bool SymbolListTools::isTriplet(const IntCoreSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::isTriplet: Incorrect specified site, size must be > 0");
  // For all site's characters
  return (SymbolListTools::getNumberOfDistinctCharacters(site) >= 3);
}

/******************************************************************************/

bool SymbolListTools::isDoubleton(const IntCoreSymbolList& site)
{
  // Empty site checking
  if (site.size() == 0)
    throw Exception("SymbolListTools::isDoubleton: Incorrect specified site, size must be > 0");
  // For all site's characters
  return (SymbolListTools::getNumberOfDistinctCharacters(site) == 2);
}

/******************************************************************************/

