// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_BINARYALPHABET_H
#define BPP_SEQ_ALPHABET_BINARYALPHABET_H


#include "AbstractAlphabet.h"

namespace bpp
{
/**
 * @brief The BinaryAlphabet class, letters are 0 and 1.
 * @author Laurent Gueguen
 *
 */
class BinaryAlphabet :
  public AbstractAlphabet
{
public:
  // class constructor
  BinaryAlphabet();

  BinaryAlphabet(const BinaryAlphabet& bia) : AbstractAlphabet(bia) {}

  BinaryAlphabet& operator=(const BinaryAlphabet& bia)
  {
    AbstractAlphabet::operator=(bia);

    return *this;
  }

  BinaryAlphabet* clone() const
  {
    return new BinaryAlphabet(*this);
  }

  // class destructor
  virtual ~BinaryAlphabet() {}

public:
  unsigned int getSize() const { return 2; }
  unsigned int getNumberOfTypes() const { return 2; }
  std::string getAlphabetType() const { return "Binary"; }
  int getUnknownCharacterCode() const { return 2; }
  bool isUnresolved(int state) const { return state > 1; }
  bool isUnresolved(const std::string& state) const { return charToInt(state) > 1; }

  bool isResolvedIn(int state1, int state2) const;

  std::vector<int> getAlias(int state) const;
  std::vector<std::string> getAlias(const std::string& state) const;
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_BINARYALPHABET_H
