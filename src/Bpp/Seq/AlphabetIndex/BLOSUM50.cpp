//
// File: BLOSUM50.cpp
// Created by: Julien Dutheil
// Created on: Tue Jan 18 10:28 2007
//


/*
   Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#include "BLOSUM50.h"

#include "../Alphabet/AlphabetTools.h"

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
  if (state1 < 0 || state1 > 19)
    throw BadIntException(state1, "BLOSUM50::getIndex(). Invalid state1.", getAlphabet());
  if (state2 < 0 || state2 > 19)
    throw BadIntException(state2, "BLOSUM50::getIndex(). Invalid state2.", getAlphabet());
  double d = distanceMatrix_(static_cast<size_t>(state1), static_cast<size_t>(state2));
  return d;
}

double BLOSUM50::getIndex(const std::string& state1, const std::string& state2) const
{
  return getIndex(getAlphabet()->charToInt(state1), getAlphabet()->charToInt(state2));
}

LinearMatrix<double>* BLOSUM50::getIndexMatrix() const
{
  return new LinearMatrix<double>(distanceMatrix_);
}

