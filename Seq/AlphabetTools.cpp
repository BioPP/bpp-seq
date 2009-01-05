//
// File: AlphabetTools.cpp
// Created by: Julien Dutheil
// Created on: Fri Oct 10 17:27:39 2003
//

#include "AlphabetTools.h"

// From Utils:
#include <Utils/TextTools.h>

using namespace bpp;

// From the STL:
#include <ctype.h>
#include <iostream>

using namespace std;

/**********************************************************************************************/

const DNA AlphabetTools::DNA_ALPHABET;
const RNA AlphabetTools::RNA_ALPHABET;
const ProteicAlphabet AlphabetTools::PROTEIN_ALPHABET;
const DefaultAlphabet AlphabetTools::DEFAULT_ALPHABET;

/**********************************************************************************************/

int AlphabetTools::getType(char state)
{
  if(state == '-') return -1;
  state = toupper(state);
  bool d = DNA_ALPHABET.isCharInAlphabet(TextTools::toString(state));
  bool r = RNA_ALPHABET.isCharInAlphabet(TextTools::toString(state));
  bool p = PROTEIN_ALPHABET.isCharInAlphabet(TextTools::toString(state));

  if(!d && !r && !p) return 0; //Unknown character
  else if(d && !r && !p) return 1; //DNA specific
  else if(!d && r && !p) return 2; //RNA specific
  else if(!d && !r && p) return 3; //Protein specific
  else if(d && r && !p) return 4; //Nucleotide specific 
  else if(d && !r && p) return 5; //DNA or Protein specific 
  else if(!d && r && p) return 6; //RNA or Protein specific 
  else return 7; //Non-specific character
}

/**********************************************************************************************/

bool AlphabetTools::checkAlphabetCodingSize(const Alphabet & alphabet) throw (AlphabetException)
{
  if(alphabet.getNumberOfChars() == 0) return true; //Will this really happen?
  unsigned int size = alphabet.intToChar(0).size();
  for(unsigned int i = 1; i < alphabet.getNumberOfTypes(); i++)
  {
    if(alphabet.intToChar(i).size() != size) return false;
  }
  return true;
}

/**********************************************************************************************/

bool AlphabetTools::checkAlphabetCodingSize(const Alphabet * alphabet) throw (AlphabetException)
{
  return checkAlphabetCodingSize(* alphabet);
}

/**********************************************************************************************/

int AlphabetTools::getAlphabetCodingSize(const Alphabet & alphabet) throw (AlphabetException)
{
  if(!checkAlphabetCodingSize(alphabet)) throw AlphabetException("Bad alphabet in function Alphabet::getAlphabetCodingSize().");
  return alphabet.intToChar(0).size();
}

/**********************************************************************************************/

int AlphabetTools::getAlphabetCodingSize(const Alphabet * alphabet) throw (AlphabetException)
{
  return getAlphabetCodingSize(* alphabet);
}

/**********************************************************************************************/

