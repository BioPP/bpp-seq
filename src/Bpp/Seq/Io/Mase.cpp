// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../StringSequenceTools.h"
#include "Mase.h"

using namespace bpp;
using namespace std;

/****************************************************************************************/

void Mase::appendSequencesFromStream(std::istream& input, SequenceContainerInterface& vsc) const
{
	if (!input)
	{
		throw IOException ("Mase::read : fail to open file");
	}

	// Initialization
	Comments seqComments, fileComments;
	string temp, name, sequence = "";
	bool comments = false;

	// Get current general comments is VectorSequenceContainer
	fileComments = vsc.getComments();

	// Main loop : for all file lines
	while (!input.eof())
	{
		getline(input, temp, '\n'); // Copy current line in temporary string

		// If first character is ;
		if (temp[0] == ';')
		{
			// If second character is also ;
			if (temp[1] == ';')
			{
				// File comments isolation
				temp.erase(0, 2); // Characters ;; deletion
				if (temp != "")
					fileComments.push_back(temp);
			}
			else
			{
				// If a name and a sequence were founded
				if ((name != "") && (sequence != ""))
				{
					// New sequence creation, and addition in existing VectorSequenceContainer
					auto alphaPtr = vsc.getAlphabet();
					auto seqPtr = make_unique<Sequence>(name, sequence, seqComments, alphaPtr);
					vsc.addSequence(seqPtr->getName(), seqPtr);
					name = "";
					sequence = "";
					seqComments.clear();
				}

				// Sequence commentaries isolation
				temp.erase(temp.begin()); // Character ; deletion
				if (temp != "")
					seqComments.push_back(temp);
				comments = true;
			}
		}
		else
		{
			// If sequence commentaries were just isolated
			if (comments)
			{
				// Sequence name isolation
				name = temp;
				comments = false;
			}
			else
				sequence += temp; // Sequence isolation
		}
	}

	// Addition of the last sequence in file
	if ((name != "") && (sequence != ""))
	{
		auto alphaPtr = vsc.getAlphabet();
		auto seqPtr = make_unique<Sequence>(name, sequence, seqComments, alphaPtr);
		vsc.addSequence(seqPtr->getName(), seqPtr);
	}

	// Set new general comments in VectorSequenceContainer (old + new comments)
	vsc.setComments(fileComments);
}

/****************************************************************************************/

void Mase::writeSequences(ostream& output, const SequenceContainerInterface& sc) const
{
	// Checking the existence of specified file, and possibility to open it in write mode
	if (!output)
	{
		throw IOException ("Mase::write : failed to open file");
	}

	Comments comments = sc.getComments();

	// Writing all general comments in file
	if (comments.size() == 0)
	{
		output << ";;" << endl;
	}
	for (size_t i = 0; i < comments.size(); i++)
	{
		output << ";;" << comments[i] << endl;
	}

	string seq, temp = ""; // Initialization

	// Main loop : for all sequences
	for (const auto& seqKey: sc.getSequenceKeys())
	{
		comments = sc.sequence(seqKey).getComments();

		// Writing all sequence comments in file
		// If no comments are associated with current sequence, an empty commentary line will be writed
		if (comments.size() == 0)
		{
			output << ";" << endl;
		}
		else
		{
			for (size_t j = 0; j < comments.size(); j++)
			{
				output << ";" << comments[j] << endl;
			}
		}

		// Sequence name writing
		output << sc.sequence(seqKey).getName() << endl;

		// Sequence cutting to specified characters number per line
		seq = sc.sequence(seqKey).toString();
		while (seq != "")
		{
			if (seq.size() > charsByLine_)
			{
				temp = seq;
				temp.erase(temp.begin() + static_cast<ptrdiff_t>(charsByLine_), temp.end());
				output << temp  << endl;
				seq.erase(seq.begin(), seq.begin() + static_cast<ptrdiff_t>(charsByLine_));
			}
			else
			{
				output << seq << endl;
				seq = "";
			}
		}
	}
}

/****************************************************************************************/

void Mase::readHeader_(std::istream& input, MaseHeader& header) const
{
	do
	{
		// Check if the line is a header line:
		if (input.peek() == ';')
		{
			char c;
			input.get(c);
			if (input.peek() == ';')
			{
				input.get(c);
				string line = FileTools::getNextLine(input);

				// Check the type of line...

				// Site selection:
				string::size_type index = line.find("# of");
				if (index < line.npos)
				{
					StringTokenizer st(string(line.begin() + static_cast<ptrdiff_t>(index + 4), line.end()), " \t=;");
					st.nextToken(); // skip next word: may be 'regions' or 'segments' or else ;-)
					unsigned int numberOfSegments = TextTools::to<unsigned int>(st.nextToken());
					string name = st.unparseRemainingTokens();
					// Then look for the set definition:
					MultiRange<size_t> siteSelection;
					while (siteSelection.size() < numberOfSegments)
					{
						line = FileTools::getNextLine(input);
						if (line[0] != ';' || line[1] != ';')
							throw Exception("Mase::readHeader_(): corrupted file, site selection " + name + " is incomplete. Aborting.");
						line = line.substr(2);
						StringTokenizer st2(line);
						while (st2.hasMoreToken())
						{
							StringTokenizer st3(st2.nextToken(), ",");
							unsigned int begin = TextTools::to<unsigned int>(st3.nextToken());
							unsigned int end   = TextTools::to<unsigned int>(st3.nextToken());
							// WARNING!!! In the mase+ format, sites numerotation is 1-based, including, while ranges are 0-based, [a, b[:
							siteSelection.addRange(Range<size_t>(begin - 1, end));
						}
						if (siteSelection.size() > numberOfSegments)
							throw Exception("Mase::readHeader_(): incorrected file, found " + TextTools::toString(siteSelection.size()) + "segments while expected " + TextTools::toString(numberOfSegments));
					}
					header.setSiteSelection(name, siteSelection);
				}
				else
				{
					// Sequence selection:
					index = line.find("@ of");
					if (index < line.npos)
					{
						StringTokenizer st(line.substr(index + 4), " \t=;");
						st.nextToken(); // skip next word: may be 'sequences' or else ;-)
						unsigned int numberOfSequences = TextTools::to<unsigned int>(st.nextToken());
						string name = st.unparseRemainingTokens();
						// The look for the set definition:
						vector<size_t> sequenceSelection;
						while (sequenceSelection.size() < numberOfSequences)
						{
							line = FileTools::getNextLine(input);
							if (line[0] != ';' || line[1] != ';')
								throw Exception("Mase::readHeader_(): corrupted file, sequence selection " + name + " is incomplete. Aborting.");
							line = line.substr(2);
							StringTokenizer st2(line, ", ");
							while (st2.hasMoreToken())
							{
								unsigned int pos = TextTools::to<unsigned int>(st2.nextToken());
								// WARNING!!! In the mase+ format, sequence numerotation is 1-based
								sequenceSelection.push_back(pos);
							}
							if (sequenceSelection.size() > numberOfSequences)
								throw Exception("Mase::readHeader_(): incorrected file, found " + TextTools::toString(sequenceSelection.size()) + "sequences while expected " + TextTools::toString(numberOfSequences));
						}
						header.setSequenceSelection(name, sequenceSelection);
					}
					else
					{
						// Tree:
						index = line.find("$");
						if (index < line.npos)
						{
							string name = TextTools::removeSurroundingWhiteSpaces(line.substr(index + 1));
							// Here we stop if the line ends with a ";"
							string tree = "";
							do
							{
								line = FileTools::getNextLine(input);
								if (line[0] != ';' || line[1] != ';')
									throw Exception("Mase::readHeader_(): corrupted file, tree " + name + " is incomplete. Aborting.");
								line = TextTools::removeSurroundingWhiteSpaces(line.substr(2));
								tree += line;
							}
							while (!TextTools::endsWith(line, ";"));
							header.setTree(name, tree);
						}
					}
				}
			}
			else
			{
				input.putback(c);
				break;
			}
		}
	}
	while (true);
}

/****************************************************************************************/

void Mase::writeHeader_(std::ostream& output, const MaseHeader& header) const
{
	// Write trees:
	vector<string> treeNames = header.getTreeNames();
	for (size_t i = 0; i < treeNames.size(); ++i)
	{
		output << ";;$ " + treeNames[i] << endl;
		output << ";;" + header.getTree(treeNames[i]);
		output << endl;
	}

	// Write site selections:
	vector<string> siteSelectionNames = header.getSiteSelectionNames();
	for (size_t i = 0; i < siteSelectionNames.size(); ++i)
	{
		MultiRange<size_t> ranges = header.getSiteSelection(siteSelectionNames[i]);
		output << ";;Site selection " << siteSelectionNames[i] << " (" << ranges.totalLength() << " sites)" << endl;
		output << ";;# of segments=" << ranges.size() << " " << siteSelectionNames[i] << endl;
		output << ";;";
		for (size_t j = 0; j < ranges.size(); ++j)
		{
			output << " " << (ranges.getRange(j).begin() + 1) << "," << ranges.getRange(j).end();
			if ((j + 1) % 10 == 0)
				output << endl << ";;";
		}
		output << endl;
	}

	// Write sequence selections:
	vector<string> sequenceSelectionNames = header.getSequenceSelectionNames();
	for (size_t i = 0; i < sequenceSelectionNames.size(); ++i)
	{
		vector<size_t> set = header.getSequenceSelection(sequenceSelectionNames[i]);
		output << ";;@ of species=" << set.size() << " " << sequenceSelectionNames[i] << endl;
		output << ";;";
		for (unsigned int j = 0; j < set.size(); ++j)
		{
			output << " " << set[j];
			if ((j + 1) % 10 == 0)
				output << endl << ";;";
		}
		output << endl;
	}
}

/****************************************************************************************/
