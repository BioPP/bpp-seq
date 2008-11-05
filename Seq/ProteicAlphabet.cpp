//
// File: ProteicAlphabet.cpp
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

#include "ProteicAlphabet.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/MapTools.h>

using namespace bpp;

// From STL:
#include <map>

/****************************************************************************************/

ProteicAlphabet::ProteicAlphabet()
{
	// Alphabet size definition
	alphabet.resize(27);

	// Alphabet content definition
	alphabet[0].num = -1;
	alphabet[0].letter = "-";
	alphabet[0].abbr = "GAP";
	alphabet[0].name = "Gap";
  
	alphabet[1].num = 0;
	alphabet[1].letter = "A";
	alphabet[1].abbr = "ALA";
	alphabet[1].name = "Alanine";
  
	alphabet[2].num = 1;
	alphabet[2].letter = "R";
	alphabet[2].abbr = "ARG";
	alphabet[2].name = "Arginine";
  
	alphabet[3].num = 2;
	alphabet[3].letter = "N";
	alphabet[3].abbr = "ASN";
	alphabet[3].name = "Asparagine";
  
	alphabet[4].num = 3;
	alphabet[4].letter = "D";
	alphabet[4].abbr = "ASP";
	alphabet[4].name = "Asparatic Acid";
  
	alphabet[5].num = 4;
	alphabet[5].letter = "C";
	alphabet[5].abbr = "CYS";
	alphabet[5].name = "Cysteine";
  
	alphabet[6].num = 5;
	alphabet[6].letter = "Q";
	alphabet[6].abbr = "GLN";
	alphabet[6].name = "Glutamine";
  
	alphabet[7].num = 6;
	alphabet[7].letter = "E";
	alphabet[7].abbr = "GLU";
	alphabet[7].name = "Glutamic acid";
  
	alphabet[8].num = 7;
	alphabet[8].letter = "G";
	alphabet[8].abbr = "GLY";
	alphabet[8].name = "Glycine";
  
	alphabet[9].num = 8;
	alphabet[9].letter = "H";
	alphabet[9].abbr = "HIS";
	alphabet[9].name = "Histidine";
  
	alphabet[10].num = 9;
	alphabet[10].letter = "I";
	alphabet[10].abbr = "ILE";
	alphabet[10].name = "Isoleucine";
  
	alphabet[11].num = 10;
	alphabet[11].letter = "L";
	alphabet[11].abbr = "LEU";
	alphabet[11].name = "Leucine";
  
	alphabet[12].num = 11;
	alphabet[12].letter = "K";
	alphabet[12].abbr = "LYS";
	alphabet[12].name = "Lysine";
  
	alphabet[13].num = 12;
	alphabet[13].letter = "M";
	alphabet[13].abbr = "MET";
	alphabet[13].name = "Methionine";
  
	alphabet[14].num = 13;
	alphabet[14].letter = "F";
	alphabet[14].abbr = "PHE";
	alphabet[14].name = "Phenylalanine";
  
	alphabet[15].num = 14;
	alphabet[15].letter = "P";
	alphabet[15].abbr = "PRO";
	alphabet[15].name = "Proline";
  
	alphabet[16].num = 15;
	alphabet[16].letter = "S";
	alphabet[16].abbr = "SER";
	alphabet[16].name = "Serine";
  
	alphabet[17].num = 16;
	alphabet[17].letter = "T";
	alphabet[17].abbr = "THR";
	alphabet[17].name = "Threonine";
  
	alphabet[18].num = 17;
	alphabet[18].letter = "W";
	alphabet[18].abbr = "TRP";
	alphabet[18].name = "Tryptophan";
  
	alphabet[19].num = 18;
	alphabet[19].letter = "Y";
	alphabet[19].abbr = "TYR";
	alphabet[19].name = "Tyrosine";
  
	alphabet[20].num = 19;
	alphabet[20].letter = "V";
	alphabet[20].abbr = "VAL";
	alphabet[20].name = "Valine";
  
	alphabet[21].num = 20;
	alphabet[21].letter = "B";
	alphabet[21].abbr = "B";
	alphabet[21].name = "N or D";
  
	alphabet[22].num = 21;
	alphabet[22].letter = "Z";
	alphabet[22].abbr = "Z";
	alphabet[22].name = "Q or E";
  
	alphabet[23].num = 22;
	alphabet[23].letter = "X";
	alphabet[23].abbr = "X";
	alphabet[23].name = "Unresolved amino acid";
  
	alphabet[24].num = 22;
	alphabet[24].letter = "O";
	alphabet[24].abbr = "O";
	alphabet[24].name = "Unresolved amino acid";
  
	alphabet[25].num = 22;
	alphabet[25].letter = "0";
	alphabet[25].abbr = "0";
	alphabet[25].name = "Unresolved amino acid";
  
	alphabet[26].num = 22;
	alphabet[26].letter = "?";
	alphabet[26].abbr = "?";
	alphabet[26].name = "Unresolved amino acid";
}

/****************************************************************************************/

string ProteicAlphabet::getAbbr(const string & aa) const throw (AlphabetException)
{
	string AA = TextTools::toUpper(aa);
	for (unsigned int i = 0 ; i < getNumberOfChars() ; i++)
		if (alphabet[i].letter == AA) return alphabet[i].abbr;
	throw AlphabetException ("ProteicAlphabet::getAbbr : Unknown specified amino acid '" + aa + ".");
}

/****************************************************************************************/

string ProteicAlphabet::getAbbr(int aa) const throw (AlphabetException)
{
	for (unsigned int i = 0 ; i < getNumberOfChars() ; i++)
		if (alphabet[i].num == aa) return alphabet[i].abbr;
	throw AlphabetException ("ProteicAlphabet::getAbbr : Unknown specified amino acid '" + TextTools::toString(aa) + "'.");
}

/****************************************************************************************/

vector<int> ProteicAlphabet::getAlias(int state) const throw (BadIntException) 
{
	if(!isIntInAlphabet(state)) throw BadIntException(state, "ProteicAlphabet::getAlias(int): Specified base unknown.");
	vector<int> v;
	if(state == 20) {// N or D
		v.resize(2); v[0] = 2; v[1] = 3;
	} else if(state == 21) {// Q or E
		v.resize(2); v[0] = 5; v[1] = 6;
	} else if(state == 22) {// all!
		v.resize(20);
    for(unsigned int i = 0; i < 20; i++) v[i] = i;
	} else {
		v.resize(1); v[0] = state;
	}		
	return v;
}

/****************************************************************************************/

vector<string> ProteicAlphabet::getAlias(const string & state) const throw (BadCharException) 
{
	if(!isCharInAlphabet(state)) throw BadCharException(state, "ProteicAlphabet::getAlias(int): Specified base unknown.");
	vector<string> v;
	if(state == "B") {// N or D
		v.resize(2); v[0] = "N"; v[1] = "D";
	} else if(state == "Z") {// Q or E
		v.resize(2); v[0] = "Q"; v[1] = "E";
	} else if(state == "X"
	       || state == "O"
	       || state == "0"
	       || state == "?") {// all!
		v.resize(20);
    for(unsigned int i = 0; i < 20; i++) v[i] = alphabet[i+1].letter;
	} else {
		v.resize(1); v[0] = state;
	}		
	return v;
}

/****************************************************************************************/

int ProteicAlphabet::getGeneric(const vector<int> & states) const throw (BadIntException) {
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
    if (!isIntInAlphabet(ve[i])) throw BadIntException(ve[i], "ProteicAlphabet::getGeneric(const vector<int>): Specified base unknown.");
    key += "_" + TextTools::toString(ve[i]);
  }
  map<string, int> g;
  g["_2_3"] = 20;
  g["_5_6"] = 21;
  int v;
  map<string, int>::iterator it = g.find(key);
  if (ve.size() == 1) {
    v = ve[0];
  } else if (it != g.end()) {
    v = it->second;
  } else {
    v = 22;
  }
  return v;
}

/****************************************************************************************/

string ProteicAlphabet::getGeneric(const vector<string> & states) const throw (BadCharException) {
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
    if (!isCharInAlphabet(ve[i])) throw BadCharException(ve[i], "ProteicAlphabet::getAlias(const vector<string>): Specified base unknown.");
    key += TextTools::toString(ve[i]);
  }
  map<string, string> g;
  g["DN"] = "B";
  g["EQ"] = "Z";
  string v;
  map<string, string>::iterator it = g.find(key);
  if (ve.size() == 1) {
    v = ve[0];
  } else if (it != g.end()) {
    v = it->second;
  } else {
    v = "?";
  }
  return v;
}

/****************************************************************************************/
