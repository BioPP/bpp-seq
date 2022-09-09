//
// File: SequenceExceptions.h
// Authors:
//   Julien Dutheil
// Created: 2003-11-03 16:35:30
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

#ifndef BPP_SEQ_SEQUENCEEXCEPTIONS_H
#define BPP_SEQ_SEQUENCEEXCEPTIONS_H

#include <Bpp/Exceptions.h>

#include "Alphabet/Alphabet.h"
#include "CoreSequence.h"

#include<string>
#include<memory>

namespace bpp
{

/**
 * @brief The sequence exception base class.
 *
 * @see Exception
 */
class SequenceException :
  public Exception
{
private:
  /**
   * @brief A pointer toward a sequence object.
   */
  const CoreSequenceInterface* sequence_;

public:
  /**
   * @brief Build a new SequenceException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param seq A const pointer toward the sequence that threw the exception. The exception object is not managing the pointer.
   */
  SequenceException(const std::string& text, const CoreSequenceInterface* seq):
      Exception("SequenceException: " + text + (seq ? "(" + seq->getName() + ")" : std::string(""))),
  sequence_(seq)
  {}
  
  SequenceException(const SequenceException& se) :
      Exception(se),
      sequence_(se.sequence_)
  {}

  SequenceException& operator=(const SequenceException& se)
  {
    Exception::operator=(se);
    sequence_ = se.sequence_;
    return *this;
  }

  virtual ~SequenceException() {}

public:
  /**
   * @brief Get the sequence that threw the exception.
   *
   * @return A const pointer toward the sequence.
   */
  virtual const CoreSequenceInterface* getSequence() const { return sequence_; }
};

/**
 * @brief Exception thrown when a sequence is found to be empty and it should not.
 */
class EmptySequenceException :
  public SequenceException
{
public:
  /**
   * @brief Build a new EmptySequenceException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param seq A const pointer toward the sequence that threw the exception.
   */
  EmptySequenceException(const std::string& text, const CoreSequenceInterface* seq) :
      SequenceException("EmptySequenceException: " + text, seq)
  {}

  virtual ~EmptySequenceException() {}
};

/**
 * @brief Exception thrown when a sequence is found to have gap and it should not.
 */
class SequenceWithGapException :
  public SequenceException
{
public:
  /**
   * @brief Build a new SequenceWithGapException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param seq A const pointer toward the sequence that threw the exception.
   */
  SequenceWithGapException(const std::string& text, const CoreSequenceInterface* seq) :
      SequenceException("SequenceWithGapException: " + text, seq)
  {}

  virtual ~SequenceWithGapException() {}
};

/**
 * @brief Exception thrown when a sequence is not align with others.
 *
 * Typically, this may occur when you try to add a bad sequence to a site container.
 */
class SequenceNotAlignedException :
  public SequenceException
{
public:
  /**
   * @brief Build a new SequenceNotAlignedException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param seq  A const pointer toward the sequence that threw the exception.
   */
  SequenceNotAlignedException(const std::string& text, const CoreSequenceInterface* seq) :
      SequenceException("SequenceNotAlignedException: " + text, seq)
  {}

  virtual ~SequenceNotAlignedException() {}
};
} // end of namespace bpp.
#endif // BPP_SEQ_SEQUENCEEXCEPTIONS_H
