// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCEPOSITIONITERATORS_H
#define BPP_SEQ_SEQUENCEPOSITIONITERATORS_H


// from STL
#include <string>

#include "Sequence.h"

namespace bpp
{
/**
 * @brief Loop over a Sequence.
 *
 * This is the SequencePositionIterator interface.
 *
 * @author Sylvain Gaillard
 */
class SequencePositionIterator
{
public:
  SequencePositionIterator()
  {}
  virtual ~SequencePositionIterator()
  {}

public:
  /**
   * @brief Get the actual position of the iterator in the Sequence.
   */
  virtual unsigned int getPosition() const = 0;
  /**
   * @brief Set the position of the iterator.
   * @param pos The position on the Sequence
   */
  virtual void setPosition(unsigned int pos) = 0;
  /**
   * @brief Get the numerical value of the Sequence at current position.
   */
  virtual int getValue() const = 0;
  /**
   * @brief Get the textual value of the Sequence at current position.
   */
  virtual std::string getChar() const = 0;

  virtual bool operator==(const SequencePositionIterator& it) const = 0;
  virtual bool operator!=(const SequencePositionIterator& it) const = 0;
  virtual SequencePositionIterator& operator+=(int i) = 0;
  virtual SequencePositionIterator& operator-=(int i) = 0;
  virtual SequencePositionIterator& operator++() = 0;

  /**
   * @brief Tells if there is more positions in the Sequence.
   * @return true if there is more positions in the Sequence
   */
  virtual bool hasMorePositions() const = 0;
  /**
   * @brief Get the Sequence on which the iterator loops.
   * @return A reference toward the Sequence object.
   */
  virtual const Sequence& getSequence() const = 0;
};

/**
 * @brief Partial implementation of the SequencePositionIterator interface.
 *
 * @author Sylvain Gaillard
 */
class AbstractSequencePositionIterator :
  public virtual SequencePositionIterator
{
private:
  const Sequence* sequence_;
  unsigned int currentPosition_;

public:
  AbstractSequencePositionIterator(const Sequence& seq, unsigned int pos = 0) :
    sequence_(&seq), currentPosition_(pos)
  {}

  AbstractSequencePositionIterator(const AbstractSequencePositionIterator& aspi) :
    sequence_(aspi.sequence_), currentPosition_(aspi.currentPosition_)
  {}

  AbstractSequencePositionIterator& operator=(const AbstractSequencePositionIterator& aspi)
  {
    sequence_ = aspi.sequence_;
    currentPosition_ = aspi.currentPosition_;
    return *this;
  }

  virtual ~AbstractSequencePositionIterator()
  {}

public:
  /**
   * @name Comparison operators
   *
   * @{
   */
  bool operator==(const SequencePositionIterator& it) const;
  bool operator!=(const SequencePositionIterator& it) const;
  /** @} */

  unsigned int getPosition() const;
  void setPosition(unsigned int pos);
  int getValue() const;
  std::string getChar() const;
  const Sequence& getSequence() const;
};

/**
 * @brief Loop over all positions in a Sequence
 *
 * This is the simplest implementation of SequencePositionIterator.
 * It just loops over all positions of a Sequence.
 *
 * @code
 * Sequence seq = Sequence("seq1", "ATTCGATCCG-G", &AlphabetTools::DNA_ALPHABET);
 * for (SimpleSequencePositionIterator it(seq) ; it.hasMorePositions() ; ++it) {
 *   cout << it.getPosition() << " : " << it.getValue() << " (" << it.getChar() << ")" << endl;
 * }
 * @endcode
 *
 * @author Sylvain Gaillard
 */
class SimpleSequencePositionIterator :
  public AbstractSequencePositionIterator
{
public:
  /**
   * @name Constructors and destructor
   *
   * @{
   */

  /**
   * @brief General constructor
   *
   * @param seq A reference toward the Sequence object we want to loop over
   * @param pos Optional integer where to start on the Sequence object
   *
   */
  SimpleSequencePositionIterator(const Sequence& seq, unsigned int pos = 0) :
    AbstractSequencePositionIterator(seq, pos)
  {}
  /**
   * @brief Copy constructor.
   *
   * @param it A reference toward a SequencePositionIterator
   */
  SimpleSequencePositionIterator(const SequencePositionIterator& it);
  virtual ~SimpleSequencePositionIterator()
  {}
  /** @} */

public:
  /**
   * @name Operators
   *
   * @{
   */
  SimpleSequencePositionIterator& operator++();
  virtual SimpleSequencePositionIterator operator++(int i);
  SimpleSequencePositionIterator& operator+=(int i);
  SimpleSequencePositionIterator& operator-=(int i);
  virtual SimpleSequencePositionIterator operator+(int i) const;
  virtual SimpleSequencePositionIterator operator-(int i) const;
  /** @} */

  bool hasMorePositions() const;
};
}
#endif // BPP_SEQ_SEQUENCEPOSITIONITERATORS_H
