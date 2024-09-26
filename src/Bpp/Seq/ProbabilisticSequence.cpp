// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "ProbabilisticSequence.h" // class's header file

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

/******************************************************************************/

void ProbabilisticSequence::setToSizeR(size_t newSize)
{
	// Size verification
	size_t seqSize = size();
	if (newSize == seqSize)
		return;

	if (newSize < seqSize)
	{
		deleteElements(newSize, seqSize - newSize);
		return;
	}

	// Add gaps up to specified size
	auto gap = std::vector<double>(alphabet().getSize(), 0);

	for (size_t i = 0; i < (size_t)(newSize - seqSize); i++)
	{
		addElement(gap);
	}
}

/******************************************************************************/

void ProbabilisticSequence::setToSizeL(size_t newSize)
{
	// Size verification
	size_t seqSize = size();
	if (newSize == seqSize)
		return;

	if (newSize < seqSize)
	{
		deleteElements(0, seqSize - newSize);
		return;
	}

	// Add gaps up to specified size
	auto gap = std::vector<double>(alphabet().getSize(), 0);

	for (size_t i = 0; i < (size_t)(newSize - seqSize); i++)
	{
		addElement(0, gap);
	}
}
