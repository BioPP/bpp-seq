//
// File: ProteicAlphabet.cpp
// Authors: Guillaume Deuchst
//          Julien Dutheil
//          Sylvain Gaillard
// Created on: Tue Jul 22 2003
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

#include "ProteicAlphabet.h"
#include "ProteicAlphabetState.h"
#include <Bpp/Text/TextTools.h>
#include <Bpp/Utils/MapTools.h>

using namespace bpp;
using namespace std;

// From STL:
#include <map>

/******************************************************************************/

ProteicAlphabet::ProteicAlphabet()
{
  // Alphabet content definition
  registerState(ProteicAlphabetState(-1, "-", "GAP", "Gap"));
  registerState(ProteicAlphabetState( 0, "A", "ALA", "Alanine"));
  registerState(ProteicAlphabetState( 1, "R", "ARG", "Arginine"));
  registerState(ProteicAlphabetState( 2, "N", "ASN", "Asparagine"));
  registerState(ProteicAlphabetState( 3, "D", "ASP", "Asparatic Acid"));
  registerState(ProteicAlphabetState( 4, "C", "CYS", "Cysteine"));
  registerState(ProteicAlphabetState( 5, "Q", "GLN", "Glutamine"));
  registerState(ProteicAlphabetState( 6, "E", "GLU", "Glutamic acid"));
  registerState(ProteicAlphabetState( 7, "G", "GLY", "Glycine"));
  registerState(ProteicAlphabetState( 8, "H", "HIS", "Histidine"));
  registerState(ProteicAlphabetState( 9, "I", "ILE", "Isoleucine"));
  registerState(ProteicAlphabetState(10, "L", "LEU", "Leucine"));
  registerState(ProteicAlphabetState(11, "K", "LYS", "Lysine"));
  registerState(ProteicAlphabetState(12, "M", "MET", "Methionine"));
  registerState(ProteicAlphabetState(13, "F", "PHE", "Phenylalanine"));
  registerState(ProteicAlphabetState(14, "P", "PRO", "Proline"));
  registerState(ProteicAlphabetState(15, "S", "SER", "Serine"));
  registerState(ProteicAlphabetState(16, "T", "THR", "Threonine"));
  registerState(ProteicAlphabetState(17, "W", "TRP", "Tryptophan"));
  registerState(ProteicAlphabetState(18, "Y", "TYR", "Tyrosine"));
  registerState(ProteicAlphabetState(19, "V", "VAL", "Valine"));
  registerState(ProteicAlphabetState(20, "B", "B", "N or D"));
  registerState(ProteicAlphabetState(21, "Z", "Z", "Q or E"));
  registerState(ProteicAlphabetState(22, "X", "X", "Unresolved amino acid"));
  registerState(ProteicAlphabetState(22, "O", "O", "Unresolved amino acid"));
  registerState(ProteicAlphabetState(22, "0", "0", "Unresolved amino acid"));
  registerState(ProteicAlphabetState(22, "?", "?", "Unresolved amino acid"));
  registerState(ProteicAlphabetState(-2, "*", "STOP", "Stop"));
}

/******************************************************************************/

string ProteicAlphabet::getAbbr(const string& aa) const throw (AlphabetException)
{
  string AA = TextTools::toUpper(aa);
  return getState(aa).getAbbreviation();
}

/******************************************************************************/

string ProteicAlphabet::getAbbr(int aa) const throw (AlphabetException)
{
  return getState(aa).getAbbreviation();
}

/******************************************************************************/

vector<int> ProteicAlphabet::getAlias(int state) const throw (BadIntException)
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "ProteicAlphabet::getAlias(int): Specified base unknown.");
  vector<int> v;
  if (state == 20)  // N or D
  {
    v.resize(2); v[0] = 2; v[1] = 3;
  }
  else if (state == 21)  // Q or E
  {
    v.resize(2); v[0] = 5; v[1] = 6;
  }
  else if (state == 22)  // all!
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

vector<string> ProteicAlphabet::getAlias(const string& state) const throw (BadCharException)
{
  string locstate = TextTools::toUpper(state);
  if (!isCharInAlphabet(locstate))
    throw BadCharException(locstate, "ProteicAlphabet::getAlias(int): Specified base unknown.");
  vector<string> v;
  if (locstate == "B")  // N or D
  {
    v.resize(2); v[0] = "N"; v[1] = "D";
  }
  else if (locstate == "Z")  // Q or E
  {
    v.resize(2); v[0] = "Q"; v[1] = "E";
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

int ProteicAlphabet::getGeneric(const vector<int>& states) const throw (BadIntException)
{
  map<int, int> m;
  for (unsigned int i = 0; i < states.size(); ++i)
  {
    vector<int> tmp_s = this->getAlias(states[i]); // get the states for generic characters
    for (unsigned int j = 0; j < tmp_s.size(); ++j)
    {
      m[tmp_s[j]]++; // add each state to the list
    }
  }
  vector<int> ve = MapTools::getKeys(m);

  string key;
  for (unsigned int i = 0; i < ve.size(); ++i)
  {
    if (!isIntInAlphabet(ve[i]))
      throw BadIntException(ve[i], "ProteicAlphabet::getGeneric(const vector<int>): Specified base unknown.");
    key += "_" + TextTools::toString(ve[i]);
  }
  map<string, int> g;
  g["_2_3"] = 20;
  g["_5_6"] = 21;
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
    v = 22;
  }
  return v;
}

/******************************************************************************/

string ProteicAlphabet::getGeneric(const vector<string>& states) const throw (BadCharException)
{
  map<string, int> m;
  for (unsigned int i = 0; i < states.size(); ++i)
  {
    vector<string> tmp_s = this->getAlias(states[i]); // get the states for generic characters
    for (unsigned int j = 0; j < tmp_s.size(); ++j)
    {
      m[tmp_s[j]]++; // add each state to the list
    }
  }
  vector<string> ve = MapTools::getKeys(m);

  string key;
  for (unsigned int i = 0; i < ve.size(); ++i)
  {
    if (!isCharInAlphabet(ve[i]))
      throw BadCharException(ve[i], "ProteicAlphabet::getAlias(const vector<string>): Specified base unknown.");
    key += TextTools::toString(ve[i]);
  }
  map<string, string> g;
  g["DN"] = "B";
  g["EQ"] = "Z";
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
