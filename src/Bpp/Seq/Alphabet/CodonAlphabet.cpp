// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "CodonAlphabet.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;


void CodonAlphabet::build_()
{
  vector<AlphabetState*> states(66);

  states[0] = new AlphabetState(-1, "---", "gap");

  size_t i = 0;
  for (int i1 = 0; i1 < 4; ++i1)
  {
    for (int i2 = 0; i2 < 4; ++i2)
    {
      for (int i3 = 0; i3 < 4; ++i3)
      {
        string s = nAlph_->intToChar(i1) + nAlph_->intToChar(i2) + nAlph_->intToChar(i3);
        states[i + 1] = new AlphabetState(static_cast<int>(i), s, s);
        i++;
      }
    }
  }


  states[65] = new AlphabetState(static_cast<int>(64), "NNN", "Unresolved");

  // Now register all states once for all:
  for (i = 0; i < states.size(); ++i)
  {
    registerState(states[i]);
  }
}

int CodonAlphabet::getGCinCodon(int codon) const
{
  int i = 0;
  int j = getFirstPosition(codon);
  if (j == 1 || j == 2)
    i++;
  j = getSecondPosition(codon);
  if (j == 1 || j == 2)
    i++;
  j = getThirdPosition(codon);
  if (j == 1 || j == 2)
    i++;

  return i;
}

bool CodonAlphabet::containsUnresolved(const std::string& state) const
{
  if (state.length() != 3)
    throw BadCharException(state, "CodonAlphabet::containsUnresolved", this);

  for (size_t i = 0; i < 3; i++)
  {
    if (nAlph_->isUnresolved(state.substr(i, 1)))
    {
      return true;
    }
  }
  return false;
}

/******************************************************************************/

bool CodonAlphabet::containsGap(const std::string& state) const
{
  if (state.length() != 3)
    throw BadCharException(state, "CodonAlphabet::containsGap", this);

  for (size_t i = 0; i < 3; i++)
  {
    if (nAlph_->isGap(state.substr(i, 1)))
      return true;
  }

  return false;
}


/****************************************************************************************/

unique_ptr<SequenceInterface> CodonAlphabet::translate(const SequenceInterface& sequence, size_t pos) const
{
  vector<int> content;

  size_t s = sequence.size();
  size_t i = pos;

  while (i + 3 <= s)
  {
    content.push_back(getWord(sequence, i));
    i += 3;
  }

  auto alphaPtr = shared_from_this();
  return make_unique<Sequence>(sequence.getName(), content, alphaPtr);
}

/****************************************************************************************/

unique_ptr<SequenceInterface> CodonAlphabet::reverse(const SequenceInterface& sequence) const
{
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

std::vector<int> CodonAlphabet::getAlias(int state) const
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "WordAlphabet::getAlias(int): Specified base unknown.", this);
  vector<int> v;

  if (state == 64)
  {
    v.resize(64);
    for (size_t i = 0; i < 64; ++i)
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

std::vector<std::string> CodonAlphabet::getAlias(const std::string& state) const
{
  string locstate = TextTools::toUpper(state);
  if (!isCharInAlphabet(locstate))
    throw BadCharException(locstate, "CodonAlphabet::getAlias(string): Specified base unknown.", this);
  vector<string> v;

  if (locstate == "NNN")
  {
    v.resize(64);
    for (size_t i = 0; i < 64; ++i)
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


bool CodonAlphabet::isResolvedIn(int state1, int state2) const
{
  if (state1 < 0 || !isIntInAlphabet(state1))
    throw BadIntException(state1, "CodonAlphabet::isResolvedIn(int, int): Specified base " + intToChar(state1) + " is unknown.", this);

  if (state2 < 0 || !isIntInAlphabet(state2))
    throw BadIntException(state2, "CodonAlphabet::isResolvedIn(int, int): Specified base " + intToChar(state2) + " is unknown.", this);

  if (isUnresolved(state2))
    throw BadIntException(state2, "CodonAlphabet::isResolvedIn(int, int): Unresolved base " + intToChar(state2), this);

  return (state1 == 64) ? (state2 >= 0) : (state1 == state2);
}
