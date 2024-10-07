// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_NEXUSIOSEQUENCE_H
#define BPP_SEQ_IO_NEXUSIOSEQUENCE_H


#include "../Container/AlignedSequenceContainer.h"
#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "../Sequence.h"
#include "AbstractIAlignment.h"

// From the STL:
#include <iostream>

namespace bpp
{
/**
 * @brief The Nexus format reader for sequences.
 *
 * An AlignedSequenceContainer is used instead of a VectorSequenceContainer.
 *
 * This reader is not supposed to be a full parser of the Nexus files,
 * but only extract the sequence data. Only a basic subset of the options
 * are and will be supported.
 *
 * This format is described in the following paper:
 * Maddison D, Swofford D, and Maddison W (1997), _Syst Biol_ 46(4):590-621
 *
 * @author Julien Dutheil
 */
class NexusIOSequence :
  public AbstractIAlignment2,
  public virtual ISequence
{
protected:
  /**
   * @brief The maximum number of chars to be written on a line.
   */
  unsigned int charsByLine_;

  bool checkNames_;

public:
  /**
   * @brief Build a new Nexus file reader.
   *
   * @param charsByLine The number of base to display in a row (ignored for now, no writing support).
   * @param checkSequenceNames Tell if the names in the file should be checked for unicity (slower, in o(n*n) where n is the number of sequences).
   */
  NexusIOSequence(unsigned int charsByLine = 100, bool checkSequenceNames = true) :
    charsByLine_(charsByLine), checkNames_(checkSequenceNames)
  {}

  virtual ~NexusIOSequence()
  {}

public:
  /**
   * @name The AbstractIAlignment interface.
   *
   * @{
   */
  void appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& sc) const override;
  /** @} */

  /**
   * @name The IOSequence interface.
   *
   * @{
   */
  const std::string getFormatName() const override;

  const std::string getFormatDescription() const override;
  /** @} */

  /**
   * @return true if the names are to be checked when reading sequences from files.
   */
  bool checkNames() const
  {
    return checkNames_;
  }

  /**
   * @brief Tell whether the sequence names should be checked when reading from files.
   *
   * @param yn whether the sequence names should be checked when reading from files.
   */
  void checkNames(bool yn)
  {
    checkNames_ = yn;
  }

private:
  // Reading tools:
  const std::vector<std::string> splitNameAndSequence_(const std::string& s) const;
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_NEXUSIOSEQUENCE_H
