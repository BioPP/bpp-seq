// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "RNY.h" // class's header file

// From Utils:
#include <Bpp/Text/TextTools.h>
#include "AlphabetTools.h"

#include <iostream>
using namespace std;
using namespace bpp;

/****************************************************************************************/

RNY::RNY( std::shared_ptr<const NucleicAlphabet> na) : nuclalph_(na)
{
  // Initialization:
  vector<AlphabetState*> states(351, nullptr);

  // Alphabet content definition:

  string s1;

  if (AlphabetTools::isDNAAlphabet(*na))
    s1 = "RCT-";
  else
    s1 = "RCU-";

  string s2;

  if (AlphabetTools::isRNAAlphabet(*na))
    s2 = "AGCT-";
  else
    s2 = "AGCU-";

  string s3 = "AGY-";
  string s = "   ";


  // NNN (0->35)

  for (size_t i = 0; i < 3; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      for (size_t k = 0; k < 3; ++k)
      {
        size_t l = i * 12 + j * 3 + k;
        s[0] = s1[i];
        s[1] = s2[j];
        s[2] = s3[k];
        states[l] = new AlphabetState(static_cast<int>(l), s, s);
      }
    }
  }

  // NN- (50->83)

  for (size_t i = 0; i < 3; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      size_t l = 50 + 12 * i + j * 3;
      s[0] = s1[i];
      s[1] = s2[j];
      s[2] = s3[3];
      states[l] = new AlphabetState(static_cast<int>(l), s, s);
    }
  }

  // N-N (100->126)

  for (size_t i = 0; i < 3; ++i)
  {
    for (size_t k = 0; k < 3; ++k)
    {
      size_t l = 100 + 12 * i + k;
      s[0] = s1[i];
      s[1] = s2[4];
      s[2] = s3[k];
      states[l] = new AlphabetState(static_cast<int>(l), s, s);
    }
  }

  // N-- (150->152)

  for (size_t i = 0; i < 3; ++i)
  {
    size_t l = 150 + 12 * i;
    s[0] = s1[i];
    s[1] = s2[4];
    s[2] = s3[3];
    states[l] = new AlphabetState(static_cast<int>(l), s, s);
  }

  // -NN (200->211)

  for (size_t j = 0; j < 4; ++j)
  {
    for (size_t k = 0; k < 3; ++k)
    {
      size_t l = 200 + j * 3 + k;
      s[0] = s1[3];
      s[1] = s2[j];
      s[2] = s3[k];
      states[l] = new AlphabetState(static_cast<int>(l), s, s);
    }
  }


  // -N- (250->253)

  for (size_t j = 0; j < 4; ++j)
  {
    size_t l = 250 + 3 * j;
    s[0] = s1[3];
    s[1] = s2[j];
    s[2] = s3[3];
    states[l] = new AlphabetState(static_cast<int>(l), s, s);
  }

  // --N (300->302)

  for (size_t k = 0; k < 3; ++k)
  {
    size_t l = 300 + k;
    s[0] = s1[3];
    s[1] = s2[4];
    s[2] = s3[k];
    states[l] = new AlphabetState(static_cast<int>(l), s, s);
  }


  // Register all states:
  registerState(new AlphabetState(-1, "---", "---")); // put gap first

  for (size_t i = 0; i < states.size() - 1; ++i)
  {
    if (states[i])
      registerState(states[i]);
  }

  registerState(new AlphabetState(350, "NNN", "NNN")); // unresolved
}

/****************************************************************************************/

vector<int> RNY::getAlias(int state) const
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "RNY::getAlias(int): Specified base unknown.", this);
  vector<int> v;

  int i, j, k;
  if (state == -1 || state == 350)
  {
    // ---
    v.resize(36);
    for (i = 0; i < 3; ++i)
    {
      for (j = 0; j < 4; ++j)
      {
        for (k = 0; k < 3; ++k)
        {
          v[static_cast<size_t>(12 * i + 3 * j + k)] = 12 * i + 3 * j + k;
        }
      }
    }
    return v;
  }

  int qs = state / 50;
  int rs = state % 50;

  switch (qs)
  {
  case 0: // NNN
    v.resize(1);
    v[0] = rs;
    break;
  case 1: // NN-
    v.resize(3);
    for (k = 0; k < 3; ++k)
    {
      v[static_cast<size_t>(k)] = k + rs;
    }
    break;
  case 2: // N-N
    v.resize(4);
    for (j = 0; j < 4; ++j)
    {
      v[static_cast<size_t>(j)] = 3 * j + rs;
    }
    break;
  case 3: // N--
    v.resize(12);
    for (j = 0; j < 4; ++j)
    {
      for (k = 0; k < 3; ++k)
      {
        v[static_cast<size_t>(3 * j + k)] = rs + 3 * j + k;
      }
    }
    break;
  case 4: // -NN
    v.resize(3);
    for (i = 0; i < 3; ++i)
    {
      v[static_cast<size_t>(i)] = 12 * i + rs;
    }
    break;
  case 5: // -N-
    v.resize(9);
    for (i = 0; i < 3; ++i)
    {
      for (k = 0; k < 3; ++k)
      {
        v[static_cast<size_t>(3 * i + k)] = rs + 12 * i + k;
      }
    }
    break;
  case 6: // --N
    v.resize(12);
    for (i = 0; i < 3; ++i)
    {
      for (j = 0; j < 4; ++j)
      {
        v[static_cast<size_t>(4 * i + j)] = rs + 12 * i + 3 * j;
      }
    }
    break;
  }
  return v;
}

/****************************************************************************************/

bool RNY::isResolvedIn(int state1, int state2) const
{
  if (!isIntInAlphabet(state1))
    throw BadIntException(state1, "RNY::isResolvedIn(int, int): Specified base unknown.", this);

  if (!isIntInAlphabet(state2))
    throw BadIntException(state2, "RNY::isResolvedIn(int, int): Specified base unknown.", this);

  if (isUnresolved(state2))
    throw BadIntException(state2, "RNY::isResolvedIn(int, int): Unresolved base.", this);

  if (state1 == -1 || state1 == 350) // ---
    return true;

  int qs = state1 / 50;
  int rs = state1 % 50;

  switch (qs)
  {
  case 0: // NNN
    return state2 == rs;
  case 1: // NN-
    return (state2 < rs + 3) && (state2 >= rs);
  case 2: // N-N
    return (state2 - rs) % 3 == 0 && (state2 >= rs) && (state2 <  rs + 12);
  case 3: // N--
    return (state2 >= rs) && (state2 < rs + 12);
  case 4: // -NN
    return (state2 - rs) % 12 == 0 && (state2 >= rs) &&  (state2 < rs + 36);
  case 5: // -N-
    return (state2 - rs) % 12 < 3 && (state2 >= rs) &&  (state2 < rs + 27);
  case 6: // --N
    return (state2 - rs) % 3 == 0 && (state2 >= rs) &&  (state2 < rs + 36);
  default:
    throw BadIntException(state1, "RNY:isResolvedIn : this should not happen.", this);
  }
}

/****************************************************************************************/

vector<string> RNY::getAlias(const string& state) const
{
  if (!isCharInAlphabet(state))
    throw BadCharException(state, "RNY::getAlias(int): Specified base unknown.", this);

  vector<int> v = getAlias(charToInt(state));
  vector<string> s;
  size_t size = v.size();
  s.resize(size);

  for (size_t i = 0; i < size; i++)
  {
    s[i] = AbstractAlphabet::intToChar(v[i]);
  }
  return s;
}

/****************************************************************************************/

string RNY::getRNY(const string& pos1, const string& pos2, const string& pos3) const
{
  string tr;

  if (pos1 == "A" || pos1 == "G")
    tr = "R";
  else
    tr = pos1;

  tr += pos2;

  if (pos3 == "T" || pos3 == "U" || pos3 == "C")
    tr += "Y";
  else
    tr += pos3;

  // teste triplet;
  charToInt(tr);
  return tr;
}

/**************************************************************************************/

int RNY::getRNY(int i, int j, int k, const Alphabet& alph) const
{
  if (!AlphabetTools::isNucleicAlphabet(alph))
  {
    throw AlphabetException("RNY::getRNY : Sequence must be Nucleic", &alph);
  }

  char li = alph.intToChar(i)[0];
  char lj = alph.intToChar(j)[0];
  char lk = alph.intToChar(k)[0];

  int r = 0;
  int s = 0;

  switch (li)
  {
  case 'A':
  case 'G':
    r += 0;
    break;
  case 'C':
    r += 1;
    break;
  case 'T':
  case 'U':
    r += 2;
    break;
  case '-':
  case 'N':
    s += 1;
    break;
  default:
    throw BadCharException(&li, "RNY::getRNY(int,int;int,alph): Specified base unknown.", this);
  }

  r *= 4;
  s *= 2;

  switch (lj)
  {
  case 'A':
    r += 0;
    break;
  case 'G':
    r += 1;
    break;
  case 'C':
    r += 2;
    break;
  case 'T':
  case 'U':
    r += 3;
    break;
  case '-':
  case 'N':
    s += 1;
    break;
  default:
    throw BadCharException(&lj, "RNY::getRNY(int,int;int,alph): Specified base unknown.", this);
  }

  r *= 3;
  s *= 2;

  switch (lk)
  {
  case 'A':
    r += 0;
    break;
  case 'G':
    r += 1;
    break;
  case 'C':
  case 'T':
  case 'U':
    r += 2;
    break;
  case '-':
  case 'N':
    s += 1;
    break;
  default:
    throw BadCharException(&lk, "RNY::getRNY(int,int;int,alph): Specified base unknown.", this);
  }

  auto v = 50 * s + r;

  return v == 350 ? -1 : v;
}

/****************************************************************************************/
bool RNY::containsGap(const string& state) const
{
  return state.find("-") != string::npos;
}

bool RNY::isUnresolved(const string& state) const
{
  return isUnresolved(charToInt(state));
}

bool RNY::isUnresolved(int state) const
{
  return state >= 50;
}

/****************************************************************************************/

int RNY::charToInt(const string& state) const
{
  if (state.size() != 3)
    throw BadCharException(state, "RNY::charToInt", this);
  else
    return AbstractAlphabet::charToInt(state);
}


/************************************************************/

string RNY::intToChar(int state) const
{
  // ---

  if (state == -1 || state== 350)
    return "---";

  int i, j, k, l;
  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      for (k = 0; k < 3; ++k)
      {
        l = i * 12 + j * 3 + k;
        if (getState(l).getNum() == state)
          return getState(l).getLetter();
      }
    }
  }

  // NN- (50->83)

  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      l = 50 + 12 * i + j * 3;
      if (getState(l).getNum() == state)
        return getState(l).getLetter();
    }
  }

  // N-N (100->126)

  for (i = 0; i < 3; ++i)
  {
    for (k = 0; k < 3; ++k)
    {
      l = 100 + 12 * i + k;
      if (getState(l).getNum() == state)
        return getState(l).getLetter();
    }
  }

  // N-- (150->152)

  for (i = 0; i < 3; ++i)
  {
    l = 150 + 12 * i;
    if (getState(l).getNum() == state)
      return getState(l).getLetter();
  }

  // -NN (200->211)

  for (j = 0; j < 4; ++j)
  {
    for (k = 0; k < 3; ++k)
    {
      l = 200 + j * 3 + k;
      if (getState(l).getNum() == state)
        return getState(l).getLetter();
    }
  }


  // -N- (250->253)

  for (j = 0; j < 4; ++j)
  {
    l = 250 + 3 * j;
    if (getState(l).getNum() == state)
      return getState(l).getLetter();
  }

  // --N (300->302)

  for (k = 0; k < 3; ++k)
  {
    l = 300 + k;
    if (getState(l).getNum() == state)
      return getState(l).getLetter();
  }


  throw BadIntException(state, "RNY::intToChar: Specified base unknown", this);
  return "XXX";
}
