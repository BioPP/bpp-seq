// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_ABSTRACTIALIGNMENT_H
#define BPP_SEQ_IO_ABSTRACTIALIGNMENT_H


#include "../Alphabet/Alphabet.h"
#include "../Container/AlignedSequenceContainer.h"
#include "../Container/VectorSiteContainer.h"
#include "AbstractISequence.h"

// From the STL:
#include <string>
#include <iostream>
#include <fstream>

namespace bpp
{
/**
 * @brief Partial implementation of the IAlignment interface, dedicated to alignment readers.
 */
class AbstractIAlignment :
  public virtual IAlignment
{
public:
  AbstractIAlignment() {}
  virtual ~AbstractIAlignment() {}

public:
  /**
   * @name IAlignment methods:
   *
   * @{
   */

  /**
   * @brief Add sequences to a container from a stream.
   *
   * @param input  The input stream to read.
   * @param sc     The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readAlignment(std::istream& input, SequenceContainerInterface& sc) const override
  {
    appendAlignmentFromStream(input, sc);
  }

  /**
   * @brief Add sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param sc    The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void readAlignment(const std::string& path, SequenceContainerInterface& sc) const override
  {
    appendAlignmentFromFile(path, sc);
  }

  std::unique_ptr<SiteContainerInterface> readAlignment(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readAlignmentFromFile(path, alpha);
  }

  std::unique_ptr<SiteContainerInterface> readAlignment(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readAlignmentFromStream(input, alpha);
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
  virtual void appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& sc) const = 0;

  /**
   * @brief Append sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param sc    The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void appendAlignmentFromFile(const std::string& path, SequenceContainerInterface& sc) const
  {
    std::ifstream input(path.c_str(), std::ios::in);
    if (!input)
      throw IOException("AbstractIAlignment::appendAlignmentFromFile: can't read file " + path);
    appendAlignmentFromStream(input, sc);
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
  virtual std::unique_ptr<SiteContainerInterface> readAlignmentFromStream(std::istream& input, std::shared_ptr<const Alphabet> alpha) const
  {
    auto asc = std::unique_ptr<SiteContainerInterface>(new AlignedSequenceContainer(alpha));
    appendAlignmentFromStream(input, *asc);
    return asc;
  }

  /**
   * @brief Read sequences from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to use.
   * @return A sequence container.
   * @throw Exception If the file is not in the specified format.
   */
  virtual std::unique_ptr<SiteContainerInterface> readAlignmentFromFile(const std::string& path, std::shared_ptr<const Alphabet> alpha) const
  {
    auto asc = std::unique_ptr<SiteContainerInterface>(new AlignedSequenceContainer(alpha));
    appendAlignmentFromFile(path, *asc);
    return asc;
  }
};


/**
 * @brief Partial implementation of the IAlignment and ISequence interface, dedicated to alignment readers.
 *
 * This implementation adds the ISequence interface, assuming that aligned sequences are no less than sequence themselves and can be read as such;
 */
class AbstractIAlignment2 :
  public virtual AbstractIAlignment,
  // In case of diamond inheritence
  public virtual ISequence
{
public:
  AbstractIAlignment2() {}
  virtual ~AbstractIAlignment2() {}

public:
  /**
   * @name The ISequence interface.
   *
   * As a SiteContainer is a subclass of SequenceContainer, we hereby implement the ISequence
   * interface by downcasting the interface.
   *
   * @{
   */
  void readSequences(std::istream& input, SequenceContainerInterface& sc) const override
  {
    appendAlignmentFromStream(input, sc);
  }

  void readSequences(const std::string& path, SequenceContainerInterface& sc) const override
  {
    appendAlignmentFromFile(path, sc);
  }

  std::unique_ptr<SequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
  {
    auto sic = readAlignment(input, alpha);
    std::unique_ptr<SequenceContainerInterface> sec = std::move(sic);
    return sec;
  }

  std::unique_ptr<SequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
  {
    auto sic = readAlignment(path, alpha);
    std::unique_ptr<SequenceContainerInterface> sec = std::move(sic);
    return sec;
  }
  /** @} */
};


class AbstractIProbabilisticAlignment :
  public virtual IProbabilisticAlignment
{
public:
  AbstractIProbabilisticAlignment() {}
  virtual ~AbstractIProbabilisticAlignment() {}

public:
  /**
   * @name IProbabilisticAlignment methods:
   *
   * @{
   */

  /**
   * @brief Add sequences to a container from a stream.
   *
   * @param input  The input stream to read.
   * @param sc     The sequence container to update.
   */
  void readAlignment(std::istream& input, ProbabilisticSequenceContainerInterface& sc) const override
  {
    appendAlignmentFromStream(input, sc);
  }

  /**
   * @brief Add sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param sc    The sequence container to update.
   */
  void readAlignment(const std::string& path, ProbabilisticSequenceContainerInterface& sc) const override
  {
    appendAlignmentFromFile(path, sc);
  }

  std::unique_ptr<ProbabilisticSiteContainerInterface> readAlignment(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readAlignmentFromFile(path, alpha);
  }

  std::unique_ptr<ProbabilisticSiteContainerInterface> readAlignment(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
  {
    return readAlignmentFromStream(input, alpha);
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
   */
  virtual void appendAlignmentFromStream(std::istream& input, ProbabilisticSequenceContainerInterface& sc) const = 0;

  /**
   * @brief Append sequences to a container from a file.
   *
   * @param path  The path to the file to read.
   * @param sc    The sequence container to update.
   * @throw Exception If the file is not in the specified format.
   */
  virtual void appendAlignmentFromFile(const std::string& path, ProbabilisticSequenceContainerInterface& sc) const
  {
    std::ifstream input(path.c_str(), std::ios::in);
    if (!input)
      throw IOException("AbstractIProbabilisticAlignment::appendAlignmentFromFile: can't read file " + path);
    appendAlignmentFromStream(input, sc);
    input.close();
  }

  /**
   * @brief Read sequences from a stream.
   *
   * @param input  The input stream to read.
   * @param alpha  The alphabet to use.
   * @return A sequence container.
   */
  virtual std::unique_ptr<ProbabilisticSiteContainerInterface> readAlignmentFromStream(std::istream& input, std::shared_ptr<const Alphabet>& alpha) const
  {
    auto vpsc = std::unique_ptr<ProbabilisticVectorSiteContainer>(new ProbabilisticVectorSiteContainer(alpha));
    appendAlignmentFromStream(input, *vpsc);
    return vpsc;
  }

  /**
   * @brief Read sequences from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to use.
   * @return A sequence container.
   */
  virtual std::unique_ptr<ProbabilisticSiteContainerInterface> readAlignmentFromFile(const std::string& path, std::shared_ptr<const Alphabet>& alpha) const
  {
    auto vpsc = std::unique_ptr<ProbabilisticVectorSiteContainer>(new ProbabilisticVectorSiteContainer(alpha));
    appendAlignmentFromFile(path, *vpsc);
    return vpsc;
  }
};


class AbstractIProbabilisticAlignment2 :
  public virtual AbstractIProbabilisticAlignment,
  // In case of diamond inheritence
  public virtual IProbabilisticSequence
{
public:
  AbstractIProbabilisticAlignment2() {}
  virtual ~AbstractIProbabilisticAlignment2() {}

public:
  /**
   * @name The IProbabilisticSequence interface.
   *
   * As a ProbabilisticSiteContainer is a subclass of ProbabilisticSequenceContainer, we hereby implement the IProbabilisticSequence
   * interface by downcasting the interface.
   *
   * @{
   */
  void readSequences(std::istream& input, ProbabilisticSequenceContainerInterface& sc) const override
  {
    appendAlignmentFromStream(input, sc);
  }

  void readSequences(const std::string& path, ProbabilisticSequenceContainerInterface& sc) const override
  {
    appendAlignmentFromFile(path, sc);
  }

  std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
  {
    auto sic = readAlignment(input, alpha);
    std::unique_ptr<ProbabilisticSequenceContainerInterface> sec = std::move(sic);
    return sec;
  }

  std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
  {
    auto sic = readAlignment(path, alpha);
    std::unique_ptr<ProbabilisticSequenceContainerInterface> sec = std::move(sic);
    return sec;
  }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_ABSTRACTIALIGNMENT_H
