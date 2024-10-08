// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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
  const Alphabet* alphabet_;

public:
  /**
   * @brief Build a new AlphabetException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  AlphabetException(const std::string& text, const Alphabet* alpha);

  /**
   * @brief Build a new AlphabetException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  AlphabetException(const std::string& text, std::shared_ptr<const Alphabet> alpha);

  AlphabetException(const AlphabetException& ae) :
    Exception(ae),
    alphabet_(ae.alphabet_)
  {}

  AlphabetException& operator=(const AlphabetException& ae)
  {
    Exception::operator=(ae);
    alphabet_ = ae.alphabet_;
    return *this;
  }

  virtual ~AlphabetException()
  {}

public:
  /**
   * @brief Get the alphabet that threw the exception.
   *
   * @return a const pointer toward the alphabet.
   */
  virtual const Alphabet* getAlphabet() const
  {
    return alphabet_;
  }
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
  BadCharException(const std::string& badChar, const std::string& text, const Alphabet* alpha);

  /**
   * @brief Build a new BadCharException.
   *
   * @param badChar The faulty character.
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  BadCharException(const std::string& badChar, const std::string& text, std::shared_ptr<const Alphabet> alpha);

  virtual ~BadCharException()
  {}

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
  BadIntException(int badInt, const std::string& text, const Alphabet* alpha);

  /**
   * @brief Build a new BadIntException.
   * @param badInt The faulty integer.
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  BadIntException(int badInt, const std::string& text, std::shared_ptr<const Alphabet> alpha);

  virtual ~BadIntException()
  {}

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
  const Alphabet* alphabet1_;
  const Alphabet* alphabet2_;

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
      const Alphabet* alpha1,
      const Alphabet* alpha2);

  /**
   * @brief Build a new AlphabetMismatchException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha1 A const smart pointer toward the first alphabet.
   * @param alpha2 A const smart pointer toward the second alphabet, i.e. the one which does not match with the first.
   */
  AlphabetMismatchException(
      const std::string& text,
      std::shared_ptr<const Alphabet> alpha1,
      std::shared_ptr<const Alphabet> alpha2);

  AlphabetMismatchException(const AlphabetMismatchException& ame) :
    Exception(ame),
    alphabet1_(ame.alphabet1_),
    alphabet2_(ame.alphabet2_)
  {}

  AlphabetMismatchException& operator=(const AlphabetMismatchException& ame)
  {
    Exception::operator=(ame);
    alphabet1_ = ame.alphabet1_;
    alphabet2_ = ame.alphabet2_;
    return *this;
  }


  virtual ~AlphabetMismatchException()
  {}

public:
  /**
   * @brief Get the first alphabet.
   *
   * @return A reference toward the first alphabet.
   */
  const Alphabet* getFirstAlphabet() const
  {
    return alphabet1_;
  }

  /**
   * @brief Get the second alphabet.
   *
   * @return A reference toward the seocnd alphabet.
   */
  const Alphabet* getSecondAlphabet() const
  {
    return alphabet2_;
  }
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
  CharStateNotSupportedException(const std::string& text, const Alphabet* alpha);

  /**
   * @brief Build a new CharStateNotSupportedException.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param alpha A const pointer toward the alphabet that threw the exception.
   */
  CharStateNotSupportedException(const std::string& text, std::shared_ptr<const Alphabet> alpha);

  virtual ~CharStateNotSupportedException()
  {}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_ALPHABETEXCEPTIONS_H
