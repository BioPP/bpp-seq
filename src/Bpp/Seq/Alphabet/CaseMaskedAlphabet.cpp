// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "CaseMaskedAlphabet.h"

using namespace bpp;

// From the STL:
#include <vector>
#include <iostream>

using namespace std;

CaseMaskedAlphabet::CaseMaskedAlphabet(std::shared_ptr<const LetterAlphabet> nocaseAlphabet) :
	LetterAlphabet(true),
	nocaseAlphabet_(nocaseAlphabet)
{
	vector<string> chars = nocaseAlphabet_->getSupportedChars();
	for (size_t i = 0; i < chars.size(); ++i)
	{
		AlphabetState* state = nocaseAlphabet_->getState(chars[i]).clone();
		registerState(state);
		char c = *chars[i].c_str();
		if (isalpha(c))
		{
			if (isupper(c))
			{
				registerState(new AlphabetState(state->getNum() + 100, TextTools::toLower(state->getLetter()), string("Masked ") + state->getName()));
			}
		}
	}
}

int CaseMaskedAlphabet::getMaskedEquivalentState(int state) const
{
	if (!isIntInAlphabet(state))
		throw BadIntException(state, "CaseMaskedAlphabet::getMaskedEquivalentState. Unsupported state code.", this);
	if (state >= 100)
		return state;
	else
	{
		state += 100;
		if (!isIntInAlphabet(state))
			throw BadIntException(state, "CaseMaskedAlphabet::getMaskedEquivalentState. State has masked equivalent.", this);
		return state;
	}
}

const string CaseMaskedAlphabet::getMaskedEquivalentState(const string& state) const
{
	if (!isCharInAlphabet(state))
		throw BadCharException(state, "CaseMaskedAlphabet::getMaskedEquivalentState. Unsupported state code.", this);
	int code = charToInt(state);
	if (code >= 100)
		return state;
	else
	{
		code += 100;
		if (!isIntInAlphabet(code))
			throw BadIntException(code, "CaseMaskedAlphabet::getMaskedEquivalentState. State has masked equivalent.", this);
		return intToChar(code);
	}
}
