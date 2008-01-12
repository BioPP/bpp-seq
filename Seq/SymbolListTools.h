//
// File: SymbolListTools.h
// Created by: Julien Dutheil
// Created on: Wed Apr 9 2004
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

#ifndef _SYMBOLLISTTOOLS_H_
#define _SYMBOLLISTTOOLS_H_

#include "SymbolList.h"
#include "AlphabetExceptions.h"

// From the STL:
#include <map>

using namespace std;

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
		 * @brief Count all states in the list.
		 *
		 * @param list The list.
		 * @return A map with all states and corresponding counts.
		 */
		static map<int, unsigned int> getCounts(const SymbolList & list);
		
		/**
		 * @brief Count all states in the list, optionaly resolving unknown characters.
		 *
     * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
		 *
     * @param list The list.
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
		 * @return A map with all states and corresponding counts.
		 */
		static map<int, double> getCounts(const SymbolList & list, bool resolveUnknowns);
		
    /**
		 * @brief Get all states frequencies in the list.
		 *
		 * @param list The list.
     * @param resolveUnknowns Tell is unknown characters must be resolved.
     * For instance, in DNA, N will be counted as A=1/4,T=1/4,C=1/4,G=1/4.
		 * @return A map with all states and corresponding frequencies.
		 */
		static map<int, double> getFrequencies(const SymbolList & list, bool resolveUnknowns = false);

    /**
     * @brief Get the GC content of a symbol list.
     *
		 * @param list The list.
		 * @return The proportion of G and C states in the list.
     * @param ignoreUnresolved Do not count unresolved states. Otherwise, weight by each state probability in case of ambiguity (e.g. the R state counts for 0.5).
     * @param ignoreGap Do not count gaps in total.
     * @throw AlphabetException If the list is not made of nucleotide states.
		 */
    static double getGCContent(const SymbolList & list, bool ignoreUnresolved = true, bool ignoreGap = true) throw (AlphabetException);
		
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
		static unsigned int getNumberOfDistinctPositions(const SymbolList & l1, const SymbolList & l2) throw (AlphabetMismatchException);
		
		/**
 		 * @brief Get the number of positions without gap.
		 *
		 * The comparison in achieved from position 0 to the minimum size of the two vectors.
		 *
		 * @param l1 SymbolList 1.
		 * @param l2 SymbolList 2.
		 * @return The number of positions without gap.
		 * @throw AlphabetMismatchException if the two lists have not the same alphabet type.
		 */
		static unsigned int getNumberOfPositionsWithoutGap(const SymbolList & l1, const SymbolList & l2) throw (AlphabetMismatchException);

    /**
     * @brief Change all gap elements to unknown characters.
     *
     * @param l The input list of characters.
     */
    static void changeGapsToUnknownCharacters(SymbolList & l);

    /**
     * @brief Change all unknown characters to gap elements.
     *
     * @param l The input list of characters.
     */
    static void changeUnresolvedCharactersToGaps(SymbolList & l);

};

} //end of namespace bpp.

#endif // _SYMBOLLISTTOOLS_H_

