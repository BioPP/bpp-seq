/***************************************************************************
                          SymbolList.cpp  -  description
                             -------------------
    begin                : ven avr 9 2004
    copyright            : (C) 2004 by Julien Dutheil
    email                : Julien.Dutheil@univ-montp2.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
