//
// File: AlphabetExceptions.cpp
// Created by: Julien Dutheil
// Created on: Mon Nov  3 16:41:53 2003
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

#include "AlphabetExceptions.h"
#include "Alphabet.h"

// From Utils:
#include <Utils/TextTools.h>

using namespace bpp;

using namespace std;

/******************************************************************************
 *                         Alphabet exceptions:                               *
 ******************************************************************************/

AlphabetException::AlphabetException(const string & text, const Alphabet * alpha) :
	Exception("AlphabetException: " + text + (alpha != NULL ? "(" + (alpha->getAlphabetType()) + ")" : string(""))),
	_alphabet(alpha) {}
		
const Alphabet * AlphabetException::getAlphabet() const { return _alphabet; }

/******************************************************************************/

BadCharException::BadCharException(const string & badChar, const string & text, const Alphabet * alpha) :
	AlphabetException("BadCharException: " + badChar + ". " + text, alpha),
	_c(badChar) {}
		
string BadCharException::getBadChar() const { return _c; }

/******************************************************************************/

BadIntException::BadIntException(int badInt, const string & text, const Alphabet * alpha) :
	AlphabetException("BadIntException: " + TextTools::toString(badInt) + ". " + text, alpha),
	_i(badInt) {}
		
int BadIntException::getBadInt() const { return _i; }

/******************************************************************************/
	
AlphabetMismatchException::AlphabetMismatchException(const char *   text, const Alphabet * alpha1, const Alphabet * alpha2) :
	Exception("AlphabetMismatchException: " + string(text) + (alpha1 != NULL && alpha2 != NULL ? "(" + alpha1 -> getAlphabetType() + ", " + alpha2 -> getAlphabetType() + ")" : string(""))),
	_alphabet1(alpha1),
	_alphabet2(alpha2) {}
		
AlphabetMismatchException::AlphabetMismatchException(const string & text, const Alphabet * alpha1, const Alphabet * alpha2) :
	Exception("AlphabetMismatchException: " + text + (alpha1 != NULL && alpha2 != NULL ? "(" + alpha1 -> getAlphabetType() + ", " + alpha2 -> getAlphabetType() + ")" : string(""))),
	_alphabet1(alpha1),
	_alphabet2(alpha2) {}
		
AlphabetMismatchException::~AlphabetMismatchException() throw () {};

vector<const Alphabet *> AlphabetMismatchException::getAlphabets() const
{
	vector<const Alphabet *> v(2);
	v[0] = _alphabet1;
	v[1] = _alphabet2;
	return v;
}

/******************************************************************************/

CharStateNotSupportedException::CharStateNotSupportedException(const string & text, const Alphabet * alpha) :
  AlphabetException("CharStateNotSupportedException: " + text, alpha) {};

/******************************************************************************/
