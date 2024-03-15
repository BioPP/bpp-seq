// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AACHOUFASMANTURNINDEX_H
#define BPP_SEQ_ALPHABETINDEX_AACHOUFASMANTURNINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Turn score for the Chou-Fasman algorithm of secondary structure prediction, according to http://prowl.rockefeller.edu/aainfo/chou.htm
 *
 *
 */
class AAChouFasmanTurnIndex :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> turn_;

public:
  AAChouFasmanTurnIndex() :
    ProteicAlphabetIndex1(),
    turn_()
  {
    turn_.resize(20);
    turn_[ 0] =  66; // A
    turn_[ 1] =  95; // R
    turn_[ 2] =  156; // N
    turn_[ 3] =  146; // D
    turn_[ 4] =  119; // C
    turn_[ 5] =  98; // Q
    turn_[ 6] =  74; // E
    turn_[ 7] =  156; // G
    turn_[ 8] =  95; // H
    turn_[ 9] =  47; // I
    turn_[10] =  59; // L
    turn_[11] =  101; // K
    turn_[12] =  60; // M
    turn_[13] =  60; // F
    turn_[14] =  152; // P
    turn_[15] =  143; // S
    turn_[16] =  96; // T
    turn_[17] =  96; // W
    turn_[18] =  114; // Y
    turn_[19] =  50; // V
  }

  virtual ~AAChouFasmanTurnIndex() {}

  AAChouFasmanTurnIndex* clone() const override {
    return new AAChouFasmanTurnIndex();
  }

public:
  double getIndex(int state) const override
  {
    return turn_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return turn_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override {
    return turn_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AACHOUFASMANTURNINDEX_H
