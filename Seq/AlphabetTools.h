//
// File: AlphabetTools.h
// Created by: Julien Dutheil
// Created on: Fri Oct 10 17:27:39 2003
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

#ifndef _ALPHABETTOOLS_H_
#define _ALPHABETTOOLS_H_

#include "alphabets"
#include <typeinfo>

/**
 * @brief Utilitary functions dealing with alphabets.
 */
class AlphabetTools
{
	public:
		AlphabetTools() {}
		virtual ~AlphabetTools() {}
	
	public:
		/**
     * @brief Character identification method for sequence's alphabet identification
     *
     * (used in method getAlphabetFromSequence() implemented in class Sequence)
		 * Return :
		 * -1 if character can't be nucleic or proteic
		 *  0 ------------------ identify alphabet type (letters A, C, D, G, H, K, M, N, R, S, V, W, X, Y, -)
		 *  1 ------------ identify nucleic alphabet    (letters B, O, ?, 0)
		 *  2 ------------ may identify DNA alphabet    (letter T)
		 *  3 ------------ identify RNA alphabet        (letter U)
		 *  4 --------------------- proteic alphabet    (letters E, F, I, L, P, Q)
     *
     * @param acid The character to test.
     * @return The type code.
		 */
		static int getType(char acid);

		/**
		 * @brief This checks that all characters in the alphabet are coded by a string of same length.
		 *
		 * This method is used when states are coded by more than one character, typically: codon alphabets.
		 *
		 * @param alphabet The alphabet to check.
		 * @return True if all text description have the same length (e.g. 3 for codon alphabet).
		 */
		static bool checkAlphabetCodingSize(const Alphabet & alphabet) throw (AlphabetException);

		/**
		 * @brief This checks that all characters in the alphabet are coded by a string of same length.
		 *
		 * This function performs the same work as the previous one, but deals with pointers
		 * instead of reference. This may be more convenient since we often use pointers on alphabets.
		 *
		 * @param alphabet a pointer toward the alphabet to check.
		 * @return True if all text description have the same length (e.g. 3 for codon alphabet).
		 */
		static bool checkAlphabetCodingSize(const Alphabet * alphabet) throw (AlphabetException);

		/**
		 * @brief In case that all states in the given alphabet have a string description of same length,
		 * send this length; e.g. 3 for codon alphabets.
		 *
		 * @param alphabet The alphabet to analyse.
		 * @return The common size of all text descriptionif there is one. Else throws an AlphabetException.
		 */
		static int getAlphabetCodingSize(const Alphabet & alphabet) throw (AlphabetException);

		/**
		 * @brief In case that all states in the given alphabet have a string description of same length,
		 * send this length; e.g. 3 for codon alphabets.
		 *
		 * This function performs the same work as the previous one, but deals with pointers
		 * instead of reference. This may be more convenient since we often use pointers on alphabets.
		 *
		 * @param alphabet a pointer toward the alphabet to analyse.
		 * @return The common size of all text descriptionif there is one. Else throws an AlphabetException.
		 */
		static int getAlphabetCodingSize(const Alphabet * alphabet) throw (AlphabetException);

		/**
		 * @return True if the alphabet is an instanciation of the NucleicAlphabet class.
		 * @param alphabet The alphabet to check.
		 */
		static bool isNucleicAlphabet(const Alphabet * alphabet) { return alphabetInheritsFrom<NucleicAlphabet>(alphabet); }
		/**
		 * @return True if the alphabet is an instanciation of the ProteicAlphabet class.
		 * @param alphabet The alphabet to check.
		 */
		static bool isProteicAlphabet(const Alphabet * alphabet) { return alphabetInheritsFrom<ProteicAlphabet>(alphabet); }
		/**
		 * @return True if the alphabet is an instanciation of the CodonAlphabet class.
		 * @param alphabet The alphabet to check.
		 */
		static bool isCodonAlphabet(const Alphabet * alphabet) { return alphabetInheritsFrom<CodonAlphabet>(alphabet); }
		/**
		 * @return True if the alphabet is an instanciation of the DefaultAlphabet class.
		 * @param alphabet The alphabet to check.
		 */
		static bool isDefaultAlphabet(const Alphabet * alphabet) { return alphabetInheritsFrom<DefaultAlphabet>(alphabet); }



	private:
	  template<class Y>
	  static bool alphabetInheritsFrom(const Alphabet * alphabet)
		{
			try {
			  dynamic_cast<const Y *>(alphabet);
				return true;
			} catch(exception & e) {
			  return false;
			}
		}

};

#endif	//_ALPHABETTOOLS_H_

