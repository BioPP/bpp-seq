// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CORESEQUENCE_H
#define BPP_SEQ_CORESEQUENCE_H


// From the STL:
#include <string>
#include <vector>

#include <Bpp/Clonable.h>
#include "CoreSymbolList.h"
#include "Commentable.h"

namespace bpp
{
/**
 * @brief The core sequence interface.
 *
 * This interface specifies the 'core' functionality of a sequence, that is,
 * an ordered set of states, a name and some meta data (comments).
 */
class CoreSequenceInterface :
  public virtual CruxSymbolListInterface,
  public virtual Commentable
{
public:

public:
  CoreSequenceInterface() {}

  virtual ~CoreSequenceInterface() {}

public:
  CoreSequenceInterface* clone() const override = 0;

  /**
   * @name Setting/getting the name of the sequence.
   *
   * @{
   */

  /**
   * @brief Get the name of this sequence.
   *
   * @return This sequence name.
   */
  virtual const std::string& getName() const = 0;

  /**
   * @brief Set the name of this sequence.
   *
   * @param name The new name of the sequence.
   */
  virtual void setName(const std::string& name) = 0;
  /** @} */


  /**
   * @name Adjusting the size of the sequence.
   *
   * @{
   */


  /**
   * @brief Set up the size of a sequence from the right side.
   *
   * All new characters are filled with gaps.
   * If the specified size is < to the sequence size, the sequence will be truncated.
   *
   * @param newSize The new size of the sequence.
   */
  virtual void setToSizeR(size_t newSize) = 0;

  /**
   * @brief Set up the size of a sequence from the left side.
   *
   * All new characters are filled with gaps.
   * If the specified size is < to the sequence size, the sequence will be truncated.
   *
   * @param newSize The new size of the sequence.
   */
  virtual void setToSizeL(size_t newSize) = 0;

  /** @} */
};


/**
 * @brief A partial implementation of the CoreSequence interface.
 */
class AbstractCoreSequence :
  public virtual CoreSequenceInterface,
  public SimpleCommentable
{
private:
  /**
   * @brief The sequence name.
   */
  std::string name_;

public:
  /**
   * @brief Constructor of the AbstractCoreSequence object.
   *
   * @param name     The sequence name.
   */
  AbstractCoreSequence(const std::string& name) :
    SimpleCommentable(),
    name_(name) {}

  /**
   * @brief Constructor of the AbstractCoreSequence object.
   *
   * @param name     The sequence name.
   * @param comments Comments to add to the sequence.
   */
  AbstractCoreSequence(const std::string& name, const Comments& comments) :
    SimpleCommentable(comments),
    name_(name) {}

  AbstractCoreSequence() :
    SimpleCommentable(),
    name_() {}

  AbstractCoreSequence(const AbstractCoreSequence& s) :
    Commentable(s),
    name_(s.name_)
  {}

  AbstractCoreSequence& operator=(const AbstractCoreSequence& s)
  {
    SimpleCommentable::operator=(s);
    name_ = s.name_;
    return *this;
  }

  AbstractCoreSequence(const CoreSequenceInterface& s) :
    SimpleCommentable(s.getComments()),
    name_(s.getName())
  {}

  AbstractCoreSequence& operator=(const CoreSequenceInterface& s)
  {
    SimpleCommentable::setComments(s.getComments());
    name_ = s.getName();
    return *this;
  }

  virtual ~AbstractCoreSequence() {}

public:
  /**
   * @name The Clonable interface
   *
   * @{
   */
  virtual AbstractCoreSequence* clone() const override = 0;
  /** @} */


  /**
   * @name Setting/getting the name of the sequence.
   *
   * @{
   */
  const std::string& getName() const override { return name_; }

  void setName(const std::string& name) override { name_ = name; }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_CORESEQUENCE_H
