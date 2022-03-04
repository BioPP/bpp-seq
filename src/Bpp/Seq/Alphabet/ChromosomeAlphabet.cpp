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


#include "ChromosomeAlphabet.h"
#include "AbstractAlphabet.h"
#include "AlphabetState.h"

// From Utils:
#include <Bpp/Text/TextTools.h>

using namespace bpp;

ChromosomeAlphabet::ChromosomeAlphabet(unsigned int min, unsigned int max) : MIN_(min),MAX_(max), numOfCompositeStates_(0), compositeAlphabetMap_()
{
  // Alphabet size definition
  //resize(MAX_);

  // Alphabet content definition
  registerState(new AlphabetState(-1, "-", "Gap"));

  for (int i = static_cast<int>(MIN_); i < static_cast<int>(MAX_)+1; i++)
  {
    registerState(new AlphabetState(i, TextTools::toString(i), ""));
  }
  registerState(new AlphabetState(static_cast<int>(MAX_)+1, "X", "Unresolved state"));
}

const AlphabetState& ChromosomeAlphabet::getState(int num) const {
      if (num == 0){
        num = (int)MIN_;
      }
      return(AbstractAlphabet::getState(num));

}
/********************************************************************************/
std::vector<int> ChromosomeAlphabet::getAlias(int state) const{
  if (!isIntInAlphabet(state)) throw BadIntException(state, "ChromosomeAlphabet::getAlias(int): Specified chromosome number unknown.");
  std::vector<int> v;
  if (state == static_cast<int>(MAX_+1)){
    for (int i = static_cast<int>(MIN_); i <= static_cast<int>(MAX_); i++){
      v.push_back(i);
    }
  }else if (state <= static_cast<int>(MAX_)) {
    v.push_back(state);
  }else{
    const std::vector<int> states = getSetOfStatesForAComposite(state);
    for (size_t i = 0; i < states.size(); i++){
      v.push_back(states[i]);
    }

  }
  return v;
}
/********************************************************************************/

std::vector<std::string> ChromosomeAlphabet::getAlias(const std::string& state) const{
  if (!isCharInAlphabet(state)) throw BadCharException(state, "ChromosomeAlphabet::getAlias(char): Specified chromosome number unknown.");
  std::vector<std::string> v;
  if (state == "X"){
    for (int i = static_cast<int>(MIN_); i <= static_cast<int>(MAX_); i++){
      v.push_back(intToChar(i));
    }
  }else if (charToInt(state) <= static_cast<int>(MAX_)){
    v.push_back(state);
  }else{
    const std::vector<int> states = getSetOfStatesForAComposite(charToInt(state));
    for (size_t i = 0; i < states.size(); i++){
      v.push_back(intToChar(states[i]));
    }
  } 
  return v;
}
/*******************************************************************/
bool ChromosomeAlphabet::isComposite(const std::string& state) const{
  if (state.find("_") == std::string::npos){
    return false;
  }
  StringTokenizer st(state, "_", false, false);
  std::vector<std::string> v;
  while(st.hasMoreToken()){
    v.push_back(st.nextToken());
  }
  if (state.find("=") == std::string::npos){
    //the probabilities are not given- should check whether all the items are integers
    for (size_t i = 0; i < v.size(); i ++){
      if (!isInteger(v[i])){
        return false;
      }
    }
  }else{
    //The probabilities are given. Should check whether the even numbers are integers and
    //the odd numbers are probabilities
    double sumOfProbabilities = 0;
    for (size_t i = 0; i < v.size(); i++){
      if (v[i].find("=") == std::string::npos){
        return false;
      }
      StringTokenizer splittedPair(v[i], "=", false, false);
      std::vector <std::string> v_state_prob;
      while(splittedPair.hasMoreToken()){
        v_state_prob.push_back(splittedPair.nextToken());
      }
      if (v_state_prob.size() > 2){
        return false;
      }
      if (!isInteger(v_state_prob[0])){
        return false;
      }
      if (!isProbability(v_state_prob[1])){
        return false;
      }
      sumOfProbabilities += atof(v_state_prob[1].c_str());
      if (sumOfProbabilities > 1){
        return false;
      }     
    }
    if (sumOfProbabilities < 1){
      return false;
    }
  }
  return true;
}
/******************************************************************/
bool ChromosomeAlphabet::isInteger(const std::string& str) const{
  for (size_t i = 0; i < str.length(); i++){
    if (!isdigit(str[i])){
      return false;
    }
  }
  int s = atoi(str.c_str());
  if ((s > static_cast<int>(MAX_)) || (s < static_cast<int>(MIN_))){
    return false;
  }
  return true;
}
/*****************************************************************/
bool ChromosomeAlphabet::isProbability(const std::string& str) const{
  for (size_t i = 0; i < str.length(); i++){
    // There is no point for the user to give a list of possible states if one of 
    // them is 1 or 0
    if ((i == 0) && (str[0] != '0')){
      return false;
    }
    if (i == 1){
      if (str[i] != '.'){
        return false;
      } 
    }
    if (i > 1){
      if (!isdigit(str[i])){
        return false;
      }
    }
  }
  return true;
}
/******************************************************************************/
void ChromosomeAlphabet::setCompositeState(std::string& state){
  if (isComposite(state)){
    if (!isCharInAlphabet(state)){
      numOfCompositeStates_ += 1;
      registerState(new AlphabetState(static_cast<int>(MAX_+1 + numOfCompositeStates_), state, "Unresolved state"));
      addCompositeStateToMap(state);

    }
  }
}
/**********************************************************************************************/
void ChromosomeAlphabet::addCompositeStateToMap(std::string& state){
  StringTokenizer st(state, "=_", false, false);
  std::vector<std::string> v;
  while(st.hasMoreToken()){
    v.push_back(st.nextToken());
  }
  int charId = charToInt(state);
  if (state.find("=") == std::string::npos){
    // only integers are expected
    //compositeAlphabetMap_[charId].reserve(v.size());
    for (size_t i = 0; i < v.size(); i++){
      //pair <int, double> stateAndProb;
      int stateInSet = atoi(v[i].c_str());
      double probability = 1;
      compositeAlphabetMap_[charId][stateInSet] = probability;
    }
  }else{
    // chromosome states and probabilities
    //compositeAlphabetMap_[charId].reserve(size_t(v.size()/2));
    for (size_t i = 0; i < (size_t)(v.size()); i++){
      //pair <int, double> stateAndProb;
      if (i % 2 != 0){
        continue;
      }
      int stateInSet = atoi(v[i].c_str());
      double probability = atof(v[i+1].c_str());
      compositeAlphabetMap_[charId][stateInSet] = probability;
    }
  }
}
/***********************************************************************/
const std::vector <int> ChromosomeAlphabet::getSetOfStatesForAComposite(int state) const{
  std::vector <int> setOfStates;
  //const std::vector <pair<int, double>> stateAndProb = getCompositeStatesAndProbsForIntLetter(state);
  const std::map <int, double> stateAndProb = getCompositeStatesAndProbsForIntLetter(state);
  std::map<int, double>::const_iterator it = stateAndProb.begin();
  while (it != stateAndProb.end()){
    int s = it->first;
    setOfStates.push_back(s);
    it++;
  }
  return setOfStates;
}
/*******************************************************************************/

double ChromosomeAlphabet::getProbabilityOfChar(int state1, int state2) const
{
  if (state1 < 0 || !isIntInAlphabet(state1))
    throw BadIntException(state1, "ChromosomeAlphabet::getProbabilityOfChar(int, int): Specified base unknown.");

  if (state2 < 0 || !isIntInAlphabet(state2))
    throw BadIntException(state2, "ChromosomeAlphabet::getProbabilityOfChar: Specified base unknown.");

  if (isUnresolved(state2))
    throw BadIntException(state2, "ChromosomeAlphabet::getProbabilityOfChar: Unresolved base."); 

  if (state2 > getMax())
    throw IndexOutOfBoundsException("ChromosomeSubstitutionModel::getProbabilityOfChar", state2, 0, getNumberOfTypes() - 1);
  vector<int> states = getAlias(state1);
  for (size_t j = 0; j < states.size(); j++)
  {
     if (state2 == states[j]){
         // it is a composite state
        if (state1 > getMax() + 1){
          return getProbabilityForState(state1, states[j]);

        }else{
          return 1.;
        }

     }
  }
  return 0;
}
/***************************************************************************************/
bool ChromosomeAlphabet::isResolvedIn(int state1, int state2) const
{
  return (getProbabilityOfChar(state1, state2) > 0);
}
