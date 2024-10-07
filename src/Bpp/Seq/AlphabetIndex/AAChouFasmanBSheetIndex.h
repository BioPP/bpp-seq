// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AACHOUFASMANBSHEETINDEX_H
#define BPP_SEQ_ALPHABETINDEX_AACHOUFASMANBSHEETINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief B-sheet score for the Chou-Fasman algorithm of secondary structure prediction, according to http://prowl.rockefeller.edu/aainfo/chou.htm
 *
 *
 */
class AAChouFasmanBSheetIndex :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> bSheet_;

public:
  AAChouFasmanBSheetIndex() :
    ProteicAlphabetIndex1(),
    bSheet_()
  {
    bSheet_.resize(20);
    bSheet_[ 0] =  83; // A
    bSheet_[ 1] =  93; // R
    bSheet_[ 2] =  89; // N
    bSheet_[ 3] =  54; // D
    bSheet_[ 4] =  119; // C
    bSheet_[ 5] =  110; // Q
    bSheet_[ 6] =  37; // E
    bSheet_[ 7] =  75; // G
    bSheet_[ 8] =  87; // H
    bSheet_[ 9] =  160; // I
    bSheet_[10] =  130; // L
    bSheet_[11] =  74; // K
    bSheet_[12] =  105; // M
    bSheet_[13] =  138; // F
    bSheet_[14] =  55; // P
    bSheet_[15] =  75; // S
    bSheet_[16] =  119; // T
    bSheet_[17] =  137; // W
    bSheet_[18] =  147; // Y
    bSheet_[19] =  170; // V
  }

  virtual ~AAChouFasmanBSheetIndex()
  {}

  AAChouFasmanBSheetIndex* clone() const override
  {
    return new AAChouFasmanBSheetIndex();
  }

public:
  double getIndex(int state) const override
  {
    return bSheet_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return bSheet_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override
  {
    return bSheet_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AACHOUFASMANBSHEETINDEX_H
