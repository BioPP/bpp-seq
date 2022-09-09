//
// File: AAMassIndex.h
// Authors:
//   Bastien Boussau
// Created: 2011-01-14 10:31:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

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

  const std::vector<double>& getIndexVector() const override { return mass_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AAMASSINDEX_H
