//
// File: AlphabetTools.h
// Created by: jdutheil <jdutheil@Deedlit>
// Created on: Fri Oct 10 17:27:39 2003
//

#include "AlphabetTools.h"

// From Utils:
#include <Utils/TextTools.h>

// From the STL:
#include <ctype.h>
#include <iostream>
using namespace std;

/**********************************************************************************************/

int AlphabetTools::getType(char acid)
{
	// up specified character to capital letter
	acid = toupper(acid);

	if (acid=='A' || acid=='C' || acid=='D' || acid=='G' || acid=='H' || acid=='K' 
		|| acid=='M' || acid=='N' || acid=='R' || acid=='S' || acid=='V' 
		|| acid=='W' || acid=='X' || acid=='Y' || acid=='-') return 0;

	if ( acid=='B' || acid=='O' || acid=='?' || acid=='0') return 1;

	if (acid=='T') return 2;

	if (acid=='U') return 3;

	if (acid=='E' || acid=='F' || acid=='I' || acid=='L' || acid=='P' || acid=='Q') return 4;

	return -1;
}

/**********************************************************************************************/

bool AlphabetTools::checkAlphabetCodingSize(const Alphabet & alphabet) throw (AlphabetException)
{
	if(alphabet.getNumberOfChars() == 0) return true; //Will this really happen?
	unsigned int size = alphabet.intToChar(0).size();
	for(unsigned int i = 1; i < alphabet.getNumberOfTypes(); i++) {
		if(alphabet.intToChar(i).size() != size) return false;
	}
	return true;
}

/**********************************************************************************************/

bool AlphabetTools::checkAlphabetCodingSize(const Alphabet * alphabet) throw (AlphabetException) {
	return checkAlphabetCodingSize(* alphabet);
}

/**********************************************************************************************/

int AlphabetTools::getAlphabetCodingSize(const Alphabet & alphabet) throw (AlphabetException) {
	if(!checkAlphabetCodingSize(alphabet)) throw AlphabetException("Bad alphabet in function Alphabet::getAlphabetCodingSize().");
	return alphabet.intToChar(0).size();
}

/**********************************************************************************************/

int AlphabetTools::getAlphabetCodingSize(const Alphabet * alphabet) throw (AlphabetException) {
	return getAlphabetCodingSize(* alphabet);
}

/**********************************************************************************************/
