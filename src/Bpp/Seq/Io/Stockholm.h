// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_STOCKHOLM_H
#define BPP_SEQ_IO_STOCKHOLM_H


#include "../Container/AlignedSequenceContainer.h"
#include "../Container/SequenceContainer.h"
#include "../Sequence.h"
#include "AbstractOAlignment.h"

namespace bpp
{
/**
 * @brief The Stockholm alignment file format.
 *
 * Write to Stockholm files.
 * Only sequence data is read/written, annotation and secondary structures are ignored.
 */
class Stockholm :
  public AbstractOAlignment
{
private:
  bool checkNames_;

public:
  /**
   * @brief Build a new Stockholm object.
   *
   * @param checkSequenceNames Tell if the names in the file should be checked for unicity (slower, in o(n*n) where n is the number of sequences).
   */
  Stockholm(bool checkSequenceNames = true) : checkNames_(checkSequenceNames)
  {}

  // Class destructor
  virtual ~Stockholm()
  {}

public:
  /**
   * @name The OAlignment interface.
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
    return "Stockholm file";
  }
  const std::string getFormatDescription() const override
  {
    return "See http://en.wikipedia.org/wiki/Stockholm_format";
  }
  /** @} */

  /**
   * @warning This is not used for now, will be when reading is implemented.
   * @return true if the names are to be checked when reading sequences from files.
   */
  bool checkNames() const
  {
    return checkNames_;
  }

  /**
   * @brief Tell whether the sequence names should be checked when reading from files.
   *
   * @warning This is not used for now, will be when reading is implemented.
   * @param yn whether the sequence names should be checked when reading from files.
   */
  void checkNames(bool yn)
  {
    checkNames_ = yn;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_STOCKHOLM_H
