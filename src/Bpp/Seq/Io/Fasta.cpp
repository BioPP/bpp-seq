// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>
#include <fstream>

#include "../StringSequenceTools.h"
#include "Fasta.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

bool Fasta::nextSequence(istream& input, Sequence& seq) const
{
	if (!input)
		throw IOException("Fasta::nextSequence: can't read from istream input");
	string seqname = "";
	string content = "";
	Comments seqcmts;
	short seqcpt = 0;
	string linebuffer = "";
	char c;
	while (!input.eof())
	{
		c = static_cast<char>(input.peek());
		if (input.eof())
			c = '\n';

		// Sequence beginning detection
		if (c == '>')
		{
			// Stop if find a new sequence
			if (seqcpt++)
				break;
		}
		getline(input, linebuffer);
		if (c == '>')
		{
			// Get the sequence name line
			seqname = string(linebuffer.begin() + 1, linebuffer.end());
		}
		if (c != '>' && !TextTools::isWhiteSpaceCharacter(c))
		{
			// Sequence content
			content += TextTools::toUpper(TextTools::removeWhiteSpaces(linebuffer));
		}
	}

	seqname = TextTools::removeWhiteSpaces(seqname);

	bool res = (!input.eof());
	// Sequence name and comments isolation
	if (strictNames_ || extended_)
	{
		size_t pos = seqname.find_first_of(" \t\n");
		string seqcmt;
		if (pos != string::npos)
		{
			seqcmt = seqname.substr(pos + 1);
			seqname = seqname.substr(0, pos);
		}
		if (extended_)
		{
			StringTokenizer st(seqcmt, " \\", true, false);
			while (st.hasMoreToken())
			{
				seqcmts.push_back(st.nextToken());
			}
		}
		else
		{
			seqcmts.push_back(seqcmt);
		}
		seq.setComments(seqcmts);
	}
	seq.setName(seqname);
	seq.setContent(content);
	return res;
}

/******************************************************************************/

void Fasta::writeSequence(ostream& output, const Sequence& seq) const
{
	if (!output)
		throw IOException("Fasta::writeSequence: can't write to ostream output");
	// Sequence name
	output << ">" << seq.getName();
	// Sequence comments
	if (extended_)
	{
		for (unsigned int i = 0; i < seq.getComments().size(); i++)
		{
			output << " \\" << seq.getComments()[i];
		}
	}
	output << endl;
	// Sequence content
	string buffer; // use a buffer to format sequence with states > 1 char
	for (size_t i = 0; i < seq.size(); ++i)
	{
		buffer += seq.getChar(i);
		if (buffer.size() >= charsByLine_)
		{
			output << string(buffer.begin(), buffer.begin() + charsByLine_) << endl;
			buffer.erase(0, charsByLine_);
		}
	}
	output << string(buffer.begin(), buffer.end()) << endl;
}

/******************************************************************************/

void Fasta::appendSequencesFromStream(istream& input, SequenceContainerInterface& vsc) const
{
	if (!input)
		throw IOException("Fasta::appendFromStream: can't read from istream input");
	char c = '\n';
	char last_c;
	bool header = false;
	bool hasSeq = true;
	string line = "";
	Comments cmts;
	while (!input.eof() && hasSeq)
	{
		last_c = c;
		input.get(c);
		// Header detection
		if (extended_ && c == '#')
		{
			header = true;
			continue;
		}
		// Header end detection
		if (c == '\n')
		{
			if (extended_ && header)
			{
				if (line[0] == '\\')
				{
					line.erase(line.begin());
					cmts.push_back(line);
				}
				line = "";
				header = false;
			}
			continue;
		}
		// Header capture
		if (header)
		{
			line.append(1, c);
		}
		// Sequence detection
		if (c == '>' && last_c == '\n')
		{
			input.putback(c);
			c = last_c;
			auto alphaPtr = vsc.getAlphabet();
			auto tmpseq = make_unique<Sequence>("", "", alphaPtr);
			hasSeq = nextSequence(input, *tmpseq);
			vsc.addSequence(tmpseq->getName(), tmpseq);
		}
	}
	if (extended_ && cmts.size())
	{
		vsc.setComments(cmts);
	}
}

/******************************************************************************/

void Fasta::writeSequences(ostream& output, const SequenceContainerInterface& sc) const
{
	if (!output)
		throw IOException("Fasta::write: can't write to ostream output");

	if (extended_)
	{
		// Loop for all general comments
		for (size_t i = 0; i < sc.getComments().size(); ++i)
		{
			output << "#\\" << sc.getComments()[i] << endl;
		}
		output << endl;
	}

	// Main loop : for all sequences in vector container
	vector<string> names = sc.getSequenceNames();
	for (size_t i = 0; i < names.size(); ++i)
	{
		writeSequence(output, sc.sequence(names[i]));
	}
}

/******************************************************************************/

// FileIndex class

void Fasta::FileIndex::build(const std::string& path, const bool strictSequenceNames)
{
	// open the file
	std::ifstream f_in(path.c_str());
	// get the size of the file
	f_in.seekg(0, std::ios::end);
	fileSize_ = f_in.tellg();
	// feed the map
	f_in.seekg(0, std::ios::beg);
	streampos pos = f_in.tellg();
	char ch;
	std::string seq_id = "";
	while (f_in.get(ch))
	{
		if (ch == '>')
		{
			pos = static_cast<int>(f_in.tellg()) - 1;
			std::getline(f_in, seq_id);
			if (strictSequenceNames)
			{
				seq_id = seq_id.substr(0, seq_id.find_first_of(" \t\n"));
			}
			index_[seq_id] = pos;
		}
	}
	f_in.close();
}

streampos Fasta::FileIndex::getSequencePosition(const std::string& id) const
{
	std::map<std::string, streampos>::const_iterator it = index_.find(id);
	if (it != index_.end())
	{
		return it->second;
	}
	throw Exception("Sequence not found: " + id);
}

void Fasta::FileIndex::read(const std::string& path)
{
	std::ifstream f_in(path.c_str());
	std::string line_buffer = "";
	while (!f_in.eof())
	{
		std::getline(f_in, line_buffer);
		if (bpp::TextTools::isEmpty(bpp::TextTools::removeSurroundingWhiteSpaces(line_buffer)))
		{
			continue;
		}
		bpp::StringTokenizer tk(line_buffer, "\t");
		index_[tk.getToken(0)] = bpp::TextTools::toInt(tk.getToken(1));
	}
	f_in.close();
}

void Fasta::FileIndex::write(const std::string& path)
{
	std::ofstream f_out(path.c_str());
	for (std::map<std::string, streampos>::const_iterator it = index_.begin(); it != index_.end(); ++it)
	{
		f_out << it->first << "\t" << bpp::TextTools::toString(it->second) << std::endl;
	}
	f_out.close();
}

void Fasta::FileIndex::getSequence(const std::string& seqid, Sequence& seq, const std::string& path) const
{
	getSequence(seqid, seq, path, false);
}

void Fasta::FileIndex::getSequence(const std::string& seqid, Sequence& seq, const std::string& path, const bool strictSequenceNames) const
{
	Fasta fs(60);
	fs.strictNames(strictSequenceNames);
	streampos seq_pos = this->getSequencePosition(seqid);
	std::ifstream fasta(path.c_str());
	fasta.seekg(seq_pos);
	fs.nextSequence(fasta, seq);
	fasta.close();
}

/******************************************************************************/
