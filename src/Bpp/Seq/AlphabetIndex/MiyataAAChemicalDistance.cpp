// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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
