//
// File: CodonAlphabet.cpp
// Created by: Julien Dutheil
// Created on: Sun Oct 12 17:41:56 2003
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

#include "CodonAlphabet.h"
#include <Bpp/Text/TextTools.h>

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;


void CodonAlphabet::build_()
{
  vector<AlphabetState*> states(66);

  states[0] = new AlphabetState(-1, "---", "gap");

  size_t i=0;
  for (int i1 = 0; i1 < 4; ++i1)
    for (int i2 = 0; i2 < 4; ++i2)
      for (int i3 = 0; i3 < 4; ++i3)
      {
        string s=nAlph_->intToChar(i1)+nAlph_->intToChar(i2)+nAlph_->intToChar(i3);
        states[i+1] = new AlphabetState(static_cast<int>(i), s, s);
        i++;
      }


  states[65] = new AlphabetState(static_cast<int>(64), "NNN", "Unresolved");

  //Now register all states once for all:
  for (i = 0; i < states.size(); ++i) 
    registerState(states[i]);

}

int CodonAlphabet::getGCinCodon(int codon) const
{
  int i=0;
  int j=getFirstPosition(codon);
  if (j==1 || j==2)
    i++;
  j=getSecondPosition(codon);
  if (j==1 || j==2)
    i++;
  j=getThirdPosition(codon);
  if (j==1 || j==2)
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

Sequence* CodonAlphabet::translate(const Sequence& sequence, size_t pos) const
{
  vector<int> content;
  
  size_t s = sequence.size();
  size_t i = pos;
  
  while (i + 3 <= s)
  {
    content.push_back(getWord(sequence, i));
    i += 3;
  }

  return new BasicSequence(sequence.getName(), content, this);
}

/****************************************************************************************/

Sequence* CodonAlphabet::reverse(const Sequence& sequence) const
{
  Sequence* pseq = new BasicSequence(sequence.getName(), "", getNAlphabet(0));

  size_t s = sequence.size();
  for (size_t i = 0; i < s; i++)
  {
    pseq->append(getPositions(sequence[i]));
  }

  return pseq;
}

/****************************************************************************************/

std::vector<int> CodonAlphabet::getAlias(int state) const
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "WordAlphabet::getAlias(int): Specified base unknown.");
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
    throw BadCharException(locstate, "CodonAlphabet::getAlias(string): Specified base unknown.");
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
  if (!isIntInAlphabet(state1))
    throw BadIntException(state1, "CodonAlphabet::isResolvedIn(int, int): Specified base unknown.");

  if (!isIntInAlphabet(state2))
    throw BadIntException(state2, "CodonAlphabet::isResolvedIn(int, int): Specified base unknown.");

  if (isUnresolved(state2))
    throw BadIntException(state2, "CodonAlphabet::isResolvedIn(int, int): Unresolved base.");

  return (state1 == 64)?(state2 >= 0):(state1==state2);
}

