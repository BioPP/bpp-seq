// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_OSEQUENCESTREAM_H
#define BPP_SEQ_IO_OSEQUENCESTREAM_H

#include <Bpp/Exceptions.h>

#include "../Alphabet/Alphabet.h"
#include "../Sequence.h"
#include "IoSequenceStream.h"

namespace bpp
{
/**
 * @brief The OSequenceStream interface.
 *
 * Interface for streaming sequences output.
 *
 * @author Sylvain Gaillard
 */
template<class SequenceType>
class TemplateOSequenceStream :
  public virtual IOSequenceStream
{
public:
  TemplateOSequenceStream()
  {}
  virtual ~TemplateOSequenceStream()
  {}

public:
  /**
   * @brief Read sequence from stream.
   *
   * Read one sequence from a stream.
   *
   * @param output The stream where write.
   * @param seq The sequence to write.
   * @throw Exception IOExecption.
   */
  virtual void writeSequence(std::ostream& output, const SequenceType& seq) const = 0;
};

using OSequenceStream = TemplateOSequenceStream<Sequence>;
using OSequenceWithQualityStream = TemplateOSequenceStream<SequenceWithQuality>;
using OProbabilisticSequenceStream = TemplateOSequenceStream<ProbabilisticSequence>;
} // end of namespace bpp.
#endif // BPP_SEQ_IO_OSEQUENCESTREAM_H
