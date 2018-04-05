//
// File: KD_AAHydropathyIndex.h
// Created by: Laurent Guéguen
// Created on: mercredi 28 mars 2018, à 11h 12
//

/*
  Copyright or © or Copr. Bio++ Development Team, (March, 28, 2018)

  This software is a computer program whose purpose is to provide classes
  for sequences analysis.

  This software is governed by the CeCILL  license under French law and
  abiding by the rules of distribution of free software.  You can  use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".

  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited
  liability.

  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and,  more generally, to use and operate it in the
  same conditions as regards security.

  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _KD_AAHYDROPATHYINDEX_H_
#define _KD_AAHYDROPATHYINDEX_H_

#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Hydropathy index defined in Kyte & Doolitle (1982).
 *
 * Kyte, J. and Doolittle, R.F., J. Mol. Biol., 157, 105-132 (1982).
 *
 */

  class KD_AAHydropathyIndex:
    public virtual ProteicAlphabetIndex1
  {
  private:
    std::vector<double> hydropathy_;

  public:
    KD_AAHydropathyIndex():
      ProteicAlphabetIndex1(),
      hydropathy_()
    {
      hydropathy_.resize(20);
      hydropathy_[ 0] = 1.8 ; // A
      hydropathy_[ 1] = -4.5; // R
      hydropathy_[ 2] = -3.5; // N
      hydropathy_[ 3] = -3.5; // D
      hydropathy_[ 4] = 2.5 ; // C
      hydropathy_[ 5] = -3.5; // Q
      hydropathy_[ 6] = -3.5; // E
      hydropathy_[ 7] = -0.4; // G
      hydropathy_[ 8] = -3.2; // H
      hydropathy_[ 9] = 4.5 ; // I
      hydropathy_[10] = 3.8 ; // L
      hydropathy_[11] = -3.9; // K
      hydropathy_[12] = 1.9 ; // M
      hydropathy_[13] = 2.8 ; // F
      hydropathy_[14] = -1.6; // P
      hydropathy_[15] = -0.8; // S
      hydropathy_[16] = -0.7; // T
      hydropathy_[17] = -0.9; // W
      hydropathy_[18] = -1.3; // Y
      hydropathy_[19] = 4.2 ; // V
    }
 
    virtual ~KD_AAHydropathyIndex() {}

    KD_AAHydropathyIndex* clone() const { return new KD_AAHydropathyIndex(); }

  public:
    double getIndex(int state) const
    {
      if (state < 0 || state > 19) throw BadIntException(state, "KD_AAHydropathyIndex::getIndex(). Invalid state.", getAlphabet());
      return hydropathy_[static_cast<size_t>(state)];
    }

    double getIndex(const std::string& state) const
    {
      return hydropathy_[static_cast<size_t>(getAlphabet()->charToInt(state))];
    }

    std::vector<double>* getIndexVector() const { return new std::vector<double>(hydropathy_); }

  };
} // end of namespace bpp.

#endif // _KD_AAHYDROPATHYINDEX_H_

