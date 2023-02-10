//
// File: MiyataAAChemicalDistance.cpp
// Authors:
//   Julien Dutheil
// Created: 2005-02-21 17:42:00
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



// from the STL:
#include <string>

using namespace std;

#include "MiyataAAChemicalDistance.h"
#include "../Alphabet/AlphabetTools.h"
#include <Bpp/Numeric/NumTools.h>

using namespace bpp;

MiyataAAChemicalDistance::MiyataAAChemicalDistance() :
  ProteicAlphabetIndex2(),
  distanceMatrix_(20, 20),
  indexMatrix_(20, 20),
  sym_(true)
{
  #include "__MiyataMatrixCode"
}

double MiyataAAChemicalDistance::getIndex(int state1, int state2) const
{
  double d = distanceMatrix_(getAlphabet()->getStateIndex(state1)-1,getAlphabet()->getStateIndex(state2)-1);
  return sym_ ? NumTools::abs<double>(d) : d;
}

double MiyataAAChemicalDistance::getIndex(const string& state1, const string& state2) const
{
  return getIndex(getAlphabet()->charToInt(state1), getAlphabet()->charToInt(state2));
}

void MiyataAAChemicalDistance::computeIndexMatrix_()
{
  indexMatrix_ = distanceMatrix_;
  if (sym_)
  {
    for (unsigned int i = 0; i < 20; i++)
    {
      for (unsigned int j = 0; j < 20; j++)
      {
        indexMatrix_(i, j) = NumTools::abs<double>(indexMatrix_(i, j));
      }
    }
  }
}
