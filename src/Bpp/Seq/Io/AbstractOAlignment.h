// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_ABSTRACTOALIGNMENT_H
#define BPP_SEQ_IO_ABSTRACTOALIGNMENT_H


#include "../Alphabet/Alphabet.h"
#include "../Container/VectorSequenceContainer.h"
#include "OSequence.h"

// From the STL:
#include <string>
#include <fstream>

namespace bpp
{
/**
 * @brief Partial implementation of the OAlignment interface.
 */
class AbstractOAlignment :
  public virtual OAlignment
{
public:
  AbstractOAlignment() {}
  virtual ~AbstractOAlignment() {}

public:
  /**
   * @name OAlignment methods:
   *
   * @{
   */
  void writeAlignment(std::ostream& output, const SiteContainerInterface& sc) const override = 0;

  void writeAlignment(const std::string& path, const SiteContainerInterface& sc, bool overwrite = true) const override
  {
    // Open file in specified mode
    std::ofstream output(path.c_str(), overwrite ? (std::ios::out) : (std::ios::out | std::ios::app));
    writeAlignment(output, sc);
    output.close();
  }
  /** @} */
};

/**
 * @brief Partial implementation of the OProbabilisticAlignment interface.
 */
class AbstractOProbabilisticAlignment :
  public virtual OProbabilisticAlignment
{
public:
  AbstractOProbabilisticAlignment() {}
  virtual ~AbstractOProbabilisticAlignment() {}

public:
  /**
   * @name OProbabilisticAlignment methods:
   *
   * @{
   */
  void writeAlignment(std::ostream& output, const ProbabilisticSiteContainerInterface& psc) const override = 0;

  void writeAlignment(const std::string& path, const ProbabilisticSiteContainerInterface& psc, bool overwrite = true) const override
  {
    // Open file in specified mode
    std::ofstream output(path.c_str(), overwrite ? (std::ios::out) : (std::ios::out | std::ios::app));
    writeAlignment(output, psc);
    output.close();
  }

  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_ABSTRACTOALIGNMENT_H
