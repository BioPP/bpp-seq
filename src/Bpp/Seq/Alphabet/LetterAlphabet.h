// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_LETTERALPHABET_H
#define BPP_SEQ_ALPHABET_LETTERALPHABET_H


// From the STL
#include <string>
#include <vector>
#include <iostream>

// From Seq
#include "AbstractAlphabet.h"
#include "AlphabetExceptions.h"

namespace bpp
{
/**
 * @brief Specialized partial implementation of Alphabet using single letters.
 *
 * @author Sylvain Gaillard
 */
class LetterAlphabet :
	public AbstractAlphabet
{
private:
static const int LETTER_UNDEF_VALUE;
std::vector<int> letters_;
bool caseSensitive_;

public:
LetterAlphabet(bool caseSensitive = false) : letters_(256, LETTER_UNDEF_VALUE), caseSensitive_(caseSensitive) {
}

LetterAlphabet(const LetterAlphabet& bia) : AbstractAlphabet(bia), letters_(bia.letters_), caseSensitive_(bia.caseSensitive_) {
}

LetterAlphabet& operator=(const LetterAlphabet& bia)
{
	AbstractAlphabet::operator=(bia);
	letters_ = bia.letters_;
	caseSensitive_ = bia.caseSensitive_;

	return *this;
}

virtual LetterAlphabet* clone() const = 0;

virtual ~LetterAlphabet() {
}

public:
bool isCharInAlphabet(char state) const
{
	return letters_[static_cast<size_t>(state)] != LETTER_UNDEF_VALUE;
}
bool isCharInAlphabet(const std::string& state) const
{
	return isCharInAlphabet(state[0]);
}
int charToInt(const std::string& state) const
{
	if (!isCharInAlphabet(state))
		throw BadCharException(state, "LetterAlphabet::charToInt: Unknown state", this);
	return letters_[static_cast<unsigned int>(state[0])];
}

protected:
void registerState(AlphabetState* st)
{
	AbstractAlphabet::registerState(st);
	if (caseSensitive_)
	{
		letters_[static_cast<size_t>(st->getLetter()[0])] = st->getNum();
	}
	else
	{
		letters_[static_cast<size_t>(tolower(st->getLetter()[0]))] = st->getNum();
		letters_[static_cast<size_t>(toupper(st->getLetter()[0]))] = st->getNum();
	}
}

void setState(size_t pos, AlphabetState* st)
{
	AbstractAlphabet::setState(pos, st);
	if (caseSensitive_)
	{
		letters_[static_cast<size_t>(st->getLetter()[0])] = st->getNum();
	}
	else
	{
		letters_[static_cast<size_t>(tolower(st->getLetter()[0]))] = st->getNum();
		letters_[static_cast<size_t>(toupper(st->getLetter()[0]))] = st->getNum();
	}
}
};
}
#endif // BPP_SEQ_ALPHABET_LETTERALPHABET_H
