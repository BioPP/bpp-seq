//
// File: CodonAlphabet.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 17:41:56 2003
//

/*
Copyright ou © ou Copr. CNRS, (17 Novembre 2004) 

Julien.Dutheil@univ-montp2.fr

Ce logiciel est un programme informatique servant à fournir des classes
pour l'analyse de séquences.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

Julien.Dutheil@univ-montp2.fr

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

// From the STL:
#include <iostream>
using namespace std;

const string CodonAlphabet::STOP = "Stop";
const string CodonAlphabet::INIT = "Init";

/****************************************************************************************/

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

/****************************************************************************************/

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

/****************************************************************************************/

string CodonAlphabet::getName(const string & letter) const throw (BadCharException)
{
	if(letter.size() != 3) throw BadCharException(letter, "CodonAlphabet::getName", this);
	if(containsUnresolved(letter)) return alphabet[65].name;
	if(containsGap(letter)) return alphabet[0].name;
	else return AbstractAlphabet::getName(letter);
}
		
/****************************************************************************************/

int CodonAlphabet::charToInt(const string & letter) const throw (BadCharException)
{
	if(letter.size() != 3) throw BadCharException(letter, "CodonAlphabet::charToInt", this);
	if(containsUnresolved(letter)) return getSize(); // Generic characters are coded by n° >= alphabet size.
	if(containsGap(letter)) return -1;
	else return AbstractAlphabet::charToInt(letter);	
}

/****************************************************************************************/

int CodonAlphabet::getCodon(int pos1, int pos2, int pos3) const throw (BadIntException) {
	return charToInt(getCodon(
		nucAlpha -> intToChar(pos1),
		nucAlpha -> intToChar(pos2),
		nucAlpha -> intToChar(pos3)
	));//This can't throw a BadCharException!
}

/****************************************************************************************/

string CodonAlphabet::getCodon(const string & pos1, const string & pos2, const string & pos3) const throw (BadCharException) {
	string codon = pos1 + pos2 + pos3;
	//test codon:
	charToInt(codon);
	return codon;
}

/****************************************************************************************/

int CodonAlphabet::getFirstPosition (int codon) const throw (BadIntException) {
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[0]));
}

/****************************************************************************************/

int CodonAlphabet::getSecondPosition(int codon) const throw (BadIntException) {
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[1]));	
}

/****************************************************************************************/

int CodonAlphabet::getThirdPosition (int codon) const throw (BadIntException) {
	string s = intToChar(codon);
	return nucAlpha -> charToInt(TextTools::toString(s[2]));
}

/****************************************************************************************/

vector<int> CodonAlphabet::getPositions(int codon) const throw (BadIntException) {
	string s = intToChar(codon);
	vector<int> positions(3);
	positions[0] = nucAlpha -> charToInt(TextTools::toString(s[0]));
	positions[1] = nucAlpha -> charToInt(TextTools::toString(s[1]));
	positions[2] = nucAlpha -> charToInt(TextTools::toString(s[2]));
	return positions;
}

/****************************************************************************************/

string CodonAlphabet::getFirstPosition (const string & codon) const throw (BadCharException) {
	//Test:
	charToInt(codon);
	return "" + codon[0]; 
}

/****************************************************************************************/

string CodonAlphabet::getSecondPosition(const string & codon) const throw (BadCharException) {
	//Test:
	charToInt(codon);
	return "" + codon[1]; 	
}

/****************************************************************************************/

string CodonAlphabet::getThirdPosition (const string & codon) const throw (BadCharException) {
	//Test:
	charToInt(codon);
	return "" + codon[2]; 
}

/****************************************************************************************/

vector<string> CodonAlphabet::getPositions(const string & codon) const throw (BadCharException) {
	//Test:
	charToInt(codon);
	vector<string> positions(3);
	positions[0] = "" + codon[0]; 
	positions[1] = "" + codon[1]; 
	positions[2] = "" + codon[2];
	return positions;	
}

/****************************************************************************************/

