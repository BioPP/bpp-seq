// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "DNAToRNA.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

int DNAToRNA::translate(int state) const
{
  dna_->intToChar(state);
  return state;
}

/******************************************************************************/

string DNAToRNA::translate(const std::string& state) const
{
  int i = dna_->charToInt(state);
  return rna_->intToChar(i);
}

/******************************************************************************/

int DNAToRNA::reverse(int state) const
{
  rna_->intToChar(state);
  return state;
}

/******************************************************************************/

string DNAToRNA::reverse(const std::string& state) const
{
  int i = rna_->charToInt(state);
  return dna_->intToChar(i);
}

/******************************************************************************/
