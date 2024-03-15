// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/NumTools.h>
#include <Bpp/Numeric/Random/RandomTools.h>
#include <Bpp/Numeric/VectorTools.h>
#include <Bpp/Utils/MapTools.h>

#include "Alphabet/AlphabetTools.h"
#include "SymbolListTools.h"

// From the STL:
#include <algorithm>

using namespace std;

using namespace bpp;

/******************************************************************************/

bool SymbolListTools::hasGap(const IntSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (list.getAlphabet()->isGap(list[i]))
      return true;
  }
  return false;
}

bool SymbolListTools::hasGap(const ProbabilisticSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (VectorTools::sum(list[i]) <= NumConstants::TINY())
      return true;
  }
  return false;
}

/******************************************************************************/

bool SymbolListTools::hasUnresolved(const IntSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (list.getAlphabet()->isUnresolved(list[i]))
      return true;
  }
  return false;
}

/******************************************************************************/

bool SymbolListTools::isGapOnly(const IntSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (!list.getAlphabet()->isGap(list[i]))
      return false;
  }
  return true;
}


bool SymbolListTools::isGapOnly(const ProbabilisticSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (VectorTools::sum(list[i]) > NumConstants::TINY())
      return false;
  }
  return true;
}

/******************************************************************************/

bool SymbolListTools::isGapOrUnresolvedOnly(const IntSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (!list.getAlphabet()->isGap(list[i]) && !list.getAlphabet()->isUnresolved(list[i]))
      return false;
  }
  return true;
}

bool SymbolListTools::isGapOrUnresolvedOnly(const ProbabilisticSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    double ss = VectorTools::sum(list[i]);
    if (ss > NumConstants::TINY() && ss < 1.)
      return false;
  }
  return true;
}

/******************************************************************************/

bool SymbolListTools::hasUnknown(const IntSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (list[i] == list.getAlphabet()->getUnknownCharacterCode())
      return true;
  }
  return false;
}

bool SymbolListTools::hasUnknown(const ProbabilisticSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    double ss = VectorTools::sum(list[i]);
    if (ss > 1.)
      return true;
  }
  return false;
}

/******************************************************************************/

bool SymbolListTools::isComplete(const IntSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (list.getAlphabet()->isGap(list[i]) || list.getAlphabet()->isUnresolved(list[i]))
      return false;
  }
  return true;
}

bool SymbolListTools::isComplete(const ProbabilisticSymbolListInterface& list)
{
  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    double ss = VectorTools::sum(list[i]);
    if (ss < NumConstants::TINY())// || ss > 1.)
      return false;
  }
  return true;
}

/******************************************************************************/

size_t SymbolListTools::numberOfGaps(const IntSymbolListInterface& list)
{
  size_t n = 0;

  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (list.getAlphabet()->isGap(list[i]))
      n++;
  }
  return n;
}

size_t SymbolListTools::numberOfGaps(const ProbabilisticSymbolListInterface& list)
{
  size_t n = 0;

  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (VectorTools::sum(list[i]) < NumConstants::TINY())
      n++;
  }
  return n;
}

/******************************************************************************/

size_t SymbolListTools::numberOfUnresolved(const IntSymbolListInterface& list)
{
  size_t n = 0;

  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (list.getAlphabet()->isUnresolved(list[i]))
      n++;
  }
  return n;
}

size_t SymbolListTools::numberOfUnresolved(const ProbabilisticSymbolListInterface& list)
{
  size_t n = 0;

  // Main loop : for all characters in list
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (VectorTools::sum(list[i]) > 1.)
      n++;
  }
  return n;
}

/******************************************************************************/


bool SymbolListTools::areSymbolListsIdentical(
    const IntSymbolListInterface& list1,
    const IntSymbolListInterface& list2)
{
  // IntCoreSymbolList's size and content checking
  if (list1.getAlphabet()->getAlphabetType() != list2.getAlphabet()->getAlphabetType())
    return false;
  if (list1.size() != list2.size())
    return false;
  else
  {
    for (size_t i = 0; i < list1.size(); ++i)
    {
      if (list1[i] != list2[i])
        return false;
    }
    return true;
  }
}

bool SymbolListTools::areSymbolListsIdentical(
    const ProbabilisticSymbolListInterface& list1,
    const ProbabilisticSymbolListInterface& list2)
{
  // IntCoreSymbolList's size and content checking
  if (list1.getAlphabet()->getAlphabetType() != list2.getAlphabet()->getAlphabetType())
    return false;
  if (list1.size() != list2.size())
    return false;
  else
  {
    for (size_t i = 0; i < list1.size(); ++i)
    {
      if (list1[i] != list2[i])
        return false;
    }
    return true;
  }
}

/******************************************************************************/

bool SymbolListTools::isConstant(
    const IntSymbolListInterface& list,
    bool ignoreUnknown,
    bool unresolvedRaisesException)
{
  // Emspty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::isConstant: Incorrect specified list, size must be > 0");

  // For all list's characters
  int gap = list.getAlphabet()->getGapCharacterCode();
  if (ignoreUnknown)
  {
    int s = list[0];
    int unknown = list.getAlphabet()->getUnknownCharacterCode();
    size_t i = 0;
    while (i < list.size() && (s == gap || s == unknown))
    {
      s = list[i];
      i++;
    }
    if (s == unknown || s == gap)
    {
      if (unresolvedRaisesException)
        throw Exception("SymbolListTools::isConstant: IntCoreSymbolList is only made of gaps or generic characters.");
      else
        return false;
    }
    while (i < list.size())
    {
      if (list[i] != s && list[i] != gap && list[i] != unknown)
        return false;
      i++;
    }
  }
  else
  {
    int s = list[0];
    size_t i = 0;
    while  (i < list.size() && s == gap)
    {
      s = list[i];
      i++;
    }
    if (s == gap)
    {
      if (unresolvedRaisesException)
        throw Exception("SymbolListTools::isConstant: IntSymbolList is only made of gaps.");
      else
        return false;
    }
    while (i < list.size())
    {
      if (list[i] != s && list[i] != gap)
        return false;
      i++;
    }
  }

  return true;
}

bool SymbolListTools::isConstant(
    const ProbabilisticSymbolListInterface& list,
    bool unresolvedRaisesException)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::isConstant: Incorrect specified list, size must be > 0");

  // For all list's characters
  Vdouble s = list[0];
  double ss = VectorTools::sum(s);
  size_t i = 0;
  while (i < list.size() && (ss < NumConstants::TINY()))
  {
    s = list[i];
    ss = VectorTools::sum(s);

    i++;
  }
  if (ss < NumConstants::TINY())
  {
    if (unresolvedRaisesException)
      throw Exception("SymbolListTools::isConstant: ProbabilisticSymbolList is only made of gaps.");
    else
      return false;
  }
  while (i < list.size())
  {
    if (list[i] != s && VectorTools::sum(list[i]) < NumConstants::TINY())
      return false;
    i++;
  }

  return true;
}



void SymbolListTools::getCountsResolveUnknowns(
    const IntSymbolListInterface& list1,
    const IntSymbolListInterface& list2,
    map< int, map<int, double> >& counts)
{
  if (list1.size() != list2.size())
    throw DimensionException("SymbolListTools::getCounts: the two lists must have the same size.", list1.size(), list2.size());
  for (size_t i = 0; i < list1.size(); ++i)
  {
    vector<int> alias1 = list1.getAlphabet()->getAlias(list1[i]);
    vector<int> alias2 = list2.getAlphabet()->getAlias(list2[i]);
    double n1 = (double)alias1.size();
    double n2 = (double)alias2.size();
    for (auto j : alias1)
    {
      for (auto k : alias2)
      {
        counts[j][k] += 1. / (n1 * n2);
      }
    }
  }
}

void SymbolListTools::getFrequencies(
    const CruxSymbolListInterface& list,
    map<int, double>& frequencies,
    bool resolveUnknowns)
{
  double n = (double)list.size();
  map<int, double> counts;

  getCounts(list, counts, resolveUnknowns);

  for (auto it : counts)
  {
    frequencies[it.first] = it.second / n;
  }
}

void SymbolListTools::getFrequencies(
    const CruxSymbolListInterface& list1,
    const CruxSymbolListInterface& list2,
    map<int, map<int, double> >& frequencies,
    bool resolveUnknowns)
{
  double n2 = static_cast<double>(list1.size()) * static_cast<double>(list1.size());
  map<int, map<int, double> > counts;
  getCounts(list1, list2, counts, resolveUnknowns);

  for (auto it1 : counts)
  {
    for (auto it2 : it1.second)
    {
      frequencies[it1.first][it2.first] = it2.second / n2;
    }
  }
}

double SymbolListTools::getGCContent(
    const IntSymbolListInterface& list,
    bool ignoreUnresolved,
    bool ignoreGap)
{
  auto alphabet = list.getAlphabet();
  if (!AlphabetTools::isNucleicAlphabet(alphabet.get()))
    throw AlphabetException("SymbolListTools::getGCContent. Method only works on nucleotides.", alphabet);
  double gc = 0;
  double total = 0;
  for (size_t i = 0; i < list.size(); i++)
  {
    int state = list.getValue(i);
    if (state > -1)  // not a gap
    {
      if (state == 1 || state == 2)  // G or C
      {
        gc++;
        total++;
      }
      else if (state == 0 || state == 3)  // A, T or U
      {
        total++;
      }
      else // Unresolved character
      {
        if (!ignoreUnresolved)
        {
          total++;
          switch (state)
          {
          case (7): gc++; break;// G or C
          case (4): gc += 0.5; break;// A or C
          case (5): gc += 0.5; break;// A or G
          case (6): gc += 0.5; break;// C or T
          case (9): gc += 0.5; break;// G or T
          case (10): gc += 2. / 3.; break;// A or C or G
          case (11): gc += 1. / 3.; break;// A or C or T
          case (12): gc += 1. / 3.; break;// A or G or T
          case (13): gc += 2. / 3.; break;// C or G or T
          case (14): gc += 0.5; break;// A or C or G or T
          }
        }
      }
    }
    else
    {
      if (!ignoreGap)
        total++;
    }
  }
  return total != 0 ? gc / total : 0;
}

size_t SymbolListTools::getNumberOfDistinctPositions(
    const IntSymbolListInterface& l1,
    const IntSymbolListInterface& l2)
{
  if (l1.getAlphabet()->getAlphabetType() != l2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
  size_t n = min(l1.size(), l2.size());
  size_t count = 0;
  for (size_t i = 0; i < n; i++)
  {
    if (l1[i] != l2[i])
      count++;
  }
  return count;
}

size_t SymbolListTools::getNumberOfPositionsWithoutGap(
    const IntSymbolListInterface& l1,
    const IntSymbolListInterface& l2)
{
  if (l1.getAlphabet()->getAlphabetType() != l2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
  size_t n = min(l1.size(), l2.size());
  size_t count = 0;
  for (size_t i = 0; i < n; i++)
  {
    if (l1[i] != -1 && l2[i] != -1)
      count++;
  }
  return count;
}

void SymbolListTools::changeGapsToUnknownCharacters(IntSymbolListInterface& l)
{
  int unknownCode = l.getAlphabet()->getUnknownCharacterCode();
  for (size_t i = 0; i < l.size(); i++)
  {
    if (l.getAlphabet()->isGap(l[i]))
      l[i] = unknownCode;
  }
}

void SymbolListTools::changeUnresolvedCharactersToGaps(IntSymbolListInterface& l)
{
  int gapCode = l.getAlphabet()->getGapCharacterCode();
  for (size_t i = 0; i < l.size(); i++)
  {
    if (l.getAlphabet()->isUnresolved(l[i]))
      l[i] = gapCode;
  }
}


void SymbolListTools::getCountsResolveUnknowns(
    const ProbabilisticSymbolListInterface& list1,
    const ProbabilisticSymbolListInterface& list2,
    map< int, map<int, double> >& counts)
{
  if (list1.size() != list2.size())
    throw DimensionException("SymbolListTools::getCounts: the two lists must have the same size.", list1.size(), list2.size());
  for (size_t i = 0; i < list1.size(); ++i)
  {
    const std::vector<double>& c1(list1[i]), &c2(list2[i]);
    double s12 = VectorTools::sum(c1) * VectorTools::sum(c2);
    if ((s12 != 0))
      for (size_t j = 0; j < c1.size(); ++j)
      {
        for (size_t k = 0; k < c2.size(); ++k)
        {
          counts[(int)j][(int)k] += c1.at(j) * c2.at(k) / s12;
        }
      }
  }
}

double SymbolListTools::getGCContent(
    const ProbabilisticSymbolListInterface& list,
    bool ignoreUnresolved,
    bool ignoreGap)
{
  auto alphabet = list.getAlphabet();
  if (!AlphabetTools::isNucleicAlphabet(alphabet.get()))
    throw AlphabetException("SymbolListTools::getGCContent. Method only works on nucleotides.", alphabet);
  double gc = 0;
  double total = 0;
  for (size_t i = 0; i < list.size(); ++i)
  {
    const Vdouble& state = list.getValue(i);
    double ss = VectorTools::sum(state);
    if (ss != 0) // not a gap
    {
      if (ss < 1)
      {
        total++;
        gc += state.at(1) + state.at(2);
      }
      else if (!ignoreUnresolved)
      {
        total++;
        gc += (state.at(1) + state.at(2)) / ss;
      }
    }
    else
    {
      if (!ignoreGap)
        total++;
    }
  }

  return total != 0 ? gc / total : 0;
}

size_t SymbolListTools::getNumberOfDistinctPositions(
     const ProbabilisticSymbolListInterface& l1,
     const ProbabilisticSymbolListInterface& l2)
{
  if (l1.getAlphabet()->getAlphabetType() != l2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());

  size_t n = min(l1.size(), l2.size());
  size_t count = 0;
  for (size_t i = 0; i < n; ++i)
  {
    if (l1[i] != l2[i])
      count++;
  }
  return count;
}

size_t SymbolListTools::getNumberOfPositionsWithoutGap(
    const ProbabilisticSymbolListInterface& l1,
    const ProbabilisticSymbolListInterface& l2)
{
  if (l1.getAlphabet()->getAlphabetType() != l2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
  size_t n = min(l1.size(), l2.size());
  size_t count = 0;
  for (size_t i = 0; i < n; ++i)
  {
    if (VectorTools::sum(l1[i]) > NumConstants::TINY() && VectorTools::sum(l2[i]) > NumConstants::TINY())
      count++;
  }
  return count;
}

void SymbolListTools::changeGapsToUnknownCharacters(ProbabilisticSymbolListInterface& l)
{
  for (size_t i = 0; i < l.size(); ++i)
  {
    if (VectorTools::sum(l[i]) < NumConstants::TINY())
      VectorTools::fill(l[i], 1.);
  }
}

void SymbolListTools::changeUnresolvedCharactersToGaps(ProbabilisticSymbolListInterface& l)
{
  for (size_t i = 0; i < l.size(); ++i)
  {
    if (VectorTools::sum(l[i]) > 1.)
      VectorTools::fill(l[i], 0.);
  }
}

double SymbolListTools::variabilityShannon(
    const CruxSymbolListInterface& list,
    bool resolveUnknown)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::variabilityShannon: Incorrect specified list, size must be > 0.");

  map<int, double> p;
  getFrequencies(list, p, resolveUnknown);
  // We need to correct frequencies for gaps:
  double s = 0.;
  for (int i = 0; i < static_cast<int>(list.getAlphabet()->getSize()); i++)
  {
    double f = p[i];
    if (f > 0)
      s += f * log(f);
  }
  return -s;
}

/******************************************************************************/


double SymbolListTools::mutualInformation(
    const CruxSymbolListInterface& list1,
    const CruxSymbolListInterface& list2,
    bool resolveUnknown)
{
  // Empty list checking
  if (list1.size() == 0)
    throw Exception("SymbolListTools::mutualInformation: Incorrect specified list, size must be > 0");
  if (list2.size() == 0)
    throw Exception("SymbolListTools::mutualInformation: Incorrect specified list, size must be > 0");
  if (list1.size() != list2.size())
    throw DimensionException("SymbolListTools::mutualInformation: lists must have the same size!", list1.size(), list2.size());
  vector<double> p1(list1.getAlphabet()->getSize());
  vector<double> p2(list2.getAlphabet()->getSize());
  map<int, map<int, double> > p12;
  getCounts(list1, list2, p12, resolveUnknown);
  double mi = 0, tot = 0, pxy;
  // We need to correct frequencies for gaps:
  for (size_t i = 0; i < list1.getAlphabet()->getSize(); i++)
  {
    for (size_t j = 0; j < list2.getAlphabet()->getSize(); j++)
    {
      pxy = p12[static_cast<int>(i)][static_cast<int>(j)];
      tot += pxy;
      p1[i] += pxy;
      p2[j] += pxy;
    }
  }
  for (size_t i = 0; i < list1.getAlphabet()->getSize(); i++)
  {
    p1[i] /= tot;
  }
  for (size_t j = 0; j < list2.getAlphabet()->getSize(); j++)
  {
    p2[j] /= tot;
  }
  for (size_t i = 0; i < list1.getAlphabet()->getSize(); i++)
  {
    for (size_t j = 0; j < list2.getAlphabet()->getSize(); j++)
    {
      pxy = p12[static_cast<int>(i)][static_cast<int>(j)] / tot;
      if (pxy > 0)
        mi += pxy * log(pxy / (p1[i] * p2[j]));
    }
  }
  return mi;
}

/******************************************************************************/

double SymbolListTools::jointEntropy(
    const CruxSymbolListInterface& list1,
    const CruxSymbolListInterface& list2,
    bool resolveUnknown)
{
  // Empty list checking
  if (list1.size() == 0)
    throw Exception("SymbolListTools::jointEntropy: Incorrect specified list, size must be > 0");
  if (list2.size() == 0)
    throw Exception("SymbolListTools::jointEntropy: Incorrect specified list, size must be > 0");
  if (list1.size() != list2.size())
    throw DimensionException("SymbolListTools::jointEntropy: lists must have the same size!", list1.size(), list2.size());
  map<int, map<int, double> > p12;
  getCounts(list1, list2, p12, resolveUnknown);
  double tot = 0, pxy, h = 0;
  // We need to correct frequencies for gaps:
  for (size_t i = 0; i < list1.getAlphabet()->getSize(); ++i)
  {
    for (size_t j = 0; j < list2.getAlphabet()->getSize(); ++j)
    {
      pxy = p12[static_cast<int>(i)][static_cast<int>(j)];
      tot += pxy;
    }
  }
  for (size_t i = 0; i < list1.getAlphabet()->getSize(); ++i)
  {
    for (size_t j = 0; j < list2.getAlphabet()->getSize(); ++j)
    {
      pxy = p12[static_cast<int>(i)][static_cast<int>(j)] / tot;
      if (pxy > 0)
        h += pxy * log(pxy);
    }
  }
  return -h;
}

/******************************************************************************/

double SymbolListTools::variabilityFactorial(
    const IntSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::variabilityFactorial: Incorrect specified list, size must be > 0");
  map<int, size_t> p;
  getCounts(list, p);
  vector<size_t> c = MapTools::getValues(p);
  size_t s = VectorTools::sum(c);
  long double l = static_cast<long double>(NumTools::fact(s)) / static_cast<long double>(VectorTools::sum(VectorTools::fact(c)));
  return static_cast<double>(std::log(l));
}

/******************************************************************************/

double SymbolListTools::heterozygosity(const CruxSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::heterozygosity: Incorrect specified list, size must be > 0");
  map<int, double> p;
  getFrequencies(list, p);
  vector<double> c = MapTools::getValues(p);
  double n = VectorTools::norm<double, double>(MapTools::getValues(p));
  return 1. - n * n;
}

/******************************************************************************/

size_t SymbolListTools::getNumberOfDistinctCharacters(const IntSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::getNumberOfDistinctCharacters(): Incorrect specified list, size must be > 0");
  // For all list's characters
  if (SymbolListTools::isConstant(list))
    return 1;
  map<int, size_t> counts;
  SymbolListTools::getCounts(list, counts);
  size_t s = 0;
  for (map<int, size_t>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if (it->second != 0)
      s++;
  }
  return s;
}

/******************************************************************************/

size_t SymbolListTools::getMajorAlleleFrequency(const IntSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::getMajorAlleleFrequency(): Incorrect specified list, size must be > 0");
  // For all list's characters
  if (SymbolListTools::isConstant(list))
    return list.size();
  map<int, size_t> counts;
  getCounts(list, counts);
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

int SymbolListTools::getMajorAllele(const CruxSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::getMajorAllele(): Incorrect specified list, size must be > 0");
  // For all list's characters
  if (dynamic_cast<const IntSymbolListInterface*>(&list) && SymbolListTools::isConstant(list))
    return (dynamic_cast<const IntSymbolListInterface&>(list))[0];

  map<int, double> counts;
  SymbolListTools::getCounts(list, counts);
  double s = 0;
  int ma = -100;
  for (auto it : counts)
  {
    if (it.second != 0)
      if (it.second > s)
      {
        s = it.second;
        ma = it.first;
      }
  }
  return ma;
}

/******************************************************************************/

size_t SymbolListTools::getMinorAlleleFrequency(const IntSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::getMinorAlleleFrequency(): Incorrect specified list, size must be > 0.");
  // For all list's characters
  if (SymbolListTools::isConstant(list))
    return list.size();
  map<int, size_t> counts;
  SymbolListTools::getCounts(list, counts);
  size_t s = list.size();
  for (auto it : counts)
  {
    if (it.second != 0)
      if (it.second < s)
        s = it.second;
  }
  return s;
}

/******************************************************************************/

int SymbolListTools::getMinorAllele(const CruxSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::getMinorAllele(): Incorrect specified list, size must be > 0.");
  // For all list's characters
  if (dynamic_cast<const IntSymbolListInterface*>(&list) && SymbolListTools::isConstant(list))
    return (dynamic_cast<const IntSymbolListInterface&>(list))[0];
  map<int, double> counts;
  SymbolListTools::getCounts(list, counts);
  double s = (double)list.size();
  int ma = -100;
  for (auto it : counts)
  {
    if (it.second != 0)
      if (it.second < s)
      {
        s = it.second;
        ma = it.first;
      }
  }
  return ma;
}

/******************************************************************************/

bool SymbolListTools::hasSingleton(const IntSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::hasSingleton: Incorrect specified list, size must be > 0");
  // For all list's characters
  if (SymbolListTools::isConstant(list))
    return false;
  map<int, size_t> counts;
  getCounts(list, counts);
  for (map<int, size_t>::iterator it = counts.begin(); it != counts.end(); it++)
  {
    if (it->second == 1)
      return true;
  }
  return false;
}

/******************************************************************************/

bool SymbolListTools::isParsimonyInformativeSite(const IntSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::isParsimonyInformativeSite: Incorrect specified list, size must be > 0");
  // For all list's characters
  if (SymbolListTools::isConstant(list, false, false))
    return false;
  map<int, size_t> counts;
  SymbolListTools::getCounts(list, counts);
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

bool SymbolListTools::isTriplet(const IntSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::isTriplet: Incorrect specified list, size must be > 0");
  // For all list's characters
  return SymbolListTools::getNumberOfDistinctCharacters(list) >= 3;
}

/******************************************************************************/

bool SymbolListTools::isDoubleton(const IntSymbolListInterface& list)
{
  // Empty list checking
  if (list.size() == 0)
    throw Exception("SymbolListTools::isDoubleton: Incorrect specified list, size must be > 0");
  // For all list's characters
  return SymbolListTools::getNumberOfDistinctCharacters(list) == 2;
}

/******************************************************************************/

