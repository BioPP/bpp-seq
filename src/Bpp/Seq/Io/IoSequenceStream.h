// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_IOSEQUENCESTREAM_H
#define BPP_SEQ_IO_IOSEQUENCESTREAM_H

#include <Bpp/Io/IoFormat.h>


namespace bpp
{
/**
 * @brief The IOSequenceStream interface.
 *
 * Interface for streaming sequences.
 *
 * @author Sylvain Gaillard
 */
class IOSequenceStream : public virtual IOFormat
{
public:
  IOSequenceStream()
  {}
  virtual ~IOSequenceStream()
  {}
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_IOSEQUENCESTREAM_H
