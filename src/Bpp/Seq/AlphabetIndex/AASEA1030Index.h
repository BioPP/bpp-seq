// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AASEA1030INDEX_H
#define BPP_SEQ_ALPHABETINDEX_AASEA1030INDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Percentage of amino acids having a Solvent Exposed Area
 * between 10 and 30 AngstrÃ¶m^2 for each type of amino acid, according
 * to http://prowl.rockefeller.edu/aainfo/access.htm.
 */

class AASEA1030Index :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> sea1030_;

public:
  AASEA1030Index() :
    sea1030_()
  {
    sea1030_.resize(20);
    sea1030_[ 0] =  0.17; // A
    sea1030_[ 1] =  0.11; // R
    sea1030_[ 2] =  0.08; // N
    sea1030_[ 3] =  0.10; // D
    sea1030_[ 4] =  0.14; // C
    sea1030_[ 5] =  0.09; // Q
    sea1030_[ 6] =  0.03; // E
    sea1030_[ 7] =  0.13; // G
    sea1030_[ 8] =  0.15; // H
    sea1030_[ 9] =  0.14; // I
    sea1030_[10] =  0.10; // L
    sea1030_[11] =  0.05; // K
    sea1030_[12] =  0.36; // M
    sea1030_[13] =  0.16; // F
    sea1030_[14] =  0.09; // P
    sea1030_[15] =  0.10; // S
    sea1030_[16] =  0.13; // T
    sea1030_[17] =  0.07; // W
    sea1030_[18] =  0.13; // Y
    sea1030_[19] =  0.10; // V
  }

  virtual ~AASEA1030Index()
  {}

  AASEA1030Index* clone() const override
  {
    return new AASEA1030Index();
  }

public:
  double getIndex(int state) const override
  {
    return sea1030_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return sea1030_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override
  {
    return sea1030_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AASEA1030INDEX_H
