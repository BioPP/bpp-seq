// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "WordAlphabet.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

WordAlphabet::WordAlphabet(const vector<std::shared_ptr<const Alphabet>>& vAlpha) :
  AbstractAlphabet(),
  vAbsAlph_(vAlpha)
{
  build_();
}

WordAlphabet::WordAlphabet(std::shared_ptr<const Alphabet> pAlpha, size_t num) :
  AbstractAlphabet(),
  vAbsAlph_(0)
{
  for (size_t i = 0; i < num; i++)
  {
    vAbsAlph_.push_back(pAlpha);
  }

  build_();
}

void WordAlphabet::build_()
{
  size_t size = 1;

  for (size_t i = 0; i < vAbsAlph_.size(); ++i)
  {
    size *= vAbsAlph_[i]->getSize();
  }

  vector<AlphabetState*> states(size + 2);

  string s = "";
  for (size_t i = 0; i < vAbsAlph_.size(); ++i)
  {
    s += "-";
  }

  states[0] = new AlphabetState(-1, s, "gap");

  for (size_t i = 0; i < size; ++i)
  {
    states[i + 1] = new AlphabetState(static_cast<int>(i), "", "");
  }

  size_t lr = size;
  char c;
  for (size_t na = 0; na < vAbsAlph_.size(); ++na)
  {
    lr /= vAbsAlph_[na]->getSize();
    size_t j = 1;
    int i = 0;
    while (j <= size)
    {
      c = vAbsAlph_[na]->intToChar(i)[0];

      for (size_t k = 0; k < lr; k++)
      {
        states[j]->setLetter(states[j]->getLetter() + c);
        j++;
        // alphabet[j++].letter += c;
      }

      if (++i == static_cast<int>(vAbsAlph_[na]->getSize()))
        i = 0;
    }
  }

  s = "";
  for (size_t i = 0; i < vAbsAlph_.size(); ++i)
  {
    s += "N";
  }

  states[size + 1] = new AlphabetState(static_cast<int>(size), s, "Unresolved");

  // Now register all states once for all:
  for (size_t i = 0; i < states.size(); ++i)
  {
    registerState(states[i]);
  }
}

/******************************************************************************/

std::string WordAlphabet::getAlphabetType() const
{
  string s = "Word(";
  for (unsigned int i = 0; i < vAbsAlph_.size(); i++)
  {
    if (i != 0)
      s += ",";

    s += "alphabet" + TextTools::toString(i + 1) + "=" + vAbsAlph_[i]->getAlphabetType();
  }

  s += ")";

  return s;
}

bool WordAlphabet::hasUniqueAlphabet() const
{
  string s = vAbsAlph_[0]->getAlphabetType();
  for (unsigned int i = 1; i < vAbsAlph_.size(); i++)
  {
    if (vAbsAlph_[i]->getAlphabetType() != s)
      return false;
  }
  return true;
}

bool WordAlphabet::containsUnresolved(const std::string& state) const
{
  size_t s = vAbsAlph_.size();
  if (state.length() != s)
    throw BadCharException(state, "WordAlphabet::containsUnresolved", this);

  for (size_t i = 0; i < vAbsAlph_.size(); i++)
  {
    if (vAbsAlph_[i]->isUnresolved(state.substr(i, 1)))
    {
      return true;
    }
  }
  return false;
}

/******************************************************************************/

bool WordAlphabet::containsGap(const std::string& state) const
{
  size_t s = vAbsAlph_.size();
  if (state.length() != s)
    throw BadCharException(state, "WordAlphabet::containsGap", this);

  for (size_t i = 0; i < vAbsAlph_.size(); i++)
  {
    if (vAbsAlph_[i]->isGap(state.substr(i, 1)))
      return true;
  }

  return false;
}

/******************************************************************************/

std::string WordAlphabet::getName(const std::string& state) const
{
  if (state.size() != vAbsAlph_.size())
    throw BadCharException(state, "WordAlphabet::getName", this);
  if (containsUnresolved(state))
    return getStateAt(getSize() + 1).getName();
  if (containsGap(state))
    return getStateAt(0).getName();
  else
    return AbstractAlphabet::getName(state);
}

/******************************************************************************/

bool WordAlphabet::isResolvedIn(int state1, int state2) const
{
  if (!isIntInAlphabet(state1))
    throw BadIntException(state1, "WordAlphabet::isResolvedIn(int, int): Specified base unknown.", this);

  if (!isIntInAlphabet(state2))
    throw BadIntException(state2, "WordAlphabet::isResolvedIn(int, int): Specified base unknown.", this);

  if (isUnresolved(state2))
    throw BadIntException(state2, "WordAlphabet::isResolvedIn(int, int): Unresolved base.", this);

  return (state1 == (int)getSize()) ? (state2 >= 0) : (state1 == state2);
}

/******************************************************************************/

std::vector<int> WordAlphabet::getAlias(int state) const
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "WordAlphabet::getAlias(int): Specified base unknown.", this);
  vector<int> v;
  size_t s = getSize();

  if (static_cast<size_t>(state) == s)
  {
    v.resize(s);
    for (size_t i = 0; i < s; ++i)
    {
      v[i] = static_cast<int>(i);
    }
  }
  else
  {
    v.resize(1); v[0] = state;
  }
  return v;
}

/******************************************************************************/

std::vector<std::string> WordAlphabet::getAlias(const std::string& state) const
{
  string locstate = TextTools::toUpper(state);
  if (!isCharInAlphabet(locstate))
    throw BadCharException(locstate, "WordAlphabet::getAlias(string): Specified base unknown.", this);
  vector<string> v;

  size_t s = getSize();

  string st = "";
  for (size_t i = 0; i < vAbsAlph_.size(); ++i)
  {
    st += "N";
  }

  if (locstate == st)
  {
    v.resize(s);
    for (size_t i = 0; i < s; ++i)
    {
      v[i] = intToChar(static_cast<int>(i));
    }
  }
  else
  {
    v.resize(1); v[0] = state;
  }
  return v;
}

/******************************************************************************/

int WordAlphabet::getGeneric(const std::vector<int>& states) const
{
  return states[0];
}

/******************************************************************************/

std::string WordAlphabet::getGeneric(const std::vector<std::string>& states) const
{
  return states[0];
}

/******************************************************************************/

int WordAlphabet::getWord(const Sequence& seq, size_t pos) const
{
  if (seq.size() < pos + vAbsAlph_.size())
    throw IndexOutOfBoundsException("WordAlphabet::getWord", pos, 0, seq.size() - vAbsAlph_.size());

  vector<string> vs;
  for (size_t i = 0; i < vAbsAlph_.size(); i++)
  {
    vs.push_back(vAbsAlph_[i]->intToChar(seq[i + pos]));
  }

  return charToInt(getWord(vs)); // This can't throw a BadCharException!
}


/******************************************************************************/

int WordAlphabet::getWord(const std::vector<int>& vint, size_t pos) const
{
  if (vint.size() < pos + vAbsAlph_.size())
    throw IndexOutOfBoundsException("WordAlphabet::getWord", pos, 0, vint.size() - vAbsAlph_.size());

  vector<string> vs;
  for (size_t i = 0; i < vAbsAlph_.size(); i++)
  {
    vs.push_back(vAbsAlph_[i]->intToChar(vint[i + pos]));
  }

  return charToInt(getWord(vs)); // This can't throw a BadCharException!
}

/****************************************************************************************/

std::string WordAlphabet::getWord(const std::vector<string>& vpos, size_t pos) const
{
  if (vpos.size() < pos + vAbsAlph_.size())
    throw IndexOutOfBoundsException("WordAlphabet::getWord", pos, 0, vpos.size() - vAbsAlph_.size());

  string s = "";
  for (size_t i = 0; i < vAbsAlph_.size(); i++)
  {
    s += vpos[pos + i];
  }
  // test
  charToInt(s);
  return s;
}

/****************************************************************************************/

unique_ptr<SequenceInterface> WordAlphabet::translate(const SequenceInterface& sequence, size_t pos) const
{
  if ((!hasUniqueAlphabet()) or
      (sequence.getAlphabet()->getAlphabetType() != vAbsAlph_[0]->getAlphabetType()))
    throw AlphabetMismatchException("No matching alphabets", sequence.getAlphabet().get(), vAbsAlph_[0].get());

  vector<int> content;

  size_t s = sequence.size();
  unsigned int l = getLength();
  size_t i = pos;

  while (i + l <= s)
  {
    content.push_back(getWord(sequence, i));
    i += l;
  }

  auto alphaPtr =  shared_from_this();
  return make_unique<Sequence>(sequence.getName(), content, alphaPtr);
}

/****************************************************************************************/

unique_ptr<SequenceInterface> WordAlphabet::reverse(const SequenceInterface& sequence) const
{
  if ((!hasUniqueAlphabet()) or
      (sequence.getAlphabet()->getAlphabetType() != getAlphabetType()))
    throw AlphabetMismatchException("No matching alphabets", sequence.getAlphabet().get(), this);

  auto alphaPtr = getNAlphabet(0);
  auto seqPtr = make_unique<Sequence>(sequence.getName(), "", alphaPtr);

  size_t s = sequence.size();
  for (size_t i = 0; i < s; i++)
  {
    seqPtr->append(getPositions(sequence[i]));
  }

  return seqPtr;
}

/****************************************************************************************/
