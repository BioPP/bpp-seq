/*
 * File RNA.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
*/

// Secured inclusion of header's file
#ifndef _RNA_H_
#define _RNA_H_

#include "NucleicAlphabet.h"

/**
 * @brief This alphabet is used to deal with RNA sequences.
 *
 * It supports all 4 nucleotides (A, U, G and C) with their standard denomination.
 * Gaps are coded by '-', unresolved characters are coded by 'X, N, O, 0 or ?'.
 * Extensive support for generic characters (e.g. 'P', 'Y', etc.) is provided.
 */

class RNA : public NucleicAlphabet
{
	public:
		// Class constructor
		RNA();
		// Class destructor
		~RNA() {}

	public:
		vector<int   > getAlias(      int      state) const throw (BadIntException);
		vector<string> getAlias(const string & state) const throw (BadCharException);
		string getAlphabetType() const;
};

#endif // _RNA_H_
