// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "LexicalAlphabet.h"
#include "AlphabetExceptions.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

LexicalAlphabet::LexicalAlphabet(const vector<std::string>& vocab) :
  AbstractAlphabet()
{
  if (vocab.size() == 0)
    throw Exception("LexicalAlphabet::LexicalAlphabet: not constructible from empty vocabulary.");

  size_t t = vocab[0].size();

  string s = "";
  for (size_t i = 0; i < t; i++)
  {
    s += "-";
  }

  registerState(new AlphabetState(-1, s, "gap"));


  for (size_t i = 0; i < vocab.size(); ++i)
  {
    if (t != vocab[i].size())
      throw Exception("LexicalAlphabet: several lengths in vocabulary.");

    try
    {
      string s2 = getName(vocab[i]);
      throw Exception("LexicalAlphabet : " + vocab[i] + " defined twice.");
    }
    catch (BadCharException& e)
    {}

    registerState(new AlphabetState(static_cast<int>(i), vocab[i], vocab[i]));
  }

  s = "";
  for (size_t i = 0; i < t; i++)
  {
    s += "?";
  }

  registerState(new AlphabetState(static_cast<int>(vocab.size()), s, "Unresolved word"));
}


/******************************************************************************/

std::string LexicalAlphabet::getAlphabetType() const
{
  string s = "Lexicon(";

  for (size_t i = 1; i < getNumberOfStates() - 1; i++)
  {
    if (i != 1)
      s += ",";

    s += getStateAt(i).getLetter();
  }

  s += ")";

  return s;
}
