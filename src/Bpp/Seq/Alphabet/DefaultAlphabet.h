// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_DEFAULTALPHABET_H
#define BPP_SEQ_ALPHABET_DEFAULTALPHABET_H

#include <Bpp/Text/TextTools.h>

#include "LetterAlphabet.h"

namespace bpp
{
/**
 * @brief The DefaultAlphabet class.
 *
 * This alphabet should match virtually any type of sequences.
 * This should be used by who does not care of the sequence type.
 */
class DefaultAlphabet :
  public LetterAlphabet
{
protected:
  std::string chars_;

public:
  // class constructor
  DefaultAlphabet();

  DefaultAlphabet(const DefaultAlphabet& bia) : LetterAlphabet(bia), chars_(bia.chars_) {}

  DefaultAlphabet& operator=(const DefaultAlphabet& bia)
  {
    LetterAlphabet::operator=(bia);
    chars_ = bia.chars_;
    return *this;
  }

  DefaultAlphabet* clone() const
  {
    return new DefaultAlphabet(*this);
  }


  // class destructor
  virtual ~DefaultAlphabet() {}

public:
  unsigned int getSize() const { return 26; }
  unsigned int getNumberOfTypes() const { return 27; }
  std::string getAlphabetType() const { return "Default"; }
  int getUnknownCharacterCode() const { return 37; }
  bool isUnresolved(int state) const { return state == 37; }
  bool isUnresolved(const std::string& state) const { return false; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_DEFAULTALPHABET_H
