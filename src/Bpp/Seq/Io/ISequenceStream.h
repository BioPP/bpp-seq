// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_ISEQUENCESTREAM_H
#define BPP_SEQ_IO_ISEQUENCESTREAM_H

#include <Bpp/Exceptions.h>

#include "../Alphabet/Alphabet.h"
#include "../Sequence.h"
#include "../SequenceWithQuality.h"
#include "../ProbabilisticSequence.h"
#include "IoSequenceStream.h"

namespace bpp
{
/**
 * @brief The ISequenceStream interface.
 *
 * Interface for streaming sequences input.
 *
 * @author Sylvain Gaillard
 */
template<class SequenceType>
class TemplateISequenceStream : public virtual IOSequenceStream
{
public:
  TemplateISequenceStream() {}
  virtual ~TemplateISequenceStream() {}

public:
  /**
   * @brief Read sequence from stream.
   *
   * Read one sequence from a stream.
   *
   * @param input The stream to read.
   * @param seq The sequence to fill.
   * @return true if a sequence was read or false if not.
   * @throw Exception IOExecption and Sequence related Exceptions.
   */
  virtual bool nextSequence(std::istream& input, SequenceType& seq) const = 0;
};

using ISequenceStream = TemplateISequenceStream<Sequence>;
using ISequenceWithQualityStream = TemplateISequenceStream<SequenceWithQuality>;
using IProbabilisticSequenceStream = TemplateISequenceStream<ProbabilisticSequence>;

} // end of namespace bpp.
#endif // BPP_SEQ_IO_ISEQUENCESTREAM_H
