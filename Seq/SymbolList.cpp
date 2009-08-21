//
// File: SymbolList.cpp
// Created by: Julien Dutheil
// Created on: Fri Apr 9 2005
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

#include "SymbolList.h"
#include "StringSequenceTools.h"

using namespace bpp;

using namespace std;

/****************************************************************************************/

SymbolList::SymbolList(const Alphabet* alpha) : alphabet_(alpha) {}

SymbolList::SymbolList(const vector<string>& list, const Alphabet* alpha) throw (BadCharException) :
	alphabet_(alpha)
{
	setContent(list);
}

SymbolList::SymbolList(const vector<int>& list, const Alphabet* alpha) throw (BadIntException) :
	alphabet_(alpha)
{
	setContent(list);
}

/****************************************************************************************/

SymbolList::SymbolList(const SymbolList & list):
  alphabet_(list.getAlphabet()), content_(list.getContent()) {}

SymbolList& SymbolList::operator=(const SymbolList& list)
{
	content_  = list.getContent();
	alphabet_ = list.getAlphabet();
	return *this;
}

/****************************************************************************************/

void SymbolList::setContent(const vector<string>& list) throw (BadCharException)
{
	// Check list for incorrect characters
	vector<int> coded(list.size());
	for (unsigned int i = 0; i < list.size(); i++)
		if(!alphabet_->isCharInAlphabet(list[i])) throw BadCharException(list[i], "SymbolList::setContent", alphabet_);
	
  for (unsigned int i = 0; i < list.size(); i++) 
		coded[i] = alphabet_->charToInt(list[i]);
	
  //SymbolList is valid:
	content_ = coded;
};

/****************************************************************************************/

void SymbolList::setContent(const vector<int>& list) throw (BadIntException)
{
	// Check list for incorrect characters
	for (unsigned int i = 0; i < list.size(); i++)
		if(!alphabet_->isIntInAlphabet(list[i]))
      throw BadIntException(list[i], "SymbolList::setContent", alphabet_);
	
  //Sequence is valid:
	content_ = list;
};

/****************************************************************************************/

string SymbolList::toString() const
{
	return StringSequenceTools::decodeSequence(content_, alphabet_);
};

/****************************************************************************************/

void SymbolList::addElement(const string& c) throw (BadCharException)
{
	content_.push_back(alphabet_->charToInt(c));
}

/****************************************************************************************/

void SymbolList::addElement(unsigned int pos, const string& c) throw (BadCharException, IndexOutOfBoundsException)
{
  if(pos > content_.size()) throw IndexOutOfBoundsException("SymbolList::addElement. Invalid position.", pos, 0, size() - 1);
	content_.insert(content_.begin() + pos, alphabet_->charToInt(c));
}

/****************************************************************************************/

void SymbolList::setElement(unsigned int pos, const string& c) throw (BadCharException, IndexOutOfBoundsException)
{
	if(pos > content_.size())
    throw IndexOutOfBoundsException("SymbolList::setElement. Invalid position.", pos, 0, size() - 1);
	content_[pos] = alphabet_->charToInt(c);
}

/****************************************************************************************/

string SymbolList::getChar(unsigned int pos) const throw (IndexOutOfBoundsException)
{
	if(pos > content_.size())
    throw IndexOutOfBoundsException("SymbolList::getChar. Invalid position.", pos, 0, size() - 1);
	string c = "";
	try
  {
		c = alphabet_->intToChar(content_[pos]);
	}
  catch(BadIntException bie)
  {
		//This should never happen!
	}
	return c;
}

/****************************************************************************************/

void SymbolList::deleteElement(unsigned int pos) throw (IndexOutOfBoundsException)
{
	if(pos > content_.size())
    throw IndexOutOfBoundsException("SymbolList::deleteElement. Invalid position.", pos, 0, size() - 1);
	 content_.erase(content_.begin() + pos);
}

/****************************************************************************************/

void SymbolList::addElement(int v) throw (BadIntException)
{
	//test:
	alphabet_->intToChar(v);
	content_.push_back(v);
}

/****************************************************************************************/

void SymbolList::addElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException)
{
	//test:
	if(pos > content_.size())
    throw IndexOutOfBoundsException("SymbolList::addElement. Invalid position.", pos, 0, size() - 1);
	alphabet_->intToChar(v);
	content_.insert(content_.begin() + pos, v);
}

/****************************************************************************************/

void SymbolList::setElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException)
{
	//test:
  if(pos > content_.size())
    throw IndexOutOfBoundsException("SymbolList::setElement. Invalid position.", pos, 0, size() - 1);
	alphabet_->intToChar(v);
	content_[pos] = v;
}

/****************************************************************************************/

int SymbolList::getValue(unsigned int pos) const throw (IndexOutOfBoundsException)
{
  if(pos > content_.size())
    throw IndexOutOfBoundsException("SymbolList::getValue. Invalid position.", pos, 0, size() - 1);
	return content_[pos];
}

/****************************************************************************************/

