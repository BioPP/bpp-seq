// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_GRANTHAMAAVOLUMEINDEX_H
#define BPP_SEQ_ALPHABETINDEX_GRANTHAMAAVOLUMEINDEX_H


#include "AlphabetIndex1.h"

// From the STL:
#include <vector>

namespace bpp
{
/**
 * @brief Volume index used in Grantham (1974).
 *
 * @code
 * Database: AAindex1
 * Entry: GRAR740103
 *
 * H GRAR740103
 * D Volume (Grantham, 1974)
 * R LIT:2004143b PMID:4843792
 * A Grantham, R.
 * T Amino acid difference formula to help explain protein evolution
 * J Science 185, 862-864 (1974)
 * I    A/L     R/K     N/M     D/F     C/P     Q/S     E/T     G/W     H/Y     I/V
 *      31.    124.     56.     54.     55.     85.     83.      3.     96.    111.
 *     111.    119.    105.    132.    32.5     32.     61.    170.    136.     84.
 * //
 * @endcode
 */
class GranthamAAVolumeIndex :
  public virtual ProteicAlphabetIndex1
{
private:
  std::vector<double> volume_;

public:
  GranthamAAVolumeIndex() :
    ProteicAlphabetIndex1(),
    volume_()
  {
    volume_.resize(20);
    volume_[ 0] =  31.; // A
    volume_[ 1] = 124.; // R
    volume_[ 2] =  56.; // N
    volume_[ 3] =  54.; // D
    volume_[ 4] =  55.; // C
    volume_[ 5] =  85.; // Q
    volume_[ 6] =  83.; // E
    volume_[ 7] =   3.; // G
    volume_[ 8] =  96.; // H
    volume_[ 9] = 111.; // I
    volume_[10] = 111.; // L
    volume_[11] = 119.; // K
    volume_[12] = 105.; // M
    volume_[13] = 132.; // F
    volume_[14] =  32.5; // P
    volume_[15] =  32.; // S
    volume_[16] =  61.; // T
    volume_[17] = 170.; // W
    volume_[18] = 136.; // Y
    volume_[19] =  84.; // V
  }

  virtual ~GranthamAAVolumeIndex() {}

  GranthamAAVolumeIndex* clone() const override { return new GranthamAAVolumeIndex(); }

public:
  double getIndex(int state) const override
  {
    return volume_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return volume_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override { return volume_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_GRANTHAMAAVOLUMEINDEX_H
