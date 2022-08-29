//
// File: BinaryAlphabet.cpp
// Authors: Laurent Gueguen
// Created on: vendredi 20 septembre 2013, � 23h 10
//

/*
   Copyright or � or Copr. Bio++ Development Team, (November 17, 2004)

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


#include "IntegerAlphabet.h"
#include "AlphabetState.h"

// From Utils:
#include <Bpp/Text/TextTools.h>

using namespace bpp;

IntegerAlphabet::IntegerAlphabet(unsigned int max, unsigned int min) : MIN_(min), MAX_(max)
{
  // Alphabet size definition
  resize(MAX_-MIN_+3); // the vector should be resized such that it will include min, max, unresolved state, and gap

  // Alphabet content definition
  registerState(new AlphabetState(-1, "-", "Gap"));

  for (int i = static_cast<int>(MIN_); i <= static_cast<int>(MAX_); i++)
  {
    registerState(new AlphabetState(i, TextTools::toString(i), ""));
  }
  registerState(new AlphabetState(MAX_+1, "X", "Unresolved state"));
}


/********************************************************************************/
std::vector<int> IntegerAlphabet::getAlias(int state) const{
  if (!isIntInAlphabet(state)) throw BadIntException(state, "IntegerAlphabet::getAlias(int): Specified integer unknown.");
  std::vector<int> v;
  if (state >= static_cast<int>(MAX_+1)){
    for (int i = static_cast<int>(MIN_); i <= static_cast<int>(MAX_); i++){
      v.push_back(i);
    }
  }else{
    v.push_back(state);
  }
  return v;
}

/********************************************************************************/
std::vector<std::string> IntegerAlphabet::getAlias(const std::string& state) const{
  if (!isCharInAlphabet(state)) throw BadCharException(state, "IntegerAlphabet::getAlias(char): Specified integer unknown.");
  std::vector<std::string> v;
  if (state == "X"){
    for (int i = static_cast<int>(MIN_); i <= static_cast<int>(MAX_); i++){
      v.push_back(intToChar(i));
    }
  }else if (charToInt(state) <= static_cast<int>(MAX_)){
    v.push_back(state);
  }else{
    throw Exception("IntegerAlphabet::getAlias(char): unknown state!");

  } 
  return v;
}
/********************************************************************************/
bool IntegerAlphabet::isResolvedIn(int state1, int state2) const{
  if (state1 < 0 || !isIntInAlphabet(state1))
    throw BadIntException(state1, "IntegerAlphabet::isResolvedIn(int, int): Specified base unknown.");

  if (state2 < 0 || !isIntInAlphabet(state2))
    throw BadIntException(state2, "IntegerAlphabet::isResolvedIn: Specified base unknown.");

  if (isUnresolved(state2))
    throw BadIntException(state2, "IntegerAlphabet::isResolvedIn: Unresolved base."); 

  if (state2 > static_cast<int>(MAX_))
    throw IndexOutOfBoundsException("IntegerAlphabet::isResolvedIn", state2, 0, getNumberOfTypes() - 1);
  std::vector<int> states = getAlias(state1);
  for (size_t j = 0; j < states.size(); j++)
  {
     if (state2 == states[j]){
        return true;

     }
  }
  return false;
}

