// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <memory>

using namespace std;

#include "Transliterator.h"

using namespace bpp;

unique_ptr<Sequence> AbstractTransliterator::translate(const SequenceInterface& sequence) const
{
	if (sequence.alphabet().getAlphabetType() != sourceAlphabet().getAlphabetType())
		throw AlphabetMismatchException("AbstractTransliterator::translate", getSourceAlphabet(), getTargetAlphabet());
	auto alphaPtr = getTargetAlphabet();
	auto tSeq = make_unique<Sequence>(sequence.getName(), "", sequence.getComments(), alphaPtr);
	int gap = sequence.getAlphabet()->getGapCharacterCode();
	for (size_t i = 0; i < sequence.size(); ++i)
	{
		int state = sequence.getValue(i);
		if (state == gap)
			tSeq->addElement(gap);
		else
			tSeq->addElement(translate(state));
	}
	return tSeq;
}

unique_ptr<Sequence> AbstractReverseTransliterator::reverse(const SequenceInterface& sequence) const
{
	if (sequence.alphabet().getAlphabetType() != targetAlphabet().getAlphabetType())
		throw AlphabetMismatchException("AbstractReverseTransliterator::reverse", getSourceAlphabet(), getTargetAlphabet());
	auto alphaPtr = getSourceAlphabet();
	auto rSeq = make_unique<Sequence>(sequence.getName(), "", sequence.getComments(), alphaPtr);
	for (size_t i = 0; i < sequence.size(); ++i)
	{
		rSeq->addElement(reverse(sequence.getValue(i)));
	}
	return rSeq;
}
