// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AASURFACEINDEX_H
#define BPP_SEQ_ALPHABETINDEX_AASURFACEINDEX_H


#include "../Alphabet/AlphabetTools.h"
#include "../Alphabet/ProteicAlphabet.h"
#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Surface (AngstrÃ¶m^2) of each amino acid, according to
 * http://www.imb-jena.de/IMAGE_AA.html
 */
class AASurfaceIndex :
  public ProteicAlphabetIndex1
{
private:
  std::vector<double> surface_;

public:
  AASurfaceIndex() :
    ProteicAlphabetIndex1(),
    surface_()
  {
    surface_.resize(20);
    surface_[ 0] =  115; // A
    surface_[ 1] =  225; // R
    surface_[ 2] =  160; // N
    surface_[ 3] =  150; // D
    surface_[ 4] =  135; // C
    surface_[ 5] =  180; // Q
    surface_[ 6] =  190; // E
    surface_[ 7] =  75; // G
    surface_[ 8] =  195; // H
    surface_[ 9] =  175; // I
    surface_[10] =  170; // L
    surface_[11] =  200; // K
    surface_[12] =  185; // M
    surface_[13] =  210; // F
    surface_[14] =  145; // P
    surface_[15] =  115; // S
    surface_[16] =  140; // T
    surface_[17] =  255; // W
    surface_[18] =  230; // Y
    surface_[19] =  155; // V
  }

  virtual ~AASurfaceIndex() {}

  AASurfaceIndex* clone() const override { return new AASurfaceIndex(); }

public:
  double getIndex(int state) const override
  {
    return surface_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return surface_[getAlphabet()->getStateIndex(state) - 1];
  }

  const std::vector<double>& indexVector() const override { return surface_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AASURFACEINDEX_H
