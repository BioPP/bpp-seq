/*
 * File NucleicAlphabet.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
 */

// Secured inclusion of Header's file
#ifndef _NUCLEICALPHABET_H_
#define _NUCLEICALPHABET_H_

#include "AbstractAlphabet.h"

/**
 * @brief <p>The abstract base class foc nucleic alphabets.
 * This class only implements a few methods, it is mainly designed for methods/classes
 * that will require to work with both RNA and DNA.</p>
 */
class NucleicAlphabet : public AbstractAlphabet
{
	public:
		// class destructor
		~NucleicAlphabet();
	
	public:
		// return 4 : A, C, G, T (or U)
		unsigned int getSize() const;

		// return 15 : gap isn't included, generic unresolved bases (N, X, ?, O, 0) count for one
		unsigned int getNumberOfTypes() const;
};

#endif // _NUCLEICALPHABET_H_
