// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_GRANTHAMAAPOLARITYINDEX_H
#define BPP_SEQ_ALPHABETINDEX_GRANTHAMAAPOLARITYINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Polarity index used in Grantham (1974).
 *
 * @code
 * Database: AAindex1
 * Entry: GRAR740102
 *
 * H GRAR740102
 * D Polarity (Grantham, 1974)
 * R LIT:2004143b PMID:4843792
 * A Grantham, R.
 * T Amino acid difference formula to help explain protein evolution
 * J Science 185, 862-864 (1974)
 * I    A/L     R/K     N/M     D/F     C/P     Q/S     E/T     G/W     H/Y     I/V
 *      8.1    10.5    11.6    13.0     5.5    10.5    12.3     9.0    10.4     5.2
 *      4.9    11.3     5.7     5.2     8.0     9.2     8.6     5.4     6.2     5.9
 * //
 * @endcode
 */
class GranthamAAPolarityIndex :
  public virtual ProteicAlphabetIndex1
{
private:
  std::vector<double> polarity_;

public:
  GranthamAAPolarityIndex() :
    ProteicAlphabetIndex1(),
    polarity_()
  {
    polarity_.resize(20);
    polarity_[ 0] =  8.1; // A
    polarity_[ 1] = 10.5; // R
    polarity_[ 2] = 11.6; // N
    polarity_[ 3] = 13.0; // D
    polarity_[ 4] =  5.5; // C
    polarity_[ 5] = 10.5; // Q
    polarity_[ 6] = 12.3; // E
    polarity_[ 7] =  9.0; // G
    polarity_[ 8] = 10.4; // H
    polarity_[ 9] =  5.2; // I
    polarity_[10] =  4.9; // L
    polarity_[11] = 11.3; // K
    polarity_[12] =  5.7; // M
    polarity_[13] =  5.2; // F
    polarity_[14] =  8.0; // P
    polarity_[15] =  9.2; // S
    polarity_[16] =  8.6; // T
    polarity_[17] =  5.4; // W
    polarity_[18] =  6.2; // Y
    polarity_[19] =  5.9; // V
  }

  virtual ~GranthamAAPolarityIndex() {}

  GranthamAAPolarityIndex* clone() const override { return new GranthamAAPolarityIndex(); }

public:
  double getIndex(int state) const override
  {
    return polarity_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return polarity_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override { return polarity_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_GRANTHAMAAPOLARITYINDEX_H
