// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AAMASSINDEX_H
#define BPP_SEQ_ALPHABETINDEX_AAMASSINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Mass (dalton) of each amino acid, according to http://www.imb-jena.de/IMAGE_AA.html.
 */
class AAMassIndex :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> mass_;

public:
  AAMassIndex() :
    ProteicAlphabetIndex1(),
    mass_()
  {
    mass_.resize(20);
    mass_[ 0] =  71.09; // A
    mass_[ 1] =  156.19; // R
    mass_[ 2] =  114.11; // N
    mass_[ 3] =  115.09; // D
    mass_[ 4] =  103.15; // C
    mass_[ 5] =  128.14; // Q
    mass_[ 6] =  129.12; // E
    mass_[ 7] =  57.05; // G
    mass_[ 8] =  137.14; // H
    mass_[ 9] =  113.16; // I
    mass_[10] =  113.16; // L
    mass_[11] =  128.17; // K
    mass_[12] =  131.19; // M
    mass_[13] =  147.18; // F
    mass_[14] =  97.12; // P
    mass_[15] =  87.08; // S
    mass_[16] =  101.11; // T
    mass_[17] =  186.12; // W
    mass_[18] =  163.18; // Y
    mass_[19] =  99.14; // V
  }

  virtual ~AAMassIndex() {}

  AAMassIndex* clone() const override { return new AAMassIndex(); }

public:
  double getIndex(int state) const override
  {
    return mass_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return mass_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override { return mass_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AAMASSINDEX_H
