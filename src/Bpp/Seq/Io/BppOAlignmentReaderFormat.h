// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_BPPOALIGNMENTREADERFORMAT_H
#define BPP_SEQ_IO_BPPOALIGNMENTREADERFORMAT_H


#include "IoSequenceFactory.h"

namespace bpp
{
/**
 * @brief Sequence I/O in BppO format.
 *
 * Creates a new IAlignment object according to
 * distribution description syntax (see the Bio++ Program Suite
 * manual for a detailed description of this syntax).
 *
 */
class BppOAlignmentReaderFormat :
  public virtual IOFormat
{
private:
  std::map<std::string, std::string> unparsedArguments_;
  int warningLevel_;

public:
  BppOAlignmentReaderFormat(int warningLevel) :
    unparsedArguments_(), warningLevel_(warningLevel)
  {}
  virtual ~BppOAlignmentReaderFormat()
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
    return "Alignment reader";
  }

  /**
   * @brief Read a IAlignment object from a string.
   *
   * @param description A string describing the reader in the keyval syntax.
   * @return A new IAlignment object according to options specified.
   * @throw Exception if an error occurred.
   */
  std::unique_ptr<IAlignment> read(const std::string& description);

  std::unique_ptr<IProbabilisticAlignment> readProbabilistic(const std::string& description);

  /**
   * @return The arguments and their unparsed values from the last call of the read function, if there are any.
   */
  virtual const std::map<std::string, std::string>& getUnparsedArguments() const
  {
    return unparsedArguments_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_BPPOALIGNMENTREADERFORMAT_H
