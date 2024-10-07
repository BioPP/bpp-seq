// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_ISEQUENCE_H
#define BPP_SEQ_IO_ISEQUENCE_H

#include <Bpp/Exceptions.h>

#include "../Container/SequenceContainer.h"
#include "../Container/SiteContainer.h"
#include "../Sequence.h"
#include "IoSequence.h"

// From the STL:
#include <iostream>
#include <string>
#include <memory>

namespace bpp
{
/**
 * @brief The ISequence interface.
 *
 * This interface defines the basic methods for reading sequences from a file.
 */
class ISequence :
  public virtual IOSequence
{
public:
  ISequence()
  {}
  virtual ~ISequence()
  {}

public:
  /**
   * @brief Create a new container from a stream.
   *
   * @param input     The input stream to read.
   * @param container The container where to add data.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readSequences(
      std::istream& input,
      SequenceContainerInterface& container) const = 0;
  /**
   * @brief Create a new container from a file.
   *
   * @param path      The path to the file to read.
   * @param container The container where to add data.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readSequences(
      const std::string& path,
      SequenceContainerInterface& container) const = 0;

  /**
   * @brief Create a new container from a stream.
   *
   * @param input  The input stream to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new SequenceContainer object.
   * @throw Exception If the file is not in the specified format.
   */
  virtual std::unique_ptr<SequenceContainerInterface> readSequences(
      std::istream& input,
      std::shared_ptr<const Alphabet> alpha) const = 0;
  /**
   * @brief Create a new container from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new SequenceContainer object.
   * @throw Exception If the file is not in the specified format.
   */
  virtual std::unique_ptr<SequenceContainerInterface> readSequences(
      const std::string& path,
      std::shared_ptr<const Alphabet> alpha) const = 0;
};

/**
 * @brief The IAlignment interface.
 *
 * This interface defines the basic methods for reading aligned sequences from a file.
 */
class IAlignment :
  public virtual IOSequence
{
public:
  IAlignment()
  {}
  virtual ~IAlignment()
  {}

public:
  /**
   * @brief Add the content of a stream to a new container.
   *
   * @param input     The input stream to read.
   * @param container The container where to add data.
   *                  Needs not be a SiteContainer, as aligned sequences can be stored in a simple SequenceContainer.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readAlignment(
      std::istream& input,
      SequenceContainerInterface& container) const = 0;

  /**
   * @brief Create a new container from a file.
   *
   * @param path      The path to the file to read.
   * @param container The container where to add data.
   *                  Needs not be a SiteContainer, as aligned sequences can be stored in a simple SequenceContainer.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readAlignment(
      const std::string& path,
      SequenceContainerInterface& container) const = 0;

  /**
   * @brief Create a new container from a stream.
   *
   * @param input  The input stream to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new SiteContainer object.
   * @throw Exception If the file is not in the specified format.
   */
  virtual std::unique_ptr<SiteContainerInterface> readAlignment(
      std::istream& input,
      std::shared_ptr<const Alphabet> alpha) const = 0;

  /**
   * @brief Create a new container from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new SiteContainer object.
   * @throw Exception If the file is not in the specified format.
   */
  virtual std::unique_ptr<SiteContainerInterface> readAlignment(
      const std::string& path,
      std::shared_ptr<const Alphabet> alpha) const = 0;
};

/**
 * @brief The IProbabilisticSequence interface.
 *
 * This interface defines the basic methods for reading aligned values from a file.
 */

class IProbabilisticSequence :
  public virtual IOProbabilisticSequence
{
public:
  IProbabilisticSequence()
  {}
  virtual ~IProbabilisticSequence()
  {}

public:
  /**
   * @brief Create a new container from a stream.
   *
   * @param input     The input stream to read.
   * @param container The container where to add data.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readSequences(
      std::istream& input,
      ProbabilisticSequenceContainerInterface& container) const = 0;
  /**
   * @brief Create a new container from a file.
   *
   * @param path      The path to the file to read.
   * @param container The container where to add data.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readSequences(
      const std::string& path,
      ProbabilisticSequenceContainerInterface& container) const = 0;

  /**
   * @brief Create a new container from a stream.
   *
   * @param input  The input stream to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new ProbabilisticSequenceContainer object.
   */
  virtual std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequences(
      std::istream& input,
      std::shared_ptr<const Alphabet> alpha) const = 0;

  /**
   * @brief Create a new container from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new ProbabilisticSequenceContainer object.
   */
  virtual std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequences(
      const std::string& path,
      std::shared_ptr<const Alphabet> alpha) const = 0;
};

/**
 * @brief The IProbabilisticSequence interface.
 *
 * This interface defines the basic methods for reading aligned sequences from a file.
 */

class IProbabilisticAlignment :
  public virtual IOProbabilisticSequence
{
public:
  IProbabilisticAlignment()
  {}
  virtual ~IProbabilisticAlignment()
  {}

public:
  /**
   * @brief Add the content of a stream to a new container.
   *
   * @param input     The input stream to read.
   * @param container The container where to add data.
   *                  Needs not be a SiteContainer, as aligned sequences can be stored in a simple SequenceContainer.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readAlignment(
      std::istream& input,
      ProbabilisticSequenceContainerInterface& container) const = 0;

  /**
   * @brief Create a new container from a file.
   *
   * @param path      The path to the file to read.
   * @param container The container where to add data.
   *                  Needs not be a SiteContainer, as aligned sequences can be stored in a simple SequenceContainer.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readAlignment(
      const std::string& path,
      ProbabilisticSequenceContainerInterface& container) const = 0;

  /**
   * @brief Create a new container from a stream.
   *
   * @param input  The input stream to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new ProbabilisticSiteContainer object.
   */
  virtual std::unique_ptr<ProbabilisticSiteContainerInterface> readAlignment(
      std::istream& input,
      std::shared_ptr<const Alphabet> alpha) const = 0;

  /**
   * @brief Create a new container from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new ProbabilisticSiteContainer object.
   */
  virtual std::unique_ptr<ProbabilisticSiteContainerInterface> readAlignment(
      const std::string& path,
      std::shared_ptr<const Alphabet> alpha) const = 0;
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_ISEQUENCE_H
