// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "Alphabet/CaseMaskedAlphabet.h"
#include "SequenceWithAnnotationTools.h"

using namespace bpp;
using namespace std;

const string SequenceMask::MASK = "Boolean mask";

/******************************************************************************/

void SequenceMask::afterSequenceChanged(const IntSymbolListEditionEvent& event)
{
	mask_.clear();
	mask_.insert(mask_.begin(), event.getCoreSymbolList()->size(), false);
}

/******************************************************************************/

void SequenceMask::afterSequenceInserted(const IntSymbolListInsertionEvent& event)
{
	mask_.insert(mask_.begin() + static_cast<ptrdiff_t>(event.getPosition()),
	             event.getLength(), false);
}

/******************************************************************************/

void SequenceMask::afterSequenceDeleted(const IntSymbolListDeletionEvent& event)
{
	mask_.erase(mask_.begin() + static_cast<ptrdiff_t>(event.getPosition()),
	            mask_.begin() + static_cast<ptrdiff_t>(event.getPosition() + event.getLength()));
}

/******************************************************************************/

unique_ptr<SequenceWithAnnotation> SequenceWithAnnotationTools::createMaskAnnotation(const Sequence& seq)
{
	auto cma = dynamic_pointer_cast<const CaseMaskedAlphabet>(seq.getAlphabet());
	if (cma)
	{
		auto alphaPtr = seq.getAlphabet();
		auto seqa = make_unique<SequenceWithAnnotation>(seq.getName(), seq.toString(), seq.getComments(), alphaPtr);
		vector<bool> mask(seq.size());
		for (size_t i = 0; i < seq.size(); ++i)
		{
			mask[i] = cma->isMasked(seq[i]);
		}
		seqa->addAnnotation(make_shared<SequenceMask>(mask));
		return seqa;
	}
	else
	{
		throw AlphabetException("SequenceWithAnnotationTools::createMaskAnnotation. Alphabet should be a CaseMaskedAlphabet.", seq.getAlphabet());
	}
}

/******************************************************************************/
