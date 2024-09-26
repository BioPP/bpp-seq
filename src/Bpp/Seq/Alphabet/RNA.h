// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_RNA_H
#define BPP_SEQ_ALPHABET_RNA_H


#include "NucleicAlphabet.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief This alphabet is used to deal with RNA sequences.
 *
 * It supports all 4 nucleotides (A, U, G and C) with their standard denomination.
 * Gaps are coded by '-', unresolved characters are coded by 'X, N, O, 0 or ?'.
 * Extensive support for generic characters (e.g. 'P', 'Y', etc.) is provided.
 */
class RNA :
	public NucleicAlphabet
{
public:
/**
 * @param exclamationMarkCountsAsGap If yes, '!' characters are replaced by gaps.
 * Otherwise, they are counted as unknown characters.
 */
RNA(bool exclamationMarkCountsAsGap = false);

RNA(const RNA& bia) : NucleicAlphabet(bia) {
}

RNA& operator=(const RNA& bia)
{
	NucleicAlphabet::operator=(bia);
	return *this;
}

RNA* clone() const
{
	return new RNA(*this);
}

virtual ~RNA() {
}

public:
bool isResolvedIn(int state1, int state2) const;
std::vector<int> getAlias(int state) const;
std::vector<std::string> getAlias(const std::string& state) const;
int getGeneric(const std::vector<int>& states) const;
std::string getGeneric(const std::vector<std::string>& states) const;
std::string getAlphabetType() const {
	return "RNA";
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_RNA_H
