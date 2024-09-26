// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_GENETICCODE_ECHINODERMMITOCHONDRIALGENETICCODE_H
#define BPP_SEQ_GENETICCODE_ECHINODERMMITOCHONDRIALGENETICCODE_H


#include "../Alphabet/NucleicAlphabet.h"
#include "GeneticCode.h"

namespace bpp
{
/**
 * @brief This class implements the Echinoderm and Faltworms
 * Mitochondrial genetic code as describe on the NCBI website:
 * http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi?mode=t#SG9
 */
class EchinodermMitochondrialGeneticCode :
	public virtual GeneticCode
{
public:
EchinodermMitochondrialGeneticCode(std::shared_ptr<const NucleicAlphabet> alphabet) :
	GeneticCode(alphabet)
{
	init_();
}

virtual ~EchinodermMitochondrialGeneticCode() {
}

virtual EchinodermMitochondrialGeneticCode* clone() const
{
	return new EchinodermMitochondrialGeneticCode(*this);
}

public:
size_t getNumberOfStopCodons() const {
	return 2;
}

std::vector<int> getStopCodonsAsInt() const
{
	return std::vector<int>({48, 50});
}

std::vector<std::string> getStopCodonsAsChar() const
{
	return std::vector<std::string>({"TAA", "TAG"});
}

bool isStop(int state) const
{
	// Test:
	codonAlphabet_->intToChar(state); // throw exception if invalid state!
	return state == 48 || state == 50;
}

bool isStop(const std::string& state) const
{
	int i = codonAlphabet_->charToInt(state);
	return i == 48 || i == 50;
}

bool isAltStart(int state) const
{
	// Test:
	codonAlphabet_->intToChar(state); // throw exception if invalid state!
	return state == 46;
}

bool isAltStart(const std::string& state) const
{
	int i = codonAlphabet_->charToInt(state);
	return i == 46;
}

private:
void init_();
};
} // end of namespace bpp.
#endif // BPP_SEQ_GENETICCODE_ECHINODERMMITOCHONDRIALGENETICCODE_H
