// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_RNY_H
#define BPP_SEQ_ALPHABET_RNY_H

#include <string>

#include "AbstractAlphabet.h"
#include "NucleicAlphabet.h"

/**
 * @brief This alphabet is used to deal quotiented triplet RNY +
 * combinations with "-".
 *
 * More explicitly, a quotiented triplet RNY is defined on the alphabet
 *
 * \f[\{R,C,T\} x \{A,G,C,T\} x \{ A,G,Y\}\f].
 *
 * Triplet "---" is the only triplet defined as gap. Other triplets
 * with "-" are defined as unresolved.
 *
 * @author Laurent Guéguen
 */

namespace bpp
{
class RNY : public AbstractAlphabet
{
private:
std::shared_ptr<const NucleicAlphabet>  nuclalph_;

public:
RNY(std::shared_ptr<const NucleicAlphabet> na);

RNY(const RNY& bia) : AbstractAlphabet(bia), nuclalph_(bia.nuclalph_) {
}

RNY& operator=(const RNY& bia)
{
	AbstractAlphabet::operator=(bia);
	return *this;
}

RNY* clone() const
{
	return new RNY(*this);
}

~RNY() {
}

public:
int charToInt(const std::string& state) const;
std::string intToChar(int state) const;

bool containsGap(const std::string& state) const;

unsigned int getSize() const {
	return 36;
}
unsigned int getNumberOfTypes() const {
	return 80;
}
int getUnknownCharacterCode() const {
	return 350;
}
bool isGap(int state) const;
bool isResolvedIn(int state1, int state2) const;
std::vector<int> getAlias(int state) const;
std::vector<std::string> getAlias(const std::string& state) const;
bool isUnresolved(int state) const;
bool isUnresolved(const std::string& state) const;

std::string getAlphabetType() const {
	return "RNY(letter=" + nuclalph_->getAlphabetType() + ")";
}

std::shared_ptr<const NucleicAlphabet> getLetterAlphabet() const
{
	return nuclalph_;
}

public:
/**
 * @brief Get the char code for a triplet given the char code of the three underlying positions.
 *
 * The char code of each position must match the nucleic alphabet specified for this alphabet.
 * NB: This performs pos1 + pos2 + pos3 after checking for each position validity.
 * @param pos1 Char description for position 1.
 * @param pos2 Char description for position 2.
 * @param pos3 Char description for position 3.
 * @return The Char code of the triplet.
 */
std::string getRNY(const std::string&, const std::string&, const std::string&) const;

int getRNY(int, int, int, const Alphabet&) const;
};
}
#endif // BPP_SEQ_ALPHABET_RNY_H
