//
// File: IntSymbolList.h
// Authors:
//   Julien Dutheil
// Created: 2005-04-09 00:00:00
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

#ifndef BPP_SEQ_INTSYMBOLLIST_H
#define BPP_SEQ_INTSYMBOLLIST_H

#include <Bpp/Clonable.h>

#include "Alphabet/Alphabet.h"
#include "SymbolList.h"
#include "StringSequenceTools.h"

// From the STL:
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace bpp
{
/**
 * @brief The specific IntSymbolList interface.
 *
 * @see Alphabet
 */
class IntSymbolListInterface :
  public virtual IntCoreSymbolListInterface
{
public:
  typedef int SymbolType;
  
public:
  IntSymbolListInterface() {}

  // Class destructor
  virtual ~IntSymbolListInterface() {}

public:
  /**
   * @name Acting on the content of the list.
   *
   */

  using IntCoreSymbolListInterface::setContent;

  /**
   * @brief Set the whole content of the list.
   *
   * @param list The new content of the list.
   * @see The list constructor for information about the way lists are internaly stored.
   */
  virtual void setContent(const std::vector<std::string>& list) = 0;

  /**
   * @name Edition methods.
   *
   * @{
   */

  using IntCoreSymbolListInterface::addElement;
  using IntCoreSymbolListInterface::setElement;

  /**
   * @brief Add a character to the end of the list.
   *
   * @param c The character to add, given as a string.
   */
  virtual void addElement(const std::string& c) = 0;

  /**
   * @brief Add a character at a certain position in the list.
   *
   * @param pos The postion where to insert the element.
   * @param c   The character to add, given as a string.
   */
  virtual void addElement(size_t pos, const std::string& c) = 0;

  /**
   * @brief Set the element at position 'pos' to character 'c'.
   *
   * @param pos The position of the character to set.
   * @param c   The value of the element, given as a string.
   */
  virtual void setElement(size_t pos, const std::string& c) = 0;

  /**
   * @brief Get the element at position 'pos' as a character.
   *
   * @param pos The position of the character to retrieve.
   */
  virtual std::string getChar(size_t pos) const = 0;

  /** @} */
};


/**
 * @brief A basic IntSymbolList object.
 *
 * This is a general purpose container, containing an ordered list of states(= letters).
 * The states that allowed to be present in the list are defined by an alphabet object,
 * which is passed to the list constructor by a pointer.
 *
 * For programming convenience, the states are stored as integers, but the translation toward
 * and from a char description is easily performed with the Alphabet classes.
 *
 * @see Alphabet
 */
class IntSymbolList :
  public virtual IntSymbolListInterface,
  public virtual AbstractTemplateSymbolList<int> //This needs to be virtual because of diamond inheritence
{
public:
  IntSymbolList(std::shared_ptr<const Alphabet> alpha) : 
    AbstractTemplateSymbolList<int>(alpha)
  {}

  /**
   * @brief Build a new IntSymbolList object with the specified alphabet.
   * The content of the site is initialized from a vector of characters.
   *
   * @param list     The content of the site.
   * @param alpha    The alphabet to use.
   */
  IntSymbolList(const std::vector<std::string>& list, std::shared_ptr<const Alphabet> alpha) :
    AbstractTemplateSymbolList<int>(alpha)
  {
    setContent(list);
  }

  /**
   * @brief Build a new IntSymbolList object with the specified alphabet.
   * The content of the site is initialized from a vector of integers.
   *
   * @param list     The content of the site.
   * @param alpha    The alphabet to use.
   */
  IntSymbolList(const std::vector<int>& list, std::shared_ptr<const Alphabet> alpha) :
    AbstractTemplateSymbolList<int>(list, alpha)
  {}

  /**
   * @brief The copy constructor.
   */
  IntSymbolList(const IntSymbolList& list):
      AbstractTemplateSymbolList<int>(list)
  {}
	
  /**
   * @brief The generic assignment operator.
   */
  IntSymbolList& operator=(const IntSymbolListInterface& list);

  /**
   * @brief The assignment operator.
   */
  IntSymbolList& operator=(const IntSymbolList& list)
  {
    AbstractTemplateSymbolList<int>::operator=(list);
    return *this;
  }

  /**
   * @name The Clonable interface
   *
   * @{
   */
  IntSymbolList* clone() const override { return new IntSymbolList(*this); }
  /** @} */

  // Class destructor
  virtual ~IntSymbolList() {}

public:
  void setContent(const std::vector<int>& list) override;

  void setContent(const std::vector<std::string>& list) override;

  std::string toString() const override
  {
    auto alphaPtr = getAlphabet();
    return StringSequenceTools::decodeSequence(content_, alphaPtr);
  }

  void addElement(const std::string& c) override
  {
    content_.push_back(getAlphabet()->charToInt(c));
  }

  using AbstractTemplateSymbolList::addElement;

  void addElement(size_t pos, const std::string& c) override
  {
    if (pos >= content_.size())
      throw IndexOutOfBoundsException("IntSymbolList::addElement. Invalid position.", pos, 0, size() - 1);
    content_.insert(content_.begin() + static_cast<ptrdiff_t>(pos), getAlphabet()->charToInt(c));
  }

  using AbstractTemplateSymbolList::setElement;
  
  void setElement(size_t pos, const std::string& c) override
  {
    if (pos >= content_.size())
      throw IndexOutOfBoundsException("IntSymbolList::setElement. Invalid position.", pos, 0, size() - 1);
    content_[pos] = getAlphabet()->charToInt(c);
  }

  std::string getChar(size_t pos) const override;

  double getStateValueAt(size_t siteIndex, int state) const override
  {
    if (siteIndex >= content_.size())
      throw IndexOutOfBoundsException("IntSymbolList::getStateValueAt.", siteIndex, 0, content_.size() - 1);

    return getAlphabet()->isResolvedIn(content_[siteIndex], state) ? 1. : 0.;
  }

  double operator()(size_t siteIndex, int state) const override
  {
    return getAlphabet()->isResolvedIn(content_[siteIndex], state) ? 1. : 0.;
  }
};

typedef CoreSymbolListListener<int> IntSymbolListListener;

typedef CoreSymbolListEditionEvent<int> IntSymbolListEditionEvent;

typedef CoreSymbolListInsertionEvent<int> IntSymbolListInsertionEvent;

typedef CoreSymbolListDeletionEvent<int> IntSymbolListDeletionEvent;

typedef CoreSymbolListSubstitutionEvent<int> IntSymbolListSubstitutionEvent;

/**
 * @brief A event-driven IntSymbolList object.
 *
 * This is a general purpose container, containing an ordered list of states(= letters).
 * The states that allowed to be present in the list are defined by an alphabet object,
 * which is passed to the list constructor by a pointer.
 *
 * For programming convenience, the states are stored as integers, but the translation toward
 * and from a char description is easily performed with the Alphabet classes.
 *
 * @see Alphabet
 */

class EventDrivenIntSymbolList :
  public IntSymbolList,
  public AbstractTemplateEventDrivenSymbolList<int>
{
public:
  /**
   * @brief Build a new void EventDrivenIntSymbolList object with the specified alphabet.
   *
   * @param alpha The alphabet to use.
   */
  EventDrivenIntSymbolList(std::shared_ptr<const Alphabet> alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    IntSymbolList(alpha),
    AbstractTemplateEventDrivenSymbolList<int>(alpha)
  {}

  /**
   * @brief Build a new EventDrivenIntSymbolList object with the specified alphabet.
   * The content of the site is initialized from a vector of characters.
   *
   * @param list     The content of the site.
   * @param alpha    The alphabet to use.
   */
  EventDrivenIntSymbolList(
      const std::vector<std::string>& list,
      std::shared_ptr<const Alphabet> alpha):
    AbstractTemplateSymbolList<int>(alpha),
    IntSymbolList(alpha),
    AbstractTemplateEventDrivenSymbolList<int>(alpha)
  {
    setContent(list);
  }


  /**
   * @brief Build a new EventDrivenIntSymbolList object with the specified alphabet.
   * The content of the site is initialized from a vector of integers.
   *
   * @param list     The content of the site.
   * @param alpha    The alphabet to use.
   */
  EventDrivenIntSymbolList(
      const std::vector<int>& list,
      std::shared_ptr<const Alphabet> alpha):
    AbstractTemplateSymbolList<int>(list, alpha),
    IntSymbolList(list, alpha),
    AbstractTemplateEventDrivenSymbolList<int>(list, alpha)
  {}


  /**
   * @brief The generic copy constructor.
   */
  EventDrivenIntSymbolList(const IntSymbolList& list) :
    AbstractTemplateSymbolList<int>(list),
    IntSymbolList(list),
    AbstractTemplateEventDrivenSymbolList<int>(list)
  {}

  /**
   * @brief The copy constructor.
   */
  EventDrivenIntSymbolList(const EventDrivenIntSymbolList& list) :
    AbstractTemplateSymbolList<int>(list),
    IntSymbolList(list),
    AbstractTemplateEventDrivenSymbolList<int>(list)
  {}


  /**
   * @brief The generic assignment operator.
   */
  EventDrivenIntSymbolList& operator=(const IntSymbolList& list)
  {
    AbstractTemplateSymbolList<int>::operator=(list);
    IntSymbolList::operator=(list);
    AbstractTemplateEventDrivenSymbolList<int>::operator=(list);
    return *this;
  }


  /**
   * @brief The assignment operator.
   */
  EventDrivenIntSymbolList& operator=(const EventDrivenIntSymbolList& list)
  {
    AbstractTemplateSymbolList<int>::operator=(list);
    IntSymbolList::operator=(list);
    AbstractTemplateEventDrivenSymbolList<int>::operator=(list);
    return *this;
  }

  /**
   * @name The Clonable interface
   *
   * @{
   */
  EventDrivenIntSymbolList* clone() const { return new EventDrivenIntSymbolList(*this); }
  /** @} */

  ~EventDrivenIntSymbolList() {}

public:
  virtual void setContent(const std::vector<std::string>& list);

  void setContent(const std::vector<int>& list);

  virtual std::string toString() const;

  using AbstractTemplateEventDrivenSymbolList<int>::addElement;

  virtual void addElement(const std::string& c);

  virtual void addElement(size_t pos, const std::string& c);

  using AbstractTemplateEventDrivenSymbolList<int>::setElement;

  virtual void setElement(size_t pos, const std::string& c);

  virtual std::string getChar(size_t pos) const;

  void addIntSymbolListListener(IntSymbolListListener* listener)
  {
    addCoreSymbolListListener(listener);
  }

protected:
  virtual void beforeSequenceChanged(const IntSymbolListEditionEvent& event) {}
  virtual void afterSequenceChanged(const IntSymbolListEditionEvent& event) {}
  virtual void beforeSequenceInserted(const IntSymbolListInsertionEvent& event) {}
  virtual void afterSequenceInserted(const IntSymbolListInsertionEvent& event) {}
  virtual void beforeSequenceDeleted(const IntSymbolListDeletionEvent& event) {}
  virtual void afterSequenceDeleted(const IntSymbolListDeletionEvent& event) {}
  virtual void beforeSequenceSubstituted(const IntSymbolListSubstitutionEvent& event) {}
  virtual void afterSequenceSubstituted(const IntSymbolListSubstitutionEvent& event) {}
};
} // end of namespace bpp.
#endif // BPP_SEQ_INTSYMBOLLIST_H
