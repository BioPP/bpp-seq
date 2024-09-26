// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>

#include "PhredPhd.h"

using namespace bpp;

/******************************************************************************/

// PhredPhd::PhredPhd() {}

/******************************************************************************/

bool PhredPhd::nextSequence(std::istream& input, Sequence& seq) const
{
	std::vector<size_t> pos;
	return nextSequence(input, seq, pos);
}

/******************************************************************************/

bool PhredPhd::nextSequence(std::istream& input, Sequence& seq, std::vector<size_t>& pos) const
{
	if (!input)
	{
		throw IOException ("PhredPhd::read: fail to open stream");
	}

	bool flag = false;
	std::string name, sequence = ""; // Initialization
	std::vector<int> q;
	std::vector<size_t> p;

	flag = parseFile_(input, name, sequence, q, p);
	// Sequence creation
	if (name == "")
		throw Exception("PhredPhd::read: sequence without name!");
	seq.setName(name);
	seq.setContent(sequence);
	try
	{
		SequenceWithQuality& sq = dynamic_cast<SequenceWithQuality&>(seq);
		sq.setQualities(q);
	}
	catch (...)
	{}
	return flag;
}

/******************************************************************************/

bool PhredPhd::parseFile_(std::istream& input, std::string& name, std::string& sequence, std::vector<int>& qual, std::vector<size_t>& pos) const
{
	bool readSeqFlag = false;
	std::string temp;
	// Read sequence info
	// Main loop : for all lines
	while (!input.eof())
	{
		std::getline(input, temp, '\n'); // Copy current line in temporary string
		StringTokenizer st(temp, " ");
		if (st.hasMoreToken())
		{
			if (st.getToken(0) == "BEGIN_SEQUENCE")
			{
				name = st.getToken(1);
			}
			std::string flag = st.getToken(0);
			while (flag != "END_SEQUENCE" && !input.eof())
			{
				getline(input, temp, '\n');
				StringTokenizer st2(temp, " ");
				if (st2.hasMoreToken())
				{
					flag = st2.getToken(0);
				}
				if (flag == "BEGIN_DNA")
				{
					readSeqFlag = parseDNA_(input, sequence, qual, pos);
					break; // End the whole loop after parsing DNA
				}
			}
		}
	}
	return readSeqFlag;
}

/******************************************************************************/

bool PhredPhd::parseDNA_(std::istream& input, std::string& sequence, std::vector<int>& qual, std::vector<size_t>& pos) const
{
	bool readSeqFlag = false;
	std::string line_buffer;
	std::string flag;
	sequence.clear();
	qual.clear();
	pos.clear();
	while (flag != "END_DNA" && !input.eof())
	{
		std::getline(input, line_buffer, '\n');
		StringTokenizer st(line_buffer, " ");
		if (st.hasMoreToken())
		{
			flag = TextTools::toUpper(st.getToken(0));
			if (st.numberOfRemainingTokens() == 3)
			{
				sequence += flag;
				qual.push_back(TextTools::toInt(st.getToken(1)));
				pos.push_back(TextTools::to<size_t>(st.getToken(2)));
				readSeqFlag = true;
			}
		}
	}
	return readSeqFlag;
}

/******************************************************************************/
