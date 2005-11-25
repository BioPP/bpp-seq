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

// From the STL:
#include <ctype.h>

/****************************************************************************************/

string AbstractAlphabet::getName(const string & state) const throw (BadCharException)
{
	string LETTER = TextTools::toUpper(state);
	for (vector<sletter>::const_iterator i = alphabet.begin() ; i < alphabet.end() ; i++)
    if (i -> letter == LETTER) return i -> name;
  throw BadCharException(state, "AbstractAlphabet::getName : Specified base unknown", this);
}

/****************************************************************************************/

string AbstractAlphabet::getName(int state) const throw (BadIntException)
{
  for (unsigned int i = 0 ; i < alphabet.size() ; i++)
    if (alphabet[i].num == state) return alphabet[i].name;
  throw BadIntException(state, "AbstractAlphabet::getName : Specified base unknown", this);
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

bool operator == (AbstractAlphabet::sletter & l1, AbstractAlphabet::sletter & l2)
{
  return (l1.name == l2.name);
}

/****************************************************************************************/

