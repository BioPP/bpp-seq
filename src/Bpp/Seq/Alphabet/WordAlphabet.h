// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_WORDALPHABET_H
#define BPP_SEQ_ALPHABET_WORDALPHABET_H


#include "AbstractAlphabet.h"

// From the STL:
#include <string>
#include <vector>

#include "../Sequence.h"

namespace bpp
{
/**
 * @brief The interface class for word alphabets.
 *
 */

class CoreWordAlphabet
{
public:
CoreWordAlphabet() {
}

virtual ~CoreWordAlphabet() {
}

virtual unsigned int getLength() const = 0;

virtual unsigned int getSize() const = 0;

virtual bool hasUniqueAlphabet() const = 0;

virtual std::shared_ptr<const Alphabet> getNAlphabet(size_t n) const = 0;

virtual int getWord(const Sequence& seq, size_t pos = 0) const = 0;


/**
 * @brief Get the int code for a word given the int code of the underlying positions.
 *
 * The int code of each position must match the corresponding alphabet specified at this position.
 * @param vint description for all the positions.
 * @param pos the start position to match in the vector.
 * @return The int code of the word.
 * @throw IndexOutOfBoundsException In case of wrong position.
 */
virtual int getWord(const std::vector<int>& vint, size_t pos = 0) const = 0;

/**
 * @brief Get the char code for a word given the char code of the
 * underlying positions.
 *
 * The char code of each position must match the corresponding alphabet specified at this position.
 * @param vpos vector description for all the positions.
 * @param pos the start position to match in the vector.
 * @return The string of the word.
 * @throw IndexOutOfBoundsException In case of wrong position.
 */
virtual std::string getWord(const std::vector<std::string>& vpos, size_t pos = 0) const = 0;

/**
 * @brief Get the int code of the n-position of a word given its int description.
 *
 * @param word The int description of the word.
 * @param n The position in the word (starting at 0).
 * @return The int description of the n-position of the word.
 */
virtual int getNPosition(int word, size_t n) const = 0;

/**
 * @brief Get the int codes of each position of a word given its int description.
 *
 * @param word The int description of the word.
 * @return The int description of the positions of the codon.
 */
virtual std::vector<int> getPositions(int word) const = 0;

/**
 * @brief Get the char code of the n-position of a word given its char description.
 *
 * @param word The char description of the word.
 * @param n The position in the word (starting at 0).
 * @return The char description of the n-position of the word.
 */
virtual std::string getNPosition(const std::string& word, size_t n) const = 0;

/**
 * @brief Get the char codes of each position of a word given its char description.
 *
 * @param word The char description of the word.
 * @return The char description of the three positions of the word.
 */
virtual std::vector<std::string> getPositions(const std::string& word) const = 0;

/**
 * @brief Translate a whole sequence from letters alphabet to words alphabet.
 *
 * @param sequence A sequence in letters alphabet.
 * @param pos the start position (default 0)
 * @return The corresponding sequence in words alphabet.
 * @throw AlphabetMismatchException If the sequence alphabet do not match the source alphabet.
 * @throw Exception                 Other kind of error, depending on the implementation.
 */
virtual std::unique_ptr<SequenceInterface> translate(const SequenceInterface& sequence, size_t pos = 0) const = 0;

/**
 * @brief Translate a whole sequence from words alphabet to letters alphabet.
 *
 * @param sequence A sequence in words alphabet.
 * @return The corresponding sequence in letters alphabet.
 * @throw AlphabetMismatchException If the sequence alphabet do not match the target alphabet.
 * @throw Exception                 Other kind of error, depending on the implementation.
 */
virtual std::unique_ptr<SequenceInterface> reverse(const SequenceInterface& sequence) const = 0;

private:
/**
 * @name Inner utilitary functions
 *
 * @{
 */

virtual bool containsUnresolved(const std::string& state) const = 0;

virtual bool containsGap(const std::string& state) const = 0;

/** @} */
};


/**
 * @brief The base class for word alphabets.
 *
 * These alphabets are compounds of several alphabets. The only
 * constraint on these alphabets is that their words have length one
 * (so it is not possible to make WordAlphabets from other
 * WordAlphabets). The construction is made from a vector of pointers
 * to AbstractAlphabets.
 *
 * The strings of the WordAlphabet are concatenations of the strings
 * of the Alphabets. They are made from the resolved letters of the
 * Alphabets.
 */
class WordAlphabet :
	public virtual CoreWordAlphabet,
	public AbstractAlphabet
{
protected:
std::vector<std::shared_ptr<const Alphabet> > vAbsAlph_;

public:
// Constructor and destructor.
/**
 * @brief Builds a new word alphabet from a vector of Alphabets.
 *
 * The unit alphabets are not owned by the world alphabet, and won't
 * be destroyed when this instance is destroyed.
 *
 * @param vAlpha The vector of Alphabets to be used.
 */
WordAlphabet(const std::vector<std::shared_ptr<const Alphabet> >& vAlpha);

/**
 * @brief Builds a new word alphabet from a pointer to number of
 * Alphabets.
 *
 * @param pAlpha The Pointer to the Alphabet to be used.
 * @param num the length of the words.
 */
WordAlphabet(std::shared_ptr<const Alphabet> pAlpha, size_t num);

WordAlphabet(const WordAlphabet& bia) : AbstractAlphabet(bia), vAbsAlph_(bia.vAbsAlph_) {
}

WordAlphabet& operator=(const WordAlphabet& bia)
{
	AbstractAlphabet::operator=(bia);
	vAbsAlph_ = bia.vAbsAlph_;
	return *this;
}

WordAlphabet* clone() const override
{
	return new WordAlphabet(*this);
}

virtual ~WordAlphabet() {
}

public:
/**
 * @name Methods redefined from Alphabet
 *
 * @{
 */
/**
 * @brief Get the complete name of a state given its string description.
 *
 * In case of undefined characters (i.e. N and X for nucleic alphabets),
 * this method will return the name of the undefined word.
 *
 * @param state The string description of the given state.
 * @return The name of the state.
 * @throw BadCharException When state is not a valid char description.
 */
std::string getName(const std::string& state) const override;

int charToInt(const std::string& state) const override
{
	if (state.size() != vAbsAlph_.size())
		throw BadCharException(state, "WordAlphabet::charToInt", this);
	if (containsUnresolved(state))
		return static_cast<int>(getSize());
	if (containsGap(state))
		return -1;
	else return AbstractAlphabet::charToInt(state);
}

unsigned int getSize() const override
{
	return getNumberOfChars() - 2;
}

/** @} */


bool isResolvedIn(int state1, int state2) const override;

/**
 * @brief Returns True if the Alphabet of the letters in the word
 * are the same type.
 *
 */
bool hasUniqueAlphabet() const override;

/**
 * @brief Returns the length of the word
 *
 */
unsigned int getLength() const override
{
	return static_cast<unsigned int>(vAbsAlph_.size());
}


/**
 * @brief Returns the number of resolved states + one for unresolved
 *
 */
unsigned int getNumberOfTypes() const override
{
	return getNumberOfChars() - 1;
}

std::string getAlphabetType() const override;

int getUnknownCharacterCode() const override
{
	return static_cast<int>(getSize());
}

bool isUnresolved(int state) const override {
	return state == getUnknownCharacterCode();
}
bool isUnresolved(const std::string& state) const override {
	return charToInt(state) == getUnknownCharacterCode();
}

std::vector<int> getAlias(int state) const override;

std::vector<std::string> getAlias(const std::string& state) const override;

int getGeneric(const std::vector<int>& states) const override;

std::string getGeneric(const std::vector<std::string>& states) const override;

private:
/**
 * @name Inner utilitary functions
 *
 * @{
 */
bool containsUnresolved(const std::string& state) const override;
bool containsGap(const std::string& state) const override;
void build_();
/** @} */

public:
/**
 * @name Word specific methods
 *
 * @{
 */

/**
 * @brief Get the pointer to the Alphabet at the n-position.
 *
 * @param n The position in the word (starting at 0).
 * @return The pointer to the Alphabet of the n-position.
 */
std::shared_ptr<const Alphabet> getNAlphabet(size_t n) const override
{
	if (n >= vAbsAlph_.size())
		throw IndexOutOfBoundsException("WordAlphabet::getNPosition", n, 0, vAbsAlph_.size());

	return vAbsAlph_[n];
}

/**
 * @brief Get the int code for a word given the int code of the underlying positions.
 *
 * The int code of each position must match the corresponding alphabet specified at this position.
 * @param seq description for all the positions as a Sequence object.
 * @param pos the start position to match in the vector.
 * @return The int code of the word.
 * @throw IndexOutOfBoundsException In case of wrong position.
 */

virtual int getWord(const Sequence& seq, size_t pos = 0) const override;


/**
 * @brief Get the int code for a word given the int code of the underlying positions.
 *
 * The int code of each position must match the corresponding alphabet specified at this position.
 * @param vint description for all the positions.
 * @param pos the start position to match in the vector.
 * @return The int code of the word.
 * @throw IndexOutOfBoundsException In case of wrong position.
 */

virtual int getWord(const std::vector<int>& vint, size_t pos = 0) const override;

/**
 * @brief Get the char code for a word given the char code of the
 * underlying positions.
 *
 * The char code of each position must match the corresponding alphabet specified at this position.
 * @param vpos vector description for all the positions.
 * @param pos the start position to match in the vector.
 * @return The string of the word.
 * @throw IndexOutOfBoundsException In case of wrong position.
 */

virtual std::string getWord(const std::vector<std::string>& vpos, size_t pos = 0) const override;

/**
 * @brief Get the int code of the n-position of a word given its int description.
 *
 * @param word The int description of the word.
 * @param n The position in the word (starting at 0).
 * @return The int description of the n-position of the word.
 */
int getNPosition(int word, size_t n) const override
{
	if (n >= vAbsAlph_.size())
		throw IndexOutOfBoundsException("WordAlphabet::getNPosition", n, 0, vAbsAlph_.size());

	std::string s = intToChar(word);
	return vAbsAlph_[n]->charToInt(s.substr(n, 1));
}

/**
 * @brief Get the int codes of each position of a word given its int description.
 *
 * @param word The int description of the word.
 * @return The int description of the positions of the codon.
 */
std::vector<int> getPositions(int word) const override
{
	std::string s = intToChar(word);
	std::vector<int> positions;
	for (size_t i = 0; i < s.size(); i++)
	{
		positions.push_back(vAbsAlph_[i]->charToInt(s.substr(i, 1)));
	}

	return positions;
}
/**
 * @brief Get the char code of the n-position of a word given its char description.
 *
 * @param word The char description of the word.
 * @param n The position in the word (starting at 0).
 * @return The char description of the n-position of the word.
 */
std::string getNPosition(const std::string& word, size_t n) const override
{
	if (n > vAbsAlph_.size())
		throw BadCharException("", "WordAlphabet::getNPosition", this);
	// Test:
	charToInt(word);

	return "" + word.substr(n, 1);
}


/**
 * @brief Get the char codes of each position of a word given its char description.
 *
 * @param word The char description of the word.
 * @return The char description of the three positions of the word.
 */
std::vector<std::string> getPositions(const std::string& word) const override
{
	charToInt(word);
	std::vector<std::string> positions;
	for (size_t i = 0; i < word.size(); i++)
	{
		positions.push_back(word.substr(i, 1));
	}

	return positions;
}

/**
 * @brief Translate a whole sequence from letters alphabet to words alphabet.
 *
 * @param sequence A sequence in letters alphabet.
 * @param pos the start position (default 0)
 * @return The corresponding sequence in words alphabet.
 * @throw AlphabetMismatchException If the sequence alphabet do not match the source alphabet.
 * @throw Exception                 Other kind of error, depending on the implementation.
 */
std::unique_ptr<SequenceInterface> translate(const SequenceInterface& sequence, size_t = 0) const override;

/**
 * @brief Translate a whole sequence from words alphabet to letters alphabet.
 *
 * @param sequence A sequence in words alphabet.
 * @return The corresponding sequence in letters alphabet.
 * @throw AlphabetMismatchException If the sequence alphabet do not match the target alphabet.
 * @throw Exception                 Other kind of error, depending on the implementation.
 */
std::unique_ptr<SequenceInterface> reverse(const SequenceInterface& sequence) const override;

/** @} */

/**
 * @name Overloaded AbstractAlphabet methods.
 * @{
 */
unsigned int getStateCodingSize() const override
{
	return static_cast<unsigned int>(vAbsAlph_.size());
}
/** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_WORDALPHABET_H
