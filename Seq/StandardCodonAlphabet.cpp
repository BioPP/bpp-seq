//
// File: StandardCodonAlphabet.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 17:51:36 2003
//

#include "StandardCodonAlphabet.h"


StandardCodonAlphabet::StandardCodonAlphabet(const NucleicAlphabet * alpha) : CodonAlphabet(alpha)
{
	// Alphabet size definition
	alphabet.resize(67);

	string A = nucAlpha -> intToChar(0);
	string G = nucAlpha -> intToChar(2);
	string T = nucAlpha -> intToChar(3);
	string stop1 = T + A + A;
	string stop2 = T + A + G;
	string stop3 = T + G + A;

	//Automatic construction:
	int count = 0;
	int num   = 0;
	for(unsigned int pos1 = 0; pos1 < nucAlpha -> getSize(); pos1++) {
		for(unsigned int pos2 = 0; pos2 < nucAlpha -> getSize(); pos2++) {
			for(unsigned int pos3 = 0; pos3 < nucAlpha -> getSize(); pos3++) {
				string codon = nucAlpha -> intToChar(pos1)
					         + nucAlpha -> intToChar(pos2)
					         + nucAlpha -> intToChar(pos3);
				if(codon == stop1) {
					//This is a stop codon:
					alphabet[count].num = -2;
					alphabet[count].name = STOP;
				} else if(codon == stop2) {
					//This is a stop codon:
					alphabet[count].num = -3;
					alphabet[count].name = STOP;
				} else if(codon == stop3) {
					//This is a stop codon:
					alphabet[count].num = -4;
					alphabet[count].name = STOP;
				}else {
					//This is a "coding" codon:
					alphabet[count].num = num;
					alphabet[count].name = "";
					num++;
				}
				alphabet[count].letter = codon;
				count++;
			}
		}
	}

	alphabet[64].num = 61;
    alphabet[64].letter = "NNN";
    alphabet[64].name = "Unresolved codon";
}

unsigned int StandardCodonAlphabet::getSize() const { return 61; }

unsigned int StandardCodonAlphabet::getNumberOfTypes() const { return 62; }

StandardCodonAlphabet::~StandardCodonAlphabet() {}

string StandardCodonAlphabet::getAlphabetType() const { return "Standard codon alphabet"; }

bool StandardCodonAlphabet::isStop(      int      codon) const { return codon < -1; }

bool StandardCodonAlphabet::isStop(const string & codon) const { return charToInt(codon) < -1; }
