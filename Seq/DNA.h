/*
 * File DNA.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
*/

// Secured inclusion of header's file
#ifndef _DNA_H_
#define _DNA_H_

#include "NucleicAlphabet.h"

/**
 * @brief This alphabet is used to deal with DNA sequences.
 *
 * It supports all 4 nucleotides (A, T, G and C) with their standard denomination.
 * Gaps are coded by '-', unresolved characters are coded by 'X, N, O, 0 or ?'.
 * Extensive support for generic characters (e.g. 'P', 'Y', etc.) is provided.
 */
class DNA : public NucleicAlphabet
{
	public:
		// class constructor
		DNA();
		// class destructor
		~DNA() {}
	
	public:
		string getAlphabetType() const;
};

#endif // _DNA_H_
