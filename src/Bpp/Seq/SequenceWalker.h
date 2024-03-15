// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCEWALKER_H
#define BPP_SEQ_SEQUENCEWALKER_H


#include "Sequence.h"

namespace bpp
{
/**
 * @brief A helper class to easily convert coordinates between sequence and alignments.
 *
 * Coordinates are 0-based.
 * The walker will be more efficient if coordinates are called in pre-ordered way.
 */
class SequenceWalker
{
private:
  const SequenceInterface* seq_;
  size_t seqPos_, alnPos_;
  int gap_;

public:
  SequenceWalker(const SequenceInterface& seq) :
    seq_(&seq), seqPos_(0), alnPos_(0), gap_(seq.getAlphabet()->getGapCharacterCode())
  {
    if (seq_->size() > 0)
    {
      while ((*seq_)[alnPos_] == gap_)
        ++alnPos_;
    }
  }
  SequenceWalker(const SequenceWalker& walker) :
    seq_(walker.seq_), seqPos_(walker.seqPos_), alnPos_(walker.alnPos_), gap_(walker.gap_) {}
  SequenceWalker& operator=(const SequenceWalker& walker)
  {
    seq_    = walker.seq_;
    seqPos_ = walker.seqPos_;
    alnPos_ = walker.alnPos_;
    gap_    = walker.gap_;
    return *this;
  }
  virtual ~SequenceWalker() {}

public:
  size_t getAlignmentPosition(size_t seqPos);
  size_t getSequencePosition(size_t alnPos);
};
} // end of namespace bpp.
#endif // BPP_SEQ_SEQUENCEWALKER_H
