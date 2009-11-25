//
// File: RNA.cpp
// Authors: Guillaume Deuchst
//          Julien Dutheil
//          Sylvain Gaillard
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

#include "RNA.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/MapTools.h>

using namespace bpp;

// From STL:
#include <map>

using namespace std;

/******************************************************************************/
// class constructor
RNA::RNA()
{
	// Alphabet size definition
	resize(20);

	// Alphabet content definition
	// all unresolved bases use n°14
  setState( 0, NucleicAlphabetState(-1, "-",  0, "Gap"));
  setState( 1, NucleicAlphabetState( 0, "A",  1, "Adenine"));
  setState( 2, NucleicAlphabetState( 1, "C",  2, "Cytosine"));
  setState( 3, NucleicAlphabetState( 2, "G",  4, "Guanine"));
  setState( 4, NucleicAlphabetState( 3, "U",  8, "Uracile"));
  setState( 5, NucleicAlphabetState( 4, "M",  3, "Adenine or Cytosine"));
  setState( 6, NucleicAlphabetState( 5, "R",  5, "Purine (Adenine or Guanine)"));
  setState( 7, NucleicAlphabetState( 6, "W",  9, "Adenine or Uracile"));
  setState( 8, NucleicAlphabetState( 7, "S",  6, "Cytosine or Guanine"));
  setState( 9, NucleicAlphabetState( 8, "Y", 10, "Pyrimidine (Cytosine or Uracile)"));
  setState(10, NucleicAlphabetState( 9, "K", 12, "Guanine or Uracile"));
  setState(11, NucleicAlphabetState(10, "V",  7, "Adenine or Cytosine or Guanine"));
  setState(12, NucleicAlphabetState(11, "H", 11, "Adenine or Cytosine or Uracile"));
  setState(13, NucleicAlphabetState(12, "D", 13, "Adenine or Guanine or Uracile"));
  setState(14, NucleicAlphabetState(13, "B", 14, "Cytosine or Guanine or Uracile"));
  setState(15, NucleicAlphabetState(14, "N", 15, "Unresolved base"));
  setState(16, NucleicAlphabetState(14, "X", 15, "Unresolved base"));
  setState(17, NucleicAlphabetState(14, "O", 15, "Unresolved base"));
  setState(18, NucleicAlphabetState(14, "0", 15, "Unresolved base"));
  setState(19, NucleicAlphabetState(14, "?", 15, "Unresolved base"));
}

/******************************************************************************/

std::vector<int> RNA::getAlias(int state) const throw (BadIntException) 
{
	if(!isIntInAlphabet(state)) throw BadIntException(state, "RNA::getAlias(int): Specified base unknown.");
	vector<int> v;
	if(state == 4) {// A or C
		v.resize(2); v[0] = 0; v[1] = 1;
	} else if(state == 5) {// A or G
		v.resize(2); v[0] = 0; v[1] = 2;
	} else if(state == 6) {// A or U
		v.resize(2); v[0] = 0; v[1] = 3;
	} else if(state == 7) {// C or G
		v.resize(2); v[0] = 1; v[1] = 2;
	} else if(state == 8) {// C or U
		v.resize(2); v[0] = 1; v[1] = 3;
	} else if(state == 9) {// G or U
		v.resize(2); v[0] = 2; v[1] = 3;
	} else if(state == 10) {// A, C or G
		v.resize(3); v[0] = 0; v[1] = 1; v[2] = 2;
	} else if(state == 11) {// A, C or U
		v.resize(3); v[0] = 0; v[1] = 1; v[2] = 3;
	} else if(state == 12) {// A, G or U
		v.resize(3); v[0] = 0; v[1] = 2; v[2] = 3;
	} else if(state == 13) {// C, G or U
		v.resize(3); v[0] = 1, v[1] = 2; v[2] = 3;
	} else if(state == 14) {// A, C, G or U
		v.resize(4); v[0] = 0; v[1] = 1; v[2] = 2; v[3] = 3;
	} else {
		v.resize(1); v[0] = state;
	}		
	return v;
}


/******************************************************************************/

std::vector<std::string> RNA::getAlias(const std::string & state) const throw (BadCharException) 
{
  string locstate = TextTools::toUpper(state);
	if(!isCharInAlphabet(locstate)) throw BadCharException(locstate, "RNA::getAlias(int): Specified base unknown.");
	vector<string> v;
	if(locstate == "M") {// A or C
		v.resize(2); v[0] = "A"; v[1] = "C";
	} else if(locstate == "R") {// A or G
		v.resize(2); v[0] = "A"; v[1] = "G";
	} else if(locstate == "W") {// A or U
		v.resize(2); v[0] = "A"; v[1] = "U";
	} else if(locstate == "S") {// C or G
		v.resize(2); v[0] = "C"; v[1] = "G";
	} else if(locstate == "Y") {// C or U
		v.resize(2); v[0] = "C"; v[1] = "U";
	} else if(locstate == "K") {// G or T
		v.resize(2); v[0] = "G"; v[1] = "U";
	} else if(locstate == "V") {// A, C or G
		v.resize(3); v[0] = "A"; v[1] = "C"; v[2] = "G";
	} else if(locstate == "H") {// A, C or U
		v.resize(3); v[0] = "A"; v[1] = "C"; v[2] = "U";
	} else if(locstate == "D") {// A, G or U
		v.resize(3); v[0] = "A"; v[1] = "G"; v[2] = "U";
	} else if(locstate == "B") {// C, G or U
		v.resize(3); v[0] = "C", v[1] = "G"; v[2] = "U";
	} else if(locstate == "N"
         || locstate == "X"
	       || locstate == "O"
	       || locstate == "0"
	       || locstate == "?") {// A, C, G or U
		v.resize(4); v[0] = "A"; v[1] = "C"; v[2] = "G"; v[3] = "U";
	} else {
		v.resize(1); v[0] = locstate;
	}		
	return v;
}

/******************************************************************************/

int RNA::getGeneric(const std::vector<int> & states) const throw (BadIntException)
{
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
    if (!isIntInAlphabet(ve[i])) throw BadIntException(ve[i], "RNA::getGeneric(const vector<int>): Specified base unknown.");
    key += "_" + TextTools::toString(ve[i]);
  }
  map<string, int> g;
  g["_0_1"] = 4;
  g["_0_2"] = 5;
  g["_0_3"] = 6;
  g["_1_2"] = 7;
  g["_1_3"] = 8;
  g["_2_3"] = 9;
  g["_0_1_2"] = 10;
  g["_0_1_3"] = 11;
  g["_0_2_3"] = 12;
  g["_1_2_3"] = 13;
  int v;
  map<string, int>::iterator it = g.find(key);
  if (ve.size() == 1) {
    v = ve[0];
  } else if (it != g.end()) {
    v = it->second;
  } else {
    v = 14;
  }
  return v;
}

/******************************************************************************/

std::string RNA::getGeneric(const std::vector<std::string> & states) const throw (BadCharException)
{
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
    if (!isCharInAlphabet(ve[i])) throw BadCharException(ve[i], "RNA::getAlias(const vector<string>): Specified base unknown.");
    key += TextTools::toString(ve[i]);
  }
  map<string, string> g;
  g["AC"] = "M";
  g["AG"] = "R";
  g["AU"] = "W";
  g["CG"] = "S";
  g["CU"] = "Y";
  g["GU"] = "K";
  g["ACG"] = "V";
  g["ACU"] = "H";
  g["AGU"] = "D";
  g["CGU"] = "B";
  string v;
  map<string, string>::iterator it = g.find(key);
  if (ve.size() == 1) {
    v = ve[0];
  } else if (it != g.end()) {
    v = it->second;
  } else {
    v = "N";
  }
  return v;
}

/******************************************************************************/

