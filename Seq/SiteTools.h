//
// File SiteTools.h
// Author: Julien Dutheil
//         Guillaume Deuchst
// Last modification : Friday August 8 2003
//

#ifndef _SITETOOLS_H_
#define _SITETOOLS_H_

#include "SymbolListTools.h"
#include "Site.h"

// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <map>

using namespace std;

/**
 * @brief Utilitary methods dealing with sites.
 */

class SiteTools: public SymbolListTools
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
		 * @return True if the site is made of only one state.
		 * @throw EmptySiteException If the site has size 0.
		 */
		static bool isConstant(const Site & site) throw (EmptySiteException);

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

#endif	//_SITETOOLS_H_

