// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_KD_AAHYDROPATHYINDEX_H
#define BPP_SEQ_ALPHABETINDEX_KD_AAHYDROPATHYINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Hydropathy index defined in Kyte & Doolitle (1982).
 *
 * Kyte, J. and Doolittle, R.F., J. Mol. Biol., 157, 105-132 (1982).
 *
 */

class KD_AAHydropathyIndex :
	public virtual ProteicAlphabetIndex1
{
private:
std::vector<double> hydropathy_;

public:
KD_AAHydropathyIndex() :
	ProteicAlphabetIndex1(),
	hydropathy_()
{
	hydropathy_.resize(20);
	hydropathy_[ 0] = 1.8; // A
	hydropathy_[ 1] = -4.5; // R
	hydropathy_[ 2] = -3.5; // N
	hydropathy_[ 3] = -3.5; // D
	hydropathy_[ 4] = 2.5; // C
	hydropathy_[ 5] = -3.5; // Q
	hydropathy_[ 6] = -3.5; // E
	hydropathy_[ 7] = -0.4; // G
	hydropathy_[ 8] = -3.2; // H
	hydropathy_[ 9] = 4.5; // I
	hydropathy_[10] = 3.8; // L
	hydropathy_[11] = -3.9; // K
	hydropathy_[12] = 1.9; // M
	hydropathy_[13] = 2.8; // F
	hydropathy_[14] = -1.6; // P
	hydropathy_[15] = -0.8; // S
	hydropathy_[16] = -0.7; // T
	hydropathy_[17] = -0.9; // W
	hydropathy_[18] = -1.3; // Y
	hydropathy_[19] = 4.2; // V
}

virtual ~KD_AAHydropathyIndex() {
}

KD_AAHydropathyIndex* clone() const override {
	return new KD_AAHydropathyIndex();
}

public:
double getIndex(int state) const override
{
	return hydropathy_[getAlphabet()->getStateIndex(state) - 1];
}

double getIndex(const std::string& state) const override
{
	return hydropathy_[getAlphabet()->getStateIndex(state) - 1];
}

const std::vector<double>& indexVector() const override {
	return hydropathy_;
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_KD_AAHYDROPATHYINDEX_H
