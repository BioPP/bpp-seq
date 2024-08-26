// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>

#include "AAIndex1Entry.h"

using namespace bpp;
using namespace std;

AAIndex1Entry::AAIndex1Entry(std::istream& input) :
	ProteicAlphabetIndex1(),
	property_(20)
{
	// Parse entry:
	string line;
	bool ok = false;
	do
	{
		line = FileTools::getNextLine(input);
		if (line[0] == 'I')
		{
			string line1 = FileTools::getNextLine(input);
			string line2 = FileTools::getNextLine(input);
			StringTokenizer st1(line1, " ");
			StringTokenizer st2(line2, " ");
			if (st1.numberOfRemainingTokens() != 10 || st1.numberOfRemainingTokens() != 10)
				break;
			// Amino acids are in the same order in the AAIndex1 database than in the ProteicAlphabet class:
			for (unsigned int i = 0; i < 10; i++)
			{
				property_[i] = TextTools::toDouble(st1.nextToken());
			}
			for (unsigned int i = 10; i < 20; i++)
			{
				property_[i] = TextTools::toDouble(st2.nextToken());
			}
			// Jump to next entry...
			FileTools::getNextLine(input);
			ok = true;
		}
	}
	while (!ok);
	if (!ok)
		throw IOException("AAIndex1Entry: invalid AAIndex1 entry.");
}
