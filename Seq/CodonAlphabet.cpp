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

const string CodonAlphabet::STOP       = "Stop";
const string CodonAlphabet::INIT       = "Init";

CodonAlphabet::CodonAlphabet(const NucleicAlphabet * alpha) : AbstractAlphabet() { nucAlpha = alpha; }

CodonAlphabet::~CodonAlphabet() {}

bool CodonAlphabet::containsUnresolved(const string & letter) const throw (BadCharException)
{
	if(letter.size() != 3) throw BadCharException(letter, "CodonAlphabet::getName", this);
	char s1 = letter[0];
	char s2 = letter[1];
	char s3 = letter[2];
	return
		(nucAlpha -> charToInt(TextTools::toString(s1)) >= 4
	|| nucAlpha -> charToInt(TextTools::toString(s2)) >= 4
	|| nucAlpha -> charToInt(TextTools::toString(s3)) >= 4);
}

bool CodonAlphabet::containsGap(const string & letter) const throw (BadCharException)
{
	if(letter.size() != 3) throw BadCharException(letter, "CodonAlphabet::getName", this);
	char s1 = letter[0];
	char s2 = letter[1];
	char s3 = letter[2];
	return
		(nucAlpha -> charToInt(TextTools::toString(s1)) == -1
	|| nucAlpha -> charToInt(TextTools::toString(s2)) == -1
	|| nucAlpha -> charToInt(TextTools::toString(s3)) == -1);
}

string CodonAlphabet::getName(const string & letter) const throw (BadCharException) {
	if(letter.size() != 3) throw BadCharException(letter, "CodonAlphabet::getName", this);
	if(containsUnresolved(letter)) return alphabet[65].name;
	if(containsGap(letter)) return alphabet[0].name;
	else return AbstractAlphabet::getName(letter);
}
		
int CodonAlphabet::charToInt(const string & letter) const throw (BadCharException) {
	if(letter.size() != 3) throw BadCharException(letter, "CodonAlphabet::charToInt", this);
	if(containsUnresolved(letter)) return 61;
	if(containsGap(letter)) return -1;
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
