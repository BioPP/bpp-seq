// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCEWITHANNOTATION_H
#define BPP_SEQ_SEQUENCEWITHANNOTATION_H


#include "CoreSequence.h"
#include "IntSymbolList.h"
#include "Sequence.h"

// From the STL:
#include <string>
#include <vector>

namespace bpp
{
class SequenceWithAnnotation;

/**
 * @brief Interface for sequence annotations.
 */
class SequenceAnnotation :
	public virtual IntSymbolListListener
{
public:
virtual SequenceAnnotation* clone() const override = 0;

/**
 * Creates a default annotation according to a given sequence.
 *
 * @param seq The template sequence (typically the one with which the annotation will be attached).
 */
virtual void init(const Sequence& seq) = 0;

/**
 * @return The type of the annotation. This is used for querying annotations.
 */
virtual const std::string& getType() const = 0;

/**
 * @brief Test is the annotation is valid for a given sequence.
 *
 * @param sequence The sequence to be validated against.
 * @param throwException If set to yes, throw an exception if the sequence is not valid.
 * @return true if this annotation is compliant with the given sequence.
 */
virtual bool isValidWith(const SequenceWithAnnotation& sequence, bool throwException = true) const = 0;

/**
 * @brief Merge the input annotation with the current one.
 *
 * @param anno The annotation to fuse.
 * @return true if the fusion was possible and successful.
 */
virtual bool merge(const SequenceAnnotation& anno) = 0;

/**
 * @return A new annotation corresponding to a part of the sequence.
 * The implementation of this highly depends on the annotation type, and might not be supported.
 * @param pos Starting point of the region.
 * @param len The length of the region, in number of positions.
 */
virtual std::unique_ptr<SequenceAnnotation> getPartAnnotation(size_t pos, size_t len) const = 0;
};

/**
 * @brief An implementation of the Sequence interface that supports annotation.
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
 * The gestion of sequence content is identical to the BasicSequence object, but edition events are
 * properly fired. Listener are therefore properly handled.
 *
 * @see BasicSequence
 */
class SequenceWithAnnotation :
	public virtual SequenceInterface,
	public AbstractCoreSequence,
	public EventDrivenIntSymbolList
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
SequenceWithAnnotation(std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList(alpha),
	AbstractCoreSequence(""),
	EventDrivenIntSymbolList(alpha)
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
SequenceWithAnnotation(
	const std::string& name,
	const std::string& sequence,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList(alpha),
	AbstractCoreSequence(name),
	EventDrivenIntSymbolList(alpha)
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
SequenceWithAnnotation(
	const std::string& name,
	const std::string& sequence,
	const Comments& comments,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList(alpha),
	AbstractCoreSequence(name, comments),
	EventDrivenIntSymbolList(alpha)
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
SequenceWithAnnotation(
	const std::string& name,
	const std::vector<std::string>& sequence,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList(alpha),
	AbstractCoreSequence(name),
	EventDrivenIntSymbolList(sequence, alpha)
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
SequenceWithAnnotation(
	const std::string& name,
	const std::vector<std::string>& sequence,
	const Comments& comments,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList(alpha),
	AbstractCoreSequence(name, comments),
	EventDrivenIntSymbolList(sequence, alpha)
{
}


/**
 * @brief General purpose constructor, can be used with any alphabet.
 *
 * @param name     The sequence name.
 * @param sequence The sequence content.
 * @param alpha    A pointer toward the alphabet to be used with this sequence.
 */
SequenceWithAnnotation(
	const std::string& name,
	const std::vector<int>& sequence,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList(sequence, alpha),
	AbstractCoreSequence(name),
	EventDrivenIntSymbolList(sequence, alpha)
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
SequenceWithAnnotation(
	const std::string& name,
	const std::vector<int>& sequence,
	const Comments& comments,
	std::shared_ptr<const Alphabet>& alpha) :
	AbstractTemplateSymbolList(sequence, alpha),
	AbstractCoreSequence(name, comments),
	EventDrivenIntSymbolList(sequence, alpha)
{
}


/**
 * @brief The Sequence generic copy constructor.
 */
SequenceWithAnnotation(const Sequence& s) :
	AbstractTemplateSymbolList(s.getContent(), s.getAlphabet()),
	AbstractCoreSequence(s.getName(), s.getComments()),
	EventDrivenIntSymbolList(s.getContent(), s.getAlphabet())
{
}


/**
 * @brief The Sequence copy constructor.
 */
SequenceWithAnnotation(const SequenceWithAnnotation& s) :
	AbstractTemplateSymbolList(s.getContent(), s.getAlphabet()),
	AbstractCoreSequence(s.getName(), s.getComments()),
	EventDrivenIntSymbolList(s.getContent(), s.getAlphabet())
{
}


/**
 * @brief The Sequence generic assignment operator.
 *
 * @return A ref toward the assigned Sequence.
 */
SequenceWithAnnotation& operator=(const Sequence& s)
{
	setContent(s.getContent());
	setName(s.getName());
	setComments(s.getComments());
	return *this;
}


/**
 * @brief The Sequence assignment operator. This does not perform a hard copy of the alphabet object.
 *
 * @return A ref toward the assigned Sequence.
 */
SequenceWithAnnotation& operator=(const SequenceWithAnnotation& s)
{
	AbstractCoreSequence::operator=(s);
	EventDrivenIntSymbolList::operator=(s);
	return *this;
}


virtual ~SequenceWithAnnotation() {
}

public:
/**
 * @name The Clonable interface
 *
 * @{
 */
SequenceWithAnnotation* clone() const override {
	return new SequenceWithAnnotation(*this);
}
/** @} */


/**
 * @name Adjusting the size of the sequence.
 *
 * @{
 */

/**
 * @brief Set the whole content of the sequence.
 *
 * @param sequence The new content of the sequence.
 * @see The Sequence constructor for information about the way sequences are internally stored.
 */
virtual void setContent(const std::string& sequence) override;

void setContent(const std::vector<std::string>& list) override
{
	EventDrivenIntSymbolList::setContent(list);
}

void setContent(const std::vector<int>& list) override
{
	EventDrivenIntSymbolList::setContent(list);
}

void setToSizeR(size_t newSize) override;

void setToSizeL(size_t newSize) override;

/**
 * @brief Append the content of a sequence to the current one.
 *
 * @param seq The sequence to append. Only the raw content is appended, not additional fields such as annotations if any.
 * @throw AlphabetMismatchException In case the alphabet does not match the current one.
 */
void append(const SequenceInterface& seq) override;

void append(const std::vector<int>& content) override;

void append(const std::vector<std::string>& content) override;

void append(const std::string& content) override;

/** @} */

/**
 * @brief Add a new annotation to the sequence.
 *
 * @param anno The annotation object to be added. Unless the annotation is shared,
 * the annotation object will be owned by the sequence object, and will be copied
 * and deleted when needed.
 * @throw Exception If the annotation is not valid for this sequence.
 * @see SequenceWithAnnotation::isValidWith
 */
virtual void addAnnotation(std::shared_ptr<SequenceAnnotation> anno)
{
	anno->isValidWith(*this);
	addIntSymbolListListener(anno);
}

virtual bool hasAnnotation(const std::string& type) const
{
	for (size_t i = 0; i < getNumberOfListeners(); ++i)
	{
		const auto& lstn = listener(i);
		try
		{
			const SequenceAnnotation& anno = dynamic_cast<const SequenceAnnotation&>(lstn);
			if (anno.getType() == type) return true;
		}
		catch (std::bad_cast&) {}
	}
	return false;
}


virtual const SequenceAnnotation& annotation(const std::string& type) const
{
	for (size_t i = 0; i < getNumberOfListeners(); ++i)
	{
		const auto& lstn = listener(i);
		try
		{
			const SequenceAnnotation& anno = dynamic_cast<const SequenceAnnotation&>(lstn);
			if (anno.getType() == type) return anno;
		}
		catch (std::bad_cast&) {}
	}
	throw Exception("SequenceWithAnnotation::getAnnotation. No annotation found with type '" + type + "'.");
}

virtual SequenceAnnotation& annotation(const std::string& type)
{
	for (size_t i = 0; i < getNumberOfListeners(); ++i)
	{
		auto& lstn = listener(i);
		try
		{
			SequenceAnnotation& anno = dynamic_cast<SequenceAnnotation&>(lstn);
			if (anno.getType() == type) return anno;
		}
		catch (std::bad_cast&) {}
	}
	throw Exception("SequenceWithAnnotation::getAnnotation. No annotation found with type '" + type + "'.");
}

/**
 * @return The list of annotation types contained in this sequence.
 */
virtual std::vector<std::string> getAnnotationTypes() const;

/**
 * @brief Merge a sequence with the current one.
 *
 * Sequences must have the same name and alphabets.
 * Only first sequence's commentaries are kept.
 * Annotations that could not be merged will not be added in the concatenated sequence.
 * See the documentation of each annotation class for more details.
 *
 * @param swa The sequence to merge with.
 * @throw AlphabetMismatchException If the two alphabets do not match.
 * @throw Exception If the sequence names do not match.
 */
virtual void merge(const SequenceWithAnnotation& swa);
};
} // end of namespace bpp.
#endif // BPP_SEQ_SEQUENCEWITHANNOTATION_H
