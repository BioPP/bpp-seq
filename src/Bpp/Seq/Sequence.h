// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCE_H
#define BPP_SEQ_SEQUENCE_H


#include "CoreSequence.h"
#include "IntSymbolList.h"
#include "SequenceExceptions.h"

// From the STL:
#include <string>
#include <vector>

namespace bpp
{
/**
 * @brief The sequence interface.
 *
 * This is a general purpose container, containing an ordered list of states.
 * The states that allowed to be present in the sequence are defined
 * by an alphabet object.
 *
 * Sequence objects also contain a name attribute and potentially several comment lines.
 * A sequence object is also event-driven, allowing easy extension.
 *
 * @see Alphabet
 */
class SequenceInterface :
	public virtual CoreSequenceInterface,
	public virtual IntSymbolListInterface
{
public:
typedef int ElementType;

public:
virtual ~SequenceInterface() {
}

public:
SequenceInterface* clone() const override = 0;


/**
 * @name Adjusting the content and size of the sequence.
 *
 * @{
 */

/**
 * @brief Set the whole content of the sequence.
 *
 * @param sequence The new content of the sequence.
 * @see The Sequence constructor for information about the way sequences are internally stored.
 */
virtual void setContent(const std::string& sequence) = 0;

virtual void setContent(const std::vector<std::string>& list) override = 0;

virtual void setContent(const std::vector<int>& list) override = 0;

/**
 * @brief Append the content of the sequence.
 *
 * @param seq The sequence to append.
 * @throw AlphabetMismatchException If the alphabet of the specified sequence does not match the current alphabet.
 */
virtual void append(const SequenceInterface& seq) = 0;

/**
 * @brief Append the specified content to the sequence.
 *
 * @param content The content to append to the sequence.
 * @throw BadIntException If the content does not match the current alphabet.
 */
virtual void append(const std::vector<int>& content) = 0;

/**
 * @brief Append the specified content to the sequence.
 *
 * @param content The content to append to the sequence.
 * @throw BadCharException If the content does not match the current alphabet.
 */
virtual void append(const std::vector<std::string>& content) = 0;

/**
 * @brief Append the specified content to the sequence.
 *
 * @param content The content to append to the sequence.
 * @throw BadCharException If the content does not match the current alphabet.
 */
virtual void append(const std::string& content) = 0;

/** @} */
};


/**
 * @brief A basic implementation of the Sequence interface.
 *
 * This is a general purpose container, containing an ordered list of states.
 * The states that allowed to be present in the sequence are defined
 * by an alphabet object, which is passed to the sequence constructor by a pointer.
 *
 * For programming convenience, the states are stored as integers, but the translation toward
 * and from a char description is easily performed with the Alphabet classes.
 *
 * Sequence objects also contain a name attribute and potentially several comment lines.
 *
 * @see Alphabet
 */
class Sequence :
	public virtual SequenceInterface,
	public AbstractCoreSequence,
	public IntSymbolList
{
public:
/**
 * @brief Empty constructor: build a void Sequence with just an Alphabet
 *
 * You can use it safely for all type of Alphabet in order to build an
 * empty Sequence i.e. without name nor sequence data.
 *
 * @param alpha    A pointer toward the Alphabet to be used with this Sequence.
 */
Sequence(std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList<int>(alpha),
	AbstractCoreSequence(),
	IntSymbolList(alpha)
{
}


/**
 * @brief Direct constructor: build a Sequence object from a std::string
 * You can use it safely for RNA, DNA and protein sequences.
 *
 * It can be used with codon sequences too, the std::string will be cut into
 * parts of size 3. But for more complicated alphabets, you should use one
 * complete constructors.
 *
 * @param name     The sequence name.
 * @param sequence The whole sequence to be parsed as a std::string.
 * @param alpha    A pointer toward the alphabet to be used with this sequence.
 */
Sequence(
	const std::string& name,
	const std::string& sequence,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList<int>(alpha),
	AbstractCoreSequence(name),
	IntSymbolList(alpha)
{
	if (sequence != "")
		setContent(sequence);
}


/**
 * @brief Direct constructor: build a Sequence object from a std::string.
 *
 * You can use it safely for RNA, DNA and protein sequences.
 *
 * It can be used with codon sequences too, the std::string will be cut into
 * tokens of size 3. But for more complicated alphabets, you should use one
 * complete constructors.
 *
 * @param name     The sequence name.
 * @param sequence The whole sequence to be parsed as a std::string.
 * @param comments Comments to add to the sequence.
 * @param alpha    A pointer toward the alphabet to be used with this sequence.
 */
Sequence(
	const std::string& name,
	const std::string& sequence,
	const Comments& comments,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList<int>(alpha),
	AbstractCoreSequence(name, comments),
	IntSymbolList(alpha)
{
	if (sequence != "")
		setContent(sequence);
}


/**
 * @brief General purpose constructor, can be used with any alphabet.
 *
 * You should note that the sequence is stored as a std::vector of int.
 * Hence each std::string in the std::vector will be translated using the alphabet object.
 *
 * @param name     The sequence name.
 * @param sequence The sequence content.
 * @param alpha    A pointer toward the alphabet to be used with this sequence.
 */
Sequence(
	const std::string& name,
	const std::vector<std::string>& sequence,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList<int>(alpha),
	AbstractCoreSequence(name),
	IntSymbolList(sequence, alpha)
{
}


/**
 * @brief General purpose constructor, can be used with any alphabet.
 *
 * You should note that the sequence is stored as a std::vector of int.
 * Hence each std::string in the std::vector will be translated using the alphabet object.
 *
 * @param name     The sequence name.
 * @param sequence The sequence content.
 * @param comments Comments to add to the sequence.
 * @param alpha    A pointer toward the alphabet to be used with this sequence.
 */
Sequence(
	const std::string& name,
	const std::vector<std::string>& sequence,
	const Comments& comments,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList<int>(alpha),
	AbstractCoreSequence(name, comments),
	IntSymbolList(sequence, alpha)
{
}


/**
 * @brief General purpose constructor, can be used with any alphabet.
 *
 * @param name     The sequence name.
 * @param sequence The sequence content.
 * @param alpha    A pointer toward the alphabet to be used with this sequence.
 */
Sequence(
	const std::string& name,
	const std::vector<int>& sequence,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList<int>(sequence, alpha),
	AbstractCoreSequence(name),
	IntSymbolList(sequence, alpha)
{
}


/**
 * @brief General purpose constructor, can be used with any alphabet.
 *
 * @param name     The sequence name.
 * @param sequence The sequence content.
 * @param comments Comments to add to the sequence.
 * @param alpha    A pointer toward the alphabet to be used with this sequence.
 */
Sequence(
	const std::string& name,
	const std::vector<int>& sequence,
	const Comments& comments,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList<int>(sequence, alpha),
	AbstractCoreSequence(name, comments),
	IntSymbolList(sequence, alpha)
{
}


/**
 * @brief The Sequence generic copy constructor. This does not perform a hard copy of the alphabet object.
 */
Sequence(const SequenceInterface& s) :
	AbstractTemplateSymbolList<int>(s.getContent(), s.getAlphabet()),
	AbstractCoreSequence(s),
	IntSymbolList(s.getContent(), s.getAlphabet())
{
}


/**
 * @brief The Sequence copy constructor. This does not perform a hard copy of the alphabet object.
 */
Sequence(const Sequence& s) :
	AbstractTemplateSymbolList<int>(s.getContent(), s.getAlphabet()),
	AbstractCoreSequence(s),
	IntSymbolList(s.getContent(), s.getAlphabet())
{
}


/**
 * @brief The Sequence generic assignment operator. This does not perform a hard copy of the alphabet object.
 *
 * @return A ref toward the assigned Sequence.
 */
Sequence& operator=(const SequenceInterface& s)
{
	AbstractCoreSequence::operator=(s);
	setContent(s.getContent());
	return *this;
}


/**
 * @brief The Sequence assignment operator. This does not perform a hard copy of the alphabet object.
 *
 * @return A ref toward the assigned Sequence.
 */
Sequence& operator=(const Sequence& s)
{
	AbstractCoreSequence::operator=(s);
	IntSymbolList::operator=(s);
	return *this;
}


virtual ~Sequence() {
}

public:
/**
 * @name The Clonable interface
 *
 * @{
 */
Sequence* clone() const override {
	return new Sequence(*this);
}
/** @} */

/**
 * @name Adjusting the size of the sequence.
 *
 * @{
 */
void setContent(const std::string& sequence) override;

using IntSymbolList::addElement;

void setContent(const std::vector<std::string>& list) override
{
	IntSymbolList::setContent(list);
}

void setContent(const std::vector<int>& list) override
{
	IntSymbolList::setContent(list);
}

std::string toString() const override
{
	return IntSymbolList::toString();
}

std::string getChar(size_t pos) const override
{
	return IntSymbolList::getChar(pos);
}

void setToSizeR(size_t newSize) override;

void setToSizeL(size_t newSize) override;

void append(const SequenceInterface& seq) override;

void append(const std::vector<int>& content) override;

void append(const std::vector<std::string>& content) override;

void append(const std::string& content) override;

/** @} */


double getStateValueAt(size_t sitePosition, int state) const override
{
	if (sitePosition  >= size()) throw IndexOutOfBoundsException("BasicSequence::getStateValueAt.", sitePosition, 0, size() - 1);
	return getAlphabet()->isResolvedIn((*this)[sitePosition], state) ? 1. : 0.;
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_SEQUENCE_H
