// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_FASTACSV_H
#define BPP_SEQ_IO_FASTACSV_H


#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "../Sequence.h"
#include "AbstractIAlignment.h"
#include "AbstractISequence.h"
#include "AbstractOSequence.h"
#include "ISequenceStream.h"
#include "OSequenceStream.h"

namespace bpp
{
/**
 * @brief The FastaCsv sequence file format.
 *
 * Read and write from/to coma separated values files : similar to the
 * Fasta format, but states are separated by ",".
 * 
 */

  class FastaCsv :
    public AbstractISequence,
    public AbstractIAlignment,
    public AbstractOSequence2,
    public virtual ISequenceStream,
    public virtual OSequenceStream
  {
protected:
  /**
   * @brief The maximum number of chars to be written on a line.
   */
  unsigned int charsByLine_;   // Number of char by line (output only)
  bool extended_;              // If using HUPO-PSI extensions
  bool strictNames_;           // If name is between '>' and first space

public:
  /**
   * @brief Build a new FastaCsv object.
   *
   * @param charsByLine Number of character per line when writing files.
   * @param extended Tells if we should read general comments and sequence comments in HUPO-PSI format.
   * @param strictSequenceNames Tells if the sequence names should be restricted to the characters between '>' and the first blank one.
   */
  FastaCsv(unsigned int charsByLine = 100, bool extended = false, bool strictSequenceNames = false) : charsByLine_(charsByLine), extended_(extended), strictNames_(strictSequenceNames)
  {}

  // Class destructor
  virtual ~FastaCsv()
  {}

public:
  /**
   * @name The AbstractISequence interface.
   *
   * @{
   */
  void appendSequencesFromStream(std::istream& input, SequenceContainerInterface& sc) const override;
  /** @} */

  /**
   * @name The AbstractIAlignment interface.
   *
   * @{
   */
  void appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& sc) const override
  {
    appendSequencesFromStream(input, sc); // This may raise an exception if sequences are not aligned!
  }
  /** @} */

  /**
   * @name The OSequence interface.
   *
   * @{
   */
  void writeSequences(std::ostream& output, const SequenceContainerInterface& sc) const override;

  void writeSequences(const std::string& path, const SequenceContainerInterface& sc, bool overwrite = true) const override
  {
    AbstractOSequence::writeSequences(path, sc, overwrite);
  }
  /** @} */

  /**
   * @name The IOSequence interface.
   *
   * @{
   */
  const std::string getFormatName() const override
  {
    return "FASTACSV file";
  }

  const std::string getFormatDescription() const override
  {
    return "Sequence name (preceded by >) in one line, sequence content, no comments";
  }
  /** @} */

  /**
   * @name The ISequenceStream interface.
   *
   * @{
   */
  bool nextSequence(std::istream& input, Sequence& seq) const override;
  /** @} */

  /**
   * @name The OSequenceStream interface.
   *
   * @{
   */
  void writeSequence(std::ostream& output, const Sequence& seq) const override;
  /** @} */

  /**
   * @return true if the sequence name is restricted to be between '>' and the first space character.
   */
  bool strictNames() const
  {
    return strictNames_;
  }

  /**
   * @brief Tell wethed the sequence name should be restrected to the first non blank characters.
   *
   * @param yn whether the sequence names should be restrected.
   */
  void strictNames(bool yn)
  {
    strictNames_ = yn;
  }

  };
} // end of namespace bpp.
#endif // BPP_SEQ_IO_FASTACSV_H
