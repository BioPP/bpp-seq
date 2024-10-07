// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

// from the STL:
#include <string>

#include "GranthamAAChemicalDistance.h"
#include "../Alphabet/AlphabetTools.h"
#include <Bpp/Numeric/NumTools.h>

using namespace bpp;
using namespace std;

short int GranthamAAChemicalDistance::SIGN_NONE = 0;
short int GranthamAAChemicalDistance::SIGN_ARBITRARY = 1;
short int GranthamAAChemicalDistance::SIGN_PC1 = 2;

GranthamAAChemicalDistance::GranthamAAChemicalDistance() :
  ProteicAlphabetIndex2(),
  distanceMatrix_(20, 20),
  signMatrix_(20, 20),
  indexMatrix_(20, 20),
  sign_(0)
{
  // Load the matrix:
  #include "__GranthamMatrixCode"
  computeIndexMatrix_();
}

double GranthamAAChemicalDistance::getIndex(int state1, int state2) const
{
  double d = distanceMatrix_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
  if (sign_ == SIGN_NONE)
    return NumTools::abs<double>(d);
  if (sign_ == SIGN_PC1)
    return signMatrix_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1) * NumTools::abs<double>(d);

  return d;
}

double GranthamAAChemicalDistance::getIndex(const std::string& state1, const std::string& state2) const
{
  return getIndex(getAlphabet()->charToInt(state1), getAlphabet()->charToInt(state2));
}

void GranthamAAChemicalDistance::computeIndexMatrix_()
{
  indexMatrix_ = distanceMatrix_;
  if (sign_ == SIGN_NONE)
  {
    for (size_t i = 0; i < 20; ++i)
    {
      for (size_t j = 0; j < 20; ++j)
      {
        indexMatrix_(i, j) = NumTools::abs<double>(indexMatrix_(i, j));
      }
    }
  }
  else if (sign_ == SIGN_PC1)
  {
    for (size_t i = 0; i < 20; ++i)
    {
      for (size_t j = 0; j < 20; ++j)
      {
        indexMatrix_(i, j) = signMatrix_(i, j) * NumTools::abs<double>(indexMatrix_(i, j));
      }
    }
  }
}
