/*
 * File ProteicAlphabet.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
*/

#include "ProteicAlphabet.h" // class's header file

// From Utils:
#include <Utils/TextTools.h>

// class constructor
ProteicAlphabet::ProteicAlphabet()
{
	// Alphabet size definition
	alphabet.resize(24);

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
	alphabet[21].letter = "X";
	alphabet[21].abbr = "X";
	alphabet[21].name = "Unresolved acid";
	alphabet[22].num = 21;
	alphabet[22].letter = "B";
	alphabet[22].abbr = "B";
	alphabet[22].name = "N or D";
	alphabet[23].num = 22;
	alphabet[23].letter = "Z";
	alphabet[23].abbr = "Z";
	alphabet[23].name = "Q or E";
}

unsigned int ProteicAlphabet::getSize() const { return 20; }

unsigned int ProteicAlphabet::getNumberOfTypes() const { return 23; }

string ProteicAlphabet::getAbbr(const string & aa) const throw (AlphabetException)
{
	string AA = TextTools::toUpper(aa);
	for (unsigned int i = 0 ; i < getNumberOfChars() ; i++)
		if (alphabet[i].letter == AA) return alphabet[i].abbr;
			throw AlphabetException ("ProteicAlphabet::getAbbr : Unknown specified amino acid");
	return "";
}

string ProteicAlphabet::getAbbr(int aa) const throw (AlphabetException)
{
	if ((aa >= 0) && (aa < (int)getNumberOfChars())) return alphabet[aa].abbr;
	throw AlphabetException ("ProteicAlphabet::getAbbr : Unknown specified amino acid");
	return "";
}

string ProteicAlphabet::getAlphabetType() const { return "Proteic alphabet"; }
