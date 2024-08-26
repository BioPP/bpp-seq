// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "AlphabetState.h"
#include "IntegerAlphabet.h"

// From Utils:
#include <Bpp/Text/TextTools.h>

using namespace bpp;

IntegerAlphabet::IntegerAlphabet(unsigned int max) : MAX_(max)
{
	// Alphabet content definition
	registerState(new AlphabetState(-1, "-", "Gap"));

	for (int i = 0; i < static_cast<int>(MAX_); i++)
	{
		registerState(new AlphabetState(i, TextTools::toString(i), TextTools::toString(i)));
	}

	registerState(new AlphabetState(static_cast<int>(MAX_), "?", "?"));
}
