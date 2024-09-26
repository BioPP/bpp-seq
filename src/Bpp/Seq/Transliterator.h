// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_TRANSLITERATOR_H
#define BPP_SEQ_TRANSLITERATOR_H


#include "Alphabet/Alphabet.h"
#include "Sequence.h"

namespace bpp
{
/**
 * @brief This interface is used when translating a sequence from an alphabet to another: it gives the translation rules, eg: RNA -> DNA.
 */
class TransliteratorInterface
{
public:
TransliteratorInterface() {
}
virtual ~TransliteratorInterface() {
}

public:
/**
 * @brief Get the source alphabet.
 *
 * @return A pointer toward the source alphabet.
 */
virtual std::shared_ptr<const Alphabet> getSourceAlphabet() const = 0;

/**
 * @brief Get the source alphabet.
 *
 * @return A reference toward the source alphabet.
 */
virtual const Alphabet& sourceAlphabet() const = 0;

/**
 * @brief Get the target alphabet.
 *
 * @return A pointer toward the target alphabet.
 */
virtual std::shared_ptr<const Alphabet> getTargetAlphabet() const = 0;

/**
 * @brief Get the target alphabet.
 *
 * @return A reference toward the target alphabet.
 */
virtual const Alphabet& targetAlphabet() const = 0;

/**
 * @brief Translate a given state coded as a int from source alphabet to target alphabet.
 *
 * @param state A state in source alphabet.
 * @return The corresponding state in target alphabet.
 */
virtual int translate(int state) const = 0;

/**
 * @brief Translate a given state coded as a string from source alphabet to target alphabet.
 *
 * @param state A state in source alphabet.
 * @return The corresponding state in target alphabet.
 */
virtual std::string translate(const std::string& state) const = 0;

/**
 * @brief Translate a whole sequence from source alphabet to target alphabet.
 *
 * @param sequence A sequence in source alphabet.
 * @return The corresponding sequence in target alphabet.
 */
virtual std::unique_ptr<Sequence> translate(const SequenceInterface& sequence) const = 0;
};

/**
 * @brief The same as previous, but can perform the reverse translation, eg: RNA -> DNA and DNA -> RNA;
 */
class ReverseTransliteratorInterface :
	public virtual TransliteratorInterface
{
public:
ReverseTransliteratorInterface() {
}
virtual ~ReverseTransliteratorInterface() {
}

public:
/**
 * @brief Translate a given state coded as a int from target alphabet to source alphabet.
 *
 * @param state A state in target alphabet.
 * @return The corresponding state in source alphabet.
 */
virtual int reverse(int state) const = 0;

/**
 * @brief Translate a given state coded as a string from target alphabet to source alphabet.
 *
 * @param state A state in target alphabet.
 * @return The corresponding state in source alphabet.
 */
virtual std::string reverse(const std::string& state) const = 0;

/**
 * @brief Translate a whole sequence from target alphabet to source alphabet.
 *
 * @param sequence A sequence in target alphabet.
 * @return The corresponding sequence in source alphabet.
 */
virtual std::unique_ptr<Sequence> reverse(const SequenceInterface& sequence) const = 0;
};

/**
 * @brief Partial implementation of the Transliterator interface.
 */
class AbstractTransliterator :
	public virtual TransliteratorInterface
{
public:
AbstractTransliterator() {
}
virtual ~AbstractTransliterator() {
}

public:
int translate(int state) const override = 0;
std::string translate(const std::string& state) const override = 0;

std::unique_ptr<Sequence> translate(const SequenceInterface& sequence) const override;
};

/**
 * @brief Partial implementation of the ReverseTransliterator interface.
 */
class AbstractReverseTransliterator :
	public virtual ReverseTransliteratorInterface,
	public AbstractTransliterator
{
public:
AbstractReverseTransliterator() {
}
virtual ~AbstractReverseTransliterator() {
}

public:
int reverse(int state) const override = 0;
std::string reverse(const std::string& state) const override = 0;

std::unique_ptr<Sequence> reverse(const SequenceInterface& sequence) const override;
};
} // end of namespace bpp.
#endif // BPP_SEQ_TRANSLITERATOR_H
