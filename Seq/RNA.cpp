/*
 * File RNA.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
*/

#include "RNA.h" // class's header file

// class constructor
RNA::RNA()
{
	// Alphabet size definition
	alphabet.resize(20);

	// Alphabet content definition
	// all unresolved bases use n°14
	alphabet[0].num = -1;
  alphabet[0].letter = "-";
  alphabet[0].name = "gap";
	
  alphabet[1].num = 0;
  alphabet[1].letter = "A";
  alphabet[1].name = "Adenine";
  
  alphabet[2].num = 1;
  alphabet[2].letter = "C";
  alphabet[2].name = "Cytosine";

	alphabet[3].num = 2;
  alphabet[3].letter = "G";
  alphabet[3].name = "Guanine";

	alphabet[4].num = 3;
  alphabet[4].letter = "U";
  alphabet[4].name = "Uracile";
  
  alphabet[5].num = 4;
  alphabet[5].letter = "M";
  alphabet[5].name = "Adenine or Cytosine";
  
  alphabet[6].num = 5;
  alphabet[6].letter = "R";
  alphabet[6].name = "Purine (Adenine or Guanine)";

	alphabet[7].num = 6;
	alphabet[7].letter = "W";
  alphabet[7].name = "Adenine or Uracile";
  
  alphabet[8].num = 7;
  alphabet[8].letter = "S";
  alphabet[8].name = "Cytosine or Guanine";
  
  alphabet[9].num = 8;
  alphabet[9].letter = "Y";
  alphabet[9].name = "Pyrimidine (Cytosine or Uracile)";

	alphabet[10].num = 9;
  alphabet[10].letter = "K";
  alphabet[10].name = "Guanine or Uracile";
  
  alphabet[11].num = 10;
  alphabet[11].letter = "V";
  alphabet[11].name = "Adenine or Cytosine or Guanine";
  
  alphabet[12].num = 11;
  alphabet[12].letter = "H";
  alphabet[12].name = "Adenine or Cytosine or Uracile";
  
  alphabet[13].num = 12;
  alphabet[13].letter = "D";
  alphabet[13].name = "Adenine or Guanine or Uracile";
  
  alphabet[14].num = 13;
  alphabet[14].letter = "B";
  alphabet[14].name = "Cytosine or Guanine or Uracile";
  
  alphabet[15].num = 14;
  alphabet[15].letter = "N";
  alphabet[15].name = "Unresolved base";
  alphabet[16].num = 14;
  alphabet[16].letter = "X";
  alphabet[16].name = "Unresolved base";
  alphabet[17].num = 14;
  alphabet[17].letter = "O";
  alphabet[17].name = "Unresolved base";
  alphabet[18].num = 14;
  alphabet[18].letter = "0";
  alphabet[18].name = "Unresolved base";
  alphabet[19].num = 14;
  alphabet[19].letter = "?";
  alphabet[19].name = "Unresolved base";
}

/****************************************************************************************/

vector<int> RNA::getAlias(int state) const throw (BadIntException) 
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


/****************************************************************************************/

vector<string> RNA::getAlias(const string & state) const throw (BadCharException) 
{
	if(!isCharInAlphabet(state)) throw BadCharException(state, "RNA::getAlias(int): Specified base unknown.");
	vector<string> v;
	if(state == "M") {// A or C
		v.resize(2); v[0] = "A"; v[1] = "C";
	} else if(state == "R") {// A or G
		v.resize(2); v[0] = "A"; v[1] = "G";
	} else if(state == "W") {// A or U
		v.resize(2); v[0] = "A"; v[1] = "U";
	} else if(state == "S") {// C or G
		v.resize(2); v[0] = "C"; v[1] = "G";
	} else if(state == "Y") {// C or U
		v.resize(2); v[0] = "C"; v[1] = "U";
	} else if(state == "K") {// G or T
		v.resize(2); v[0] = "G"; v[1] = "U";
	} else if(state == "V") {// A, C or G
		v.resize(3); v[0] = "A"; v[1] = "C"; v[2] = "G";
	} else if(state == "H") {// A, C or U
		v.resize(3); v[0] = "A"; v[1] = "C"; v[2] = "U";
	} else if(state == "D") {// A, G or U
		v.resize(3); v[0] = "A"; v[1] = "G"; v[2] = "U";
	} else if(state == "B") {// C, G or U
		v.resize(3); v[0] = "C", v[1] = "G"; v[2] = "U";
	} else if(state == "N"
         || state == "X"
	       || state == "O"
	       || state == "0"
	       || state == "?") {// A, C, G or U
		v.resize(4); v[0] = "A"; v[1] = "C"; v[2] = "G"; v[3] = "U";
	} else {
		v.resize(1); v[0] = state;
	}		
	return v;
}

/****************************************************************************************/
string RNA::getAlphabetType() const { return "RNA alphabet"; }
