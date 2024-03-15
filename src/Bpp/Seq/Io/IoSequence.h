// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_IOSEQUENCE_H
#define BPP_SEQ_IO_IOSEQUENCE_H

#include <Bpp/Exceptions.h>
#include <Bpp/Io/IoFormat.h>


// From STL:
#include <string>

namespace bpp
{
/**
 * @brief The IOSequence interface.
 *
 * Interface for sequences input/ouput.
 */
class IOSequence : public virtual IOFormat
{
public:
  IOSequence() {}
  virtual ~IOSequence() {}

public:
  const std::string getDataType() const { return "Sequence container"; }
};

/**
 * @brief The IOProbabislisticSequence interface.
 *
 * Interface for probabilistic sequences input/ouput.
 */
class IOProbabilisticSequence : public virtual IOFormat
{
public:
  IOProbabilisticSequence() {}
  virtual ~IOProbabilisticSequence() {}

public:
  const std::string getDataType() const { return "Probabilistic sequence container"; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_IOSEQUENCE_H
