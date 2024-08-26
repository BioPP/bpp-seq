// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Alphabet/AlphabetTools.h"
#include "AAIndex2Entry.h"

using namespace bpp;
using namespace std;

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/TextTools.h>
#include <Bpp/Text/StringTokenizer.h>

AAIndex2Entry::AAIndex2Entry(std::istream& input, bool sym) :
	ProteicAlphabetIndex2(),
	property_(20, 20),
	sym_(sym)
{
	// Parse entry:
	string line;
	bool ok = false;
	bool diag = false;
	do
	{
		line = FileTools::getNextLine(input);
		if (line[0] == 'M')
		{
			for (size_t i = 0; i < 20; ++i)
			{
				line = FileTools::getNextLine(input);
				StringTokenizer st1(line, " ");
				if (i == 0 && st1.numberOfRemainingTokens() == 1)
				{
					// Lower triangle only:
					diag = true;
				}
				// Amino acids are in the same order in the AAIndex1 database than in the ProteicAlphabet class:
				if (diag)
				{
					if (st1.numberOfRemainingTokens() != i + 1)
						break;
					for (size_t j = 0; j <= i; ++j)
					{
						property_(i, j) = TextTools::toDouble(st1.nextToken());
					}
				}
				else
				{
					if (st1.numberOfRemainingTokens() != 20)
						break;
					for (size_t j = 0; j < 20; ++j)
					{
						property_(i, j) = TextTools::toDouble(st1.nextToken());
					}
				}
			}
			// Jump to next entry...
			FileTools::getNextLine(input);
			ok = true;
		}
	}
	while (!ok);
	if (!ok)
		throw IOException("AAIndex2Entry: invalid AAIndex2 entry.");
	if (!diag)
		sym_ = false;
}
