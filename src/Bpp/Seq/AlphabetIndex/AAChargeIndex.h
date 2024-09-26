// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AACHARGEINDEX_H
#define BPP_SEQ_ALPHABETINDEX_AACHARGEINDEX_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Charge of each amino acid.
 *
 * @code
 * Database: AAindex1
 * Entry: FAUJ880111
 *
 * H FAUJ880111
 * D Positive charge (Fauchere et al., 1988)
 * R LIT:1414114 PMID:3209351
 * A Fauchere, J.L., Charton, M., Kier, L.B., Verloop, A. and Pliska, V.
 * T Amino acid side chain parameters for correlation studies in biology and
 *   pharmacology
 * J Int. J. Peptide Protein Res. 32, 269-278 (1988)
 * C ZIMJ680104    0.813
 * I    A/L     R/K     N/M     D/F     C/P     Q/S     E/T     G/W     H/Y     I/V
 *      0.      1.      0.      0.      0.      0.      0.      0.      1.      0.
 *      0.      1.      0.      0.      0.      0.      0.      0.      0.      0.
 * //
 * @endcode
 *
 * @code
 * Database: AAindex1
 * Entry: FAUJ880111
 *
 * H FAUJ880112
 * D Negative charge (Fauchere et al., 1988)
 * R LIT:1414114 PMID:3209351
 * A Fauchere, J.L., Charton, M., Kier, L.B., Verloop, A. and Pliska, V.
 * T Amino acid side chain parameters for correlation studies in biology and
 *   pharmacology
 * J Int. J. Peptide Protein Res. 32, 269-278 (1988)
 * C RICJ880106    0.849
 * I    A/L     R/K     N/M     D/F     C/P     Q/S     E/T     G/W     H/Y     I/V
 *       0.      0.      0.      1.      0.      0.      1.      0.      0.      0.
 *       0.      0.      0.      0.      0.      0.      0.      0.      0.      0.
 * //
 * @endcode
 *
 * Hence, combining the two:
 * @code
 * I    A/L     R/K     N/M     D/F     C/P     Q/S     E/T     G/W     H/Y     I/V
 *       0.      1.      0.     -1.      0.      0.     -1.      0.      1.      0.
 *       0.      1.      0.      0.      0.      0.      0.      0.      0.      0.
 * @endcode
 */
class AAChargeIndex :
	public ProteicAlphabetIndex1
{
private:
std::vector<double> charge_;

public:
AAChargeIndex() :
	ProteicAlphabetIndex1(),
	charge_()
{
	charge_.resize(20);
	charge_[ 0] =  0.;// A
	charge_[ 1] =  1.;// R
	charge_[ 2] =  0.;// N
	charge_[ 3] = -1.; // D
	charge_[ 4] =  0.;// C
	charge_[ 5] =  0.;// Q
	charge_[ 6] = -1.; // E
	charge_[ 7] =  0.;// G
	charge_[ 8] =  1.;// H
	charge_[ 9] =  0.;// I
	charge_[10] =  0.;// L
	charge_[11] =  1.;// K
	charge_[12] =  0.;// M
	charge_[13] =  0.;// F
	charge_[14] =  0.;// P
	charge_[15] =  0.;// S
	charge_[16] =  0.;// T
	charge_[17] =  0.;// W
	charge_[18] =  0.;// Y
	charge_[19] =  0.;// V
}

virtual ~AAChargeIndex() {
}

AAChargeIndex* clone() const override {
	return new AAChargeIndex();
}

public:
double getIndex(int state) const override
{
	return charge_[getAlphabet()->getStateIndex(state) - 1];
}

double getIndex(const std::string& state) const override
{
	return charge_[getAlphabet()->getStateIndex(state) - 1];
}

const std::vector<double>& indexVector() const override {
	return charge_;
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AACHARGEINDEX_H
