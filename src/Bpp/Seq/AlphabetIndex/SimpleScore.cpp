// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

// from the STL:
#include <string>

using namespace std;

#include "SimpleScore.h"

using namespace bpp;

SimpleScore::SimpleScore(const Alphabet* alphabet, double match, double mismatch) :
  distanceMatrix_(alphabet->getSize(), alphabet->getSize()),
  alphabet_(alphabet)
{
  // Load the matrix:
  size_t n = alphabet_->getSize();
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 0; j < n; ++j)
    {
      distanceMatrix_(i, j) = (i == j ? match : mismatch);
    }
  }
}

double SimpleScore::getIndex(int state1, int state2) const
{
  size_t stateIndex1 = alphabet_->getStateIndex(state1) - 1;
  size_t stateIndex2 = alphabet_->getStateIndex(state2) - 1;
  return distanceMatrix_(stateIndex1, stateIndex2);
}

double SimpleScore::getIndex(const std::string& state1, const std::string& state2) const
{
  size_t stateIndex1 = alphabet_->getStateIndex(state1) - 1;
  size_t stateIndex2 = alphabet_->getStateIndex(state2) - 1;
  return distanceMatrix_(stateIndex1, stateIndex2);
}

