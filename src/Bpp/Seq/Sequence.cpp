// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "Alphabet/AlphabetTools.h"
#include "Sequence.h" // class's header file
#include "StringSequenceTools.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

/******************************************************************************/

void Sequence::setContent(const std::string& sequence)
{
	// Remove blanks in sequence
	auto alphaPtr = getAlphabet();
	content_ = StringSequenceTools::codeSequence(TextTools::removeWhiteSpaces(sequence), alphaPtr);
	// Warning, an exception may be thrown here!
}

/******************************************************************************/

void Sequence::setToSizeR(size_t newSize)
{
	// Size verification
	size_t seqSize = content_.size();
	if (newSize == seqSize)
		return;

	if (newSize < seqSize)
	{
		content_.resize(newSize);
		return;
	}

	// Add gaps up to specified size
	int gap = getAlphabet()->getGapCharacterCode();
	while (content_.size() < newSize)
		content_.push_back(gap);
}

/******************************************************************************/

void Sequence::setToSizeL(size_t newSize)
{
	// Size verification
	size_t seqSize = content_.size();
	if (newSize == seqSize)
		return;

	if (newSize < seqSize)
	{
		// We must truncate sequence from the left.
		// This is a very unefficient method!
		content_.erase(content_.begin(), content_.begin() + static_cast<ptrdiff_t>(seqSize - newSize));
		return;
	}

	// Add gaps up to specified size
	int gap = getAlphabet()->getGapCharacterCode();
	content_.insert(content_.begin(), newSize - seqSize, gap);
}

/******************************************************************************/

void Sequence::append(const SequenceInterface& seq)
{
	if (seq.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
		throw AlphabetMismatchException("Sequence::append", getAlphabet(), seq.getAlphabet());
	// Check list for incorrect characters
	for (auto i : seq.getContent())
	{
		content_.push_back(i);
	}
}

void Sequence::append(const std::vector<int>& content)
{
	// Check list for incorrect characters
	for (auto i : content)
	{
		if (!getAlphabet()->isIntInAlphabet(i))
			throw BadIntException(i, "Sequence::append", getAlphabet());
	}
	// Sequence is valid:
	for (auto i : content)
	{
		content_.push_back(i);
	}
}

void Sequence::append(const std::vector<std::string>& content)
{
	// Check list for incorrect characters
	for (auto i : content)
	{
		if (!getAlphabet()->isCharInAlphabet(i))
			throw BadCharException(i, "Sequence::append", getAlphabet());
	}

	// BasicSequence is valid:
	for (auto i : content)
	{
		content_.push_back(getAlphabet()->charToInt(i));
	}
}

void Sequence::append(const std::string& content)
{
	auto alphaPtr = getAlphabet();
	append(StringSequenceTools::codeSequence(content, alphaPtr));
}

/******************************************************************************/
