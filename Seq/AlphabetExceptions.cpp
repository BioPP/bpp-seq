//
// File: AlphabetExceptions.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Nov  3 16:41:53 2003
//

#include "AlphabetExceptions.h"
#include "Alphabet.h"

// From Utils:
#include <Utils/TextTools.h>

/******************************************************************************
 *                         Alphabet exceptions:                               *
 ******************************************************************************/

//AlphabetException::AlphabetException(const char *   text, const Alphabet * alpha) :
//	Exception("AlphabetException: " + text + (alpha != NULL ? "(" + (alpha -> getAlphabetType()) + ")" : "")),
//	alphabet(alpha) {};
AlphabetException::AlphabetException(const string & text, const Alphabet * alpha) :
	Exception("AlphabetException: " + text + (alpha != NULL ? "(" + (alpha -> getAlphabetType()) + ")" : string(""))),
	alphabet(alpha) {};
		
const Alphabet * AlphabetException::getAlphabet() const { return alphabet; }

/******************************************************************************/

//BadCharException::BadCharException(const string badChar, const char *   text, const Alphabet * alpha) :
//	AlphabetException("BadCharException: " + badChar + ". " + additionalText, alpha) {};
BadCharException::BadCharException(const string badChar, const string & text, const Alphabet * alpha) :
	AlphabetException("BadCharException: " + badChar + ". " + text, alpha),
	c(badChar) {};
		
string BadCharException::getBadChar() const { return c; }

/******************************************************************************/

//BadIntException::BadIntException(int badInt, const char *   additionalText, const Alphabet * alpha) :
//	AlphabetException("BadIntException: " + textTools::toString(badInt) + ". " + string(text), alpha) {};
BadIntException::BadIntException(int badInt, const string & text, const Alphabet * alpha) :
	AlphabetException("BadIntException: " + TextTools::toString(badInt) + ". " + text, alpha),
	i(badInt) {};
		
int BadIntException::getBadInt() const { return i; }

/******************************************************************************/
