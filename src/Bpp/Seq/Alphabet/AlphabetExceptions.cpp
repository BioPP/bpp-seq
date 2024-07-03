// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "Alphabet.h"
#include "AlphabetExceptions.h"

using namespace bpp;

using namespace std;

/******************************************************************************
*                         Alphabet exceptions:                               *
******************************************************************************/

AlphabetException::AlphabetException(const std::string& text, const Alphabet* alpha) :
  Exception("AlphabetException: " + text + "(" + (alpha->getAlphabetType()) + ")"),
  alphabet_(alpha) {}

AlphabetException::AlphabetException(const std::string& text, std::shared_ptr<const Alphabet> alpha) :
  Exception("AlphabetException: " + text + "(" + (alpha->getAlphabetType()) + ")"),
  alphabet_(alpha.get()) {}

/******************************************************************************/

BadCharException::BadCharException(const std::string& badChar, const std::string& text, const Alphabet* alpha) :
  AlphabetException("BadCharException: " + badChar + ". " + text, alpha),
  c_(badChar) {}

BadCharException::BadCharException(const std::string& badChar, const std::string& text, std::shared_ptr<const Alphabet> alpha) :
  AlphabetException("BadCharException: " + badChar + ". " + text, alpha),
  c_(badChar) {}

string BadCharException::getBadChar() const { return c_; }

/******************************************************************************/

BadIntException::BadIntException(int badInt, const std::string& text, const Alphabet* alpha) :
  AlphabetException("BadIntException: " + TextTools::toString(badInt) + ". " + text, alpha),
  i_(badInt) {}

BadIntException::BadIntException(int badInt, const std::string& text, std::shared_ptr<const Alphabet> alpha) :
  AlphabetException("BadIntException: " + TextTools::toString(badInt) + ". " + text, alpha),
  i_(badInt) {}

int BadIntException::getBadInt() const { return i_; }

/******************************************************************************/

AlphabetMismatchException::AlphabetMismatchException(const std::string& text, const Alphabet* alpha1, const Alphabet* alpha2) :
  Exception("AlphabetMismatchException: " + text + "(" + alpha1->getAlphabetType() + ", " + alpha2->getAlphabetType() + ")"),
  alphabet1_(alpha1),
  alphabet2_(alpha2) {}

/******************************************************************************/

AlphabetMismatchException::AlphabetMismatchException(const std::string& text, std::shared_ptr<const Alphabet> alpha1, std::shared_ptr<const Alphabet> alpha2) :
  Exception("AlphabetMismatchException: " + text + "(" + alpha1->getAlphabetType() + ", " + alpha2->getAlphabetType() + ")"),
  alphabet1_(alpha1.get()),
  alphabet2_(alpha2.get()) {}

/******************************************************************************/

CharStateNotSupportedException::CharStateNotSupportedException(const string& text, const Alphabet* alpha) :
  AlphabetException("CharStateNotSupportedException: " + text, alpha) {}

/******************************************************************************/

CharStateNotSupportedException::CharStateNotSupportedException(const string& text, std::shared_ptr<const Alphabet> alpha) :
  AlphabetException("CharStateNotSupportedException: " + text, alpha) {}

/******************************************************************************/
