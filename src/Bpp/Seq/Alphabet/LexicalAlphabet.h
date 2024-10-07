// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_LEXICALALPHABET_H
#define BPP_SEQ_ALPHABET_LEXICALALPHABET_H


#include "AbstractAlphabet.h"

// From the STL:
#include <string>
#include <vector>

namespace bpp
{
/**
 * @brief Alphabet made of given words.
 *
 */
class LexicalAlphabet :
  public AbstractAlphabet
{
public:
  // Constructor and destructor.
  /**
   * @brief Builds a new word alphabet from a vector of given words.
   *
   * Words unicity is checked.
   *
   * @param vocab The vector of words to be used.
   */

  LexicalAlphabet(const std::vector<std::string>& vocab);

  LexicalAlphabet(const LexicalAlphabet& bia) : AbstractAlphabet(bia)
  {}

  LexicalAlphabet& operator=(const LexicalAlphabet& bia)
  {
    AbstractAlphabet::operator=(bia);
    return *this;
  }

  LexicalAlphabet* clone() const
  {
    return new LexicalAlphabet(*this);
  }

  virtual ~LexicalAlphabet()
  {}

public:
  /**
   * @name Methods redefined from Alphabet
   *
   * @{
   */
  unsigned int getSize() const
  {
    return getNumberOfChars() - 2;
  }

  /** @} */


  unsigned int getNumberOfTypes() const
  {
    return getNumberOfChars() - 1;
  }

  std::string getAlphabetType() const;

  int getUnknownCharacterCode() const
  {
    return static_cast<int>(getSize());
  }

  bool isUnresolved(int state) const
  {
    return state == getUnknownCharacterCode();
  }
  bool isUnresolved(const std::string& state) const
  {
    return charToInt(state) == getUnknownCharacterCode();
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_LEXICALALPHABET_H
