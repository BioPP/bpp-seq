// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "AlphabetTools.h"

using namespace bpp;

// From the STL:
#include <ctype.h>
#include <iostream>
#include <memory>

using namespace std;

/**********************************************************************************************/
shared_ptr<const DNA> AlphabetTools::DNA_ALPHABET = make_shared<const DNA>();
shared_ptr<const RNA> AlphabetTools::RNA_ALPHABET = make_shared<const RNA>();
shared_ptr<const CodonAlphabet> AlphabetTools::DNA_CODON_ALPHABET = make_shared<const CodonAlphabet>(dynamic_pointer_cast<const NucleicAlphabet>(DNA_ALPHABET));
shared_ptr<const CodonAlphabet> AlphabetTools::RNA_CODON_ALPHABET = make_shared<const CodonAlphabet>(dynamic_pointer_cast<const NucleicAlphabet>(RNA_ALPHABET));
shared_ptr<const ProteicAlphabet> AlphabetTools::PROTEIN_ALPHABET = make_shared<const ProteicAlphabet>();
shared_ptr<const DefaultAlphabet> AlphabetTools::DEFAULT_ALPHABET = make_shared<const DefaultAlphabet>();

/**********************************************************************************************/

int AlphabetTools::getType(char state)
{
	if (state == '-')
		return -1;
	state = static_cast<char>(toupper(static_cast<int>(state))); // toupper works on int
	bool d = DNA_ALPHABET->isCharInAlphabet(TextTools::toString(state));
	bool r = RNA_ALPHABET->isCharInAlphabet(TextTools::toString(state));
	bool p = PROTEIN_ALPHABET->isCharInAlphabet(TextTools::toString(state));

	if (!d && !r && !p)
		return 0; // Unknown character
	else if (d && !r && !p)
		return 1; // DNA specific
	else if (!d && r && !p)
		return 2; // RNA specific
	else if (!d && !r && p)
		return 3; // Protein specific
	else if (d && r && !p)
		return 4; // Nucleotide specific
	else if (d && !r && p)
		return 5; // DNA or Protein specific
	else if (!d && r && p)
		return 6; // RNA or Protein specific
	else
		return 7; // Non-specific character
}

/**********************************************************************************************/

bool AlphabetTools::checkAlphabetCodingSize(const Alphabet& alphabet)
{
	if (alphabet.getNumberOfChars() == 0)
		return true; // Will this really happen?
	size_t size = alphabet.intToChar(0).size();

	for (size_t i = 1; i < alphabet.getNumberOfTypes(); ++i)
	{
		if (alphabet.intToChar(alphabet.getStateAt(i).getNum()).size() != size)
			return false;
	}
	return true;
}

/**********************************************************************************************/

bool AlphabetTools::checkAlphabetCodingSize(const Alphabet* alphabet)
{
	return checkAlphabetCodingSize(*alphabet);
}

/**********************************************************************************************/

unsigned int AlphabetTools::getAlphabetCodingSize(const Alphabet& alphabet)
{
	if (!checkAlphabetCodingSize(alphabet))
		throw AlphabetException("Bad alphabet in function Alphabet::getAlphabetCodingSize().", &alphabet);
	return static_cast<unsigned int>(alphabet.intToChar(0).size());
}

/**********************************************************************************************/

unsigned int AlphabetTools::getAlphabetCodingSize(const Alphabet* alphabet)
{
	return getAlphabetCodingSize(*alphabet);
}
