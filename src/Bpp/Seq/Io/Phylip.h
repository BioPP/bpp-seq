//
// File: Phylip.h
// Authors:
//   Julien Dutheil
// Created: 2003-10-27 12:22:56
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_IO_PHYLIP_H
#define BPP_SEQ_IO_PHYLIP_H


#include "../Container/AlignedSequenceContainer.h"
#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "../Sequence.h"
#include "AbstractIAlignment.h"
#include "AbstractOAlignment.h"

// From the STL:
#include <iostream>

namespace bpp
{
/**
 * @brief The Phylip & co format.
 *
 * Sequences must be aligned.
 * This format is described on the Phylip package documentation website:
 * http://evolution.genetics.washington.edu/phylip/doc/sequence.html
 */
class Phylip :
  public AbstractIAlignment2,
  public AbstractOAlignment
{
private:
  /* this class allows two kinds of Phylip format:
   * traditional, with names limited to 10 chars,
   * and 'extended', defined by PAML, with names separated from sequences by at least 6 white spaces.
   */
  bool extended_;
  /* tells if sequences are in the seuqential or the interleave format/
   */
  bool sequential_;

  /**
   * @brief The maximum number of chars to be written on a line.
   */
  unsigned int charsByLine_;

  std::string namesSplit_;

public:
  /**
   * @brief Build a new Phylip file reader.
   *
   * @param extended If true, sequences with names longer than 10 characters are allowed.
   * @param sequential If false, sequences are supposed to be interlaved.
   * @param charsByLine The number of base to display in a row.
   * @param split The string to use to split sequence name from content (only for 'extended' format). This will typically be "  " (two spaces) or "\t" (a tabulation).
   */
  Phylip(bool extended = true, bool sequential = true, unsigned int charsByLine = 100, const std::string& split = "  ") :
    extended_(extended), sequential_(sequential), charsByLine_(charsByLine), namesSplit_(split) {}

  virtual ~Phylip() {}

public:
  /**
   * @name The AbstractIAlignment2 interface.
   *
   * @{
   */
  void appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& sc) const override;
  /** @} */

  /**
   * @name The AbstractISequence interface.
   *
   * As a SiteContainer is a subclass of SequenceContainer, we hereby implement the ISequence
   * interface by downcasting the interface.
   *
   * @{
   */
  virtual std::unique_ptr<SequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readAlignment(input, alpha);
  }
  virtual std::unique_ptr<SequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readAlignment(path, alpha);
  }
  /** @} */

  /**
   * @return The number of sequences contained in the specified file.
   *
   * This methods parses the firt line of the phylip file.
   * @param path The path of the file to parse.
   */
  unsigned int getNumberOfSequences(const std::string& path) const;

  /**
   * @name The AbstractOSequence interface.
   *
   * @{
   */
  void writeAlignment(std::ostream& output, const SiteContainerInterface& sc) const override;
  void writeAlignment(const std::string& path, const SiteContainerInterface& sc, bool overwrite) const override
  {
    AbstractOAlignment::writeAlignment(path, sc, overwrite);
  }
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
   * @return The string used to split sequence name from content.
   */
  const std::string& getSplit() const { return namesSplit_; }

  /**
   * @param split The string to be used to split sequence name from content.
   */
  void setSplit(const std::string& split) { namesSplit_ = split; }

protected:
  // Reading tools:
  const std::vector<std::string> splitNameAndSequence(const std::string& s) const;
  void readSequential (std::istream& in, SequenceContainerInterface & asc) const;
  void readInterleaved(std::istream& in, SequenceContainerInterface & asc) const;
  // Writing tools:
  std::vector<std::string> getSizedNames(const std::vector<std::string>& names) const;
  void writeSequential(std::ostream& out, const SiteContainerInterface& sc) const;
  void writeInterleaved(std::ostream& out, const SiteContainerInterface& sc) const;
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_PHYLIP_H
