// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_OSEQUENCE_H
#define BPP_SEQ_IO_OSEQUENCE_H

#include <Bpp/Exceptions.h>

#include "../Container/SequenceContainer.h"
#include "../Container/SiteContainer.h"
#include "IoSequence.h"

namespace bpp
{
/**
 * @brief The OSequence interface.
 *
 * This interface defines the basic methods for writing sequences to a file.
 */
class OSequence :
  public virtual IOSequence
{
public:
  OSequence() {}
  virtual ~OSequence() {}

public:
  /**
   * @brief Write a container to a stream.
   *
   * @param output The output stream where to write.
   * @param sc        The container to write.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void writeSequences(std::ostream& output, const SequenceContainerInterface& sc) const = 0;

  /**
   * @brief Write a container to a file.
   *
   * @param path      The path to the file to write.
   * @param sc        The container to write.
   * @param overwrite If true the sequences are written at the beginning of the file instead of being appended.
   *                  Any previous content will be lost.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void writeSequences(const std::string& path, const SequenceContainerInterface& sc, bool overwrite) const = 0;
};


/**
 * @brief The OAlignment interface.
 *
 * This interface defines the basic methods for writing alignments to a file.
 */
class OAlignment :
  public virtual IOSequence
{
public:
  OAlignment() {}
  virtual ~OAlignment() {}

public:
  /**
   * @brief Write a container to a stream.
   *
   * @param output The output stream where to write.
   * @param sc        The container to write.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void writeAlignment(std::ostream& output, const SiteContainerInterface& sc) const = 0;

  /**
   * @brief Write a container to a file.
   *
   * @param path      The path to the file to write.
   * @param sc        The container to write.
   * @param overwrite If true the sequences are written at the beginning of the file instead of being appended.
   *                  Any previous content will be lost.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void writeAlignment(const std::string& path, const SiteContainerInterface& sc, bool overwrite) const = 0;
};


/**
 * @brief The OProbabilisticSequence interface.
 *
 * This interface defines the basic methods for writing proba
 * sequence to a file.
 */
class OProbabilisticSequence :
  public virtual IOProbabilisticSequence
{
public:
  OProbabilisticSequence() {}
  virtual ~OProbabilisticSequence() {}

public:
  /**
   * @brief Write a container to a stream.
   *
   * @param output The output stream where to write.
   * @param sc        The container to write.
   */
  virtual void writeSequences(std::ostream& output, const ProbabilisticSequenceContainerInterface& sc) const = 0;

  /**
   * @brief Write a container to a file.
   *
   * @param path      The path to the file to write.
   * @param sc        The container to write.
   * @param overwrite If true the sequences are written at the beginning of the file instead of being appended.
   *                  Any previous content will be lost.
   */
  virtual void writeSequences(const std::string& path, const ProbabilisticSequenceContainerInterface& sc, bool overwrite) const = 0;
};

/**
 * @brief The OProbabilisticAlignment interface.
 *
 * This interface defines the basic methods for writing alignments to a file.
 */

class OProbabilisticAlignment :
  public virtual IOProbabilisticSequence
{
public:
  OProbabilisticAlignment() {}
  virtual ~OProbabilisticAlignment() {}

public:
  /**
   * @brief Write a container to a stream.
   *
   * @param output The output stream where to write.
   * @param sc        The container to write.
   */
  virtual void writeAlignment(std::ostream& output, const ProbabilisticSiteContainerInterface& sc) const = 0;

  /**
   * @brief Write a container to a file.
   *
   * @param path      The path to the file to write.
   * @param sc        The container to write.
   * @param overwrite If true the sequences are written at the beginning of the file instead of being appended.
   *                  Any previous content will be lost.
   */
  virtual void writeAlignment(const std::string& path, const ProbabilisticSiteContainerInterface& sc, bool overwrite) const = 0;
  
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_OSEQUENCE_H
