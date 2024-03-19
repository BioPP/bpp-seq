// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>
#include <Bpp/Utils/MapTools.h>

#include "ProteicAlphabet.h"
#include "ProteicAlphabetState.h"

using namespace bpp;
using namespace std;

// From STL:
#include <map>

/******************************************************************************/

ProteicAlphabet::ProteicAlphabet()
{
  // Alphabet content definition
  registerState(new ProteicAlphabetState(-1, "-", "GAP", "Gap"));
  registerState(new ProteicAlphabetState( 0, "A", "ALA", "Alanine"));
  registerState(new ProteicAlphabetState( 1, "R", "ARG", "Arginine"));
  registerState(new ProteicAlphabetState( 2, "N", "ASN", "Asparagine"));
  registerState(new ProteicAlphabetState( 3, "D", "ASP", "Asparatic Acid"));
  registerState(new ProteicAlphabetState( 4, "C", "CYS", "Cysteine"));
  registerState(new ProteicAlphabetState( 5, "Q", "GLN", "Glutamine"));
  registerState(new ProteicAlphabetState( 6, "E", "GLU", "Glutamic acid"));
  registerState(new ProteicAlphabetState( 7, "G", "GLY", "Glycine"));
  registerState(new ProteicAlphabetState( 8, "H", "HIS", "Histidine"));
  registerState(new ProteicAlphabetState( 9, "I", "ILE", "Isoleucine"));
  registerState(new ProteicAlphabetState(10, "L", "LEU", "Leucine"));
  registerState(new ProteicAlphabetState(11, "K", "LYS", "Lysine"));
  registerState(new ProteicAlphabetState(12, "M", "MET", "Methionine"));
  registerState(new ProteicAlphabetState(13, "F", "PHE", "Phenylalanine"));
  registerState(new ProteicAlphabetState(14, "P", "PRO", "Proline"));
  registerState(new ProteicAlphabetState(15, "S", "SER", "Serine"));
  registerState(new ProteicAlphabetState(16, "T", "THR", "Threonine"));
  registerState(new ProteicAlphabetState(17, "W", "TRP", "Tryptophan"));
  registerState(new ProteicAlphabetState(18, "Y", "TYR", "Tyrosine"));
  registerState(new ProteicAlphabetState(19, "V", "VAL", "Valine"));
  registerState(new ProteicAlphabetState(20, "B", "B", "N or D"));
  registerState(new ProteicAlphabetState(21, "Z", "Z", "Q or E"));
  registerState(new ProteicAlphabetState(22, "J", "J", "I or L"));
  registerState(new ProteicAlphabetState(23, "X", "X", "Unresolved amino acid"));
  registerState(new ProteicAlphabetState(23, "O", "O", "Unresolved amino acid"));
  registerState(new ProteicAlphabetState(23, "0", "0", "Unresolved amino acid"));
  registerState(new ProteicAlphabetState(23, "?", "?", "Unresolved amino acid"));
  registerState(new ProteicAlphabetState(-2, "*", "STOP", "Stop"));
}

/******************************************************************************/

string ProteicAlphabet::getAbbr(const string& aa) const
{
  string AA = TextTools::toUpper(aa);
  return getState(aa).getAbbreviation();
}

/******************************************************************************/

string ProteicAlphabet::getAbbr(int aa) const
{
  return getState(aa).getAbbreviation();
}

/******************************************************************************/

bool ProteicAlphabet::isResolvedIn(int state1, int state2) const
{
  if (!isIntInAlphabet(state1))
    throw BadIntException(state1, "DNA::isResolvedIn(int, int): Specified base unknown.", this);

  if (!isIntInAlphabet(state2))
    throw BadIntException(state2, "DNA::isResolvedIn(int, int): Specified base unknown.", this);

  if (isUnresolved(state2))
    throw BadIntException(state2, "DNA::isResolvedIn(int, int): Unresolved base.", this);

  if (state1 == 20)
    return state2 == 2 || state2 == 3;
  else if (state1 == 21)
    return state2 == 5 || state2 == 6;
  else if (state1 == 22)
    return state2 == 9 || state2 == 10;
  else if (state1 == 23)
    return state2 > 0;
  else
    return state1 == state2;
}

/******************************************************************************/


vector<int> ProteicAlphabet::getAlias(int state) const
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "ProteicAlphabet::getAlias(int): Specified base unknown.", this);
  vector<int> v;
  if (state == 20)  // N or D
  {
    v.resize(2); v[0] = 2; v[1] = 3;
  }
  else if (state == 21)  // Q or E
  {
    v.resize(2); v[0] = 5; v[1] = 6;
  }
  else if (state == 22)  // I or L
  {
    v.resize(2); v[0] = 9; v[1] = 10;
  }
  else if (state == 23)  // all!
  {
    v.resize(20);
    for (size_t i = 0; i < 20; i++)
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

vector<string> ProteicAlphabet::getAlias(const string& state) const
{
  string locstate = TextTools::toUpper(state);
  if (!isCharInAlphabet(locstate))
    throw BadCharException(locstate, "ProteicAlphabet::getAlias(int): Specified base unknown.", this);
  vector<string> v;
  if (locstate == "B")  // N or D
  {
    v.resize(2); v[0] = "N"; v[1] = "D";
  }
  else if (locstate == "Z")  // Q or E
  {
    v.resize(2); v[0] = "Q"; v[1] = "E";
  }
  else if (locstate == "J")  // I or L
  {
    v.resize(2); v[0] = "I"; v[1] = "L";
  }
  else if (locstate == "X"
      || locstate == "O"
      || locstate == "0"
      || locstate == "?")  // all!
  {
    v.resize(20);
    for (int i = 0; i < 20; i++)
    {
      v[static_cast<size_t>(i)] = getState(i).getLetter();
    }
  }
  else
  {
    v.resize(1); v[0] = locstate;
  }
  return v;
}

/******************************************************************************/

int ProteicAlphabet::getGeneric(const vector<int>& states) const
{
  map<int, int> m;
  for (size_t i = 0; i < states.size(); ++i)
  {
    vector<int> tmp_s = this->getAlias(states[i]); // get the states for generic characters
    for (size_t j = 0; j < tmp_s.size(); ++j)
    {
      m[tmp_s[j]]++; // add each state to the list
    }
  }
  vector<int> ve = MapTools::getKeys(m);

  string key;
  for (size_t i = 0; i < ve.size(); ++i)
  {
    if (!isIntInAlphabet(ve[i]))
      throw BadIntException(ve[i], "ProteicAlphabet::getGeneric(const vector<int>): Specified base unknown.", this);
    key += "_" + TextTools::toString(ve[i]);
  }
  map<string, int> g;
  g["_2_3"] = 20;
  g["_5_6"] = 21;
  g["_9_10"] = 22;
  int v;
  map<string, int>::iterator it = g.find(key);
  if (ve.size() == 1)
  {
    v = ve[0];
  }
  else if (it != g.end())
  {
    v = it->second;
  }
  else
  {
    v = 23;
  }
  return v;
}

/******************************************************************************/

string ProteicAlphabet::getGeneric(const vector<string>& states) const
{
  map<string, int> m;
  for (size_t i = 0; i < states.size(); ++i)
  {
    vector<string> tmp_s = this->getAlias(states[i]); // get the states for generic characters
    for (size_t j = 0; j < tmp_s.size(); ++j)
    {
      m[tmp_s[j]]++; // add each state to the list
    }
  }
  vector<string> ve = MapTools::getKeys(m);

  string key;
  for (size_t i = 0; i < ve.size(); ++i)
  {
    if (!isCharInAlphabet(ve[i]))
      throw BadCharException(ve[i], "ProteicAlphabet::getAlias(const vector<string>): Specified base unknown.", this);
    key += TextTools::toString(ve[i]);
  }
  map<string, string> g;
  g["DN"] = "B";
  g["EQ"] = "Z";
  g["IL"] = "J";
  string v;
  map<string, string>::iterator it = g.find(key);
  if (ve.size() == 1)
  {
    v = ve[0];
  }
  else if (it != g.end())
  {
    v = it->second;
  }
  else
  {
    v = "?";
  }
  return v;
}

/******************************************************************************/
