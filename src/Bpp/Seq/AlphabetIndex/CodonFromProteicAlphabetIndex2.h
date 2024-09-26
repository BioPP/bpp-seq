// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_CODONFROMPROTEICALPHABETINDEX2_H
#define BPP_SEQ_ALPHABETINDEX_CODONFROMPROTEICALPHABETINDEX2_H

#include <Bpp/Numeric/Matrix/Matrix.h>

#include "../Alphabet/CodonAlphabet.h"
#include "../GeneticCode/GeneticCode.h"
#include "AlphabetIndex2.h"

namespace bpp
{
/*
 * @brief AlphabetIndex2 for codon based on ProteicAlphabetIndex2.
 *
 */

class CodonFromProteicAlphabetIndex2 :
	public AlphabetIndex2
{
private:
std::shared_ptr<const CodonAlphabet> alpha_;
std::shared_ptr<const GeneticCode> gencode_;

LinearMatrix<double> vIndex_;

bool isSymmetric_;

public:
CodonFromProteicAlphabetIndex2(std::shared_ptr<const GeneticCode> gencode, std::shared_ptr<const AlphabetIndex2> protalphindex) :
	AlphabetIndex2(),
	alpha_(AlphabetTools::DNA_CODON_ALPHABET),
	gencode_(gencode),
	vIndex_(64, 64),
	isSymmetric_(protalphindex->isSymmetric())
{
	if (!AlphabetTools::isProteicAlphabet(protalphindex->alphabet()))
		throw Exception("CodonFromProteicAlphabetIndex2: Not a Proteic Alphabet for CodonAlphabetIndex2.");
	fillIndex_(protalphindex);
}

CodonFromProteicAlphabetIndex2(const CodonFromProteicAlphabetIndex2& cfp) :
	alpha_(cfp.alpha_),
	gencode_(cfp.gencode_),
	vIndex_(cfp.vIndex_),
	isSymmetric_(cfp.isSymmetric_)
{
}

CodonFromProteicAlphabetIndex2& operator=(const CodonFromProteicAlphabetIndex2& cfp)
{
	alpha_ = cfp.alpha_;
	gencode_ = cfp.gencode_;
	vIndex_ = cfp.vIndex_;
	isSymmetric_ = cfp.isSymmetric_;

	return *this;
}

virtual ~CodonFromProteicAlphabetIndex2() {
}

CodonFromProteicAlphabetIndex2* clone() const override {
	return new CodonFromProteicAlphabetIndex2(*this);
}

public:
double getIndex(int state1, int state2) const override
{
	return vIndex_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
}

double getIndex(const std::string& state1, const std::string& state2) const override
{
	return vIndex_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
}

std::shared_ptr<const Alphabet> getAlphabet() const override
{
	return alpha_;
}

const Alphabet& alphabet() const override
{
	return *alpha_;
}

const Matrix<double>& getIndexMatrix() const override
{
	return vIndex_;
}

bool isSymmetric() const override
{
	return isSymmetric_;
}

private:
void fillIndex_(std::shared_ptr<const AlphabetIndex2>& protAlphIndex_)
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			vIndex_(getAlphabet()->getStateIndex(i) - 1, getAlphabet()->getStateIndex(j) - 1) =
				gencode_->isStop(i) ? 0 :
				gencode_->isStop(j) ? 0 :
				protAlphIndex_->getIndex(gencode_->translate(i), gencode_->translate(j));
		}
	}
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_CODONFROMPROTEICALPHABETINDEX2_H
