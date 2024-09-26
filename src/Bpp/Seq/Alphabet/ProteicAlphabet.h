// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_PROTEICALPHABET_H
#define BPP_SEQ_ALPHABET_PROTEICALPHABET_H


#include "LetterAlphabet.h"
#include "ProteicAlphabetState.h"

namespace bpp
{
/**
 * @brief This alphabet is used to deal with proteins.
 *
 * It supports all 20 amino-acids with their standard denomination.
 * Gaps are coded by '-', unresolved characters are coded by 'X'.
 * Generic characters: B={N,D}, Z={Q,E}, J={I,L}.
 */

class ProteicAlphabet :
	public LetterAlphabet
{
/**
 * @name Overloaded methods from AbstractAlphabet
 * @{
 */

public:
const ProteicAlphabetState& getState(const std::string& letter) const
{
	return dynamic_cast<const ProteicAlphabetState&>(
		AbstractAlphabet::getState(letter)
		);
}

const ProteicAlphabetState& getState(int num) const
{
	return dynamic_cast<const ProteicAlphabetState&>(
		AbstractAlphabet::getState(num)
		);
}

protected:
const ProteicAlphabetState& getStateAt(size_t pos) const
{
	return dynamic_cast<const ProteicAlphabetState&>(
		AbstractAlphabet::getStateAt(pos)
		);
}

ProteicAlphabetState& getStateAt(size_t pos)
{
	return dynamic_cast<ProteicAlphabetState&>(
		AbstractAlphabet::getStateAt(pos)
		);
}

/** @} */

public:
ProteicAlphabet();

ProteicAlphabet(const ProteicAlphabet& bia) : LetterAlphabet(bia) {
}

ProteicAlphabet& operator=(const ProteicAlphabet& bia)
{
	LetterAlphabet::operator=(bia);
	return *this;
}

ProteicAlphabet* clone() const
{
	return new ProteicAlphabet(*this);
}


virtual ~ProteicAlphabet() {
}

public:
unsigned int getSize() const {
	return 20;
}
unsigned int getNumberOfTypes() const {
	return 24;
}
int getUnknownCharacterCode() const {
	return 23;
}
bool isResolvedIn(int state1, int state2) const;
std::vector<int> getAlias(int state) const;
std::vector<std::string> getAlias(const std::string& state) const;
int getGeneric(const std::vector<int>& states) const;
std::string getGeneric(const std::vector<std::string>& states) const;
bool isUnresolved(int state) const {
	return state > 19;
}
bool isUnresolved(const std::string& state) const {
	return charToInt(state) > 19;
}
std::string getAlphabetType() const {
	return "Proteic";
}

public:
/**
 * @name Specific methods
 *
 * @{
 */

/**
 * @brief Get the abbreviation (3 letter code) for a state coded as char.
 *
 * @param aa Char description of the amino-acid to analyse.
 */
std::string getAbbr(const std::string& aa) const;

/**
 * @brief Get the abbreviation (3 letter code) for a state coded as int.
 *
 * @param aa Int description of the amino-acid to analyse.
 */
std::string getAbbr(int aa) const;
/** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_PROTEICALPHABET_H
