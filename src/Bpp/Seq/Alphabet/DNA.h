// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_DNA_H
#define BPP_SEQ_ALPHABET_DNA_H


#include "NucleicAlphabet.h"

// From the STL:
#include <string>
#include <vector>

namespace bpp
{
/**
 * @brief This alphabet is used to deal with DNA sequences.
 *
 * It supports all 4 nucleotides (A, T, G and C) with their standard denomination.
 * Gaps are coded by '-', unresolved characters are coded by 'X, N, O, 0 or ?'.
 * Extensive support for generic characters (e.g. 'P', 'Y', etc.) is provided.
 */
class DNA :
  public NucleicAlphabet
{
public:
  /**
   * @param exclamationMarkCountsAsGap If yes, '!' characters are replaced by gaps.
   * Otherwise, they are counted as unknown characters.
   */
  DNA(bool exclamationMarkCountsAsGap = false);

  DNA(const DNA& bia) : NucleicAlphabet(bia)
  {}

  DNA& operator=(const DNA& bia)
  {
    NucleicAlphabet::operator=(bia);
    return *this;
  }

  DNA* clone() const
  {
    return new DNA(*this);
  }

  virtual ~DNA()
  {}

public:
  bool isResolvedIn(int state1, int state2) const;

  std::vector<int> getAlias(int state) const;
  std::vector<std::string> getAlias(const std::string& state) const;
  int getGeneric(const std::vector<int>& states) const;
  std::string getGeneric(const std::vector<std::string>& states) const;
  std::string getAlphabetType() const
  {
    return "DNA";
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_DNA_H
