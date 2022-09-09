//
// File: ISequence.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
// Created: 2003-07-30 00:00:00
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
  ISequence() {}
  virtual ~ISequence() {}

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
       std::shared_ptr<const Alphabet>& alpha) const = 0;
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
      std::shared_ptr<const Alphabet>& alpha) const = 0;
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
  IAlignment() {}
  virtual ~IAlignment() {}

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
      std::shared_ptr<const Alphabet>& alpha) const = 0;

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
      std::shared_ptr<const Alphabet>& alpha) const = 0;
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
  IProbabilisticSequence() {}
  virtual ~IProbabilisticSequence() {}

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
      std::shared_ptr<const Alphabet>& alpha) const = 0;

  /**
   * @brief Create a new container from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new ProbabilisticSequenceContainer object.
   */
  virtual std::unique_ptr<ProbabilisticSequenceContainerInterface> readSequences(
      const std::string& path,
      std::shared_ptr<const Alphabet>& alpha) const = 0;
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
  IProbabilisticAlignment() {}
  virtual ~IProbabilisticAlignment() {}

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
      std::shared_ptr<const Alphabet>& alpha) const = 0;

  /**
   * @brief Create a new container from a file.
   *
   * @param path  The path to the file to read.
   * @param alpha The alphabet to be associated to the container.
   * @return A new ProbabilisticSiteContainer object.
   */
  virtual std::unique_ptr<ProbabilisticSiteContainerInterface> readAlignment(
      const std::string& path,
      std::shared_ptr<const Alphabet>& alpha) const = 0;
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_ISEQUENCE_H
