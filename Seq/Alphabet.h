/*
 * File Alphabet.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
 */

// Secured inclusion of header's file
#ifndef _ALPHABET_H_
#define _ALPHABET_H_

#include <string>
#include <vector>

#include "AlphabetExceptions.h"

using namespace std;


/**
 * @brief The Alphabet virtual class.
 * Interface for alphabets manipulation.
 * 
 * An alphabet object defines all the states allowed for a particular type of
 * sequence. These states are coded as a charater (a string) and an integer.
 * The char description is the one found in the text descritpion of sequences,
 * typically in sequence files. This is the more intuitive. However, for
 * computionnal needs, this is often more efficient to store the sequences as
 * vector of integers. The link between the two descriptions is made <i>via</i>
 * the Alphabet classes, and the two methods intToChar() and charToInt().
 * The Alphabet interface also provides other tools, like getting the full name
 * of the states and so on.
 * 
 * The alphabet objects may throw some exceptions derived of the AlphabetException
 * class.
 *
 * @see AlphabetException, BadCharException, BadIntException 
 */
class Alphabet
{
    public:	// Class destructor
		virtual ~Alphabet() {}
	
	public:
    
		/**
		 * @brief Get the complete name of a state given its int description.
		 *
		 * In case of many bases with same number (i.e. N and X for nucleic alphabets),
		 * this method will return the name of the first found in the vector.
		 *
		 * @param letter The int description of the given state.
		 * @return The name of the state.
		 */
		virtual string getName(int letter) const throw (BadIntException)  = 0;
        
		/**
		 * @brief Get the complete name of a state given its char description.
		 *
		 * In case of many bases with same number (i.e. N and X for nucleic alphabets),
		 * this method will return the name of the first found in the vector.
		 *
		 * @param letter The char description of the given state.
		 * @return The name of the state.
		 */
		virtual string getName(const string & letter) const throw (BadCharException) = 0;

		/**
         * @name = Tests
         *
         * @{
         */

		/**
		 * @brief Tell if a state (specified by its int descritption) is allowed by the
		 * the alphabet.
		 *
		 * @param letter The int description.
		 * @return 'True' if the state in known.
		 */
		virtual bool  isIntInAlphabet(int letter) const = 0;
		
		/**
		 * @brief Tell if a state (specified by its char descritption) is allowed by the
		 * the alphabet.
		 *
		 * @param letter The char description.
		 * @return 'True' if the state in known.
		 */
		virtual bool isCharInAlphabet(const string & letter) const = 0;
	    /** @} */
        
		/**
         * @name Conversion methods
         *
         * @}
         */
		
		/**
		 * @brief Give the char description of a state given its int description.
		 *
		 * @param letter The int description.
		 * @return The char description.
		 */
		virtual string intToChar(int letter) const throw (BadIntException) = 0;
        
		/**
		 * @brief Give the int description of a state given its char description.
		 *
		 * @param letter The char description.
		 * @return The int description.
		 */
		virtual int charToInt(const string & letter) const throw (BadCharException) = 0;
        /** @} */
        
		/**
         * @name Sizes.
         *
         * @{
         */
		
		/**
		 * @brief Get the number of allowed states in Alphabet (e.g. return 20 for DNA alphabet).
		 * This is the number of characters used for state description.
		 *
		 * @return The total number of states.
		 */
		virtual unsigned int getNumberOfChars() const = 0;
        
		/**
		 * @brief Get the number of <strong>distinct</strong> states in alphabet (e.g. return 15 for DNA alphabet).
		 * This is the number of integers used for state description.
		 *
		 * @return The number of distinct states.
		 */
		virtual unsigned int getNumberOfTypes() const = 0;
        
		/**
		 * @brief Get the number of <strong>resolved</strong> states in the alphabet (e.g. return 4 for DNA alphabet).
		 * This is the method you'll need in most cases.
		 *
		 * @return The number of resolved states.
		 */
		virtual unsigned int getSize() const = 0;
        /** @} */
        
		// Some util:
		
		/**
		 * @brief Identification method.
		 *
		 * Used to tell if two alphabet describe the same type of sequences.
		 * For instance, this method is used by sequence containers to compare two alphabets and
		 * autorize or deny addition of sequences.
		 *
		 * @return A text describing the alphabet.
		 */
		virtual string getAlphabetType() const = 0;
};

#endif // _ALPHABET_H_
