//
// File: AllelicAlphabet.cpp
// Authors:
//   Laurent Gueguen
// Created: lundi 20 décembre 2021, à 10h 59
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

#include <Bpp/Text/TextTools.h>

#include "AllelicAlphabet.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

AllelicAlphabet::AllelicAlphabet(const Alphabet& alph, uint nbAlleles) :
  alph_(alph),
  nbAlleles_(nbAlleles)
{
  if (nbAlleles_<=1)
    throw BadIntException((int)nbAlleles_, "AllelicAlphabet::AllelicAlphabet : wrong number of alleles", this);
  
  uint size = alph_.getSize();

  auto sword=alph_.getStateCodingSize();
  auto snb=std::to_string(nbAlleles_).size();

  // Gap is such as "-6-0"
  
  string gapchar = alph_.getState(alph_.getGapCharacterCode()).getLetter();

  auto gapword =  gapchar + std::string("0",snb);

  registerState(new AlphabetState(-1, gapchar + std::to_string(nbAlleles_) + gapword, "gap"));

  
  // Monomorphic states are such as "A6-0"

  
  for (int i = 0; i < size; ++i)
  {
    auto desc= alph_.intToChar(i)+std::to_string(nbAlleles_)+ gapword;
    registerState(new AlphabetState(i, desc, desc));
  }

  // Polymorphic states are such as "A4C2"

  for (int i = 0; i < size-1; ++i)
    for (int j = i+1; j < size; ++j)
    {
      auto nbl = (i * (int)size + j) * (int)(nbAlleles_-1) + (int)size;
      for (int nba=1 ; nba<nbAlleles_; nba++)
      {
        auto sni=std::string("0",snb-std::to_string((int)nbAlleles_-nba).size()) + std::to_string((int)nbAlleles_-nba);
        auto snj=std::string("0",snb-std::to_string(nba).size()) + std::to_string(nba);
        auto desc= alph_.intToChar(i)+ sni + alph_.intToChar(j)+snj;
        registerState(new AlphabetState((int)(nbl + nba -1), desc, desc));
      }
    }

  // Unknown

  nbUnknown_=(int)(size*size*(nbAlleles_-1));
  
  auto desc=std::string("?",sword)+ std::to_string(nbAlleles_) + gapword;
  registerState(new AlphabetState(nbUnknown_, desc, desc));
}

/******************************************************************************/

std::string AllelicAlphabet::getAlphabetType() const
{
  return "Allelic(alphabet="+alph_.getAlphabetType()+",nbAlleles_="+std::to_string(nbAlleles_);
}


// /******************************************************************************/

 bool AllelicAlphabet::isResolvedIn(int state1, int state2) const
 {
   if (!isIntInAlphabet(state1))
     throw BadIntException(state1, "AllelicAlphabet::isResolvedIn(int, int): Specified base unknown.");

   if (!isIntInAlphabet(state2))
     throw BadIntException(state2, "AllelicAlphabet::isResolvedIn(int, int): Specified base unknown.");

   if (isUnresolved(state2))
     throw BadIntException(state2, "AllelicAlphabet::isResolvedIn(int, int): Unresolved base.");

   auto size=alph_.getSize();
   return (state1 == size*size*(nbAlleles_-1)) ? (state2 >= 0) : (state1 == state2);
 }

/******************************************************************************/

std::vector<int> AllelicAlphabet::getAlias(int state) const
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "AllelicAlphabet::getAlias(int): Specified base unknown.");

  if (state == nbUnknown_)
    return getSupportedInts();
  else
    return vector<int>(1, state);

}

/******************************************************************************/

std::vector<std::string> AllelicAlphabet::getAlias(const std::string& state) const
{
  string locstate = TextTools::toUpper(state);
  if (!isCharInAlphabet(locstate))
    throw BadCharException(locstate, "AllelicAlphabet::getAlias(string): Specified base unknown.");

  if (state == getState(nbUnknown_).getLetter())
    return getSupportedChars();
  else
    return vector<string>(1, state);
}

/******************************************************************************/

Sequence*  AllelicAlphabet::convertFromStateAlphabet(const Sequence& sequence) const
{
  AllelicAlphabet::AllelicTransliterator t(*this);
  return t.translate(sequence);
}
