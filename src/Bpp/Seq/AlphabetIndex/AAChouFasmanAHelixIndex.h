// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AACHOUFASMANAHELIXINDEX_H
#define BPP_SEQ_ALPHABETINDEX_AACHOUFASMANAHELIXINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief A-Helix score for the Chou-Fasman algorithm of secondary structure prediction, according to http://prowl.rockefeller.edu/aainfo/chou.htm
 *
 *
 */
class AAChouFasmanAHelixIndex :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> aHelix_;

public:
  AAChouFasmanAHelixIndex() :
    ProteicAlphabetIndex1(),
    aHelix_()
  {
    aHelix_.resize(20);
    aHelix_[ 0] =  142; // A
    aHelix_[ 1] =  98; // R
    aHelix_[ 2] =  67; // N
    aHelix_[ 3] =  101; // D
    aHelix_[ 4] =  70; // C
    aHelix_[ 5] =  111; // Q
    aHelix_[ 6] =  151; // E
    aHelix_[ 7] =  57; // G
    aHelix_[ 8] =  100; // H
    aHelix_[ 9] =  108; // I
    aHelix_[10] =  121; // L
    aHelix_[11] =  114; // K
    aHelix_[12] =  145; // M
    aHelix_[13] =  113; // F
    aHelix_[14] =  57; // P
    aHelix_[15] =  77; // S
    aHelix_[16] =  83; // T
    aHelix_[17] =  108; // W
    aHelix_[18] =  69; // Y
    aHelix_[19] =  106; // V
  }

  virtual ~AAChouFasmanAHelixIndex() {}

  AAChouFasmanAHelixIndex* clone() const override
  {
    return new AAChouFasmanAHelixIndex();
  }

public:
  double getIndex(int state) const override
  {
    return aHelix_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return aHelix_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override { return aHelix_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AACHOUFASMANAHELIXINDEX_H
