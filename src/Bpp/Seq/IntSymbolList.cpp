//
// File: IntSymbolList.cpp
// Authors:
//   Julien Dutheil
// Created: 2005-04-09 00:00:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/


#include "IntSymbolList.h"
#include "StringSequenceTools.h"

using namespace bpp;

using namespace std;


/****************************************************************************************/

void IntSymbolList::setContent(const vector<string>& list)
{
  // Check list for incorrect characters
  vector<int> coded(list.size());
  for (auto i : list)
  {
    if (!getAlphabet()->isCharInAlphabet(i))
      throw BadCharException(i, "IntSymbolList::setContent", getAlphabet());
  }

  size_t j = 0;
  for (auto i : list)
  {
    coded[j++] = getAlphabet()->charToInt(i);
  }

  // BasicIntSymbolList is valid:
  AbstractTemplateSymbolList<int>::setContent(coded);
}

/****************************************************************************************/

void IntSymbolList::setContent(const vector<int>& list)
{
  // Check list for incorrect characters
  for (auto i : list)
  {
    if (!getAlphabet()->isIntInAlphabet(i))
      throw BadIntException(i, "IntSymbolList::setContent", getAlphabet());
  }

  // Sequence is valid:
  AbstractTemplateSymbolList<int>::setContent(list);
}

/****************************************************************************************/

string IntSymbolList::getChar(size_t pos) const
{
  if (pos >= content_.size())
    throw IndexOutOfBoundsException("BasicIntSymbolList::getChar. Invalid position.", pos, 0, size() - 1);
  string c = "";
  try
  {
    c = getAlphabet()->intToChar(content_[pos]);
  }
  catch (BadIntException& bie)
  {
    // This should never happen!
  }
  return c;
}

/****************************************************************************************/

void EventDrivenIntSymbolList::setContent(const vector<string>& list)
{
  // Check list for incorrect characters
  vector<int> coded(list.size());
  for (auto i : list)
  {
    if (!getAlphabet()->isCharInAlphabet(i))
      throw BadCharException(i, "EventDrivenIntSymbolList::setContent", getAlphabet());
  }

  size_t j = 0;
  for (auto i : list)
  {
    coded[j++] = getAlphabet()->charToInt(i);
  }

  AbstractTemplateEventDrivenSymbolList<int>::setContent(coded);
}

/****************************************************************************************/

void EventDrivenIntSymbolList::setContent(const vector<int>& list)
{
  // Check list for incorrect characters
  for (auto i : list)
  {
    if (!getAlphabet()->isIntInAlphabet(i))
      throw BadIntException(i, "EventDrivenIntSymbolList::setContent", getAlphabet());
  }

  AbstractTemplateEventDrivenSymbolList<int>::setContent(list);
}

/****************************************************************************************/

string EventDrivenIntSymbolList::toString() const
{
  auto alphaPtr = getAlphabet();
  return StringSequenceTools::decodeSequence(content_, alphaPtr);
}

/****************************************************************************************/

void EventDrivenIntSymbolList::addElement(const string& c)
{
  AbstractTemplateEventDrivenSymbolList<int>::addElement(getAlphabet()->charToInt(c));
}

/****************************************************************************************/

void EventDrivenIntSymbolList::addElement(size_t pos, const string& c)
{
  AbstractTemplateEventDrivenSymbolList<int>::addElement(pos, getAlphabet()->charToInt(c));
}

/****************************************************************************************/

void EventDrivenIntSymbolList::setElement(size_t pos, const string& c)
{
 AbstractTemplateEventDrivenSymbolList<int>::setElement(pos, getAlphabet()->charToInt(c));
}

/****************************************************************************************/

string EventDrivenIntSymbolList::getChar(size_t pos) const
{
  if (pos >= content_.size())
    throw IndexOutOfBoundsException("EventDrivenIntSymbolList::getChar. Invalid position.", pos, 0, size() - 1);
  string c = "";
  try
  {
    c = getAlphabet()->intToChar(content_[pos]);
  }
  catch (BadIntException& bie)
  {
    // This should never happen!
  }
  return c;
}

