//
// File: SymbolList.cpp
// Created by: Guillaume Deuchst
// Created on: Tue Jul 22 2003
//

/*
Copyright ou � ou Copr. CNRS, (17 Novembre 2004) 

Julien.Dutheil@univ-montp2.fr

Ce logiciel est un programme informatique servant � fournir des classes
pour l'analyse de s�quences.

Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilit� au code source et des droits de copie,
de modification et de redistribution accord�s par cette licence, il n'est
offert aux utilisateurs qu'une garantie limit�e.  Pour les m�mes raisons,
seule une responsabilit� restreinte p�se sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les conc�dants successifs.

A cet �gard  l'attention de l'utilisateur est attir�e sur les risques
associ�s au chargement,  � l'utilisation,  � la modification et/ou au
d�veloppement et � la reproduction du logiciel par l'utilisateur �tant 
donn� sa sp�cificit� de logiciel libre, qui peut le rendre complexe � 
manipuler et qui le r�serve donc � des d�veloppeurs et des professionnels
avertis poss�dant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invit�s � charger  et  tester  l'ad�quation  du
logiciel � leurs besoins dans des conditions permettant d'assurer la
s�curit� de leurs syst�mes et ou de leurs donn�es et, plus g�n�ralement, 
� l'utiliser et l'exploiter dans les m�mes conditions de s�curit�. 

Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accept� les
termes.
*/

/*
Copyright or � or Copr. CNRS, (November 17, 2004)

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

#include "AbstractAlphabet.h"

// From Utils:
#include <Utils/TextTools.h>

// From the STL:
#include <ctype.h>

/****************************************************************************************/

string AbstractAlphabet::getName(const string & letter) const throw (BadCharException)
{
	string LETTER = TextTools::toUpper(letter);
	for (vector<sletter>::const_iterator i = alphabet.begin() ; i < alphabet.end() ; i++)
    if (i -> letter == LETTER) return i -> name;
  throw BadCharException(letter, "AbstractAlphabet::getName : Specified base unknown", this);
}

/****************************************************************************************/

string AbstractAlphabet::getName(int letter) const throw (BadIntException)
{
  for (unsigned int i = 0 ; i < alphabet.size() ; i++)
    if (alphabet[i].num == letter) return alphabet[i].name;
  throw BadIntException(letter, "AbstractAlphabet::getName : Specified base unknown", this);
}

/****************************************************************************************/

int AbstractAlphabet::charToInt(const string & letter) const throw (BadCharException)
{
  string LETTER = TextTools::toUpper(letter);
  for (unsigned int i = 0; i < alphabet.size(); i++)
    if (alphabet[i].letter == LETTER) return alphabet[i].num;
  throw BadCharException(letter, "AbstractAlphabet::charToInt : Specified base unknown", this);
}

/****************************************************************************************/

string AbstractAlphabet::intToChar(int letter) const throw (BadIntException)
{
  for (unsigned int i = 0; i < alphabet.size(); i++)
    if (alphabet[i].num == letter) return alphabet[i].letter;
  throw BadIntException(letter, "AbstractAlphabet::intToChar : Specified base unknown", this);
}

/****************************************************************************************/

bool  AbstractAlphabet::isIntInAlphabet(int letter) const
{
  for (unsigned int i = 0; i < alphabet.size(); i++)
    if (alphabet[i].num == letter) return true;
	return false;
}

/****************************************************************************************/

bool AbstractAlphabet::isCharInAlphabet(const string & letter) const
{
  string C = TextTools::toUpper(letter);
  for (unsigned int i = 0; i < alphabet.size(); i++)
    if (alphabet[i].letter == C) return true;
	return false;
}	

/****************************************************************************************/

vector<int> AbstractAlphabet::getAlias(int state) const throw (BadIntException) 
{
	if(!isIntInAlphabet(state)) throw BadIntException(state, "AbstractAlphabet::getAlias(int): Specified base unknown.");
	vector<int> v(1);
	v[0] = state;
	return v;
}

/****************************************************************************************/

vector<string> AbstractAlphabet::getAlias(const string & state) const throw (BadCharException) 
{
	if(!isCharInAlphabet(state)) throw BadCharException(state, "AbstractAlphabet::getAlias(char): Specified base unknown.");
	vector<string> v(1);
	v[0] = state;
	return v;
}

/****************************************************************************************/

bool operator == (AbstractAlphabet::sletter & l1, AbstractAlphabet::sletter & l2)
{
  return (l1.name == l2.name);
}

/****************************************************************************************/

