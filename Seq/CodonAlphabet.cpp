//
// File: CodonAlphabet.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 17:41:56 2003
//

#include "CodonAlphabet.h"

// From Utils:
#include <Utils/TextTools.h>

// From the STL:
#include <iostream>
using namespace std;

const string CodonAlphabet::UNRESOLVED = "Nn?Oo0";
const string CodonAlphabet::STOP       = "Stop";
const string CodonAlphabet::INIT       = "Init";

CodonAlphabet::CodonAlphabet(const NucleicAlphabet * alpha) : AbstractAlphabet() { nucAlpha = alpha; }


CodonAlphabet::~CodonAlphabet() {}

string CodonAlphabet::getName(string letter) const throw (BadCharException) {
	if(letter.size() != 3) throw BadCharException(letter, "CodonAlphabet::getName", this);
	int i = letter.find_first_of(UNRESOLVED);
	if(i >= 0 || i < (int)letter.size()) return alphabet[64].name;
	else return AbstractAlphabet::getName(letter);
}
		
int    CodonAlphabet::charToInt(string letter) const throw (BadCharException) {
	if(letter.size() != 3) throw BadCharException(letter, "CodonAlphabet::charToInt", this);
	int i = letter.find_first_of(UNRESOLVED);
	if(i >= 0 || i < (int)letter.size()) return 64;
	else return AbstractAlphabet::charToInt(letter);	
}

string CodonAlphabet::getAlphabetType() const { return "Codon alphabet"; }

int CodonAlphabet::getCodon(int pos1, int pos2, int pos3) const throw (BadIntException) {
	return charToInt(getCodon(
		nucAlpha -> intToChar(pos1),
		nucAlpha -> intToChar(pos2),
		nucAlpha -> intToChar(pos3)
	));//This can't throw a BadCharException!
}

string CodonAlphabet::getCodon(const string & pos1, const string & pos2, const string & pos3) const throw (BadCharException) {
	string codon = pos1 + pos2 + pos3;
	//test codon:
	charToInt(codon);
	return codon;
}

int CodonAlphabet::getFirstPosition (int codon) const throw (BadIntException) {
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[0]));
}

int CodonAlphabet::getSecondPosition(int codon) const throw (BadIntException) {
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[1]));	
}

int CodonAlphabet::getThirdPosition (int codon) const throw (BadIntException) {
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[2]));
}

vector<int> CodonAlphabet::getPositions(int codon) const throw (BadIntException) {
	string s = intToChar(codon);
	vector<int> positions(3);
	positions[0] = nucAlpha -> charToInt(TextTools::toString(s[0]));
	positions[1] = nucAlpha -> charToInt(TextTools::toString(s[1]));
	positions[2] = nucAlpha -> charToInt(TextTools::toString(s[2]));
	return positions;
}

string CodonAlphabet::getFirstPosition (const string & codon) const throw (BadCharException) {
	//Test:
	charToInt(codon);
	return "" + codon[0]; 
}

string CodonAlphabet::getSecondPosition(const string & codon) const throw (BadCharException) {
	//Test:
	charToInt(codon);
	return "" + codon[1]; 	
}

string CodonAlphabet::getThirdPosition (const string & codon) const throw (BadCharException) {
	//Test:
	charToInt(codon);
	return "" + codon[2]; 
}

vector<string> CodonAlphabet::getPositions(const string & codon) const throw (BadCharException) {
	//Test:
	charToInt(codon);
	vector<string> positions(3);
	positions[0] = "" + codon[0]; 
	positions[1] = "" + codon[1]; 
	positions[2] = "" + codon[2];
	return positions;	
}
