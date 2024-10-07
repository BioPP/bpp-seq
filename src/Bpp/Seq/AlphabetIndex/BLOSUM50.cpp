// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Alphabet/AlphabetTools.h"
#include "BLOSUM50.h"

// from the STL:
#include <string>

using namespace std;
using namespace bpp;

BLOSUM50::BLOSUM50() :
  ProteicAlphabetIndex2(),
  distanceMatrix_(20, 20)
{
  #include "__BLOSUM50MatrixCode"
}

double BLOSUM50::getIndex(int state1, int state2) const
{
  return distanceMatrix_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
}

double BLOSUM50::getIndex(const std::string& state1, const std::string& state2) const
{
  return distanceMatrix_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
}

const Matrix<double>& BLOSUM50::getIndexMatrix() const
{
  return distanceMatrix_;
}
