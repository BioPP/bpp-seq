//
// File: InvertebrateMitochondrialCodonAlphabet.h
// Created by: bazin <bazin@univ-montp2.fr>
// Created on: thu mar  1 14:25:09 CET 2005
//

#include "InvertebrateMitochondrialCodonAlphabet.h"


InvertebrateMitochondrialCodonAlphabet::InvertebrateMitochondrialCodonAlphabet(const NucleicAlphabet * alpha) : CodonAlphabet(alpha)
{
	// Alphabet size definition
	alphabet.resize(66);

	string A = nucAlpha -> intToChar(0);
	string G = nucAlpha -> intToChar(2);
	string T = nucAlpha -> intToChar(3);
	string stop1 = T + A + A;
	string stop2 = T + A + G;
	alphabet[0].num = -1;
  	alphabet[0].letter = "---";
  	alphabet[0].name = "Gap";

	//Automatic construction:
	int count = 1;
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
				} else {
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

  alphabet[65].num = num;
  alphabet[65].letter = "NNN";
  alphabet[65].name = "Unresolved codon";
}

unsigned int InvertebrateMitochondrialCodonAlphabet::getSize() const { return 62; }

unsigned int InvertebrateMitochondrialCodonAlphabet::getNumberOfTypes() const { return 63; }

InvertebrateMitochondrialCodonAlphabet::~InvertebrateMitochondrialCodonAlphabet() {}

string InvertebrateMitochondrialCodonAlphabet::getAlphabetType() const { return "Invertebrate mitochondrial codon alphabet"; }

bool InvertebrateMitochondrialCodonAlphabet::isStop(int codon) const { return codon < -1; }

bool InvertebrateMitochondrialCodonAlphabet::isStop(const string & codon) const { return charToInt(codon) < -1; }
