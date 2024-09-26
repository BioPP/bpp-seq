// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/VectorTools.h>

#include "SequenceWithQuality.h"

using namespace bpp;

#include <string>
#include <iostream>

using namespace std;

const string SequenceQuality::QUALITY_SCORE = "Quality score";
const int SequenceQuality::DEFAULT_QUALITY_VALUE = 20;

/******************************************************************************/

void SequenceQuality::afterSequenceChanged(const IntSymbolListEditionEvent& event)
{
	qualScores_.clear();
	qualScores_.insert(qualScores_.begin(), event.getCoreSymbolList()->size(), DEFAULT_QUALITY_VALUE);
}

/******************************************************************************/

void SequenceQuality::afterSequenceInserted(const IntSymbolListInsertionEvent& event)
{
	qualScores_.insert(
		qualScores_.begin() + static_cast<ptrdiff_t>(event.getPosition()),
		event.getLength(), DEFAULT_QUALITY_VALUE);
}

/******************************************************************************/

void SequenceQuality::afterSequenceDeleted(const IntSymbolListDeletionEvent& event)
{
	qualScores_.erase(
		qualScores_.begin() + static_cast<ptrdiff_t>(event.getPosition()),
		qualScores_.begin() + static_cast<ptrdiff_t>(event.getPosition() + event.getLength()));
}

/******************************************************************************/
