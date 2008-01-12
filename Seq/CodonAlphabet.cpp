//
// File: CodonAlphabet.h
// Created by: Julien Dutheil
// Created on: Sun Oct 12 17:41:56 2003
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

#include "CodonAlphabet.h"

// From Utils:
#include <Utils/TextTools.h>

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

const string CodonAlphabet::STOP = "Stop";
const string CodonAlphabet::INIT = "Init";

/****************************************************************************************/

bool CodonAlphabet::containsUnresolved(const string & state) const throw (BadCharException)
{
	if(state.size() != 3) throw BadCharException(state, "CodonAlphabet::getName", this);
	char s1 = state[0];
	char s2 = state[1];
	char s3 = state[2];
	return
		(nucAlpha -> charToInt(TextTools::toString(s1)) >= 4
	|| nucAlpha -> charToInt(TextTools::toString(s2)) >= 4
	|| nucAlpha -> charToInt(TextTools::toString(s3)) >= 4);
}

/****************************************************************************************/

bool CodonAlphabet::containsGap(const string & state) const throw (BadCharException)
{
	if(state.size() != 3) throw BadCharException(state, "CodonAlphabet::getName", this);
	char s1 = state[0];
	char s2 = state[1];
	char s3 = state[2];
	return
		(nucAlpha -> charToInt(TextTools::toString(s1)) == -1
	|| nucAlpha -> charToInt(TextTools::toString(s2)) == -1
	|| nucAlpha -> charToInt(TextTools::toString(s3)) == -1);
}

/****************************************************************************************/

string CodonAlphabet::getName(const string & state) const throw (BadCharException)
{
	if(state.size() != 3) throw BadCharException(state, "CodonAlphabet::getName", this);
	if(containsUnresolved(state)) return alphabet[65].name;
	if(containsGap(state)) return alphabet[0].name;
	else return AbstractAlphabet::getName(state);
}
		
/****************************************************************************************/

int CodonAlphabet::charToInt(const string & state) const throw (BadCharException)
{
	if(state.size() != 3) throw BadCharException(state, "CodonAlphabet::charToInt", this);
	if(containsUnresolved(state)) return getSize(); // Generic characters are coded by n° >= alphabet size.
	if(containsGap(state)) return -1;
	else return AbstractAlphabet::charToInt(state);	
}

/****************************************************************************************/

int CodonAlphabet::getCodon(int pos1, int pos2, int pos3) const throw (BadIntException)
{
	return charToInt(getCodon(
		nucAlpha -> intToChar(pos1),
		nucAlpha -> intToChar(pos2),
		nucAlpha -> intToChar(pos3)
	));//This can't throw a BadCharException!
}

/****************************************************************************************/

string CodonAlphabet::getCodon(const string & pos1, const string & pos2, const string & pos3) const throw (BadCharException)
{
	string codon = pos1 + pos2 + pos3;
	//test codon:
	charToInt(codon);
	return codon;
}

/****************************************************************************************/

int CodonAlphabet::getFirstPosition (int codon) const throw (BadIntException)
{
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[0]));
}

/****************************************************************************************/

int CodonAlphabet::getSecondPosition(int codon) const throw (BadIntException)
{
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[1]));	
}

/****************************************************************************************/

int CodonAlphabet::getThirdPosition (int codon) const throw (BadIntException)
{
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[2]));
}

/****************************************************************************************/

vector<int> CodonAlphabet::getPositions(int codon) const throw (BadIntException)
{
	string s = intToChar(codon);
	vector<int> positions(3);
	positions[0] = nucAlpha -> charToInt(TextTools::toString(s[0]));
	positions[1] = nucAlpha -> charToInt(TextTools::toString(s[1]));
	positions[2] = nucAlpha -> charToInt(TextTools::toString(s[2]));
	return positions;
}

/****************************************************************************************/

string CodonAlphabet::getFirstPosition (const string & codon) const throw (BadCharException)
{
	//Test:
	charToInt(codon);
	return "" + codon[0]; 
}

/****************************************************************************************/

string CodonAlphabet::getSecondPosition(const string & codon) const throw (BadCharException)
{
	//Test:
	charToInt(codon);
	return "" + codon[1]; 	
}

/****************************************************************************************/

string CodonAlphabet::getThirdPosition (const string & codon) const throw (BadCharException)
{
	//Test:
	charToInt(codon);
	return "" + codon[2]; 
}

/****************************************************************************************/

vector<string> CodonAlphabet::getPositions(const string & codon) const throw (BadCharException)
{
	//Test:
	charToInt(codon);
	vector<string> positions(3);
	positions[0] = "" + codon[0]; 
	positions[1] = "" + codon[1]; 
	positions[2] = "" + codon[2];
	return positions;	
}

/****************************************************************************************/

