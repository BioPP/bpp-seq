//
// File: SymbolListTools.h
// Authors:
//   Julien Dutheil
// Created: 2004-04-09 00:00:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_SYMBOLLISTTOOLS_H
#define BPP_SEQ_SYMBOLLISTTOOLS_H

#include <Bpp/Numeric/VectorExceptions.h>

#include "Alphabet/AlphabetExceptions.h"
#include "IntSymbolList.h"
#include "ProbabilisticSymbolList.h"

// From the STL:
#include <map>

namespace bpp
{
/**
 * @brief Utilitary functions dealing with both sites and sequences.
 */
class SymbolListTools
{
public:
  SymbolListTools() {}
  virtual ~SymbolListTools() {}

public:
  /**
   * @param site A site.
   * @return True if the site contains one or several gap(s).
   */
  static bool hasGap(const IntSymbolListInterface& site);
  static bool hasGap(const ProbabilisticSymbolListInterface& site);

  static bool hasGap(const CruxSymbolListInterface& site)
  {
    try {
      return hasGap(dynamic_cast<const ProbabilisticSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    try {
      return hasGap(dynamic_cast<const IntSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::hasGap : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @param site A site.
   * @return True if the site contains one or several unresolved state.
   */
  static bool hasUnresolved(const IntSymbolListInterface& site);

  /**
   * @param site A site.
   * @return True if the site contains only gaps.
   */
  static bool isGapOnly(const IntSymbolListInterface& site);
  static bool isGapOnly(const ProbabilisticSymbolListInterface& site);

  static bool isGapOnly(const CruxSymbolListInterface& site)
  {
    try {
      return isGapOnly(dynamic_cast<const ProbabilisticSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    try {
      return isGapOnly(dynamic_cast<const IntSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::numberOfGaps : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @param site A site.
   * @return the numbed of gaps.
   */
  static size_t numberOfGaps(const IntSymbolListInterface& site);
  static size_t numberOfGaps(const ProbabilisticSymbolListInterface& site);

  static size_t numberOfGaps(const CruxSymbolListInterface& site)
  {
    try {
      return numberOfGaps(dynamic_cast<const ProbabilisticSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    try {
      return numberOfGaps(dynamic_cast<const IntSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::numberOfGaps : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @param site A site.
   * @return True if the site contains only gaps.
   */
  static bool isGapOrUnresolvedOnly(const IntSymbolListInterface& site);
  static bool isGapOrUnresolvedOnly(const ProbabilisticSymbolListInterface& site);

  static bool isGapOrUnresolvedOnly(const CruxSymbolListInterface& site)
  {
    try {
      return isGapOrUnresolvedOnly(dynamic_cast<const ProbabilisticSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    try {
      return isGapOrUnresolvedOnly(dynamic_cast<const IntSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::isGapOrUnresolvedOnly : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @param site A site.
   * @return the numbed of unresolved.
   */
  static size_t numberOfUnresolved(const IntSymbolListInterface& site);
  static size_t numberOfUnresolved(const ProbabilisticSymbolListInterface& site);

  static size_t numberOfUnresolved(const CruxSymbolListInterface& site)
  {
    try {
      return numberOfUnresolved(dynamic_cast<const ProbabilisticSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    try {
      return numberOfUnresolved(dynamic_cast<const IntSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::numberOfUnresolved : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @param site A site.
   * @return True if the site contains one or several unknwn characters.
   */
  static bool hasUnknown(const IntSymbolListInterface& site);
  static bool hasUnknown(const ProbabilisticSymbolListInterface& site);

  static bool hasUnknown(const CruxSymbolListInterface& site)
  {
    try {
      return hasUnknown(dynamic_cast<const ProbabilisticSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    try {
      return hasUnknown(dynamic_cast<const IntSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::hasUnknown : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @param site A site.
   * @return True if the site contains no gap and no unknown characters.
   */
  static bool isComplete(const IntSymbolListInterface& site);
  static bool isComplete(const ProbabilisticSymbolListInterface& site);

  static bool isComplete(const CruxSymbolListInterface& site)
  {
    try {
      return isComplete(dynamic_cast<const ProbabilisticSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    try {
      return isComplete(dynamic_cast<const IntSymbolListInterface&>(site));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::isComplete : unsupported CruxSymbolListInterface implementation.");
  }


  /**
   * @brief Tell if a site is constant, that is displaying the same state in all sequences that do not present a gap.
   *
   * @param site A site.
   * @param ignoreUnknown If true, positions with unknown positions will be ignored.
   * Otherwise, a site with one single state + any uncertain state will not be considered as constant.
   * @param unresolvedRaisesException In case of ambiguous case (gap only site for instance), throw an exception. Otherwise returns false.
   * @return True if the site is made of only one state.
   */
  static bool isConstant(
		  const IntSymbolListInterface& site,
		  bool ignoreUnknown = false,
		  bool unresolvedRaisesException = true);

  static bool isConstant(
		  const ProbabilisticSymbolListInterface& site,
		  bool unresolvedRaisesException = true);

  static bool isConstant(
		  const CruxSymbolListInterface& site,
		  bool ignoreUnknown = false,
		  bool unresolvedRaisesException = true)
  {
    try {
      return isConstant(dynamic_cast<const ProbabilisticSymbolListInterface&>(site), unresolvedRaisesException);
    } catch(std::bad_cast&) {}
    try {
      return isConstant(dynamic_cast<const IntSymbolListInterface&>(site), ignoreUnknown, unresolvedRaisesException);
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::isConstant : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @param list1 The first site.
   * @param list2 The second site.
   * @return True if the two sites have the same content (and, of course, alphabet).
   */
  static bool areSymbolListsIdentical(
		  const IntSymbolListInterface& list1,
		  const IntSymbolListInterface& list2);

  static bool areSymbolListsIdentical(
		  const ProbabilisticSymbolListInterface& list1,
		  const ProbabilisticSymbolListInterface& list2);

  static bool areSymbolListsIdentical(
		  const CruxSymbolListInterface& l1,
		  const CruxSymbolListInterface& l2)
  {
    try {
      return areSymbolListsIdentical(dynamic_cast<const ProbabilisticSymbolListInterface&>(l1), dynamic_cast<const ProbabilisticSymbolListInterface&>(l2));
    } catch(std::bad_cast&) {}
    try {
      return areSymbolListsIdentical(dynamic_cast<const IntSymbolListInterface&>(l1), dynamic_cast<const IntSymbolListInterface&>(l2));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::areSymbolListsIdentical : unsupported CruxSymbolListInterface implementation.");
  }


  /**
   * @brief Count all states in the list.
   *
   * @author J. Dutheil
   * @param list The list.
   * @param counts The output map to store the counts (existing counts will be incremented).
   */
  template<class count_type>
  static void getCounts(
      const IntSymbolListInterface& list,
      std::map<int, count_type>& counts)
  {
    for (size_t i = 0; i < list.size(); ++i)
    {
      counts[list[i]]++;
    }
  }

  /**
   * @brief Sum all states in the list.
   *
   * @param list The list.
   * @param counts The output map to store the sum for all states
   * (existing counts will be summed).
   */
  static void getCounts(
      const ProbabilisticSymbolListInterface& list,
      std::map<int, double_t>& counts)
  {
    for (size_t i = 0; i < list.size(); ++i)
    {
      const std::vector<double>& c = list[i];
      for (size_t j = 0; j < c.size(); ++j)
      {
        counts[(int)j] += c.at(j);
      }
    }
  }

  /**
   * @brief Count all states in the list normalizing unknown characters.
   *
   * For instance, (1,1,1,1) will be counted as (1/4,1/4,1/4,1/4).
   *
   * @author J. Dutheil
   * @param list The list.
   * @param counts The output map to store the counts (existing ocunts will be incremented).
   * @return A map with all states and corresponding counts.
   */
  static void getCountsResolveUnknowns(
      const IntSymbolListInterface& list,
      std::map<int, double>& counts)
  {
    for (size_t i = 0; i < list.size(); ++i)
    {
      std::vector<int> alias = list.getAlphabet()->getAlias(list[i]);
      double n = static_cast<double>(alias.size());
      for (auto j : alias)
      {
        counts[j] += 1. / n;
      }
    }
  }

  /**
   * @brief Count all states in the list normalizing unknown characters.
   *
   * For instance, (1,1,1,1) will be counted as (1/4,1/4,1/4,1/4).
   *
   * @author J. Dutheil
   * @param list The list.
   * @param counts The output map to store the counts (existing ocunts will be incremented).
   * @return A map with all states and corresponding counts.
   */
  static void getCountsResolveUnknowns(
		  const ProbabilisticSymbolListInterface& list,
		  std::map<int, double>& counts)
  {
    for (size_t i = 0; i < list.size(); ++i)
    {
      const std::vector<double>& c = list[i];
      double s = VectorTools::sum(c);

      if (s != 0)
        for (size_t j = 0; j < c.size(); j++)
        {
          counts[(int)j] += c.at(j) / s;
        }
    }
  }


  /**
   * @brief Count all states in the list, optionaly resolving unknown characters.
   *
   * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
   *
   * @author J. Dutheil
   * @param list The list.
   * @param counts The output map to store the counts (existing ocunts will be incremented).
   * @param resolveUnknowns Tell is unknown characters must be resolved.
   * @return A map with all states and corresponding counts.
   */
  static void getCounts(
		  const CruxSymbolListInterface& list,
		  std::map<int, double>& counts,
		  bool resolveUnknowns = false)
  {
    try {
      if (resolveUnknowns) {
        getCountsResolveUnknowns(dynamic_cast<const ProbabilisticSymbolListInterface&>(list), counts);
	return;
      } else {
        getCounts(dynamic_cast<const ProbabilisticSymbolListInterface&>(list), counts);
	return;
      }
    } catch (std::bad_cast&) {}
    
    try {
      if (resolveUnknowns) {
        getCountsResolveUnknowns(dynamic_cast<const IntSymbolListInterface&>(list), counts);
	return;
      } else {
	getCounts<double>(dynamic_cast<const IntSymbolListInterface&>(list), counts);
	return;
      }
    } catch (std::bad_cast&) {}

    throw Exception("SymbolListTools::getCounts : unsupported CruxSymbolListInterface implementation (" + std::string(typeid(list).name()) + ").");
  }


  /**
   * @brief Count all pair of states for two lists of the same size.
   *
   * NB: The two lists do node need to share the same alphabet!
   * The states of the first list will be used as the first index in the output,
   * and the ones from the second list as the second index.
   *
   * @author J. Dutheil
   * @param list1 The first list.
   * @param list2 The second list.
   * @param counts The output map to store the counts (existing counts will be incremented).
   */
  template<class count_type>
  static void getCounts(
		  const IntSymbolListInterface& list1,
		  const IntSymbolListInterface& list2,
		  std::map<int, std::map<int, count_type> >& counts)
  {
    if (list1.size() != list2.size()) throw DimensionException("SymbolListTools::getCounts: the two sites must have the same size.", list1.size(), list2.size());
    for (size_t i = 0; i < list1.size(); ++i)
    {
      counts[list1[i]][list2[i]]++;
    }
  }

  /**
   * @brief Sum along the lists the joined probabilities for all
   * pair of states for two lists of the same size.
   *
   * NB: The two lists do node need to share the same alphabet!
   * The states of the first list will be used as the first index in the output,
   * and the ones from the second list as the second index.
   *
   * @author J. Dutheil
   * @param list1 The first list.
   * @param list2 The second list.
   * @param counts The output map to store the counts (existing counts will be summed).
   */
  static void getCounts(
		  const ProbabilisticSymbolListInterface& list1,
		  const ProbabilisticSymbolListInterface& list2,
		  std::map<int, std::map<int, double> >& counts)
  {
    if (list1.size() != list2.size()) throw DimensionException("SymbolListTools::getCounts: the two sites must have the same size.", list1.size(), list2.size());
    for (size_t i = 0; i < list1.size(); ++i)
    {
      const std::vector<double>& c1(list1[i]), &c2(list2[i]);
      for (size_t j = 0; j < c1.size(); ++j)
      {
        for (size_t k = 0; k < c2.size(); ++k)
        {
          counts[(int)j][(int)k] += c1.at(j) * c2.at(k);
        }
      }
    }
  }

	
  /**
   * @brief Count all pairs of states for two lists of the same size resolving unknown characters.
   *
   * For instance, (1,1,1,1) will be counted as (1/4,1/4,1/4,1/4).
   *
   * NB: The two lists do node need to share the same alphabet!
   * The states of the first list will be used as the first index in the output,
   * and the ones from the second list as the second index.
   *
   * @author J. Dutheil
   * @param list1 The first list.
   * @param list2 The second list.
   * @param counts The output map to store the counts (existing ocunts will be incremented).
   * @return A map with all states and corresponding counts.
   */
  static void getCountsResolveUnknowns(
      const IntSymbolListInterface& list1,
      const IntSymbolListInterface& list2,
      std::map< int, std::map<int, double> >& counts);

  /**
   * @brief Count all pairs of states for two lists of the same size resolving unknown characters.
   *
   * For instance, (1,1,1,1) will be counted as (1/4,1/4,1/4,1/4).
   *
   * NB: The two lists do node need to share the same alphabet!
   * The states of the first list will be used as the first index in the output,
   * and the ones from the second list as the second index.
   *
   * @author J. Dutheil
   * @param list1 The first list.
   * @param list2 The second list.
   * @param counts The output map to store the counts (existing ocunts will be incremented).
   * @return A map with all states and corresponding counts.
   */
  static void getCountsResolveUnknowns(
      const ProbabilisticSymbolListInterface& list1,
      const ProbabilisticSymbolListInterface& list2,
      std::map< int, std::map<int, double> >& counts);

  /**
   * @brief Count all pairs of states for two lists of the same size, optionaly resolving unknown characters.
   *
   * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
   *
   * NB: The two lists do node need to share the same alphabet!
   * The states of the first list will be used as the first index in the output,
   * and the ones from the second list as the second index.
   *
   * @author J. Dutheil
   * @param list1 The first list.
   * @param list2 The second list.
   * @param counts The output map to store the counts (existing ocunts will be incremented).
   * @param resolveUnknowns Tell is unknown characters must be resolved.
   * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
   * @return A map with all states and corresponding counts.
   */
  static void getCounts(
      const CruxSymbolListInterface& list1,
      const CruxSymbolListInterface& list2,
      std::map<int, std::map<int, double> >& counts,
      bool resolveUnknowns)
  {
    try {
      if (resolveUnknowns) {
        getCountsResolveUnknowns(dynamic_cast<const ProbabilisticSymbolListInterface&>(list1), dynamic_cast<const ProbabilisticSymbolListInterface&>(list2), counts);
	return;
      } else {
        getCounts(dynamic_cast<const ProbabilisticSymbolListInterface&>(list1), dynamic_cast<const ProbabilisticSymbolListInterface&>(list2), counts);
	return;
      }
    } catch (std::bad_cast&) {}

    try {
      if (resolveUnknowns) {
        getCountsResolveUnknowns(dynamic_cast<const IntSymbolListInterface&>(list1), dynamic_cast<const IntSymbolListInterface&>(list2), counts);
	return;
      } else {
        getCounts<double>(dynamic_cast<const IntSymbolListInterface&>(list1), dynamic_cast<const IntSymbolListInterface&>(list2), counts);
	return;
      }
    } catch(std::bad_cast&) {}
    
    throw Exception("SymbolListTools::getCounts : unsupported CruxSymbolListInterface implementation (" + std::string(typeid(list1).name()) + ", " + std::string(typeid(list2).name()) + ").");
  }



  /**
   * @brief Get all states frequencies in the list.
   *
   * @author J. Dutheil
   * @param list The list.
   * @param resolveUnknowns Tell is unknown characters must be resolved.
   * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
   * @param frequencies The output map with all states and corresponding frequencies. Existing frequencies will be erased if any.
   */
  static void getFrequencies(
      const CruxSymbolListInterface& list,
      std::map<int, double>& frequencies,
      bool resolveUnknowns = false);

  /**
   * @brief Get all state pairs frequencies for two lists of the same size.
   *
   * @author J. Dutheil
   * @param list1 The first list.
   * @param list2 The second list.
   * @param resolveUnknowns Tell is unknown characters must be resolved.
   * For instance, in DNA, N will be counted as
   * A=1/4,T=1/4,C=1/4,G=1/4. For ProbabilisticSymbolList, (1,1,1,1)
   * states will be counted as (1/4,1/4,1/4,1/4).
   * @param frequencies The output map with all state pairs and corresponding frequencies. Existing frequencies will be erased if any.
   */

  static void getFrequencies(
      const CruxSymbolListInterface& list1,
      const CruxSymbolListInterface& list2,
      std::map<int, std::map<int, double> >& frequencies,
      bool resolveUnknowns = false);

  /**
   * @brief Get the GC content of a symbol list.
   *
   * @param list The list.
   * @return The proportion of G and C states in the list.
   * @param ignoreUnresolved Do not count unresolved states (or
   * columns that sum > 1). Otherwise, weight by each state
   * probability in case of ambiguity (e.g. the R state counts for
   * 0.5) (or columns are normalized).
   * @param ignoreGap Do not count gaps (or null columns) in total
   * @throw AlphabetException If the list is not made of nucleotide states.
   */
  static double getGCContent(
      const IntSymbolListInterface& list,
      bool ignoreUnresolved = true,
      bool ignoreGap = true);

  static double getGCContent(
      const ProbabilisticSymbolListInterface& list,
      bool ignoreUnresolved = true,
      bool ignoreGap = true);

  static double getGCContent(
      const CruxSymbolListInterface& list,
      bool ignoreUnresolved = true,
      bool ignoreGap = true)
  {
    try {
      return getGCContent(dynamic_cast<const ProbabilisticSymbolListInterface&>(list));
    } catch(std::bad_cast&) {}
    try {
      return getGCContent(dynamic_cast<const IntSymbolListInterface&>(list));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::getGCContent : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @brief Get the number of distinct positions.
   *
   * The comparison in achieved from position 0 to the minimum size of the two vectors.
   *
   * @param l1 SymbolList 1.
   * @param l2 SymbolList 2.
   * @return The number of distinct positions.
   * @throw AlphabetMismatchException if the two lists have not the same alphabet type.
   */

  static size_t getNumberOfDistinctPositions(
		  const IntSymbolListInterface& l1,
		  const IntSymbolListInterface& l2);

  static size_t getNumberOfDistinctPositions(
		  const ProbabilisticSymbolListInterface& l1,
		  const ProbabilisticSymbolListInterface& l2);

  static size_t getNumberOfDistinctPositions(
		  const CruxSymbolListInterface& l1,
		  const CruxSymbolListInterface& l2)
  {
    try {
      return getNumberOfDistinctPositions(dynamic_cast<const ProbabilisticSymbolListInterface&>(l1), dynamic_cast<const ProbabilisticSymbolListInterface&>(l2));
    } catch(std::bad_cast&) {}
    try {
      return getNumberOfDistinctPositions(dynamic_cast<const IntSymbolListInterface&>(l1), dynamic_cast<const IntSymbolListInterface&>(l2));
    } catch(std::bad_cast&) {}
    throw Exception("SymbolListTools::getNumberOfDistinctPositions : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @brief Get the number of positions without gap (or without null column).
   *
   * The comparison in achieved from position 0 to the minimum size of the two vectors.
   *
   * @param l1 SymbolList 1.
   * @param l2 SymbolList 2.
   * @return The number of positions without gap (or columns with at
   * least a non zero value)
   * @throw AlphabetMismatchException if the two lists have not the same alphabet type.
   */
  static size_t getNumberOfPositionsWithoutGap(
		  const IntSymbolListInterface& l1,
		  const IntSymbolListInterface& l2);

  static size_t getNumberOfPositionsWithoutGap(
		  const ProbabilisticSymbolListInterface& l1,
		  const ProbabilisticSymbolListInterface& l2);

  static size_t getNumberOfPositionsWithoutGap(
		  const CruxSymbolListInterface& l1,
		  const CruxSymbolListInterface& l2)
  {
    try {
      return getNumberOfPositionsWithoutGap(dynamic_cast<const ProbabilisticSymbolListInterface&>(l1), dynamic_cast<const ProbabilisticSymbolListInterface&>(l2));
    } catch (std::bad_cast&) {}
    try {
      return getNumberOfPositionsWithoutGap(dynamic_cast<const IntSymbolListInterface&>(l1), dynamic_cast<const IntSymbolListInterface&>(l2));
    } catch (std::bad_cast&) {}
    throw Exception("SymbolListTools::getNumberOfPositionsWithoutGap : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @brief Change all gap elements to unknown characters (or
   * columns of 1).
   *
   * @param l The input list of characters.
   */
  static void changeGapsToUnknownCharacters(IntSymbolListInterface& l);

  static void changeGapsToUnknownCharacters(ProbabilisticSymbolListInterface& l);

  static void changeGapsToUnknownCharacters(CruxSymbolListInterface& l)
  {
    try {
      changeGapsToUnknownCharacters(dynamic_cast<ProbabilisticSymbolListInterface&>(l));
      return;
    } catch (std::bad_cast&) {}
    try {
      changeGapsToUnknownCharacters(dynamic_cast<IntSymbolListInterface&>(l));
      return;
    } catch (std::bad_cast&) {}
    throw Exception("SymbolListTools::changeGapsToUnknownCharacters : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @brief Change all unknown characters to gap elements (or
   * columns of 0).
   *
   * @param l The input list of characters.
   */
  static void changeUnresolvedCharactersToGaps(IntSymbolListInterface& l);

  static void changeUnresolvedCharactersToGaps(ProbabilisticSymbolListInterface& l);

  static void changeUnresolvedCharactersToGaps(CruxSymbolListInterface& l)
  {
    try {
      changeUnresolvedCharactersToGaps(dynamic_cast<ProbabilisticSymbolListInterface&>(l));
      return;
    } catch (std::bad_cast&) {}
    try {
      changeUnresolvedCharactersToGaps(dynamic_cast<IntSymbolListInterface&>(l));
      return;
    } catch (std::bad_cast&) {}
    throw Exception("SymbolListTools::changeUnresolvedCharactersToGaps : unsupported CruxSymbolListInterface implementation.");
  }

  /**
   * @brief Compute the Shannon entropy index of a SymbolList.
   *
   * \f[
   * I = - \sum_x f_x\cdot \ln(f_x)
   * \f]
   * where \f$f_x\f$ is the frequency of state \f$x\f$.
   *
   * @author J. Dutheil
   * @param list A list.
   * @param resolveUnknowns Tell is unknown characters must be resolved.
   * @return The Shannon entropy index of this list.
   */
  static double variabilityShannon(
      const CruxSymbolListInterface& list,
      bool resolveUnknowns);

  /**
   * @brief Compute the factorial diversity index of a site.
   *
   * \f[
   * F = \frac{log\left(\left(\sum_x p_x\right)!\right)}{\sum_x \log(p_x)!}
   * \f]
   * where \f$p_x\f$ is the number of times state \f$x\f$ is observed in the site.
   *
   * @author J. Dutheil
   * @param list A list.
   * @return The factorial diversity index of this list.
   */
  static double variabilityFactorial(const IntSymbolListInterface& list);

  /**
   * @brief Compute the mutual information between two lists.
   *
   * \f[
   * MI = \sum_x \sum_y p_{x,y}\ln\left(\frac{p_{x,y}}{p_x \cdot p_y}\right)
   * \f]
   * where \f$p_x\f$ and \f$p_y\f$ are the frequencies of states \f$x\f$ and \f$y\f$, and
   * \f$p_{x,y}\f$ is the frequency of the pair \f$(x,y)\f$.
   *
   * @author J. Dutheil
   * @param list1 First list
   * @param list2 Second list
   * @param resolveUnknowns Tell is unknown characters must be resolved.
   * @return The mutual information for the pair of lists.
   */
  static double mutualInformation(
      const CruxSymbolListInterface& list1,
      const CruxSymbolListInterface& list2,
      bool resolveUnknowns);

  /**
   * @brief Compute the entropy of a site. This is an alias of method variabilityShannon.
   *
   * \f[
   * I = - \sum_x f_x\cdot \ln(f_x)
   * \f]
   * where \f$f_x\f$ is the frequency of state \f$x\f$.
   *
   * @author J. Dutheil
   * @param list A list.
   * @param resolveUnknowns Tell is unknown characters must be resolved.
   * @return The Shannon entropy index of this list.
   */
  static double entropy(const CruxSymbolListInterface& list, bool resolveUnknowns)
  {
    return variabilityShannon(list, resolveUnknowns);
  }

  /**
   * @brief Compute the joint entropy between two lists.
   *
   * \f[
   * H_{i,j} = - \sum_x \sum_y p_{x,y}\ln\left(p_{x,y}\right)
   * \f]
   * where \f$p_{x,y}\f$ is the frequency of the pair \f$(x,y)\f$.
   *
   * @author J. Dutheil
   * @param list1 First list
   * @param list2 Second list
   * @param resolveUnknowns Tell is unknown characters must be resolved.
   * @return The mutual information for the pair of lists.
   */
  static double jointEntropy(
      const CruxSymbolListInterface& list1,
      const CruxSymbolListInterface& list2,
      bool resolveUnknowns);

  /**
   * @brief Compute the heterozygosity index of a list.
   *
   * \f[
   * H = 1 - \sum_x f_x^2
   * \f]
   * where \f$f_x\f$ is the frequency of state \f$x\f$.
   *
   * @param list A list.
   * @return The heterozygosity index of this list.
   */
  static double heterozygosity(const CruxSymbolListInterface& list);

  /**
   * @brief Give the number of distinct characters at a list.
   *
   * @param list A list
   * @return The number of distinct characters in the given list.
   */
  static size_t getNumberOfDistinctCharacters(const IntSymbolListInterface& list);

  /**
   * @brief return the number of occurrences of the most common allele.
   *
   * @param list A list
   * @return The frequency (number of sequences) displaying the most frequent state.
   */
  static size_t getMajorAlleleFrequency(const IntSymbolListInterface& list);

  /**
   * @brief return the state corresponding to the most common allele.
   *
   * @param list A list
   * @return The most frequent state.
   */
  static int getMajorAllele(const CruxSymbolListInterface& list);

  /**
   * @brief return the number of occurrences of the least common allele.
   *
   * @param list A list
   * @return The frequency (number of sequences) displaying the less frequent state.
   */
  static size_t getMinorAlleleFrequency(const IntSymbolListInterface& list);

  /**
   * @brief return the state corresponding to the least common allele.
   *
   * @param list A list
   * @return The less frequent state.
   */
  static int getMinorAllele(const CruxSymbolListInterface& list);

  /**
   * @brief Tell if a list has singletons
   *
   *
   * @param list A list.
   * @return True if the list has singletons.
   */
  static bool hasSingleton(const IntSymbolListInterface& list);

  /**
   * @brief Tell if a site is a parsimony informative site.
   *
   * At least two distinct characters must be present.
   *
   * @param site a Site.
   * @return True if the site is parsimony informative.
   */
  static bool isParsimonyInformativeSite(const IntSymbolListInterface& site);


  /**
   * @brief Tell if a list has more than 2 distinct characters
   *
   * @param list A list.
   * @return True if the list has more than 2 distinct characters
   */
  static bool isTriplet(const IntSymbolListInterface& list);

  /**
   * @brief Tell if a list has exactly 2 distinct characters
   *
   * @param list A list.
   * @return True if the site has exactly 2 distinct characters
   */
  static bool isDoubleton(const IntSymbolListInterface& list);
};
} // end of namespace bpp.
#endif // BPP_SEQ_SYMBOLLISTTOOLS_H
