// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/KeyvalTools.h>
#include <memory>
#include <string>

#include "BppOSequenceReaderFormat.h"
#include "Clustal.h"
#include "Dcse.h"
#include "Fasta.h"
#include "GenBank.h"
#include "Mase.h"
#include "NexusIoSequence.h"
#include "Phylip.h"

using namespace bpp;
using namespace std;

unique_ptr<ISequence> BppOSequenceReaderFormat::read(const std::string& description)
{
	unparsedArguments_.clear();
	string format = "";
	KeyvalTools::parseProcedure(description, format, unparsedArguments_);
	unique_ptr<ISequence> iSeq;
	if (format == "Mase")
	{
		iSeq.reset(new Mase());
	}
	else if (format == "Phylip")
	{
		bool sequential = true, extended = true;
		string split = "  ";
		string order = ApplicationTools::getStringParameter("order", unparsedArguments_, "sequential", "", true, warningLevel_);
		if (order == "sequential")
			sequential = true;
		else if (order == "interleaved")
			sequential = false;
		else
			throw Exception("BppOAlignmentReaderFormat::read. Invalid argument 'order' for phylip format: " + order);

		string type = ApplicationTools::getStringParameter("type", unparsedArguments_, "extended", "", true, warningLevel_);
		if (type == "extended")
		{
			extended = true;
			split = ApplicationTools::getStringParameter("split", unparsedArguments_, "spaces", "", true, warningLevel_);
			if (split == "spaces")
				split = "  ";
			else if (split == "tab")
				split = "\t";
			else
				throw Exception("BppOAlignmentReaderFormat::read. Invalid argument 'split' for phylip format: " + split);
		}
		else if (type == "classic")
			extended = false;
		else
			throw Exception("BppOAlignmentReaderFormat::read. Invalid argument 'type' for phylip format: " + type);

		iSeq.reset(new Phylip(extended, sequential, 100, split));
	}
	else if (format == "Fasta")
	{
		bool strictNames = ApplicationTools::getBooleanParameter("strict_names", unparsedArguments_, false, "", true, warningLevel_);
		bool extended    = ApplicationTools::getBooleanParameter("extended", unparsedArguments_, false, "", true, warningLevel_);
		iSeq.reset(new Fasta(100, true, extended, strictNames));
	}
	else if (format == "Clustal")
	{
		unsigned int extraSpaces = ApplicationTools::getParameter<unsigned int>("extraSpaces", unparsedArguments_, 0, "", true, warningLevel_);
		iSeq.reset(new Clustal(true, extraSpaces));
	}
	else if (format == "Dcse")
	{
		iSeq.reset(new DCSE());
	}
	else if (format == "GenBank")
	{
		iSeq.reset(new GenBank()); // This is required to remove a strict-aliasing warning in gcc 4.4
	}
	else if (format == "Nexus")
	{
		iSeq.reset(new NexusIOSequence());
	}
	else
	{
		throw IOException("Sequence format '" + format + "' unknown.");
	}

	return iSeq;
}
