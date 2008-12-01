//
// File: AbstractAlphabet.cpp
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Tue Jul 22 2003
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

#include "AbstractAlphabet.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/MapTools.h>

using namespace bpp;

// From the STL:
#include <ctype.h>
#include <map>

using namespace std;

/****************************************************************************************/

string AbstractAlphabet::getName(const string & state) const throw (BadCharException)
{
	string LETTER = TextTools::toUpper(state);
	for(vector<sletter>::const_iterator i = alphabet.begin(); i < alphabet.end(); i++)
    if(i->letter == LETTER) return i->name;
  throw BadCharException(state, "AbstractAlphabet::getName(string): Specified base unknown", this);
}

/****************************************************************************************/

string AbstractAlphabet::getName(int state) const throw (BadIntException)
{
  for(unsigned int i = 0; i < alphabet.size(); i++)
    if(alphabet[i].num == state) return alphabet[i].name;
  throw BadIntException(state, "AbstractAlphabet::getName(int): Specified base unknown", this);
}

/****************************************************************************************/

int AbstractAlphabet::charToInt(const string & state) const throw (BadCharException)
{
  string LETTER = TextTools::toUpper(state);
  for (unsigned int i = 0; i < alphabet.size(); i++)
    if (alphabet[i].letter == LETTER) return alphabet[i].num;
  throw BadCharException(state, "AbstractAlphabet::charToInt: Specified base unknown", this);
}

/****************************************************************************************/

string AbstractAlphabet::intToChar(int state) const throw (BadIntException)
{
  for (unsigned int i = 0; i < alphabet.size(); i++)
    if (alphabet[i].num == state) return alphabet[i].letter;
  throw BadIntException(state, "AbstractAlphabet::intToChar: Specified base unknown", this);
}

/****************************************************************************************/

bool AbstractAlphabet::isIntInAlphabet(int state) const
{
  for (unsigned int i = 0; i < alphabet.size(); i++)
    if (alphabet[i].num == state) return true;
	return false;
}

/****************************************************************************************/

bool AbstractAlphabet::isCharInAlphabet(const string & state) const
{
  string C = TextTools::toUpper(state);
  for (unsigned int i = 0; i < alphabet.size(); i++)
    if (alphabet[i].letter == C) return true;
	return false;
}	

/****************************************************************************************/

vector<int> AbstractAlphabet::getAlias(int state) const throw (BadIntException) 
{
	if(!isIntInAlphabet(state)) throw BadIntException(state, "AbstractAlphabet::getAlias(int): Specified base unknown.");
	vector<int> v(1);
	v[0] = state;
	return v;
}

/****************************************************************************************/

vector<string> AbstractAlphabet::getAlias(const string & state) const throw (BadCharException) 
{
	if(!isCharInAlphabet(state)) throw BadCharException(state, "AbstractAlphabet::getAlias(char): Specified base unknown.");
	vector<string> v(1);
	v[0] = state;
	return v;
}

/****************************************************************************************/

int AbstractAlphabet::getGeneric(const vector<int> & states) const throw (BadIntException) {
  map<int, int> m;
  for (unsigned int i = 0 ; i < states.size() ; ++i) {
    vector<int> tmp_s = this->getAlias(states[i]); // get the states for generic characters
    for (unsigned int j = 0 ; j < tmp_s.size() ; ++j) {
      m[tmp_s[j]] ++; // add each state to the list
    }
  }
  vector<int> ve = MapTools::getKeys(m);

  string key;
  for (unsigned int i = 0 ; i < ve.size() ; ++i) {
    if (!isIntInAlphabet(ve[i])) throw BadIntException(ve[i], "AbstractAlphabet::getGeneric(const vector<int>): Specified base unknown.");
    key += "_" + TextTools::toString(ve[i]);
  }
  int v;
  if (ve.size() == 1) {
    v = ve[0];
  } else {
    v = this->getUnknownCharacterCode();
  }
  return v;
}

/****************************************************************************************/

string AbstractAlphabet::getGeneric(const vector<string> & states) const throw (AlphabetException) {
  map <string, int> m;
  for (unsigned int i = 0 ; i < states.size() ; ++i) {
    vector<string> tmp_s = this->getAlias(states[i]); // get the states for generic characters
    for (unsigned int j = 0 ; j < tmp_s.size() ; ++j) {
       m[tmp_s[j]] ++; // add each state to the list
    }
  }
  vector<string> ve = MapTools::getKeys(m);

  string key;
  for (unsigned int i = 0 ; i < ve.size() ; ++i) {
    if (!isCharInAlphabet(ve[i])) throw BadCharException(ve[i], "AbstractAlphabet::getAlias(const vector<string>): Specified base unknown.");
    key += TextTools::toString(ve[i]);
  }
  string v;
  if (ve.size() == 1) {
    v = ve[0];
  } else {
    throw CharStateNotSupportedException("AbstractAlphabet::getAlias(const vector<string>): No generic char state.");
  }
  return v;
}

/****************************************************************************************/

const vector<int> & AbstractAlphabet::getSupportedInts() const
{
  if(_intList.size() == 0)
  {
    _intList.resize(alphabet.size());
    _charList.resize(alphabet.size());
    for(unsigned int i = 0; i < alphabet.size(); i++)
    {
      _intList[i]  = alphabet[i].num;
      _charList[i] = alphabet[i].letter;
    }
  }
  return _intList;
}
    
/****************************************************************************************/
    
const vector<string> & AbstractAlphabet::getSupportedChars() const
{
  if(_charList.size() == 0)
  {
    _intList.resize(alphabet.size());
    _charList.resize(alphabet.size());
    for(unsigned int i = 0; i < alphabet.size(); i++)
    {
      _intList[i]  = alphabet[i].num;
      _charList[i] = alphabet[i].letter;
    }
  }
  return _charList;
}
 
/****************************************************************************************/

