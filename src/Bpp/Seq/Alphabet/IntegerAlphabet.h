// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_INTEGERALPHABET_H
#define BPP_SEQ_ALPHABET_INTEGERALPHABET_H


#include "AbstractAlphabet.h"
#include <Bpp/Text/TextTools.h>

namespace bpp
{
/**
 * @brief The Integer Alphabet class, letters are from 0 to a given number, MAX.
 * @author Laurent Gueguen
 *
 */
class IntegerAlphabet :
	public AbstractAlphabet
{
private:
unsigned int MAX_;

public:
// class constructor
IntegerAlphabet(unsigned int max);

IntegerAlphabet(const IntegerAlphabet& bia) : AbstractAlphabet(bia), MAX_(bia.MAX_) {
}

IntegerAlphabet& operator=(const IntegerAlphabet& bia)
{
	AbstractAlphabet::operator=(bia);
	MAX_ = bia.MAX_;

	return *this;
}

IntegerAlphabet* clone() const
{
	return new IntegerAlphabet(*this);
}
// class destructor
virtual ~IntegerAlphabet() {
}

public:
unsigned int getSize() const {
	return MAX_;
}

unsigned int getNumberOfTypes() const {
	return MAX_ + 1;
}

std::string getAlphabetType() const {
	return "Integer(N=" + TextTools::toString(MAX_) + ")";
}

int getUnknownCharacterCode() const {
	return static_cast<int>(MAX_);
}

bool isUnresolved(int state) const {
	return state == static_cast<int>(MAX_);
}

bool isUnresolved(const std::string& state) const {
	return state == "-" or state == "?";
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_INTEGERALPHABET_H
