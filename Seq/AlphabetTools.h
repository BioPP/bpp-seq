//
// File: AlphabetTools.h
// Created by: jdutheil <jdutheil@Deedlit>
// Created on: Fri Oct 10 17:27:39 2003
//

#ifndef _ALPHABETTOOLS_H_
#define _ALPHABETTOOLS_H_

#include "Alphabet.h"

class AlphabetTools
{
	public:
		AlphabetTools() {}
		 ~AlphabetTools() {}
	
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

};


#endif	//_ALPHABETTOOLS_H_
