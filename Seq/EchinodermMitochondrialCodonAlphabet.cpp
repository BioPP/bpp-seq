//
// File: EchinodermMitochondrialCodonAlphabet.h
// Created by: Eric Bazin
// Created on: 14 11:31:27 CET 2005
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


#include "EchinodermMitochondrialCodonAlphabet.h"

using namespace bpp;

EchinodermMitochondrialCodonAlphabet::EchinodermMitochondrialCodonAlphabet(const NucleicAlphabet * alpha) : CodonAlphabet(alpha)
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

unsigned int EchinodermMitochondrialCodonAlphabet::getSize() const { return 62; }

unsigned int EchinodermMitochondrialCodonAlphabet::getNumberOfTypes() const { return 63; }

EchinodermMitochondrialCodonAlphabet::~EchinodermMitochondrialCodonAlphabet() {}

string EchinodermMitochondrialCodonAlphabet::getAlphabetType() const { return "EchinodermMitochondrial codon alphabet"; }

bool EchinodermMitochondrialCodonAlphabet::isStop(int codon) const { return codon < -1; }

bool EchinodermMitochondrialCodonAlphabet::isStop(const string & codon) const { return charToInt(codon) < -1; }

