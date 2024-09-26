// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>
#include <Bpp/Utils/MapTools.h>

#include "AlphabetState.h"
#include "DNA.h"

using namespace bpp;

// From STL:
#include <map>

using namespace std;

/******************************************************************************/

DNA::DNA(bool exclamationMarkCountsAsGap)
{
	// Alphabet content definition
	// all unresolved bases use nÂ°14
	registerState(new NucleicAlphabetState(-1, "-",  0, "Gap"));
	registerState(new NucleicAlphabetState( 0, "A",  1, "Adenine"));
	registerState(new NucleicAlphabetState( 1, "C",  2, "Cytosine"));
	registerState(new NucleicAlphabetState( 2, "G",  4, "Guanine"));
	registerState(new NucleicAlphabetState( 3, "T",  8, "Thymine"));
	registerState(new NucleicAlphabetState( 4, "M",  3, "Adenine or Cytosine"));
	registerState(new NucleicAlphabetState( 5, "R",  5, "Purine (Adenine or Guanine)"));
	registerState(new NucleicAlphabetState( 6, "W",  9, "Adenine or Thymine"));
	registerState(new NucleicAlphabetState( 7, "S",  6, "Cytosine or Guanine"));
	registerState(new NucleicAlphabetState( 8, "Y", 10, "Pyrimidine (Cytosine or Thymine)"));
	registerState(new NucleicAlphabetState( 9, "K", 12, "Guanine or Thymine"));
	registerState(new NucleicAlphabetState(10, "V",  7, "Adenine or Cytosine or Guanine"));
	registerState(new NucleicAlphabetState(11, "H", 11, "Adenine or Cytosine or Thymine"));
	registerState(new NucleicAlphabetState(12, "D", 13, "Adenine or Guanine or Thymine"));
	registerState(new NucleicAlphabetState(13, "B", 14, "Cytosine or Guanine or Thymine"));
	registerState(new NucleicAlphabetState(14, "N", 15, "Unresolved base"));
	registerState(new NucleicAlphabetState(14, "X", 15, "Unresolved base"));
	registerState(new NucleicAlphabetState(14, "O", 15, "Unresolved base"));
	registerState(new NucleicAlphabetState(14, "0", 15, "Unresolved base"));
	registerState(new NucleicAlphabetState(14, "?", 15, "Unresolved base"));
	if (exclamationMarkCountsAsGap)
		registerState(new NucleicAlphabetState(-1, "!", 0, "Frameshift"));
	else
		registerState(new NucleicAlphabetState(14, "!", 15, "Unresolved base"));
}

/******************************************************************************/

bool DNA::isResolvedIn(int state1, int state2) const
{
	if (state1 < 0 || !isIntInAlphabet(state1))
		throw BadIntException(state1, "AbstractAlphabet::isResolvedIn(int, int): Specified base " + intToChar(state1) + " is unknown.", this);

	if (state2 < 0 || !isIntInAlphabet(state2))
		throw BadIntException(state2, "AbstractAlphabet::isResolvedIn(int, int): Specified base " + intToChar(state2) + " is unknown.", this);

	if (isUnresolved(state2))
		throw BadIntException(state2, "AbstractAlphabet::isResolvedIn(int, int): Unresolved base " + intToChar(state2), this);

	if (state1 == -1)
		return state2 == -1;

	const NucleicAlphabetState& st1 = getState(state1);

	switch (state2)
	{
	case 0:
		return st1.getBinaryCode() & 1;
	case 1:
		return st1.getBinaryCode() & 2;
	case 2:
		return st1.getBinaryCode() & 4;
	case 3:
		return st1.getBinaryCode() & 8;
	default:
		throw BadIntException(state2, "DNA::isResolvedIn : this should not happen", this);
	}
}

/******************************************************************************/

std::vector<int> DNA::getAlias(int state) const
{
	if (!isIntInAlphabet(state))
		throw BadIntException(state, "DNA::getAlias(int): Specified base unknown.", this);
	vector<int> v;
	const NucleicAlphabetState& st = getState(state);
	if (state == -1)
		v.push_back(-1);
	if (st.getBinaryCode() & 1)
		v.push_back(0);
	if (st.getBinaryCode() & 2)
		v.push_back(1);
	if (st.getBinaryCode() & 4)
		v.push_back(2);
	if (st.getBinaryCode() & 8)
		v.push_back(3);
	return v;
}


/******************************************************************************/

std::vector<std::string> DNA::getAlias(const std::string& state) const
{
	string locstate = TextTools::toUpper(state);
	if (!isCharInAlphabet(locstate))
		throw BadCharException(locstate, "DNA::getAlias(int): Specified base unknown.", this);
	vector<int> vi = this->getAlias(this->charToInt(state));
	vector<string> v;
	for (unsigned int i = 0; i < vi.size(); i++)
	{
		v.push_back(this->intToChar(vi[i]));
	}
	return v;
}

/******************************************************************************/

int DNA::getGeneric(const std::vector<int>& states) const
{
	int v = 0;
	for (size_t i = 0; i < states.size(); ++i)
	{
		if (!isIntInAlphabet(states[i]))
			throw BadIntException(states[i], "DNA::getGeneric(const vector<int>& states): Specified base unknown.", this);
		v |= getState(states[i]).getBinaryCode();
	}
	return getStateByBinCode(v).getNum();
}

/******************************************************************************/

std::string DNA::getGeneric(const std::vector<std::string>& states) const
{
	vector<int> vi;
	for (unsigned int i = 0; i < states.size(); ++i)
	{
		if (!isCharInAlphabet(states[i]))
			throw BadCharException(states[i], "DNA::getGeneric(const vector<string>& states): Specified base unknown.", this);
		vi.push_back(this->charToInt(states[i]));
	}
	return intToChar(getGeneric(vi));
}

/******************************************************************************/
