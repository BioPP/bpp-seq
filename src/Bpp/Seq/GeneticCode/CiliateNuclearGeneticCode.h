// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_GENETICCODE_CILIATENUCLEARGENETICCODE_H
#define BPP_SEQ_GENETICCODE_CILIATENUCLEARGENETICCODE_H


#include "../Alphabet/NucleicAlphabet.h"
#include "GeneticCode.h"

namespace bpp
{
/**
 * @brief This class implements the mold, protozoan, and coelenterate mitochondrial code
 * and the Mycoplasma/Spiroplasma code as describe on the NCBI website:
 * http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi?mode=t#SG4
 */
class CiliateNuclearGeneticCode :
	public virtual GeneticCode
{
public:
CiliateNuclearGeneticCode(std::shared_ptr<const NucleicAlphabet> alphabet) :
	GeneticCode(alphabet)
{
	init_();
}

virtual ~CiliateNuclearGeneticCode() {
}

virtual CiliateNuclearGeneticCode* clone() const
{
	return new CiliateNuclearGeneticCode(*this);
}

public:
size_t getNumberOfStopCodons() const {
	return 1;
}

std::vector<int> getStopCodonsAsInt() const
{
	return std::vector<int>({56});
}

std::vector<std::string> getStopCodonsAsChar() const
{
	return std::vector<std::string>({"TGA"});
}

bool isStop(int state) const
{
	// Test:
	codonAlphabet_->intToChar(state); // throw exception if invalid state!
	return state == 56;
}

bool isStop(const std::string& state) const
{
	return codonAlphabet_->charToInt(state) == 56;
}

bool isAltStart(int state) const
{
	// Test:
	codonAlphabet_->intToChar(state); // throw exception if invalid state!
	return state == 62 || state == 30;
}

bool isAltStart(const std::string& state) const
{
	int i = codonAlphabet_->charToInt(state);
	return i == 62 || i == 30;
}

private:
void init_();
};
} // end of namespace bpp.
#endif // BPP_SEQ_GENETICCODE_CILIATENUCLEARGENETICCODE_H
