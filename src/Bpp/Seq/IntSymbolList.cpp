//
// File: IntSymbolList.cpp
// Created by: Julien Dutheil
// Created on: Fri Apr 9 2005
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#include "IntSymbolList.h"
#include "StringSequenceTools.h"

using namespace bpp;

using namespace std;

/****************************************************************************************/

BasicIntSymbolList::BasicIntSymbolList(const std::vector<string>& list, const Alphabet* alpha) :
  SymbolList<int>(alpha)
{
  setContent(list);
}

BasicIntSymbolList::BasicIntSymbolList(const std::vector<int>& list, const Alphabet* alpha) :
  SymbolList<int>(alpha)
{
  setContent(list);
}

/****************************************************************************************/

// BasicIntSymbolList::BasicIntSymbolList(const IntSymbolList& list):
//   alphabet_(list.getAlphabet()), content_(list.size())
// {
//   for (size_t i = 0; i < list.size(); ++i)
//     content_[i] = list[i];
// }

BasicIntSymbolList::BasicIntSymbolList(const BasicIntSymbolList& list):
  SymbolList<int>(list) {}

// BasicIntSymbolList& BasicIntSymbolList::operator=(const IntSymbolList& list)
// {
//   content_.resize(list.size());
//   for (size_t i = 0; i < list.size(); ++i)
//     content_[i] = list[i];
//   alphabet_ = list.getAlphabet();
//   return *this;
// }

BasicIntSymbolList& BasicIntSymbolList::operator=(const BasicIntSymbolList& list)
{
  SymbolList<int>::operator=(list);
  return *this;
}

/****************************************************************************************/

void BasicIntSymbolList::setContent(const vector<string>& list)
{
  // Check list for incorrect characters
  vector<int> coded(list.size());
  for (size_t i = 0; i < list.size(); i++)
    if(!getAlphabet()->isCharInAlphabet(list[i])) throw BadCharException(list[i], "BasicIntSymbolList::setContent", getAlphabet());
  
  for (size_t i = 0; i < list.size(); i++) 
    coded[i] = getAlphabet()->charToInt(list[i]);
	
  //BasicIntSymbolList is valid:
  SymbolList<int>::setContent(coded);
}

/****************************************************************************************/

void BasicIntSymbolList::setContent(const vector<int>& list)
{
  // Check list for incorrect characters
  for (size_t i = 0; i < list.size(); i++)
    if(!getAlphabet()->isIntInAlphabet(list[i]))
      throw BadIntException(list[i], "BasicIntSymbolList::setContent", getAlphabet());
	
  //Sequence is valid:
  SymbolList<int>::setContent(list);
}

/****************************************************************************************/

string BasicIntSymbolList::toString() const
{
  return StringSequenceTools::decodeSequence(content_, getAlphabet());
};

/****************************************************************************************/

void BasicIntSymbolList::addElement(const string& c)
{
  content_.push_back(getAlphabet()->charToInt(c));
}

/****************************************************************************************/

void BasicIntSymbolList::addElement(size_t pos, const string& c)
{
  if(pos >= content_.size()) throw IndexOutOfBoundsException("BasicIntSymbolList::addElement. Invalid position.", pos, 0, size() - 1);
  content_.insert(content_.begin() + static_cast<ptrdiff_t>(pos), getAlphabet()->charToInt(c));
}

/****************************************************************************************/

void BasicIntSymbolList::setElement(size_t pos, const string& c) 
{
  if(pos >= content_.size())
    throw IndexOutOfBoundsException("BasicIntSymbolList::setElement. Invalid position.", pos, 0, size() - 1);
  content_[pos] = getAlphabet()->charToInt(c);
}

/****************************************************************************************/

string BasicIntSymbolList::getChar(size_t pos) const
{
  if(pos >= content_.size())
    throw IndexOutOfBoundsException("BasicIntSymbolList::getChar. Invalid position.", pos, 0, size() - 1);
  string c = "";
  try
  {
    c = getAlphabet()->intToChar(content_[pos]);
  }
  catch(BadIntException bie)
  {
    //This should never happen!
  }
  return c;
}

/****************************************************************************************/

EdIntSymbolList::EdIntSymbolList(const std::vector<string>& list, const Alphabet* alpha) :
  EdSymbolList<int>(alpha)
{
  setContent(list);
}

EdIntSymbolList::EdIntSymbolList(const std::vector<int>& list, const Alphabet* alpha) :
  EdSymbolList<int>(list, alpha)
{
}

/****************************************************************************************/

EdIntSymbolList::EdIntSymbolList(const EdIntSymbolList& list):
  EdSymbolList<int>(list)
{
}

EdIntSymbolList& EdIntSymbolList::operator=(const EdIntSymbolList& list)
{
  EdSymbolList<int>::operator=(list);
  return *this;
}

/****************************************************************************************/

void EdIntSymbolList::setContent(const vector<string>& list)
{
  // Check list for incorrect characters
  vector<int> coded(list.size());
  for (size_t i = 0; i < list.size(); i++)
    if (!getAlphabet()->isCharInAlphabet(list[i])) throw BadCharException(list[i], "EdIntSymbolList::setContent", getAlphabet());
	
  for (size_t i = 0; i < list.size(); i++) 
    coded[i] = getAlphabet()->charToInt(list[i]);

  EdSymbolList<int>::setContent(coded);
};

/****************************************************************************************/

void EdIntSymbolList::setContent(const vector<int>& list)
{
  // Check list for incorrect characters
  for (size_t i = 0; i < list.size(); i++)
    if(!getAlphabet()->isIntInAlphabet(list[i]))
      throw BadIntException(list[i], "EdIntSymbolList::setContent", getAlphabet());

  EdSymbolList<int>::setContent(list);
};

/****************************************************************************************/

string EdIntSymbolList::toString() const
{
  return StringSequenceTools::decodeSequence(content_, getAlphabet());
};

/****************************************************************************************/

void EdIntSymbolList::addElement(const string& c)
{
  EdSymbolList<int>::addElement(getAlphabet()->charToInt(c));
}

/****************************************************************************************/

void EdIntSymbolList::addElement(size_t pos, const string& c)
{
  EdSymbolList<int>::addElement(pos, getAlphabet()->charToInt(c));
}

/****************************************************************************************/

void EdIntSymbolList::setElement(size_t pos, const string& c) 
{
  EdSymbolList<int>::setElement(pos, getAlphabet()->charToInt(c));
}

/****************************************************************************************/

string EdIntSymbolList::getChar(size_t pos) const
{
  if (pos >= content_.size())
    throw IndexOutOfBoundsException("EdIntSymbolList::getChar. Invalid position.", pos, 0, size() - 1);
  string c = "";
  try {
    c = getAlphabet()->intToChar(content_[pos]);
  } catch(BadIntException bie) {
    //This should never happen!
  }
  return c;
}

