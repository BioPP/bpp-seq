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

string RNA::getAlphabetType() const { return "RNA alphabet"; }
