//
// File: SymbolList.cpp
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Fri Apr 9 2005
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

#include "SymbolList.h"
#include "StringSequenceTools.h"

/****************************************************************************************/

SymbolList::SymbolList(const Alphabet * alpha) : _alphabet(alpha) {}

SymbolList::SymbolList(const vector<string> & list, const Alphabet * alpha) throw (BadCharException) :
	_alphabet(alpha)
{
	setContent(list);
}

SymbolList::SymbolList(const vector<int> & list, const Alphabet * alpha) throw (BadIntException) :
	_alphabet(alpha)
{
	setContent(list);
}

/****************************************************************************************/

SymbolList::SymbolList(const SymbolList & list): Clonable(), _alphabet(list.getAlphabet()), _content(list.getContent()) {}

SymbolList & SymbolList::operator = (const SymbolList & list)
{
	_content  = list.getContent();
	_alphabet = list.getAlphabet();
	return * this;
}

/****************************************************************************************/

SymbolList::~SymbolList() {}

/****************************************************************************************/

Clonable * SymbolList::clone() const { return new SymbolList(*  this); }

/****************************************************************************************/

const Alphabet * SymbolList::getAlphabet() const { return _alphabet; }

/****************************************************************************************/

vector<int> SymbolList::getContent() const { return _content; }

/****************************************************************************************/

void SymbolList::setContent(const vector<string> & list) throw (BadCharException)
{
	// Check list for incorrect characters
	vector<int> coded(list.size());
	for (unsigned int i = 0; i < list.size(); i++) {
		if(!_alphabet -> isCharInAlphabet(list[i])) throw BadCharException(list[i], "SymbolList::setContent", _alphabet);
	}
	for (unsigned int i = 0; i < list.size(); i++) {
		coded[i] = _alphabet -> charToInt(list[i]);
	}
	//SymbolList is valid:
	_content = coded;
};

/****************************************************************************************/

void SymbolList::setContent(const vector<int> & list) throw (BadIntException)
{
	// Check list for incorrect characters
	for (unsigned int i = 0; i < list.size(); i++) {
		if(!_alphabet -> isIntInAlphabet(list[i])) throw BadIntException(list[i], "SymbolList::setContent", _alphabet);
	}
	//Sequence is valid:
	_content = list;
};

/****************************************************************************************/

string SymbolList::toString() const {
	return StringSequenceTools::decodeSequence(_content, _alphabet);
};

/****************************************************************************************/

unsigned int SymbolList::size() const { return _content.size(); }

/****************************************************************************************/


int SymbolList::operator [] (unsigned int i) const { return _content[i]; }

/****************************************************************************************/

void SymbolList::addElement(string c) throw (BadCharException) {
	_content.push_back(_alphabet -> charToInt(c));
}

/****************************************************************************************/

void SymbolList::addElement(unsigned int pos, string c) throw (BadCharException, IndexOutOfBoundsException)
{
  if(pos > _content.size()) throw IndexOutOfBoundsException("SymbolList::addElement. Invalid position.", pos, 0, size() - 1);
	_content.insert(_content.begin() + pos, _alphabet -> charToInt(c));
}

/****************************************************************************************/

void SymbolList::setElement(unsigned int pos, string c) throw (BadCharException, IndexOutOfBoundsException)
{
	if(pos > _content.size()) throw IndexOutOfBoundsException("SymbolList::setElement. Invalid position.", pos, 0, size() - 1);
	_content[pos] = _alphabet -> charToInt(c);
}

/****************************************************************************************/

string SymbolList::getChar(unsigned int pos) const throw (IndexOutOfBoundsException)
{
	if(pos > _content.size()) throw IndexOutOfBoundsException("SymbolList::getChar. Invalid position.", pos, 0, size() - 1);
	string c = "";
	try {
		c = _alphabet -> intToChar(_content[pos]);
	} catch(BadIntException bie) {

		//This should never happen!
	}
	return c;
}

/****************************************************************************************/

void SymbolList::deleteElement(unsigned int pos) throw (IndexOutOfBoundsException)
{
	if(pos > _content.size()) throw IndexOutOfBoundsException("SymbolList::deleteElement. Invalid position.", pos, 0, size() - 1);
	 _content.erase(_content.begin() + pos);
}

/****************************************************************************************/

void SymbolList::addElement(int v) throw (BadIntException)
{
	//test:
	_alphabet -> intToChar(v);
	_content.push_back(v);
}

/****************************************************************************************/

void SymbolList::addElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException)
{
	//test:
	if(pos > _content.size()) throw IndexOutOfBoundsException("SymbolList::addElement. Invalid position.", pos, 0, size() - 1);
	_alphabet -> intToChar(v);
	_content.insert(_content.begin() + pos, v);
}

/****************************************************************************************/

void SymbolList::setElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException)
{
	//test:
  if(pos > _content.size()) throw IndexOutOfBoundsException("SymbolList::setElement. Invalid position.", pos, 0, size() - 1);
	_alphabet -> intToChar(v);
	_content[pos] = v;
}

/****************************************************************************************/

int SymbolList::getValue(unsigned int pos) const throw (IndexOutOfBoundsException)
{
  if(pos > _content.size()) throw IndexOutOfBoundsException("SymbolList::getValue. Invalid position.", pos, 0, size() - 1);
	return _content[pos];
}

/****************************************************************************************/
