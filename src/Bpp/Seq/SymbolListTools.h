//
// File: SymbolListTools.h
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

#ifndef _SYMBOLLISTTOOLS_H_
#define _SYMBOLLISTTOOLS_H_

#include "CoreSequenceSymbolList.h"
#include "ProbabilisticSymbolList.h"
#include "Alphabet/AlphabetExceptions.h"
#include <Bpp/Numeric/VectorExceptions.h>

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
    static bool hasGap(const IntCoreSymbolList& site);
    static bool hasGap(const ProbabilisticSymbolList& site);

    static bool hasGap(const CruxSymbolList& site)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&site))
        return hasGap(dynamic_cast<const ProbabilisticSymbolList&>(site));
      else if (dynamic_cast<const IntCoreSymbolList*>(&site))
        return hasGap(dynamic_cast<const IntCoreSymbolList&>(site));
      else
        throw Exception("SymbolListTools::hasGap : this should not happen.");
    }
    /**
     * @param site A site.
     * @return True if the site contains only gaps.
     */
    static bool isGapOnly(const IntCoreSymbolList& site);
    static bool isGapOnly(const ProbabilisticSymbolList& site);

    /**
     * @param site A site.
     * @return the numbed of gaps.
     */
    static size_t numberOfGaps(const IntCoreSymbolList& site);
    static size_t numberOfGaps(const ProbabilisticSymbolList& site);

    static size_t numberOfGaps(const CruxSymbolList& site)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&site))
        return numberOfGaps(dynamic_cast<const ProbabilisticSymbolList&>(site));
      else if (dynamic_cast<const IntCoreSymbolList*>(&site))
        return numberOfGaps(dynamic_cast<const IntCoreSymbolList&>(site));
      else
        throw Exception("SymbolListTools::numberOfGaps : this should not happen.");
    }

    /**
     * @param site A site.
     * @return True if the site contains only gaps.
     */
    static bool isGapOrUnresolvedOnly(const IntCoreSymbolList& site);
    static bool isGapOrUnresolvedOnly(const ProbabilisticSymbolList& site);

    /**
     * @param site A site.
     * @return the numbed of unresolved.
     */
    static size_t numberOfUnresolved(const IntCoreSymbolList& site);
    static size_t numberOfUnresolved(const ProbabilisticSymbolList& site);

    static size_t numberOfUnresolved(const CruxSymbolList& site)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&site))
        return numberOfUnresolved(dynamic_cast<const ProbabilisticSymbolList&>(site));
      else if (dynamic_cast<const IntCoreSymbolList*>(&site))
        return numberOfUnresolved(dynamic_cast<const IntCoreSymbolList&>(site));
      else
        throw Exception("SymbolListTools::numberOfUnresolved : this should not happen.");
    }

    /**
     * @param site A site.
     * @return True if the site contains one or several unknwn characters.
     */
    static bool hasUnknown(const IntCoreSymbolList& site);
    static bool hasUnknown(const ProbabilisticSymbolList& site);

    /**
     * @param site A site.
     * @return True if the site contains no gap and no unknown characters.
     */
    static bool isComplete(const IntCoreSymbolList& site);
    static bool isComplete(const ProbabilisticSymbolList& site);

    static bool isComplete(const CruxSymbolList& site)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&site))
        return isComplete(dynamic_cast<const ProbabilisticSymbolList&>(site));
      else if (dynamic_cast<const IntCoreSymbolList*>(&site))
        return isComplete(dynamic_cast<const IntCoreSymbolList&>(site));
      else
        throw Exception("SymbolListTools::isComplete : this should not happen.");
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
    static bool isConstant(const IntCoreSymbolList& site, bool ignoreUnknown = false, bool unresolvedRaisesException = true);

    static bool isConstant(const CruxSymbolList& site, bool ignoreUnknown = false, bool unresolvedRaisesException = true)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&site))
        return isConstant(dynamic_cast<const ProbabilisticSymbolList&>(site), unresolvedRaisesException);
      else if (dynamic_cast<const IntCoreSymbolList*>(&site))
        return isConstant(dynamic_cast<const IntCoreSymbolList&>(site), ignoreUnknown, unresolvedRaisesException);
      else
        throw Exception("SymbolListTools::isConstant : this should not happen.");
    }

    static bool isConstant(const ProbabilisticSymbolList& site, bool unresolvedRaisesException = true);

    /**
     * @param site1 The first site.
     * @param site2 The second site.
     * @return True if the two sites have the same content (and, of course, alphabet).
     */
    static bool areSymbolListsIdentical(const IntCoreSymbolList& site1, const IntCoreSymbolList& site2);
    static bool areSymbolListsIdentical(const ProbabilisticSymbolList& site1, const ProbabilisticSymbolList& site2);

    static bool areSymbolListsIdentical(const CruxSymbolList& l1, const CruxSymbolList& l2)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&l1) && dynamic_cast<const ProbabilisticSymbolList*>(&l2))
        return areSymbolListsIdentical(dynamic_cast<const ProbabilisticSymbolList&>(l1), dynamic_cast<const ProbabilisticSymbolList&>(l2));
      else if (dynamic_cast<const IntCoreSymbolList*>(&l1) && dynamic_cast<const IntCoreSymbolList*>(&l2))
        return areSymbolListsIdentical(dynamic_cast<const IntCoreSymbolList&>(l1), dynamic_cast<const IntCoreSymbolList&>(l2));
      else
        throw Exception("SymbolListTools::areSymbolListsIdentical : this should not happen.");
    }


    /**
     * @brief Count all states in the list.
     *
     * @author J. Dutheil
     * @param list The list.
     * @param counts The output map to store the counts (existing counts will be incremented).

     */

    static void getCounts(const IntCoreSymbolList& list, std::map<int, size_t>& counts)
    {
      for (size_t i = 0; i < list.size(); ++i)
        counts[list[i]]++;
    }

    /**
    * @brief Sum all states in the list.
    *
    * @param list The list.
    * @param counts The output map to store the sum for all states
    * (existing counts will be summed).
    */

    static void getCounts(const ProbabilisticSymbolList& list, std::map<int, double_t>& counts)
    {
      for (size_t i = 0; i < list.size(); ++i)
      {
        const std::vector<double>& c=list[i];
        for (size_t j=0; j < c.size(); j++)
          counts[(int)j]+=c.at(j);
      }
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
    
    static void getCounts(const IntCoreSymbolList& list1, const IntCoreSymbolList& list2, std::map<int, std::map<int, size_t> >& counts)
    {
      if(list1.size() != list2.size()) throw DimensionException("SymbolListTools::getCounts: the two sites must have the same size.", list1.size(), list2.size());
      for(size_t i = 0; i < list1.size(); i++)
        counts[list1[i]][list2[i]]++;
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
    
    static void getCounts(const ProbabilisticSymbolList& list1, const ProbabilisticSymbolList& list2, std::map<int, std::map<int, double> >& counts)
    {
      if (list1.size() != list2.size()) throw DimensionException("SymbolListTools::getCounts: the two sites must have the same size.", list1.size(), list2.size());
      for (size_t i = 0; i < list1.size(); i++)
      {
        const std::vector<double>& c1(list1[i]), &c2(list2[i]);;
        for (size_t j=0; j < c1.size(); j++)
          for (size_t k=0; k < c2.size(); k++)
            counts[(int)j][(int)k]+=c1.at(j)*c2.at(k);
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

    static void getCounts(const CruxSymbolList& list, std::map<int, double>& counts, bool resolveUnknowns = false)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&list))
        getCounts(dynamic_cast<const ProbabilisticSymbolList&>(list), counts, resolveUnknowns);
      else if (dynamic_cast<const IntCoreSymbolList*>(&list))
        getCounts(dynamic_cast<const IntCoreSymbolList&>(list), counts, resolveUnknowns);
      else
        throw Exception("SymbolListTools::getCounts : this should not happen.");
    }
    

    static void getCounts(const IntCoreSymbolList& list, std::map<int, double>& counts, bool resolveUnknowns);

    /**
     * @brief Count all states in the list, optionaly normalizing unknown characters.
     *
     * For instance, (1,1,1,1) will be counted as (1/4,1/4,1/4,1/4).
     *
     * @author J. Dutheil
     * @param list The list.
     * @param counts The output map to store the counts (existing ocunts will be incremented).
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
     * @return A map with all states and corresponding counts.
     */

    static void getCounts(const ProbabilisticSymbolList& list, std::map<int, double>& counts, bool resolveUnknowns);
    
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

    static void getCounts(const CruxSymbolList& list1, const CruxSymbolList& list2, std::map<int, std::map<int, double> >& counts, bool resolveUnknowns)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&list1) && dynamic_cast<const ProbabilisticSymbolList*>(&list2))
        getCounts(dynamic_cast<const ProbabilisticSymbolList&>(list1), dynamic_cast<const ProbabilisticSymbolList&>(list2), counts, resolveUnknowns);
      else if (dynamic_cast<const IntCoreSymbolList*>(&list1) && dynamic_cast<const IntCoreSymbolList*>(&list2))
        getCounts(dynamic_cast<const IntCoreSymbolList&>(list1), dynamic_cast<const IntCoreSymbolList&>(list2), counts, resolveUnknowns);
      else
        throw Exception("SymbolListTools::getCounts : this should not happen.");
    }
    
    static void getCounts(const IntCoreSymbolList& list1, const IntCoreSymbolList& list2,  std::map< int, std::map<int, double> >& counts, bool resolveUnknowns);

    /**
     * @brief Count all pairs of states for two lists of the same size, optionaly resolving unknown characters.
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
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * @return A map with all states and corresponding counts.
     */

    static void getCounts(const ProbabilisticSymbolList& list1, const ProbabilisticSymbolList& list2,  std::map< int, std::map<int, double> >& counts, bool resolveUnknowns);

    /**
     * @brief Get all states frequencies in the list.
     *
     * @author J. Dutheil
     * @param list The list.
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
     * @param frequencies The output map with all states and corresponding frequencies. Existing frequencies will be erased if any.
     */

    static void getFrequencies(const CruxSymbolList& list, std::map<int, double>& frequencies, bool resolveUnknowns = false);

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

    static void getFrequencies(const CruxSymbolList& list1, const CruxSymbolList& list2, std::map<int, std::map<int, double> >& frequencies, bool resolveUnknowns = false);

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

    static double getGCContent(const IntCoreSymbolList& list, bool ignoreUnresolved = true, bool ignoreGap = true);

    static double getGCContent(const ProbabilisticSymbolList& list, bool ignoreUnresolved = true, bool ignoreGap = true);

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

    static size_t getNumberOfDistinctPositions(const IntCoreSymbolList& l1, const IntCoreSymbolList& l2);

    static size_t getNumberOfDistinctPositions(const ProbabilisticSymbolList& l1, const ProbabilisticSymbolList& l2);

    static size_t getNumberOfDistinctPositions(const CruxSymbolList& l1, const CruxSymbolList& l2)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&l1) && dynamic_cast<const ProbabilisticSymbolList*>(&l2))
        return getNumberOfDistinctPositions(dynamic_cast<const ProbabilisticSymbolList&>(l1), dynamic_cast<const ProbabilisticSymbolList&>(l2));
      else if (dynamic_cast<const IntCoreSymbolList*>(&l1) && dynamic_cast<const IntCoreSymbolList*>(&l2))
        return getNumberOfDistinctPositions(dynamic_cast<const IntCoreSymbolList&>(l1), dynamic_cast<const IntCoreSymbolList&>(l2));
      else
        throw Exception("SymbolListTools::getNumberOfDistinctPositions : this should not happen.");
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

    static size_t getNumberOfPositionsWithoutGap(const IntCoreSymbolList& l1, const IntCoreSymbolList& l2);

    static size_t getNumberOfPositionsWithoutGap(const ProbabilisticSymbolList& l1, const ProbabilisticSymbolList& l2);

    static size_t getNumberOfPositionsWithoutGap(const CruxSymbolList& l1, const CruxSymbolList& l2)
    {
      if (dynamic_cast<const ProbabilisticSymbolList*>(&l1) && dynamic_cast<const ProbabilisticSymbolList*>(&l2))
        return getNumberOfPositionsWithoutGap(dynamic_cast<const ProbabilisticSymbolList&>(l1), dynamic_cast<const ProbabilisticSymbolList&>(l2));
      else if (dynamic_cast<const IntCoreSymbolList*>(&l1) && dynamic_cast<const IntCoreSymbolList*>(&l2))
        return getNumberOfPositionsWithoutGap(dynamic_cast<const IntCoreSymbolList&>(l1), dynamic_cast<const IntCoreSymbolList&>(l2));
      else
        throw Exception("SymbolListTools::getNumberOfPositionsWithoutGap : this should not happen.");
    }
    
    /**
     * @brief Change all gap elements to unknown characters (or
     * columns of 1).
     *
     * @param l The input list of characters.
     */
    
    static void changeGapsToUnknownCharacters(IntCoreSymbolList& l);

    static void changeGapsToUnknownCharacters(ProbabilisticSymbolList& l);

    static void changeGapsToUnknownCharacters(CruxSymbolList& l)
    {
      if (dynamic_cast<ProbabilisticSymbolList*>(&l))
        changeGapsToUnknownCharacters(dynamic_cast<ProbabilisticSymbolList&>(l));
      else if (dynamic_cast<IntCoreSymbolList*>(&l))
        changeGapsToUnknownCharacters(dynamic_cast<IntCoreSymbolList&>(l));
      else
        throw Exception("SymbolListTools::changeGapsToUnknownCharacters : this should not happen.");
    }

    /**
     * @brief Change all unknown characters to gap elements (or
     * columns of 0).
     *
     * @param l The input list of characters.
     */
    
    static void changeUnresolvedCharactersToGaps(IntCoreSymbolList& l);

    static void changeUnresolvedCharactersToGaps(ProbabilisticSymbolList& l);

    /**
     * @brief Compute the Shannon entropy index of a SymbolList.
     *
     * \f[
     * I = - \sum_x f_x\cdot \ln(f_x)
     * \f]
     * where \f$f_x\f$ is the frequency of state \f$x\f$.
     *
     * @author J. Dutheil
     * @param site A site.
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * @return The Shannon entropy index of this site.
     */
    
    static double variabilityShannon(const CruxSymbolList& site, bool resolveUnknowns);

    /**
     * @brief Compute the factorial diversity index of a site.
     *
     * \f[
     * F = \frac{log\left(\left(\sum_x p_x\right)!\right)}{\sum_x \log(p_x)!}
     * \f]
     * where \f$p_x\f$ is the number of times state \f$x\f$ is observed in the site.
     *
     * @author J. Dutheil
     * @param site A site.
     * @return The factorial diversity index of this site.
     */
    static double variabilityFactorial(const IntCoreSymbolList& site);

    /**
     * @brief Compute the mutual information between two sites.
     *
     * \f[
     * MI = \sum_x \sum_y p_{x,y}\ln\left(\frac{p_{x,y}}{p_x \cdot p_y}\right)
     * \f]
     * where \f$p_x\f$ and \f$p_y\f$ are the frequencies of states \f$x\f$ and \f$y\f$, and
     * \f$p_{x,y}\f$ is the frequency of the pair \f$(x,y)\f$.
     *
     * @author J. Dutheil
     * @param site1 First site
     * @param site2 Second site
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * @return The mutual information for the pair of sites.
     */
    static double mutualInformation(const CruxSymbolList& site1, const CruxSymbolList& site2, bool resolveUnknowns);

    /**
     * @brief Compute the entropy of a site. This is an alias of method variabilityShannon.
     *
     * \f[
     * I = - \sum_x f_x\cdot \ln(f_x)
     * \f]
     * where \f$f_x\f$ is the frequency of state \f$x\f$.
     *
     * @author J. Dutheil
     * @param site A site.
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * @return The Shannon entropy index of this site.
     */
    static double entropy(const CruxSymbolList& site, bool resolveUnknowns) {
      return variabilityShannon(site, resolveUnknowns); 
    }


    /**
     * @brief Compute the joint entropy between two sites.
     *
     * \f[
     * H_{i,j} = - \sum_x \sum_y p_{x,y}\ln\left(p_{x,y}\right)
     * \f]
     * where \f$p_{x,y}\f$ is the frequency of the pair \f$(x,y)\f$.
     *
     * @author J. Dutheil
     * @param site1 First site
     * @param site2 Second site
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * @return The mutual information for the pair of sites.
     */
    static double jointEntropy(const CruxSymbolList& site1, const CruxSymbolList& site2, bool resolveUnknowns);

    /**
     * @brief Compute the heterozygosity index of a site.
     *
     * \f[
     * H = 1 - \sum_x f_x^2
     * \f]
     * where \f$f_x\f$ is the frequency of state \f$x\f$.
     *
     * @param site A site.
     * @return The heterozygosity index of this site.
     */
    static double heterozygosity(const CruxSymbolList& site);

    /**
     * @brief Give the number of distinct characters at a site.
     *
     * @param site a Site
     * @return The number of distinct characters in the given site.
     */
    static size_t getNumberOfDistinctCharacters(const IntCoreSymbolList& site);

    /**
     * @brief return the number of occurrences of the most common allele.
     *
     * @param site a Site
     * @return The frequency (number of sequences) displaying the most frequent state.
     */
    static size_t getMajorAlleleFrequency(const IntCoreSymbolList& site);

    /**
     * @brief return the state corresponding to the most common allele.
     *
     * @param site a Site
     * @return The most frequent state.
     */
    static int getMajorAllele(const CruxSymbolList& site);

    /**
     * @brief return the number of occurrences of the least common allele.
     *
     * @param site a Site
     * @return The frequency (number of sequences) displaying the less frequent state.
     */
    static size_t getMinorAlleleFrequency(const IntCoreSymbolList& site);

    /**
     * @brief return the state corresponding to the least common allele.
     *
     * @param site a Site
     * @return The less frequent state.
     */
    static int getMinorAllele(const CruxSymbolList& site);

    /**
     * @brief Tell if a site has singletons
     *
     *
     * @param site a Site.
     * @return True if the site has singletons.
     */
    static bool hasSingleton(const IntCoreSymbolList& site);

    /**
     * @brief Tell if a site is a parsimony informative site.
     *
     * At least two distinct characters must be present.
     *
     * @param site a Site.
     * @return True if the site is parsimony informative.
     */
    static bool isParsimonyInformativeSite(const IntCoreSymbolList& site);


    /**
     * @brief Tell if a site has more than 2 distinct characters
     *
     * @param site a Site.
     * @return True if the site has more than 2 distinct characters
     */
    static bool isTriplet(const IntCoreSymbolList& site);

    /**
     * @brief Tell if a site has exactly 2 distinct characters
     *
     * @param site a Site.
     * @return True if the site has exactly 2 distinct characters
     */
    static bool isDoubleton(const IntCoreSymbolList& site);

  };

} //end of namespace bpp.

#endif // _SYMBOLLISTTOOLS_H_

