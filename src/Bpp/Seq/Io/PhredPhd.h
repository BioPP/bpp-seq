// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_PHREDPHD_H
#define BPP_SEQ_IO_PHREDPHD_H


#include "../Sequence.h"
#include "../SequenceWithQuality.h"
#include "ISequenceStream.h"

namespace bpp
{
/**
 * @brief The phd sequence file format from phred software.
 *
 * This class read DNA SequenceWithQuality from phd files produced by the
 * phred program from the University of Washington.
 *
 * @par Usage
 *
 * @code
 * // Creating a SequenceWithQuality object
 * DNA alpha;
 * SequenceWithQuality seq(&alpha);
 * std::vector<int> pos;
 *
 * // Create a PhredPhd parser
 * PhredPhd pp;
 *
 * // Opening the file
 * std::ifstream in("my_sequence.phd");
 *
 * // Read the sequence
 * pp.nextSequence(in, seq, pos);
 *
 * // Close the file
 * in.close();
 * @endcode
 *
 * @author Sylvain Gaillard
 */
class PhredPhd : public ISequenceStream
{
public:
  /**
   * @brief Build a new PhredPhd object.
   */
  PhredPhd() {}

  virtual ~PhredPhd() {}

public:
  /**
   * @name The ISequenceStream interface.
   *
   * @{
   */
  bool nextSequence(
    std::istream& input,
    Sequence& seq
    ) const;
  /** @} */

  /**
   * @brief Read a SequenceWithQuality from stream and store chromatographic positions
   *
   * A more complete parser that read a SequenceWithQuality and store
   * the position of each base call on the chromatogram in a vector of
   * int.
   *
   * @param input The stram to read.
   * @param seq The sequence to fill.
   * @param pos The vector of positions to fill.
   * @throw Exception IOException and Sequence related exceptions.
   */
  bool nextSequence(
    std::istream& input,
    Sequence& seq,
    std::vector<size_t>& pos
    ) const;

  /**
   * @name The IOFormat interface.
   *
   * @{
   */
  const std::string getDataType() const { return "SequenceWithQuality"; }
  const std::string getFormatName() const { return "phd file"; }
  const std::string getFormatDescription() const
  {
    return "Sequences following the phd format as describe in the phred documentation.";
  }
  /** @} */

private:
  /**
   * @brief Global file parser
   *
   * @param input The stream to read
   * @param name The string to store the sequence name
   * @param sequence The string to store the sequence
   * @param qual The vector to store qualities
   * @param pos The vector to store positions
   */
  bool parseFile_(std::istream& input, std::string& name, std::string& sequence, std::vector<int>& qual, std::vector<size_t>& pos) const;

  /**
   * @brief Parse the DNA part of the file
   *
   * Read the DNA part until `END_DNA' or EOF.
   *
   * @param input The stream to read
   * @param sequence The string to store the sequence
   * @param qual The vector to store qualities
   * @param pos The vector to store positions
   */
  bool parseDNA_(std::istream& input, std::string& sequence, std::vector<int>& qual, std::vector<size_t>& pos) const;
};
} // end of namespace bpp
#endif // BPP_SEQ_IO_PHREDPHD_H
