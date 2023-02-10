//
// File: AbstractOSequence.h
// Authors:
//   Julien Dutheil
// Created: ?
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

#ifndef BPP_SEQ_IO_ABSTRACTOSEQUENCE_H
#define BPP_SEQ_IO_ABSTRACTOSEQUENCE_H


#include "../Alphabet/Alphabet.h"
#include "../Container/VectorSequenceContainer.h"
#include "../ProbabilisticSequence.h"
#include "OSequence.h"

// From the STL:
#include <string>
#include <fstream>

namespace bpp
{
/**
 * @brief Partial implementation of the OSequence interface.
 */
class AbstractOSequence :
  public virtual OSequence
{
public:
  AbstractOSequence() {}
  virtual ~AbstractOSequence() {}

public:

  /**
   * @name OSequence methods:
   *
   * @{
   */
  void writeSequences(std::ostream& output, const SequenceContainerInterface& sc) const override = 0;

  void writeSequences(const std::string& path, const SequenceContainerInterface& sc, bool overwrite = true) const override
  {
    // Open file in specified mode
    std::ofstream output(path.c_str(), overwrite ? (std::ios::out) : (std::ios::out | std::ios::app));
    writeSequences(output, sc);
    output.close();
  }
  /** @} */

};

/**
 * @brief Partial implementation of the OAlignment interface.
 *
 * This implementation is for sequence formats that can store both aligned and non-aligned sequences.
 * Alignment sequences are written as simple sequences with gaps.
 */
class AbstractOSequence2 :
  public virtual AbstractOSequence, //in case for Diamond inheritence
  public virtual OAlignment
{
public:
  AbstractOSequence2() {}
  virtual ~AbstractOSequence2() {}

public:

  /**
   * @name OAlignment methods:
   *
   * As a SiteContainer is a specialization of SequenceContainer, it is assumed that a OSequence
   * object can write aligned sequence just like a OAlignment object.
   * Therefore it implements the OAlignment interface by down-casting the SiteContainer
   * to a SequenceContainer.
   * @{
   */
  void writeAlignment(std::ostream& output, const SiteContainerInterface& sc) const override
  {
    writeSequences(output, sc);
  }

  void writeAlignment(const std::string& path, const SiteContainerInterface& sc, bool overwrite = true) const override
  {
    writeSequences(path, sc, overwrite);
  }
  /** @} */
};

/**
 * @brief Partial implementation of the OProbabilisticSequence interface.
 */
class AbstractOProbabilisticSequence :
  public virtual OProbabilisticSequence
{
public:
  AbstractOProbabilisticSequence() {}
  virtual ~AbstractOProbabilisticSequence() {}

public:
  /**
   * @name OProbabilisticSequence methods:
   *
   * @{
   */
  void writeSequences(std::ostream& output, const ProbabilisticSequenceContainerInterface& psc) const override = 0;

  void writeSequences(const std::string& path, const ProbabilisticSequenceContainerInterface& psc, bool overwrite = true) const override
  {
    // Open file in specified mode
    std::ofstream output(path.c_str(), overwrite ? (std::ios::out) : (std::ios::out | std::ios::app));
    writeSequences(output, psc);
    output.close();
  }

  /** @} */
};

/**
 * @brief Partial implementation of the OProbabilisticAlignment interface.
 * 
 * This implementation is for formats that can store both aligned and non-aligned sequences.
 * Alignment sequences are written as simple sequences with gaps.
 */
class AbstractOProbabilisticSequence2 :
  public AbstractOProbabilisticSequence,
  public virtual OProbabilisticAlignment
{
public:
  AbstractOProbabilisticSequence2() {}
  virtual ~AbstractOProbabilisticSequence2() {}

public:
  /**

   * @name OProbabilisticAlignment methods:
   *
   * As a ProbabilisticSiteContainer is a specialization of
   * ProbabilisticSequenceContainer, it is assumed that a OProbabilisticSequence
   * object can write aligned sequence just like a OProbabilisticAlignment
   * object. Therefore it implements the OProbabilisticAlignment interface by
   * down-casting the ProbabilisticSiteContainer to a
   * ProbabilisticSequenceContainer.
   *
   * @{
   */
  void writeAlignment(std::ostream& output, const ProbabilisticSiteContainerInterface& sc) const override
  {
    writeSequences(output, sc);
  }

  void writeAlignment(const std::string& path, const ProbabilisticSiteContainerInterface& sc, bool overwrite = true) const override
  {
    writeSequences(path, sc, overwrite);
  }

  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_ABSTRACTOSEQUENCE_H
