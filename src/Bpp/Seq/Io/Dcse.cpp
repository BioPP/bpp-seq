// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>

#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "../Sequence.h"
#include "AbstractIAlignment.h"
#include "Dcse.h"

using namespace bpp;
using namespace std;

void DCSE::appendAlignmentFromStream(istream& input, SequenceContainerInterface& sc) const
{
	// Checking the existence of specified file
	if (!input)
	{
		throw IOException ("DCSE::read : fail to open file");
	}

	// Initialization
	auto alphaPtr = sc.getAlphabet();
	string line, name, sequence = "";

	line = FileTools::getNextLine(input); // Copy current line in temporary string
	// StringTokenizer st(line);
	// st.nextToken();
	// First line ignored for now!
	// int n1 = TextTools::toInt(st.nextToken());
	// int n2 = TextTools::toInt(st.nextToken());
	// int nbSites = n2 - n1
	// cout << nbSpecies << " species and " << nbSites << " sites." << endl;

	// Main loop : for all file lines
	while (!input.eof())
	{
		line = FileTools::getNextLine(input); // Copy current line in temporary string
		if (line == "")
			break;
		string::size_type endOfSeq = line.find("     ");
		if (endOfSeq == line.npos)
			break;
		sequence = string(line.begin(), line.begin() + static_cast<ptrdiff_t>(endOfSeq));
		sequence = TextTools::removeWhiteSpaces(sequence);
		sequence = TextTools::removeChar(sequence, '{');
		sequence = TextTools::removeChar(sequence, '}');
		sequence = TextTools::removeChar(sequence, '[');
		sequence = TextTools::removeChar(sequence, ']');
		sequence = TextTools::removeChar(sequence, '(');
		sequence = TextTools::removeChar(sequence, ')');
		sequence = TextTools::removeChar(sequence, '^');
		name     = string(line.begin() + static_cast<ptrdiff_t>(endOfSeq + 1), line.end()),
		name     = TextTools::removeFirstWhiteSpaces(name);
		if (name.find("Helix numbering") == name.npos
		    && name.find("mask") == name.npos)
		{
			auto seqPtr = make_unique<Sequence>(name, sequence, alphaPtr);
			sc.addSequence(name, seqPtr);
		}
	}
}

const string DCSE::getFormatName() const {
	return "DCSE";
}

const string DCSE::getFormatDescription() const {
	return "RNA structure format";
}
