// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <iostream>


using namespace std;

#include "SequenceWalker.h"

#include <Bpp/Text/TextTools.h>

using namespace bpp;

size_t SequenceWalker::getAlignmentPosition(size_t seqPos)
{
  if (seqPos == seqPos_)
    return alnPos_;
  if (seqPos > seqPos_)
  {
    // Move forward
    while (alnPos_ < seq_->size() && seqPos_ < seqPos)
    {
      if (alnPos_ == seq_->size() - 1)
        throw Exception("SequenceWalker::getAlignmentPosition(). Forward1. Position out of bound.");
      ++alnPos_;

      if ((*seq_)[alnPos_] != gap_)
      {
        ++seqPos_;
      }
    }
    if (seqPos_ != seqPos)
      throw Exception("SequenceWalker::getAlignmentPosition(). Forward2. Position out of bound (" + TextTools::toString(alnPos_) + ")");
  }
  else
  {
    // Move backward
    if (alnPos_ == 0)
      throw Exception("SequenceWalker::getAlignmentPosition(). Backward. Position out of bound.");
    while (alnPos_ > 0 && seqPos_ > seqPos)
    {
      --alnPos_;
      if ((*seq_)[alnPos_] != gap_)
      {
        --seqPos_;
      }
    }
    if (seqPos_ != seqPos)
      throw Exception("SequenceWalker::getAlignmentPosition(). Position out of bound.");
  }
  return alnPos_;
}

size_t SequenceWalker::getSequencePosition(size_t alnPos)
{
  if (alnPos == alnPos_)
    return seqPos_;
  if (alnPos >= seq_->size())
    throw Exception("SequenceWalker::getSequencePosition(). Position out of bound.");
  if (alnPos > alnPos_)
  {
    // Move forward
    while (alnPos_ < alnPos)
    {
      ++alnPos_;
      if ((*seq_)[alnPos_] != gap_)
      {
        ++seqPos_;
      }
    }
  }
  else
  {
    // Move backward
    while (alnPos_ > alnPos)
    {
      if (seqPos_ == 0)
        return 0;
      --alnPos_;
      if ((*seq_)[alnPos_ + 1] != gap_)
      {
        --seqPos_;
      }
    }
  }
  return seqPos_;
}
