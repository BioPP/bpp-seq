// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/App/ApplicationTools.h>
#include <Bpp/Text/KeyvalTools.h>
#include <memory>
#include <string>

#include "../Alphabet/AlphabetTools.h"
#include "../AlphabetIndex/AAChargeIndex.h"
#include "../AlphabetIndex/AAChenGuHuangHydrophobicityIndex.h"
#include "../AlphabetIndex/AAChouFasmanAHelixIndex.h"
#include "../AlphabetIndex/AAChouFasmanBSheetIndex.h"
#include "../AlphabetIndex/AAChouFasmanTurnIndex.h"
#include "../AlphabetIndex/AAIndex1Entry.h"
#include "../AlphabetIndex/AAMassIndex.h"
#include "../AlphabetIndex/AASEA1030Index.h"
#include "../AlphabetIndex/AASEAInf10Index.h"
#include "../AlphabetIndex/AASEASup30Index.h"
#include "../AlphabetIndex/AASurfaceIndex.h"
#include "../AlphabetIndex/AAVolumeIndex.h"
#include "../AlphabetIndex/CodonFromProteicAlphabetIndex1.h"
#include "../AlphabetIndex/GranthamAAPolarityIndex.h"
#include "../AlphabetIndex/GranthamAAVolumeIndex.h"
#include "../AlphabetIndex/KD_AAHydropathyIndex.h"
#include "../AlphabetIndex/KleinAANetChargeIndex.h"
#include "BppOAlphabetIndex1Format.h"

using namespace bpp;
using namespace std;

unique_ptr<AlphabetIndex1> BppOAlphabetIndex1Format::read(const std::string& description)
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
			BppOAlphabetIndex1Format reader1(alphaPtr, message_, false);

			shared_ptr<AlphabetIndex1> ai2(reader1.read(description));
			if (!AlphabetTools::isProteicAlphabet(ai2->alphabet()))
				throw Exception("BppOAlphabetIndex2Format::read. Not a Proteic Alphabet for CodonAlphabetIndex1.");

			return make_unique<CodonFromProteicAlphabetIndex1>(gencode_, ai2);
		}

		// Currently, only protein indices are supported:
		if (!AlphabetTools::isProteicAlphabet(*alphabet_))
			throw Exception("BppOAlphabetIndex1Format::read. This index is only supported with a protein alphabet.");
		if (name == "GranthamPolarity")
		{
			return make_unique<GranthamAAPolarityIndex>();
		}
		else if (name == "GranthamVolume")
		{
			return make_unique<GranthamAAVolumeIndex>();
		}
		else if (name == "KleinCharge")
		{
			return make_unique<KleinAANetChargeIndex>();
		}
		else if (name == "KDHydropathy")
		{
			return make_unique<KD_AAHydropathyIndex>();
		}
		else if (name == "ChouFasmanAHelix")
		{
			return make_unique<AAChouFasmanAHelixIndex>();
		}
		else if (name == "ChouFasmanBSheet")
		{
			return make_unique<AAChouFasmanBSheetIndex>();
		}
		else if (name == "ChouFasmanTurn")
		{
			return make_unique<AAChouFasmanTurnIndex>();
		}
		else if (name == "ChenGuHuangHydrophobicity")
		{
			return make_unique<AAChenGuHuangHydrophobicityIndex>();
		}
		else if (name == "Surface")
		{
			return make_unique<AASurfaceIndex>();
		}
		else if (name == "Mass")
		{
			return make_unique<AAMassIndex>();
		}
		else if (name == "Volume")
		{
			return make_unique<AAVolumeIndex>();
		}
		else if (name == "Charge")
		{
			return make_unique<AAChargeIndex>();
		}
		else if (name == "SEAMedium")
		{
			return make_unique<AASEA1030Index>();
		}
		else if (name == "SEAHigh")
		{
			return make_unique<AASEASup30Index>();
		}
		else if (name == "SEALow")
		{
			return make_unique<AASEAInf10Index>();
		}
		else if (name == "User")
		{
			string aax1FilePath = ApplicationTools::getAFilePath("file", args, true, true, "", false);
			ifstream aax1File(aax1FilePath.c_str(), ios::in);
			auto I = make_unique<AAIndex1Entry>(aax1File);
			aax1File.close();
			return I;
		}
		else
		{
			throw Exception("Invalid index1 '" + name + "'.");
		}
	}
	else
	{
		return nullptr;
	}
}
