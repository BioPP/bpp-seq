// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_CASEMASKEDALPHABET_H
#define BPP_SEQ_ALPHABET_CASEMASKEDALPHABET_H


#include "LetterAlphabet.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief Case-sensitive letter alphabet.
 *
 * This alphabet is used for parsing comodity.
 * It takes as input another LetterAlphabet and will create duplicate any aphanumerical and upper case state,
 * by creating a lower case version of the state, also named "masked" state.
 * Helper functions are provided to determine whether a given state is masked or unmasked.
 */
class CaseMaskedAlphabet :
	public LetterAlphabet
{
public:
std::shared_ptr<const LetterAlphabet> nocaseAlphabet_;

public:
CaseMaskedAlphabet(std::shared_ptr<const LetterAlphabet> nocaseAlphabet);

CaseMaskedAlphabet(const CaseMaskedAlphabet& cma) :
	LetterAlphabet(cma), nocaseAlphabet_(cma.nocaseAlphabet_) {
}

CaseMaskedAlphabet& operator=(const CaseMaskedAlphabet& cma)
{
	LetterAlphabet::operator=(cma);
	nocaseAlphabet_ = cma.nocaseAlphabet_;
	return *this;
}

CaseMaskedAlphabet* clone() const override
{
	return new CaseMaskedAlphabet(*this);
}

public:
unsigned int getSize() const override {
	return nocaseAlphabet_->getSize();
}
unsigned int getNumberOfTypes() const override {
	return nocaseAlphabet_->getNumberOfTypes();
}
std::string getAlphabetType() const override {
	return "CaseMasked";
}
int getUnknownCharacterCode() const override {
	return nocaseAlphabet_->getUnknownCharacterCode();
}
bool isUnresolved(int state) const override {
	return nocaseAlphabet_->isUnresolved(state);
}
bool isUnresolved(const std::string& state) const override {
	return nocaseAlphabet_->isUnresolved(state);
}

std::shared_ptr<const Alphabet> getUnmaskedAlphabet() const {
	return nocaseAlphabet_;
}

bool isMasked(int state) const {
	return state >= 100;
}
bool isMasked(const std::string& state) const
{
	char c = state.c_str()[0];
	return isMasked(c);
}
bool isMasked(char state) const
{
	return isalpha(state) && !isupper(state);
}

/**
 * @brief Get the masked state equivalent to the input one.
 *
 * If the input state is masked, returns it "as is".
 * @param state The input state.
 * @throw BadIntException if the input state is not supported, or if there is no quivallent masked state.
 */
int getMaskedEquivalentState(int state) const;
/**
 * @brief Get the masked state equivalent to the input one.
 *
 * If the input state is masked, returns it "as is".
 * @param state The input state.
 * @throw BadCharException if the input state is not supported.
 * @throw BadIntException if there is no equivalent masked state.
 */
const std::string getMaskedEquivalentState(const std::string& state) const;
};
} // end of namespace bpp
#endif // BPP_SEQ_ALPHABET_CASEMASKEDALPHABET_H
