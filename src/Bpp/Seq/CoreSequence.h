//
// File: CoreSequence.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
// Created: 2015-09-29 00:00:00
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

  /**
   * @brief get value of a state at a position
   *
   * @param sitePosition index of the looked value in the sequence
   * @param state        state in the alphabet
   * @return             The state value at the given position.
   */
  virtual double getStateValueAt(size_t sitePosition, int state) const override = 0;

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
    name_(s.name_) {}

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
  virtual AbstractCoreSequence* clone() const = 0;
  /** @} */


  /**
   * @name Setting/getting the name of the sequence.
   *
   * @{
   */
  const std::string& getName() const { return name_; }

  void setName(const std::string& name) { name_ = name; }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_CORESEQUENCE_H
