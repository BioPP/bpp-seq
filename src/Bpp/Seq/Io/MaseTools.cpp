// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>
#include <iostream>

#include "../Container/AlignedSequenceContainer.h"
#include "../Container/SequenceContainerTools.h"
#include "../Container/VectorSequenceContainer.h"
#include "MaseTools.h"

using namespace std;
using namespace bpp;

SiteSelection MaseTools::getSiteSet(const Comments& maseFileHeader, const string& setName)
{
	SiteSelection selection;
	for (size_t i = 0; i < maseFileHeader.size(); i++)
	{
		string current = maseFileHeader[i];
		string::size_type index = current.find("# of");
		if (index < current.npos)
		{
			StringTokenizer st(string(current.begin() + static_cast<ptrdiff_t>(index + 4), current.end()), " \t=;");
			st.nextToken(); // skip next word: may be 'regions' or 'segments' or else ;-)
			size_t numberOfSegments = TextTools::to<size_t>(st.nextToken());
			string name = st.unparseRemainingTokens();
			if (name == setName)
			{
				// cout << numberOfSegments << " segments found." << endl;
				// Then look for the set definition:
				i++; // next line.
				size_t counter = 0;
				while (i < maseFileHeader.size())
				{
					current = maseFileHeader[i++];
					StringTokenizer st2(current);
					// st.nextToken(); //Skip ';;'
					while (st2.hasMoreToken())
					{
						StringTokenizer st3(st2.nextToken(), ",");
						size_t begin = TextTools::to<size_t>(st3.nextToken());
						size_t end   = TextTools::to<size_t>(st3.nextToken());
						// WARNING!!! In the mase+ format, sites are numbered from 1 to nbSites,
						// Whereas in SiteContainer the index begins at 0.
						for (size_t j = begin; j <= end; j++)
						{
							selection.push_back(j - 1); // bounds included.
						}
						counter++;
						if (counter == numberOfSegments)
							return selection;
					}
				}
			}
		}
	}
	if (selection.size() == 0)
	{
		throw IOException("Site set " + setName + " has not been found in the sequence file.");
	}
	return selection;
}

/******************************************************************************/

SequenceSelection MaseTools::getSequenceSet(const Comments& maseFileHeader, const string& setName)
{
	SequenceSelection selection;
	for (size_t i = 0; i < maseFileHeader.size(); i++)
	{
		string current = maseFileHeader[i];

		string::size_type index = current.find("@ of");
		if (index < current.npos)
		{
			StringTokenizer st(string(current.begin() + static_cast<ptrdiff_t>(index + 4), current.end()), " \t=;");
			st.nextToken(); // skip next word: may be 'sequences' or else ;-)
			size_t numberOfSequences = TextTools::to<size_t>(st.nextToken());
			string name = st.unparseRemainingTokens();
			size_t counter = 0;
			if (name == setName)
			{
				// cout << numberOfSequences << " segments found." << endl;
				// Then look for the set definition:
				i++; // next line.
				while (i < maseFileHeader.size())
				{
					current = maseFileHeader[i++];
					StringTokenizer st2(current, ",");
					while (st2.hasMoreToken())
					{
						size_t seqIndex = TextTools::to<size_t>(st2.nextToken());
						// WARNING!!! In the mase+ format, sequences are numbered from 1 to nbSequences,
						// Whereas in SequenceContainer the index begins at 0.
						selection.push_back(seqIndex - 1); // bounds included.
						counter++;
						if (counter == numberOfSequences)
							return selection;
					}
				}
			}
		}
	}
	if (selection.size() == 0)
	{
		throw IOException("Sequence set " + setName + " has not been found in the sequence file.");
	}
	return selection;
}

/******************************************************************************/

map<string, size_t> MaseTools::getAvailableSiteSelections(const Comments& maseHeader)
{
	map<string, size_t> selections;
	for (size_t i = 0; i < maseHeader.size(); i++)
	{
		string current = maseHeader[i];

		string::size_type index = current.find("# of");
		if (index < current.npos)
		{
			StringTokenizer st(string(current.begin() + static_cast<ptrdiff_t>(index + 4), current.end()), " \t\n\f\r=;");
			st.nextToken(); // skip next word: may be 'sequences' or else ;-)
			size_t numberOfSegments = TextTools::to<size_t>(st.nextToken());
			string name = st.nextToken();
			while (st.hasMoreToken())
			{
				name += " " + st.nextToken();
			}
			size_t counter = 0;
			size_t nbSites = 0;
			while (i < maseHeader.size())
			{
				i++;
				current = maseHeader[i];
				StringTokenizer st2(current);
				// st.nextToken(); //Skip ';;'
				while (st2.hasMoreToken())
				{
					StringTokenizer st3(st2.nextToken(), ",");
					size_t begin = TextTools::to<size_t>(st3.nextToken());
					size_t end   = TextTools::to<size_t>(st3.nextToken());
					counter++;
					nbSites += end - begin + 1;
				}
				if (counter == numberOfSegments)
				{
					selections[name] = nbSites;
					break;
				}
			}
		}
	}
	return selections;
}

/******************************************************************************/

map<string, size_t> MaseTools::getAvailableSequenceSelections(const Comments& maseHeader)
{
	map<string, size_t> selections;
	for (size_t i = 0; i < maseHeader.size(); i++)
	{
		string current = maseHeader[i];

		string::size_type index = current.find("@ of");
		if (index < current.npos)
		{
			StringTokenizer st(string(current.begin() + static_cast<ptrdiff_t>(index + 4), current.end()), " \t\n\f\r=;");
			st.nextToken(); // skip next word: may be 'sequences' or else ;-)
			size_t numberOfSequences = TextTools::fromString<size_t>(st.nextToken());
			string name = st.nextToken();
			while (st.hasMoreToken())
			{
				name += st.nextToken();
			}
			selections[name] = numberOfSequences;
		}
	}
	return selections;
}

/******************************************************************************/

size_t MaseTools::getPhase(const Comments& maseFileHeader, const string& setName)
{
	size_t phase = 0;
	string::size_type index = 0;
	for (size_t i = 0; i < maseFileHeader.size(); i++)
	{
		string current = maseFileHeader[i];

		index = current.find("# of");
		if (index < current.npos)
		{
			StringTokenizer st(string(current.begin() + static_cast<ptrdiff_t>(index + 12), current.end()), " \t\n\f\r=;");
			// size_t numberOfSegments = TextTools::toInt(st.nextToken());
			// cout << "Number of regions: " << st.nextToken() << endl;
			string name;
			while (st.hasMoreToken())
			{
				name = st.nextToken();
				// cout << "Name of regions: " << name << endl;
			}
			if (name == setName)
			{
				return phase;
			}
		}

		index = current.find("/codon_start");
		if (index < current.npos)
		{
			StringTokenizer st(string(current.begin() + static_cast<ptrdiff_t>(index + 12), current.end()), " \t\n\f\r=;");
			phase = TextTools::to<size_t>(st.nextToken());
		}
	}
	throw Exception("PolymorphismSequenceContainer::getPhase: no /codon_start found, or site selection missing.");
}

/******************************************************************************/
