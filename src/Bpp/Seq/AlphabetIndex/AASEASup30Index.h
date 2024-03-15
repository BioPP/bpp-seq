// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AASEASUP30INDEX_H
#define BPP_SEQ_ALPHABETINDEX_AASEASUP30INDEX_H


#include "../Alphabet/AlphabetTools.h"
#include "../Alphabet/ProteicAlphabet.h"
#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Percentage of amino acids having a Solvent Exposed Area
 * above 30 AngstrÃ¶m^2 for each type of amino acid, according to
 * http://prowl.rockefeller.edu/aainfo/access.htm
 */
class AASEASup30Index :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> seaSup30_;

public:
  AASEASup30Index() :
    ProteicAlphabetIndex1(),
    seaSup30_()
  {
    seaSup30_.resize(20);
    seaSup30_[ 0] =  0.48; // A
    seaSup30_[ 1] =  0.84; // R
    seaSup30_[ 2] =  0.82; // N
    seaSup30_[ 3] =  0.81; // D
    seaSup30_[ 4] =  0.32; // C
    seaSup30_[ 5] =  0.81; // Q
    seaSup30_[ 6] =  0.93; // E
    seaSup30_[ 7] =  0.51; // G
    seaSup30_[ 8] =  0.66; // H
    seaSup30_[ 9] =  0.39; // I
    seaSup30_[10] =  0.41; // L
    seaSup30_[11] =  0.93; // K
    seaSup30_[12] =  0.44; // M
    seaSup30_[13] =  0.42; // F
    seaSup30_[14] =  0.78; // P
    seaSup30_[15] =  0.70; // S
    seaSup30_[16] =  0.71; // T
    seaSup30_[17] =  0.49; // W
    seaSup30_[18] =  0.67; // Y
    seaSup30_[19] =  0.40; // V
  }

  virtual ~AASEASup30Index() {}

  AASEASup30Index* clone() const override { return new AASEASup30Index(); }

public:
  double getIndex(int state) const override
  {
    return seaSup30_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return seaSup30_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override { return seaSup30_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AASEASUP30INDEX_H
