// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AAVOLUMEINDEX_H
#define BPP_SEQ_ALPHABETINDEX_AAVOLUMEINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Volume (AngstrÃ¶m^3) of each amino acid, according to http://www.imb-jena.de/IMAGE_AA.html (Note: original webpage is not available anymore)
 *
 *
 */
class AAVolumeIndex :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> volume_;

public:
  AAVolumeIndex() :
    ProteicAlphabetIndex1(),
    volume_()
  {
    volume_.resize(20);
    volume_[ 0] =  88.6; // A
    volume_[ 1] =  173.4; // R
    volume_[ 2] =  114.1; // N
    volume_[ 3] =  111.1; // D
    volume_[ 4] =  108.5; // C
    volume_[ 5] =  143.8; // Q
    volume_[ 6] =  138.4; // E
    volume_[ 7] =  60.1; // G
    volume_[ 8] =  153.2; // H
    volume_[ 9] =  166.7; // I
    volume_[10] =  166.7; // L
    volume_[11] =  168.6; // K
    volume_[12] =  162.9; // M
    volume_[13] =  189.9; // F
    volume_[14] =  112.7; // P
    volume_[15] =  89; // S
    volume_[16] =  116.1; // T
    volume_[17] =  227.8; // W
    volume_[18] =  193.6; // Y
    volume_[19] =  140; // V
  }

  virtual ~AAVolumeIndex() {}

  AAVolumeIndex* clone() const override { return new AAVolumeIndex(); }

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
#endif // BPP_SEQ_ALPHABETINDEX_AAVOLUMEINDEX_H
