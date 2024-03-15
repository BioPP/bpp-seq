// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AASEAINF10INDEX_H
#define BPP_SEQ_ALPHABETINDEX_AASEAINF10INDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Percentage of amino acids having a Solvent Exposed Area below 10 AngstrÃ¶m^2 for each type of amino acid, according to http://prowl.rockefeller.edu/aainfo/access.htm.
 */
class AASEAInf10Index :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> seaInf10_;

public:
  AASEAInf10Index() :
    ProteicAlphabetIndex1(),
    seaInf10_()
  {
    seaInf10_.resize(20);
    seaInf10_[ 0] =  0.35; // A
    seaInf10_[ 1] =  0.05; // R
    seaInf10_[ 2] =  0.10; // N
    seaInf10_[ 3] =  0.09; // D
    seaInf10_[ 4] =  0.54; // C
    seaInf10_[ 5] =  0.10; // Q
    seaInf10_[ 6] =  0.04; // E
    seaInf10_[ 7] =  0.36; // G
    seaInf10_[ 8] =  0.19; // H
    seaInf10_[ 9] =  0.47; // I
    seaInf10_[10] =  0.49; // L
    seaInf10_[11] =  0.02; // K
    seaInf10_[12] =  0.20; // M
    seaInf10_[13] =  0.42; // F
    seaInf10_[14] =  0.13; // P
    seaInf10_[15] =  0.20; // S
    seaInf10_[16] =  0.16; // T
    seaInf10_[17] =  0.44; // W
    seaInf10_[18] =  0.20; // Y
    seaInf10_[19] =  0.50; // V
  }

  virtual ~AASEAInf10Index() {}

  AASEAInf10Index* clone() const override { return new AASEAInf10Index(); }

public:
  double getIndex(int state) const override
  {
    return seaInf10_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return seaInf10_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override { return seaInf10_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AASEAINF10INDEX_H
