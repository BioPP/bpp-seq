//
// File: StringSequenceTools.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Nov 30 11:29:07 2003
//

#ifndef _STRINGSEQUENCETOOLS_H_
#define _STRINGSEQUENCETOOLS_H_

#include "Alphabet.h"
#include "AlphabetExceptions.h"
#include "SequenceExceptions.h"

// From utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <string>
#include <vector>

using namespace std;

class StringSequenceTools
{
	public:
		StringSequenceTools() {};
		 ~StringSequenceTools() {};
	
	public:
    
		/**
     * @brief Get a subsequence.
     *
     * @param sequence The input sequence.
     * @param begin    The begining position (included).
     * @param end      The ending position (included).
     * @return A string with the subsequence.
     * @throw Exception If position does not not match the interval [0, length].
     */
		static string subseq(const string & sequence, int begin, int end) throw (Exception);

		/**
		 * @brief Set up the size of a sequence from the right side.
		 *
		 * All new characters are filled with gaps.
		 * If the specified size is < to the sequence size, the sequence will be truncated.
		 *
     * @param sequence The input sequence.
		 * @param size The new size of the sequence.
		 */
		static string setToSizeR(const string & sequence, int size);

    /**
		 * @brief Set up the size of a sequence from the left side.
		 *
		 * All new characters are filled with gaps.
		 * If the specified size is < to the sequence size, the sequence will be truncated.
		 *
     * @param sequence The input sequence.
		 * @param size The new size of the sequence.
		 */
		static string setToSizeL(const string & sequence, int size);

		/**
     * @brief Delete all occurence of a character in the sequence.
     *
     * @param sequence The sequence to parse.
     * @param chars    The character to remove.
     * @return         The sequence with all specified characters removed.
     */
		static string deleteChar(const string & sequence, char chars);

    /**
     * @brief Delete all occurence of several characters in the sequence.
     *
     * @param sequence The sequence to parse.
     * @param chars    The characters to remove.
     * @return         The sequence with all specified characters removed.
     */
		static string deleteChar(const string & sequence, string chars);

		/**
     * @brief Tell if a sequence is a palindrome.
     *
     * @param sequence the sequence to parse.
     * @return true If the sequence is a palindrome.
     */
		static bool isPalindrome(const string & sequence);

    /**
     * @brief Reverse the sequence.
     *
     * @param sequence The sequence to reverse.
     * @return The reversed sequence.
     */
    static string * reverse(const string & sequence);

    // !!! Upgrade to Translators! !!!
    /**
     * @brief Get the complement of a sequence.
     *
     * For this method, sequence is supposed to be of type DNA.
     *
     * @param sequence The sequence to complement.
     * @return The complementary sequence.
     */
		static string * complement(const string & sequence);

    /**
     * @brief Calculate the local GC content of a sequence.
     *
		 * GC contents are calculated using a window of specified size around the given position.
     * Note : Calculus for last nucleotides (sequence's size - window size) will return
		 * the last possible rate calculated.
     *
     * @param sequence The sequence to analyse.
     * @param pos      The position where to compute the GC content.
     * @param window   The size of the window to use.
     * @return The GC content as a ratio (# of GC / window).
     * @throw BadIntException If the sequence is not of type DNA or RNA.
     * @throw Exception       Error in calculus (if the sequence contain gaps for instance).
     */
		static double getGCcontent(const string & sequence, unsigned int pos, unsigned int window) throw (BadIntegerException, Exception);

		// Method to shuffle a sequence by windows and with or without throw in used values
		static string randomShuffle(const string & sequence, unsigned int window, bool throwIn);
		
        /**
         * @brief Convert a string sequence to a vector of int.
         *
		 * This method is used in the sequence constructor.
		 * This deals with the most simple cases: the sequence (string) contains
		 * a succession of all characters. This is indeed the case for simple
		 * alphabets, but may be more complicated if the alphabet is coded with
		 * variable code length.
         *
         * @param sequence The sequence to parse.
         * @param alphabet The alphabet to use to code the sequence.
         * @return A vector of int codes.
         * @throw BarCharException If some character does not match the specified alphabet.
         */
		static vector<int> codeSequence(const string & sequence, const Alphabet * alphabet) throw (BadCharException);

        /**
         * @brief Convert a sequence to its string representation.
         *
         * @param sequence The sequence object to convert.
         * @param alphabet The alphabet to use to decode the sequence.
         * @return A string representation of the sequence.
         * @throw BarIntException If some value does not match the specified alphabet.
         */
        static string decodeSequence(const vector<int> & sequence, const Alphabet * alphabet) throw (BadIntException);

		/**
		 * @brief Parse a sequence and try to guess the correct alphabet to use.
         *
		 * @param sequence The sequence to parse.
         * @return A pointer toward a new Alphabet object.
         * @throw EmptySequenceException if the sequence is empty.
         * @throw SequenceException if their is an ambiguity between several alphabet.
         * @throw AlphabetException if the sequence does not match any alphabet.
		 */
		static Alphabet * getAlphabetFromSequence(const string sequence) throw (EmptySequenceException, SequenceException, AlphabetException);

};


#endif	//_STRINGSEQUENCETOOLS_H_
