/*
 * File ProteicAlphabet.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
*/

// Secured inclusion of header's file
#ifndef _PROTEICALPHABET_H_
#define _PROTEICALPHABET_H_

#include "AbstractAlphabet.h"

/**
 * @brief This alphabet is used to deal with proteins.
 *
 * It supports all 20 amino-acids with their standard denomination.
 * Gaps are coded by '-', unresolved characters are coded by 'X'.
 */

class ProteicAlphabet : public AbstractAlphabet
{
	public:
		// class constructor
		ProteicAlphabet();
	
		// class destructor
		~ProteicAlphabet() {}
	
	public:
		unsigned int getSize() const;
		unsigned int getNumberOfTypes() const;
		string getAlphabetType() const;
	
	public:

		/**
		 * @name Specific methods
		 *
		 * @{
		 */
    
		/**
		 * @brief Get the abbreviation (3 letter code) for a state coded as char.
		 *
		 * @param aa Char description of the amino-acid to analyse.
		 */
		string getAbbr(const string & aa) const throw (AlphabetException);
	
		/**
		 * @brief Get the abbreviation (3 letter code) for a state coded as int.
		 *
		 * @param aa Int description of the amino-acid to analyse.
		 */
		string getAbbr(int aa) const throw (AlphabetException);
		/** @} */
		
};

#endif // _PROTEICALPHABET_H_
