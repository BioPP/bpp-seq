// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AAINDEX1ENTRY_H
#define BPP_SEQ_ALPHABETINDEX_AAINDEX1ENTRY_H


#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief Create a AlphabetIndex1 object from an AAIndex2 entry.
 */
class AAIndex1Entry :
	public ProteicAlphabetIndex1
{
private:
std::vector<double> property_;

public:
/**
 * @brief Create a new AAIndex1Entry from an input stream.
 *
 * @param input The input stream to use.
 * @throw IOException if the stream content does not follow the AAIndex1 database entry format.
 */
AAIndex1Entry(std::istream& input);

AAIndex1Entry(const AAIndex1Entry& index) :
	ProteicAlphabetIndex1(index),
	property_(index.property_)
{
}

AAIndex1Entry& operator=(const AAIndex1Entry& index)
{
	ProteicAlphabetIndex1::operator=(*this);
	property_ = index.property_;
	return *this;
}

virtual ~AAIndex1Entry() {
}

AAIndex1Entry* clone() const override {
	return new AAIndex1Entry(*this);
}

public:
double getIndex(int state) const override
{
	return property_[getAlphabet()->getStateIndex(state) - 1];
}

double getIndex(const std::string& state) const override
{
	return property_[getAlphabet()->getStateIndex(state) - 1];
}

const std::vector<double>& indexVector() const override {
	return property_;
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AAINDEX1ENTRY_H
