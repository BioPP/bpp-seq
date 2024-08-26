// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_SEQUENCECONTAINEREXCEPTIONS_H
#define BPP_SEQ_CONTAINER_SEQUENCECONTAINEREXCEPTIONS_H

#include <Bpp/Exceptions.h>


namespace bpp
{
class SequencedValuesContainer;

/**
 * @brief Exception thrown when a sequence is not found The sequence not found exception base class.
 */
class SequenceNotFoundException :
	public Exception
{
protected:
/**
 * @brief The id of the sequence that was to be found.
 */
const std::string id;

public:
/**
 * @brief Build a new SequenceNotFoundException object.
 *
 * @param text  A message to be passed to the exception hierarchy.
 * @param seqId A the id of the sequence that was to be found.
 */
SequenceNotFoundException(const char* text, const char* seqId = "") :
	Exception("SequenceNotFoundException: " + std::string(text) + "(" + seqId + ")"),
	id(seqId) {
}

/**
 * @brief Build a new SequenceNotFoundException object.
 *
 * @param text  A message to be passed to the exception hierarchy.
 * @param seqId A the id of the sequence that was to be found.
 */
SequenceNotFoundException(const std::string& text, const std::string& seqId = "") :
	Exception("SequenceNotFoundException: " + text + "(" + seqId + ")"),
	id(seqId) {
}

// Class destructor
virtual ~SequenceNotFoundException() {
}

public:
/**
 * @brief Get the id of the sequence that was to be found.
 *
 * @return The id of the sequence that was to be found.
 */
virtual const std::string getSequenceId() const {
	return id;
}
};

/**
 * @brief Exception thrown when an empty container is found.
 */
class EmptyContainerException :
	public Exception
{
private:
/**
 * @brief The empty container.
 */
const SequencedValuesContainer* container_;

public:
/**
 * @brief Build a new EmptyContainerException object.
 *
 * @param text  A message to be passed to the exception hierarchy.
 * @param container The empty container.
 */
EmptyContainerException(const std::string& text, const SequencedValuesContainer* container) :
	Exception("EmptyContainerException: " + text),
	container_(container) {
}

EmptyContainerException(const EmptyContainerException& ece) :
	Exception(ece), container_(ece.container_) {
}

EmptyContainerException& operator=(const EmptyContainerException& ece)
{
	Exception::operator=(ece);
	container_ = ece.container_;
	return *this;
}

// Class destructor
virtual ~EmptyContainerException() {
}

public:
/**
 * @return The empty container.
 */
virtual const SequencedValuesContainer* getContainer() const {
	return container_;
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCECONTAINEREXCEPTIONS_H
