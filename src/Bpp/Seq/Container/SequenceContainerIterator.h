// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_SEQUENCECONTAINERITERATOR_H
#define BPP_SEQ_CONTAINER_SEQUENCECONTAINERITERATOR_H


#include "../Sequence.h"
#include "../SequenceIterator.h"
#include "SequenceContainer.h"

namespace bpp
{
/**
 * @brief Partial implementation of the SequenceIterator interface, allowing to loop over a sequence container.
 */
template<class SequenceType, class HashType = std::string>
class AbstractTemplateSequenceContainerIterator :
	public virtual TemplateSequenceIteratorInterface<const SequenceType>
{
protected:
const TemplateSequenceContainerInterface<SequenceType, HashType>* sequences_;
size_t currentPosition_;

public:
AbstractTemplateSequenceContainerIterator(const TemplateSequenceContainerInterface<SequenceType, HashType>& sequences) :
	sequences_(&sequences),
	currentPosition_(0)
{
}


AbstractTemplateSequenceContainerIterator(const AbstractTemplateSequenceContainerIterator& atsi) :
	sequences_(atsi.sequences_),
	currentPosition_(atsi.currentPosition_)
{
}

AbstractSequenceContainerIterator& operator=(const AbstractTemplateSequenceContainerIterator& atsi)
{
	sequences_ = atsi.sequences_;
	currentPosition_ = atsi.currentPosition_;
	return *this;
}

virtual ~AbstractTemplateSequenceContainerIterator() {
}
};

/**
 * @brief Loop over all sequences in a SequenceContainer.
 */
template<class SequenceType, class HashType = std::string>
class SimpleTemplateSequenceContainerIterator :
	public AbstractTemplateSequenceContainerIterator<SequenceType, HashType>
{
public:
SimpleSequenceContainerIterator(const TemplateSequenceContainerInterface<SequenceType, HashType>& sequences) :
	AbstractTemplateSequenceContainerIterator<SequenceType, HashType>(&sequences) {
}

virtual ~SimpleTemplateSequenceContainerIterator() {
}

public:
const SequenceType& nextSequence() override
{
	{
		const Sequence& seq = sequences_->sequence(currentPosition_);
		currentPosition_++;
		return s;
	}

	bool hasMoreSequences() const override
	{
		return currentPosition_ < sequences_->getNumberOfSequences();
	}
}
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCECONTAINERITERATOR_H
