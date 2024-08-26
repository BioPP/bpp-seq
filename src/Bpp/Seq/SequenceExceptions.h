// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCEEXCEPTIONS_H
#define BPP_SEQ_SEQUENCEEXCEPTIONS_H

#include <Bpp/Exceptions.h>

#include "Alphabet/Alphabet.h"
#include "CoreSequence.h"

#include <string>
#include <memory>

namespace bpp
{
/**
 * @brief The sequence exception base class.
 *
 * @see Exception
 */
class SequenceException :
	public Exception
{
private:
/**
 * @brief A pointer toward a sequence object.
 */
const CoreSequenceInterface* sequence_;

public:
/**
 * @brief Build a new SequenceException object.
 *
 * @param text A message to be passed to the exception hierarchy.
 * @param seq A const pointer toward the sequence that threw the exception. The exception object is not managing the pointer.
 */
SequenceException(const std::string& text, const CoreSequenceInterface* seq) :
	Exception("SequenceException: " + text + (seq ? "(" + seq->getName() + ")" : std::string(""))),
	sequence_(seq)
{
}

SequenceException(const SequenceException& se) :
	Exception(se),
	sequence_(se.sequence_)
{
}

SequenceException& operator=(const SequenceException& se)
{
	Exception::operator=(se);
	sequence_ = se.sequence_;
	return *this;
}

virtual ~SequenceException() {
}

public:
/**
 * @brief Get the sequence that threw the exception.
 *
 * @return A const pointer toward the sequence.
 */
virtual const CoreSequenceInterface* getSequence() const {
	return sequence_;
}
};

/**
 * @brief Exception thrown when a sequence is found to be empty and it should not.
 */
class EmptySequenceException :
	public SequenceException
{
public:
/**
 * @brief Build a new EmptySequenceException object.
 *
 * @param text A message to be passed to the exception hierarchy.
 * @param seq A const pointer toward the sequence that threw the exception.
 */
EmptySequenceException(const std::string& text, const CoreSequenceInterface* seq) :
	SequenceException("EmptySequenceException: " + text, seq)
{
}

virtual ~EmptySequenceException() {
}
};

/**
 * @brief Exception thrown when a sequence is found to have gap and it should not.
 */
class SequenceWithGapException :
	public SequenceException
{
public:
/**
 * @brief Build a new SequenceWithGapException object.
 *
 * @param text A message to be passed to the exception hierarchy.
 * @param seq A const pointer toward the sequence that threw the exception.
 */
SequenceWithGapException(const std::string& text, const CoreSequenceInterface* seq) :
	SequenceException("SequenceWithGapException: " + text, seq)
{
}

virtual ~SequenceWithGapException() {
}
};

/**
 * @brief Exception thrown when a sequence is not align with others.
 *
 * Typically, this may occur when you try to add a bad sequence to a site container.
 */
class SequenceNotAlignedException :
	public SequenceException
{
public:
/**
 * @brief Build a new SequenceNotAlignedException object.
 *
 * @param text A message to be passed to the exception hierarchy.
 * @param seq  A const pointer toward the sequence that threw the exception.
 */
SequenceNotAlignedException(const std::string& text, const CoreSequenceInterface* seq) :
	SequenceException("SequenceNotAlignedException: " + text, seq)
{
}

virtual ~SequenceNotAlignedException() {
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_SEQUENCEEXCEPTIONS_H
