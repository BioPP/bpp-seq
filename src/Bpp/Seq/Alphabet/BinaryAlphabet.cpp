// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "AlphabetState.h"
#include "BinaryAlphabet.h"
#include "AlphabetExceptions.h"

// From Utils:
#include <Bpp/Text/TextTools.h>

using namespace bpp;

BinaryAlphabet::BinaryAlphabet()
{
  // Alphabet content definition
  registerState(new AlphabetState(-1, "-", "Gap"));
  for (int i = 0; i < 2; i++)
  {
    registerState(new AlphabetState(i, TextTools::toString(i), ""));
  }
  registerState(new AlphabetState(2, "?", "Unresolved state"));
}


/******************************************************************************/
bool BinaryAlphabet::isResolvedIn(int state1, int state2) const
{
  if (state1 < 0 || !isIntInAlphabet(state1))
    throw BadIntException(state1, "BinaryAlphabet::isResolvedIn(int, int): Specified base " + intToChar(state1) + " is unknown.", this);

  if (state2 < 0 || !isIntInAlphabet(state2))
    throw BadIntException(state2, "BinaryAlphabet::isResolvedIn(int, int): Specified base " + intToChar(state2) + " is unknown.", this);

  if (isUnresolved(state2))
    throw BadIntException(state2, "BinaryAlphabet::isResolvedIn(int, int): Unresolved base " + intToChar(state2), this);

  return (state1 == 2) || (state1 == state2);
}

/******************************************************************************/

std::vector<int> BinaryAlphabet::getAlias(int state) const
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "BinaryAlphabet::getAlias(int): Specified base unknown.", this);
  std::vector<int> v;
  switch (state)
  {
  case -1:
    v.push_back(-1);
    break;
  case 0:
    v.push_back(0);
    break;
  case 1:
    v.push_back(1);
    break;
  case 2:
    v.push_back(0);
    v.push_back(1);
    break;
  }

  return v;
}

/******************************************************************************/

std::vector<std::string> BinaryAlphabet::getAlias(const std::string& state) const
{
  if (!isCharInAlphabet(state))
    throw BadCharException(state, "BinaryAlphabet::getAlias(char): Specified base unknown.", this);

  std::vector<std::string> v(1);
  if (state == "?")
  {
    v.push_back("0");
    v.push_back("1");
  }
  else
    v.push_back(state);

  return v;
}
