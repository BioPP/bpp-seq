// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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

