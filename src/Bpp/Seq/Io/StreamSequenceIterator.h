// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_STREAMSEQUENCEITERATOR_H
#define BPP_SEQ_IO_STREAMSEQUENCEITERATOR_H


#include "../SequenceIterator.h"
#include "ISequenceStream.h"

// From the STL:
#include <istream>

namespace bpp
{
/**
 * @brief A sequence iterator based on a sequence stream.
 */
template<class SequenceType>
class TemplateStreamSequenceIterator :
	public virtual TemplateSequenceIteratorInterface<SequenceType>
{
private:
std::shared_ptr<const Alphabet> alphabet_;
std::shared_ptr<const ISequenceStream<SequenceType> > seqStream_;
str::shared_ptr<std::istream> stream_;
SequenceType* nextSeq_;

public:
StreamSequenceIterator(
	std::shared_ptr<ISequenceStream> seqStream,
	std::shared_ptr<std::istream> stream,
	std::shared_ptr<const Alphabet> alphabet) :
	alphabet_(alphabet),
	seqStream_(seqStream),
	stream_(stream),
	nextSeq_(new SequenceType(alphabet_))

{
	bool test = seqStream_->nextSequence(*stream_, *nextSeq_);
	if (!test)
	{
		delete nextSeq_;
		nextSeq_ = 0; // No more sequence available
	}
}

virtual ~StreamSequenceIterator() {
}

private:
// Recopy is forbidden
StreamSequenceIterator(const StreamSequenceIterator& ssi) :
	alphabet_(ssi.alphabet_),
	seqStream_(ssi.seqStream_),
	stream_(ssi.stream_),
	nextSeq_(0) {
}

StreamSequenceIterator& operator=(const StreamSequenceIterator& ssi)
{
	alphabet_  = ssi.alphabet_;
	seqStream_ = ssi.seqStream_;
	stream_    = ssi.stream_;
	nextSeq_   = 0;
	return *this;
}

public:
std::unique_ptr<SequenceType> nextSequence() override
{
	unique_ptr<SequenceType> seq(nextSeq_);
	if (nextSeq_)
	{
		nextSeq_ = new SequenceType(alphabet_);
		bool test = seqStream_->nextSequence(*stream_, *nextSeq_);
		if (!test)
		{
			delete nextSeq_;
			nextSeq_ = 0; // No more sequence available
		}
	}
	return seq;
}

bool hasMoreSequences() const override {
	return nextSeq_ != 0;
}
};

using StreamSequenceIterator = TemplateStreamSequenceIterator<Sequence>
                               using StreamSequenceWithQualityIterator = TemplateStreamSequenceIterator<SequenceWithQuality>
                                                                         using StreamProbabilisticSequenceIterator = TemplateStreamSequenceIterator<ProbabilisticSequence>
} // end of namespace bpp.
#endif // BPP_SEQ_IO_STREAMSEQUENCEITERATOR_H
