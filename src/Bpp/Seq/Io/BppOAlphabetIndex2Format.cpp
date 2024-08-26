// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/App/ApplicationTools.h>
#include <Bpp/Text/KeyvalTools.h>
#include <memory>
#include <string>

#include "../Alphabet/AlphabetTools.h"
#include "../AlphabetIndex/AAIndex2Entry.h"
#include "../AlphabetIndex/AlphabetIndex1.h"
#include "../AlphabetIndex/BLOSUM50.h"
#include "../AlphabetIndex/CodonFromProteicAlphabetIndex2.h"
#include "../AlphabetIndex/GranthamAAChemicalDistance.h"
#include "../AlphabetIndex/MiyataAAChemicalDistance.h"
#include "../AlphabetIndex/SimpleIndexDistance.h"
#include "BppOAlphabetIndex1Format.h"
#include "BppOAlphabetIndex2Format.h"

using namespace bpp;
using namespace std;

unique_ptr<AlphabetIndex2> BppOAlphabetIndex2Format::read(const std::string& description)
{
	if (description != "None")
	{
		string name;
		map<string, string> args;
		KeyvalTools::parseProcedure(description, name, args);
		if (verbose_)
			ApplicationTools::displayResult(message_, description);

		if (AlphabetTools::isCodonAlphabet(*alphabet_))
		{
			if (!gencode_)
				throw Exception("BppOAlphabetIndex2Format::read. Missing genetic code for codon alphabet.");

			auto alphaPtr = gencode_->getTargetAlphabet();
			BppOAlphabetIndex2Format reader2(alphaPtr, message_, false);

			shared_ptr<AlphabetIndex2> ai2(reader2.read(description));
			if (!AlphabetTools::isProteicAlphabet(ai2->alphabet()))
				throw Exception("BppOAlphabetIndex2Format::read. Not a Proteic Alphabet for CodonAlphabetIndex2.");

			return make_unique<CodonFromProteicAlphabetIndex2>(gencode_, ai2);
		}

		// Currently, only protein indices are supported:
		if (!AlphabetTools::isProteicAlphabet(*alphabet_))
			throw Exception("BppOAlphabetIndex2Format::read. This index is only supported with a protein alphabet.");

		if (name == "Blosum50")
		{
			return make_unique<BLOSUM50>();
		}
		else if (name == "Grantham")
		{
			bool sym = ApplicationTools::getBooleanParameter("symmetrical", args, true, "", true, 1);
			auto M = make_unique<GranthamAAChemicalDistance>();
			M->setSymmetric(sym);
			if (!sym)
				M->setPC1Sign(true);
			return M;
		}
		else if (name == "Miyata")
		{
			bool sym = ApplicationTools::getBooleanParameter("symmetrical", args, true, "", true, 1);
			auto M = make_unique<MiyataAAChemicalDistance>();
			M->setSymmetric(sym);
			return M;
		}
		else if (name == "Diff")
		{
			string index1Desc = ApplicationTools::getStringParameter("index1", args, "None", "", true, 1);
			bool sym = ApplicationTools::getBooleanParameter("symmetrical", args, true, "", true);
			BppOAlphabetIndex1Format index1Reader(alphabet_, "", false);
			auto index1 = index1Reader.read(index1Desc);
			if (index1)
			{
				auto M = make_unique<SimpleIndexDistance>(std::move(index1));
				M->setSymmetric(sym);
				return M;
			}
			else
			{
				throw Exception("BppOAlphabetIndex2Format::read. Diff: index1 should be provided.");
			}
		}
		else if (name == "User")
		{
			bool sym = ApplicationTools::getBooleanParameter("symmetrical", args, true, "", true, 1);
			string aax2FilePath = ApplicationTools::getAFilePath("file", args, true, true, "", false);
			ifstream aax2File(aax2FilePath.c_str(), ios::in);
			auto M = make_unique<AAIndex2Entry>(aax2File, sym);
			aax2File.close();
			return M;
		}
		else
		{
			throw Exception("Invalid index2 '" + name + "'.");
		}
	}
	else
	{
		return 0;
	}
}
