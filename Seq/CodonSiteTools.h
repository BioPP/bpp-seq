/*
 * File CodonTools.h
 * Author : Sylvain Glémin <glemin@univ-montp2.fr>
 * Last modification : ? September ? 2004
*/

#ifndef _CODONSITETOOLS_H_
#define _CODONSITETOOLS_H_

#include "SymbolListTools.h"
#include "Site.h"
#include "CodonAlphabet.h"
#include "GeneticCode.h"


// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <map>

using namespace std;

/* CodonTools static class
 * Implements methods to get informations about codons
 *
 * This class can't be instanciated
*/

class CodonSiteTools: public SymbolListTools
{
	public:
		~CodonSiteTools() {}

	public:

		/*
		 * @brief Method to know if a codon site contains gap(s) or not
		 *
		 * This method is equivalent to SiteTools::hasGap
		 * @param site a Site
		 */
		static bool hasGap(const Site & site);

		/* @brief Method to know if a condon site contains undefined characters or not
		 *
		 * This method is fully equivalent to SiteTools::hasUnknown
		 * @param site a Site
		 */
		static bool hasUnknown(const Site & site);

		/* @brief Method to know if a codon site contains stop codon or not
		 * @param site a Site
		 * @param alpha a CodonAlphabet
		 */
		static bool hasStop(const Site & site, const CodonAlphabet & alpha);

		/* @brief Method to know if a codon site is complete or not (doesn't contains gap or undefined character)
		 *
		 * This method is equivalent to SiteTools::isComplete
		 * @param site a Site
		 */
		static bool isComplete(const Site & site);

		/* @brief Method to know if a codon site is constant or not (contain one character type only)
		 *
		 * This method is equivalent to SiteTools::isConstant
		 * @param site a Site
		 */
		static bool isConstant(const Site & site) throw (EmptySiteException);

		/* @brief Method to know if a polymorphic codon site is polymorphic at only one site
		 * @param site a Site
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 */
		static bool isMonoSitePolymorphic(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca) throw (Exception);

		/* @brief Method to know if polymorphism at a codon site is synonymous
		 * @param site a Site
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 */
		static bool isSynonymousPolymorphic(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca) throw (Exception);

		/* @brief Compute the number of differences between two codons
		 * @param i a int
		 * @param j a int
		 * @param ca a CodonAlphabet
		 */
		static unsigned int numberOfDifferences(int i, int j, const CodonAlphabet & ca);

		/* @brief Compute the number of synonymous differences between two codons
		 *
		 * For complex codon:
		 * If minchange = false (default option) the different paths are equally weighted
		 * If minchange = true the path with the minimum number of non-synonymous change is chosen
		 * paths included stop codons are excluded
		 * @param i a int
		 * @param j a int
		 * @param ca a CondonAlphabet
		 * @param gc a GeneticCode
		 * @param minchange a boolean set by default to false
		 */
    static double numberOfSynonymousDifferences(int i, int j, const CodonAlphabet & ca, const GeneticCode & gc, bool minchange=false);

		/* @brief Compute the synonymous pi per codon site
		 *
		 * the following formula is used:
		 * @f[
		 * pi = frac{n}{n-1}\sum_{i,j}x_{i}x_{j}P_{ij}\
		 * @f]
		 * where n is the number of sequence, Xi and Xj the frequencies of each codon type occuring at the site
		 * Pij the number of synonymous difference between these codons
		 * Be careful: here, pi is not normalized by the number of synonymous sites
		 * @param site a Site
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @minchange a bollean set by default to false
		 */
		static double piSynonymous(const Site & site, const CodonAlphabet & ca, const GeneticCode & gc, bool minchange=false) throw(Exception);

		/* @brief Compute the non-synonymous pi per codon site
		 *
		 * the following formula is used:
		 * @f[
		 * pi = frac{n}{n-1}\sum_{i,j}x_{i}x_{j}P_{ij}\
		 * @f]
		 * where n is the number of sequence, Xi and Xj the frequencies of each codon type occuring at the site
		 * Pij the number of nonsynonymous difference between these codons
		 * Be careful: here, pi is not normalized by the number of non-synonymous sites
		 * @param site a Site
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @minchange a bollean set by default to false
		 */
		static double piNonSynonymous(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca, const GeneticCode & gc, bool minchange=false) throw(Exception);

		/* @brief Return the number of synonymous positions of a codon
		 *
		 * a site is consider as x% synonymous if x% of the possible mutations are synonymous
		 * Transition/transversion ratio can be taken into account (use the variable ratio)
		 * @param i a int
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @param ratio a double set by default to 1
		 */
		static double NumberOfSynonymousPositions(int i, const CodonAlphabet & ca, const GeneticCode & gc, double ratio=1);

		/* @brief Return the mean number of synonymous position per codon site
		 *
		 * a site is consider as x% synonymous if x% of the possible mutations are synonymous
		 * Transition/transversion ratio can be taken into account (use the variable ratio)
		 * @param site a Site
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @param ratio a double set by default to 1
		 */
		static double MeanNumberOfSynonymousPositions(const Site & site,  const CodonAlphabet & ca, const GeneticCode & gc, double ratio=1) throw(Exception);


};

#endif	//_CONDONSITETOOLS_H_
