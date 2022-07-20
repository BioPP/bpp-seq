//
// File: AlphabetExceptions.h
// Authors:
//   Julien Dutheil
// Created: 2003-11-03 16:41:53
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_ALPHABET_ALPHABETEXCEPTIONS_H
#define BPP_SEQ_ALPHABET_ALPHABETEXCEPTIONS_H

#include <Bpp/Exceptions.h>
#include "Alphabet.h"

// From the STL:
#include <vector>
#include <memory>

namespace bpp
{

/**
 * @brief The alphabet exception base class.
 *
 * @see Alphabet, Exception
 */
class AlphabetException :
  public Exception
{
private:
  const Alphabet& alphabet_;

public:
  /**
   * @brief Build a new AlphabetException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  AlphabetException(const std::string& text, const Alphabet& alpha);

  virtual ~AlphabetException() {}

public:
  /**
   * @brief Get the alphabet that threw the exception.
   *
   * @return a const pointer toward the alphabet.
   */
  virtual const Alphabet& getAlphabet() const { return alphabet_; }
};

/**
 * @brief An alphabet exception thrown when trying to specify a bad char to the alphabet.
 */
class BadCharException :
  public AlphabetException
{
protected:
  std::string c_;

public:
  /**
   * @brief Build a new BadCharException.
   *
   * @param badChar The faulty character.
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  BadCharException(const std::string& badChar, const std::string& text, const Alphabet& alpha);

  virtual ~BadCharException() {}

public:
  /**
   * @brief Get the character that threw the exception.
   *
   * @return the faulty character.
   */
  virtual std::string getBadChar() const;
};

/**
 * @brief An alphabet exception thrown when trying to specify a bad int to the alphabet.
 */
class BadIntException :
  public AlphabetException
{
protected:
  int i_;

public:
  /**
   * @brief Build a new BadIntException.
   * @param badInt The faulty integer.
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  BadIntException(int badInt, const std::string& text, const Alphabet& alpha);

  virtual ~BadIntException() {}

public:
  /**
   * @brief Get the integer that threw the exception.
   *
   * @return the faulty integer.
   */
  virtual int getBadInt() const;
};

/**
 * @brief Exception thrown when two alphabets do not match.
 *
 * Typically, this may occur when you try to add a bad sequence to a container,
 * or concatenate two kinds of sequences, and so on.
 */
class AlphabetMismatchException : 
  public Exception
{
private:
  const Alphabet& alphabet1_;
  const Alphabet& alphabet2_;

public:
  /**
   * @brief Build a new AlphabetMismatchException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha1 A const pointer toward the first alphabet.
   * @param alpha2 A const pointer toward the second alphabet, i.e. the one which does not match with the first.
   */
  AlphabetMismatchException(
		  const std::string& text,
		  const Alphabet& alpha1,
		  const Alphabet& alpha2);

  virtual ~AlphabetMismatchException() {}

public:
  /**
   * @brief Get the first alphabet.
   *
   * @return A reference toward the first alphabet.
   */
  const Alphabet& getFirstAlphabet() const { return alphabet1_; }

  /**
   * @brief Get the second alphabet.
   *
   * @return A reference toward the seocnd alphabet.
   */
  const Alphabet& getSecondAlphabet() const { return alphabet2_; }
};

/**
 * @brief Exception thrown in case no character is available for a certain state in an alphabet.
 */
class CharStateNotSupportedException : 
  public AlphabetException
{
public:
  /**
   * @brief Build a new CharStateNotSupportedException.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  CharStateNotSupportedException(const std::string& text, const Alphabet& alpha);

  virtual ~CharStateNotSupportedException() {}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_ALPHABETEXCEPTIONS_H
