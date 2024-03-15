// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AACHENGUHUANGHYDROPHOBICITYINDEX_H
#define BPP_SEQ_ALPHABETINDEX_AACHENGUHUANGHYDROPHOBICITYINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Hydrophobicity of each amino acid, according to Table 1 in Chen, Gu and Huang, BMC Bioinformatics 2006.
 *
 * @code
 * Database: AAChenGuHuangHydrophobicity
 * Entry: CHENH06
 *
 * H FAUJ880111
 * D Hydrophobicity (Chen et al., 2006)
 * R PMCID:PMC1780123 PMID:17217506
 * A Hang Chen, Fei Gu, and Zhengge Huang.
 * T Improved Chou-Fasman method for protein secondary structure prediction
 * J BMC Bioinformatics. 2006; 7(Suppl 4): S14.  (2006)
 * //
 * @endcode
 *
 */
class AAChenGuHuangHydrophobicityIndex :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> hydrophobicity_;

public:
  AAChenGuHuangHydrophobicityIndex() :
    ProteicAlphabetIndex1(),
    hydrophobicity_()
  {
    hydrophobicity_.resize(20);
    hydrophobicity_[ 0] =  0.87; // A
    hydrophobicity_[ 1] =  0.85; // R
    hydrophobicity_[ 2] =  0.09; // N
    hydrophobicity_[ 3] =  0.66; // D
    hydrophobicity_[ 4] =  1.52; // C
    hydrophobicity_[ 5] =  0.00; // Q
    hydrophobicity_[ 6] =  0.67; // E
    hydrophobicity_[ 7] =  0.00; // G
    hydrophobicity_[ 8] =  0.87; // H
    hydrophobicity_[ 9] =  3.15; // I
    hydrophobicity_[10] =  2.17; // L
    hydrophobicity_[11] =  1.64; // K
    hydrophobicity_[12] =  1.67; // M
    hydrophobicity_[13] =  2.87; // F
    hydrophobicity_[14] =  2.77; // P
    hydrophobicity_[15] =  0.07; // S
    hydrophobicity_[16] =  0.07; // T
    hydrophobicity_[17] =  3.77; // W
    hydrophobicity_[18] =  2.76; // Y
    hydrophobicity_[19] =  1.87; // V
  }

  virtual ~AAChenGuHuangHydrophobicityIndex() {}

  AAChenGuHuangHydrophobicityIndex* clone() const override {
    return new AAChenGuHuangHydrophobicityIndex();
  }

public:
  double getIndex(int state) const override
  {
    return hydrophobicity_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return hydrophobicity_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override { return hydrophobicity_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AACHENGUHUANGHYDROPHOBICITYINDEX_H
