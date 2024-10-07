// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_CLUSTAL_H
#define BPP_SEQ_IO_CLUSTAL_H


#include "../Container/SiteContainer.h"
#include "AbstractIAlignment.h"
#include "AbstractOAlignment.h"

// From the STL:
#include <iostream>

namespace bpp
{
/**
 * @brief The clustal sequence file format.
 *
 * An AlignedSequenceContainer object is used instead of a VectorSequenceContainer.
 */
class Clustal :
  public AbstractIAlignment2,
  public AbstractOAlignment
{
private:
  bool checkNames_;
  unsigned int nbSpacesBeforeSeq_;
  unsigned int charsByLine_;

public:
  /**
   * @brief Build a new Clustal object.
   *
   * @param checkSequenceNames Tell if the names in the file should be checked for unicity (slower, in o(n*n) where n is the number of sequences).
   * @param nbExtraSpacesBeforeSeq Specify the number of extra space characters separating the sequence name form content. The default is 5 (hence 6 spaces in total) for backward compatibility, using 0 will not allow for any space in the sequence names.
   * @param charsByLine Number of character per line when writing file.
   */
  Clustal(bool checkSequenceNames = true, unsigned int nbExtraSpacesBeforeSeq = 5, unsigned int charsByLine = 100) :
    checkNames_(checkSequenceNames),
    nbSpacesBeforeSeq_(nbExtraSpacesBeforeSeq + 1),
    charsByLine_(charsByLine)
  {}

  virtual ~Clustal()
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
   * @name The ISequence interface.
   *
   * As a SiteContainer is a subclass of SequenceContainer, we hereby implement the ISequence
   * interface by downcasting the interface.
   *
   * @{
   */
  std::unique_ptr<SequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readAlignment(input, alpha);
  }

  std::unique_ptr<SequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readAlignment(path, alpha);
  }
  /** @} */

  /**
   * @name The AbstractOAlignment interface.
   *
   * @{
   */
  void writeAlignment(std::ostream& output, const SiteContainerInterface& sc) const override;

  void writeAlignment(const std::string& path, const SiteContainerInterface& sc, bool overwrite = true) const override
  {
    AbstractOAlignment::writeAlignment(path, sc, overwrite);
  }
  /** @} */

  /**
   * @name The IOSequence interface.
   *
   * @{
   */
  const std::string getFormatName() const override
  {
    return "Clustal";
  }

  const std::string getFormatDescription() const override
  {
    return "The Clustal alignment tool output format.";
  }

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
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_CLUSTAL_H
