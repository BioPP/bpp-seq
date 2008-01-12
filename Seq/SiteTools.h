//
// File SiteTools.h
// Author: Julien Dutheil
//         Guillaume Deuchst
// Last modification : Friday August 8 2003
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

#ifndef _SITETOOLS_H_
#define _SITETOOLS_H_

#include "SymbolListTools.h"
#include "Site.h"

// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <map>

using namespace std;

namespace bpp
{

/**
 * @brief Utilitary methods dealing with sites.
 */

class SiteTools:
  public SymbolListTools
{
	public:
		SiteTools() {}
		virtual ~SiteTools() {}

	public:
		/**
		 * @param site A site.
		 * @return True if the site contains one or several gap(s).
		 */
		static bool hasGap(const Site & site);

		/**
		 * @param site A site.
		 * @return True if the site contains only gaps.
		 */
		static bool isGapOnly(const Site & site);

    /**
		 * @param site A site.
		 * @return True if the site contains only gaps.
		 */
		static bool isGapOrUnresolvedOnly(const Site & site);

		/**
		 * @param site A site.
		 * @return True if the site contains one or several unknwn characters.
		 */
		static bool hasUnknown(const Site & site);

		/**
		 * @param site A site.
		 * @return True if the site contains no gap and no unknown characters.
		 */
		static bool isComplete(const Site & site);
		
		/**
		 * @param site A site.
     * @param ignoreUnknown If true, positions with unknown positions will be ignored.
     * Otherwise, a site with one single state + any uncertain state will not be considered as constant.
		 * @return True if the site is made of only one state.
		 * @throw EmptySiteException If the site has size 0.
		 */
		static bool isConstant(const Site & site, bool ignoreUnknown = false) throw (EmptySiteException);

 		/**
		 * @param site1 The first site.
		 * @param site2 The second site.
		 * @return True if the two states have the same content (and, of course, alphabet).
 		 */
		static bool areSitesIdentical(const Site & site1, const Site & site2);

		/**
		 * @brief Compute the Shannon entropy index of a site.
		 *
		 * \f[
		 * I = \sum_{i=1}{n} f_iln(f_i)
		 * \f]
		 * where \f$f_i\f$ is the frequency of state \f$i\f$.
		 * 
		 * @param site A site.
		 * @return The Shannon entropy index of this site.
		 * @throw EmptySiteException If the site has size 0.
		 */
		static double variabilityShannon(const Site & site) throw (EmptySiteException);
		
		/**
		 * @brief Compute the factorial diversity index of a site.
		 *
		 * \f[
		 * F = \frac{log\left(\left(\sum_{i=1}{n} p_i\right)!\right)}{\sum_{i=1}^n log(p_i)!}
		 * \f]
		 * where \f$p_i\f$ is the number of times state \f$i\f$ is observed in the site.
		 * 
		 * @param site A site.
		 * @return The factorial diversity index of this site.
		 * @throw EmptySiteException If the site has size 0.
		 */
		static double variabilityFactorial(const Site & site) throw (EmptySiteException);

		/**
		 * @brief Compute the heterozygosity index of a site.
		 *
		 * \f[
		 * H = 1 - \sum_{i=1}{n} f_i^2
		 * \f]
		 * where \f$f_i\f$ is the frequency of state \f$i\f$.
		 * 
		 * @param site A site.
		 * @return The heterozygosity index of this site.
		 * @throw EmptySiteException If the site has size 0.
		 */
		static double heterozygosity(const Site & site) throw (EmptySiteException);

		/**
		 * @brief Give the number of distinct characters at a site.
		 *
		 * @param site a Site
		 * @return The number of distinct characters in the given site.
		 */
    static unsigned int getNumberOfDistinctCharacters(const Site & site) throw (EmptySiteException);

		/**
		 * @brief Tell if a site has singletons
		 *
		 *
		 * @param site a Site.
		 * @return True if the site has singletons.
		 */
		static bool hasSingleton(const Site & site) throw (EmptySiteException);

		/**
		 * @brief Tell if a site is a parsimony informative site.
		 *
		 * At least two distinct characters must be present.
		 *
		 * @param site a Site.
		 * @return True if the site is parsimony informative.
		 */
		static bool isParsimonyInformativeSite(const Site & site) throw (EmptySiteException);


		/**
		 * @brief Tell if a site has more than 2 distinct characters
		 *
		 * @param site a Site.
		 * @return True if the site has more than 2 distinct characters
		 */
    static bool isTriplet(const Site & site) throw (EmptySiteException);

};

} //end of namespace bpp.

#endif	//_SITETOOLS_H_

