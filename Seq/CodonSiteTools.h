//
// File CodonSiteTools.h
// Author : Sylvain Glémin
// Last modification : ? September ? 2004
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

		/**
		 * @brief Method to know if a codon site contains gap(s) or not
		 *
		 * This method is equivalent to SiteTools::hasGap
		 * @param site a Site
		 */
		static bool hasGap(const Site & site);

		/**
		 * @brief Method to know if a condon site contains undefined characters or not
		 *
		 * This method is fully equivalent to SiteTools::hasUnknown
		 * @param site a Site
		 */
		static bool hasUnknown(const Site & site);

		/**
		 * @brief Method to know if a codon site contains stop codon or not
		 *
		 * @param site a Site
		 * @param alpha a CodonAlphabet
		 */
		static bool hasStop(const Site & site, const CodonAlphabet & alpha);

		/**
		 * @brief Method to know if a codon site is complete or not (doesn't contains gap or undefined character)
		 *
		 * This method is equivalent to SiteTools::isComplete
		 * @param site a Site
		 */
		static bool isComplete(const Site & site);

		/**
		 * @brief Method to know if a codon site is constant or not (contain one character type only)
		 *
		 * This method is equivalent to SiteTools::isConstant
		 * @param site a Site
		 */
		static bool isConstant(const Site & site) throw (EmptySiteException);

		/**
		 * @brief Method to know if a polymorphic codon site is polymorphic at only one site
		 *
		 * @param site a Site
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 */
		static bool isMonoSitePolymorphic(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca) throw (Exception);

		/**
		 * @brief Method to know if polymorphism at a codon site is synonymous
		 *
		 * @param site a Site
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 */
		static bool isSynonymousPolymorphic(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca) throw (Exception);

		/**
		 * @brief Method to know if polymorphism at a codon site is synonymous
		 *
		 * @param site a Site
		 * @param gc a GeneticCode
		 */
    static bool isSynonymousPolymorphic(const Site & site, const GeneticCode & gc) throw (Exception);



		/**
		 * @brief generate a codon site without rare variants
		 *
		 * rare variants are replaced by the most frequent allele
		 * @param site a Site
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 * @param freqmin a double, allele in frequency stricly lower than freqmin are replaced
		 */
	static Site * generateCodonSiteWithoutRareVariant(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca, double freqmin) throw(Exception);


		/**
		 * @brief Compute the number of differences between two codons
		 *
		 * @param i a int
		 * @param j a int
		 * @param ca a CodonAlphabet
		 */
	static unsigned int numberOfDifferences(int i, int j, const CodonAlphabet & ca);

		/**
		 * @brief Compute the number of synonymous differences between two codons
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

		/**
		 * @brief Compute the synonymous pi per codon site
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
		 * @param minchange a bollean set by default to false
		 */
		static double piSynonymous(const Site & site, const CodonAlphabet & ca, const GeneticCode & gc, bool minchange=false) throw(Exception);

		/**
		 * @brief Compute the synonymous pi per codon site
		 *
		 * the following formula is used:
		 * @f[
		 * pi = frac{n}{n-1}\sum_{i,j}x_{i}x_{j}P_{ij}\
		 * @f]
		 * where n is the number of sequence, Xi and Xj the frequencies of each codon type occuring at the site
		 * Pij the number of synonymous difference between these codons
		 * Be careful: here, pi is not normalized by the number of synonymous sites
		 * @param site a Site
		 * @param gc a GeneticCode
		 * @param minchange a bollean set by default to false
		 */
		static double piSynonymous(const Site & site, const GeneticCode & gc, bool minchange=false) throw(Exception);


		/**
		 * @brief Compute the non-synonymous pi per codon site
		 *
		 * the following formula is used:
		 * @f[
		 * pi = frac{n}{n-1}\sum_{i,j}x_{i}x_{j}P_{ij}\
		 * @f]
		 * where n is the number of sequence, Xi and Xj the frequencies of each codon type occuring at the site
		 * Pij the number of nonsynonymous difference between these codons
		 * Be careful: here, pi is not normalized by the number of non-synonymous sites
		 * @param site a Site
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @param minchange a bollean set by default to false
		 */
		static double piNonSynonymous(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca, const GeneticCode & gc, bool minchange=false) throw(Exception);

		/**
		 * @brief Compute the non-synonymous pi per codon site
		 *
		 * the following formula is used:
		 * @f[
		 * pi = frac{n}{n-1}\sum_{i,j}x_{i}x_{j}P_{ij}\
		 * @f]
		 * where n is the number of sequence, Xi and Xj the frequencies of each codon type occuring at the site
		 * Pij the number of nonsynonymous difference between these codons
		 * Be careful: here, pi is not normalized by the number of non-synonymous sites
		 * @param site a Site
		 * @param gc a GeneticCode
		 * @param minchange a bollean set by default to false
		 */
		static double piNonSynonymous(const Site & site, const GeneticCode & gc, bool minchange = false) throw(Exception);

		/**
		 * @brief Return the number of synonymous positions of a codon
		 *
		 * a site is consider as x% synonymous if x% of the possible mutations are synonymous
		 * Transition/transversion ratio can be taken into account (use the variable ratio)
		 * @param i a int
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @param ratio a double set by default to 1
		 */
		static double numberOfSynonymousPositions(int i, const CodonAlphabet & ca, const GeneticCode & gc, double ratio=1) throw(Exception);

		/**
		 * @brief Return the number of synonymous positions of a codon
		 *
		 * a site is consider as x% synonymous if x% of the possible mutations are synonymous
		 * Transition/transversion ratio can be taken into account (use the variable ratio)
		 * @param i a int
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @param stopflag a boolean set by default to true if you want to take gap into account
		 * @param ratio a double set by default to 1
		 */
		static double numberOfSynonymousPositions(int i, const CodonAlphabet & ca, const GeneticCode & gc, bool stopflag = true, double ratio=1.0) throw(Exception);

		/**
		 * @brief Return the mean number of synonymous positions per codon site
		 *
		 * a site is consider as x% synonymous if x% of the possible mutations are synonymous
		 * Transition/transversion ratio can be taken into account (use the variable ratio)
		 * @param site a Site
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @param ratio a double set by default to 1
		 */
		static double meanNumberOfSynonymousPositions(const Site & site,  const CodonAlphabet & ca, const GeneticCode & gc, double ratio=1) throw(Exception);

		/**
		 * @brief Return the mean number of synonymous positions per codon site
		 *
		 * a site is consider as x% synonymous if x% of the possible mutations are synonymous
		 * Transition/transversion ratio can be taken into account (use the variable ratio)
		 * @param site a Site
		 * @param gc a GeneticCode
		 * @param ratio a double set by default to 1
		 */
		static double meanNumberOfSynonymousPositions(const Site & site, const GeneticCode & gc, double ratio=1) throw(Exception);



		/**
		 * @brief Return the number of subsitutions per codon site
		 *
		 * no recombination is assumed
		 * @param site a Site
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 * @param freqmin a double, to exclude snp in frequency strictly lower than freqmin
		 */
		static unsigned int numberOfSubsitutions(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca, double freqmin =0) throw(Exception);


		/**
		 * @brief Return the number of Non Synonymous subsitutions per codon site
		 *
		 * It is assumed that the path linking amino acids only involved one substitution by step
		 * @param site a Site
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 * @param freqmin a double, to exclude snp in frequency strictly lower than freqmin
		 */
		static unsigned int numberOfNonSynonymousSubstitutions(const Site & site, const NucleicAlphabet & na, const CodonAlphabet & ca, const GeneticCode & gc, double freqmin =0) throw(Exception);


		/**
		 * @brief Return a vector with the number of fixed synonymous and non-synonymous differences per codon site
		 *
		 * @param siteIn a Site
		 * @param siteOut a Site
		 * @param i an integer
		 * @param j an integer
		 * @param na a NucleicAlphabet
		 * @param ca a CodonAlphabet
		 * @param gc a GeneticCode
		 */
		static vector<unsigned int> fixedDifferences(const Site & siteIn, const Site & siteOut, int i, int j, const NucleicAlphabet & na, const CodonAlphabet & ca, const GeneticCode & gc) throw(Exception);

};

#endif	//_CONDONSITETOOLS_H_
