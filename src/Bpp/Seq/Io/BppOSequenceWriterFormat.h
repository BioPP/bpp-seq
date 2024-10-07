// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_BPPOSEQUENCEWRITERFORMAT_H
#define BPP_SEQ_IO_BPPOSEQUENCEWRITERFORMAT_H


#include "IoSequenceFactory.h"

namespace bpp
{
/**
 * @brief Sequence I/O in BppO format.
 *
 * Creates a new OSequence object according to
 * distribution description syntax (see the Bio++ Program Suite
 * manual for a detailed description of this syntax).
 *
 */
class BppOSequenceWriterFormat :
  public virtual IOFormat
{
private:
  std::map<std::string, std::string> unparsedArguments_;
  int warningLevel_;

public:
  BppOSequenceWriterFormat(int warningLevel) :
    unparsedArguments_(), warningLevel_(warningLevel)
  {}
  virtual ~BppOSequenceWriterFormat()
  {}

public:
  const std::string getFormatName() const
  {
    return "BppO";
  }

  const std::string getFormatDescription() const
  {
    return "Bpp Options format.";
  }

  const std::string getDataType() const
  {
    return "Sequence reader";
  }

  /**
   * @brief Read a OSequence object from a string.
   *
   * @param description A string describing the reader in the keyval syntax.
   * @return A new OSequence object according to options specified.
   * @throw Exception if an error occurred.
   */
  std::unique_ptr<OSequence> read(const std::string& description);

  /**
   * @return The arguments and their unparsed values from the last call of the read function, if there are any.
   */
  virtual const std::map<std::string, std::string>& getUnparsedArguments() const
  {
    return unparsedArguments_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_BPPOSEQUENCEWRITERFORMAT_H
