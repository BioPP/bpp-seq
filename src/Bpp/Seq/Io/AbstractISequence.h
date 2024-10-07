// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_ABSTRACTISEQUENCE_H
#define BPP_SEQ_IO_ABSTRACTISEQUENCE_H


#include "../Alphabet/Alphabet.h"
#include "../Container/VectorSequenceContainer.h"
#include "ISequence.h"

// From the STL:
#include <string>
#include <iostream>
#include <fstream>

namespace bpp
{
/**
 * @brief Partial implementation of the ISequence interface.
 */
class AbstractISequence :
  public virtual ISequence
{
public:
  AbstractISequence()
  {}
  virtual ~AbstractISequence()
  {}

public:
  /**
   * @name ISequence methods:
   *
   * @{
   */

public:
  /**
   * @brief Add sequences to a container from a stream.
   *
   * @param input  The input stream to read.
   * @param sc     The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  void readSequences(std::istream& input, SequenceContainerInterface& sc) const override
  {
    appendSequencesFromStream(input, sc);
  }

  /**
   * @brief Add sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param sc    The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  void readSequences(const std::string& path, SequenceContainerInterface& sc) const override
  {
    appendSequencesFromFile(path, sc);
  }

  std::unique_ptr<SequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readSequencesFromStream(input, alpha);
  }

  std::unique_ptr<SequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readSequencesFromFile(path, alpha);
  }
  /** @} */

protected:
  /**
   * @brief Append sequences to a container from a stream.
   *
   * This is the unique method to implement!
   *
   * @param input  The input stream to read.
   * @param sc     The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void appendSequencesFromStream(std::istream& input, SequenceContainerInterface& sc) const = 0;

  /**
   * @brief Append sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param sc    The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void appendSequencesFromFile(const std::string& path, SequenceContainerInterface& sc) const
  {
    std::ifstream input(path.c_str(), std::ios::in);
    if (!input)
      throw IOException("AbstractIAlignment::appendSequencesFromFile: can't read file " + path);
    appendSequencesFromStream(input, sc);
    input.close();
  }

  /**
   * @brief Read sequences from a stream.
   *
   * @param input  The input stream to read.
   * @param alpha  The alphabet to use.
   * @return A sequence container.
   * @throw Exception If the file is not in the specified format.
   */
  std::unique_ptr<SequenceContainerInterface> readSequencesFromStream(std::istream& input, std::shared_ptr<const Alphabet>& alpha) const
  {
    auto vsc = std::unique_ptr<SequenceContainerInterface>(new VectorSequenceContainer(alpha));
    appendSequencesFromStream(input, *vsc);
    return vsc;
  }

  /**
   * @brief Append sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to use.
   * @throw Exception If the file is not in the specified format.
   */
  std::unique_ptr<SequenceContainerInterface> readSequencesFromFile(const std::string& path, std::shared_ptr<const Alphabet>& alpha) const
  {
    auto vsc = std::unique_ptr<SequenceContainerInterface>(new VectorSequenceContainer(alpha));
    appendSequencesFromFile(path, *vsc);
    return vsc;
  }
};


/**
 * @brief Partial implementation of the IProbabilisticSequence interface.
 */
class AbstractIProbabilisticSequence :
  public virtual IProbabilisticSequence
{
public:
  AbstractIProbabilisticSequence()
  {}
  virtual ~AbstractIProbabilisticSequence()
  {}

public:
  /**
   * @name ISequence methods:
   *
   * @{
   */

public:
  /**
   * @brief Add sequences to a container from a stream.
   *
   * @param input  The input stream to read.
   * @param sc     The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  void readSequences(std::istream& input, ProbabilisticSequenceContainerInterface& sc) const override
  {
    appendSequencesFromStream(input, sc);
  }

  /**
   * @brief Add sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param sc    The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  void readSequences(const std::string& path, ProbabilisticSequenceContainerInterface& sc) const override
  {
    appendSequencesFromFile(path, sc);
  }

  std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readSequencesFromStream(input, alpha);
  }

  std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readSequencesFromFile(path, alpha);
  }
  /** @} */

protected:
  /**
   * @brief Append sequences to a container from a stream.
   *
   * This is the unique method to implement!
   *
   * @param input  The input stream to read.
   * @param sc     The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void appendSequencesFromStream(std::istream& input, ProbabilisticSequenceContainerInterface& sc) const = 0;

  /**
   * @brief Append sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param sc    The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void appendSequencesFromFile(const std::string& path, ProbabilisticSequenceContainerInterface& sc) const
  {
    std::ifstream input(path.c_str(), std::ios::in);
    if (!input)
      throw IOException("AbstractIProbabilisticSequences::appendSequencesFromFile: can't read file " + path);
    appendSequencesFromStream(input, sc);
    input.close();
  }

  /**
   * @brief Read sequences from a stream.
   *
   * @param input  The input stream to read.
   * @param alpha  The alphabet to use.
   * @return A sequence container.
   * @throw Exception If the file is not in the specified format.
   */
  std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequencesFromStream(std::istream& input, std::shared_ptr<const Alphabet>& alpha) const
  {
    auto vsc = std::unique_ptr<ProbabilisticSequenceContainerInterface>(new ProbabilisticVectorSequenceContainer(alpha));
    appendSequencesFromStream(input, *vsc);
    return vsc;
  }

  /**
   * @brief Append sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to use.
   * @throw Exception If the file is not in the specified format.
   */
  std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequencesFromFile(const std::string& path, std::shared_ptr<const Alphabet>& alpha) const
  {
    auto vsc = std::unique_ptr<ProbabilisticSequenceContainerInterface>(new ProbabilisticVectorSequenceContainer(alpha));
    appendSequencesFromFile(path, *vsc);
    return vsc;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_ABSTRACTISEQUENCE_H
