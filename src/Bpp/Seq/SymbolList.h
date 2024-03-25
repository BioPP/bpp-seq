// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SYMBOLLIST_H
#define BPP_SEQ_SYMBOLLIST_H

#include <Bpp/Clonable.h>
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Random/RandomTools.h>

#include "Alphabet/Alphabet.h"
#include "CoreSymbolList.h"

// From the STL:
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

namespace bpp
{
/**
 * @brief A partial implementation of a SymbolList object.
 *
 * This class implements most of the CoreSymbolList interface, with the exception of the getStateValueAt function, which depend on the template type.
 *
 * @see Alphabet
 */
template<class T>
class AbstractTemplateSymbolList :
  public virtual TemplateCoreSymbolListInterface<T>
{
private:
  /**
   * @brief The Alphabet attribute must be initialized in constructor and then can never be changed.
   *
   * To apply another alphabet to a list you'll have to create a new list.
   */
  std::shared_ptr<const Alphabet> alphabet_;

protected:
  /**
   * @brief The list content.
   */
  std::vector<T> content_;

public:
  /**
   * @brief Build a new void SymbolList object with the specified alphabet.
   *
   * @param alpha The alphabet to use.
   */
  AbstractTemplateSymbolList(std::shared_ptr<const Alphabet> alpha) :
    alphabet_(alpha), content_() {}

  /**
   * @brief Build a new SymbolList object with the specified alphabet.
   * The content of the site is initialized from a vector of T objects.
   *
   * @param list     The content of the site.
   * @param alpha    The alphabet to use.
   */
  AbstractTemplateSymbolList(const std::vector<T>& list, std::shared_ptr<const Alphabet> alpha) :
    alphabet_(alpha), content_()
  {
    setContent(list);
  }

  /**
   * @brief The generic copy constructor.
   */
  AbstractTemplateSymbolList(const AbstractTemplateSymbolList<T>& list) :
    alphabet_(list.alphabet_), content_(list.content_)
  {}


  /**
   * @brief The copy constructor.
   */
  AbstractTemplateSymbolList(const TemplateCoreSymbolListInterface<T>& list) :
    alphabet_(list.getAlphabet()), content_(list.size())
  {
    for (size_t i = 0; i < list.size(); ++i)
    {
      content_[i] = list[i];
    }
  }


  /**
   * @brief The generic assignment operator.
   */
  AbstractTemplateSymbolList<T>& operator=(const TemplateCoreSymbolListInterface<T>& list)
  {
    content_.resize(list.size());
    for (size_t i = 0; i < list.size(); ++i)
    {
      content_[i] = list[i];
    }
    alphabet_ = list.getAlphabet();
    return *this;
  }

  /**
   * @brief The assignment operator.
   */
  AbstractTemplateSymbolList<T>& operator=(const AbstractTemplateSymbolList<T>& list)
  {
    content_  = list.content_;
    alphabet_ = list.alphabet_;
    return *this;
  }

  // Class destructor
  virtual ~AbstractTemplateSymbolList() {}

public:
  std::shared_ptr<const Alphabet> getAlphabet() const override { return alphabet_; }

  const Alphabet& alphabet() const override { return *alphabet_; }

  size_t size() const override { return content_.size(); }

  virtual void setContent(const std::vector<T>& list) override
  {
    content_ = list;
  }

  virtual const std::vector<T>& getContent() const override { return content_; }

  virtual std::string toString() const override
  {
    return "";
  }

  void deleteElement(size_t pos) override
  {
    if (pos >= content_.size())
      throw IndexOutOfBoundsException("SymbolList::deleteElement. Invalid position.", pos, 0, size() - 1);
    content_.erase(content_.begin() + static_cast<std::ptrdiff_t>(pos));
  }

  void deleteElements(size_t pos, size_t len) override
  {
    if (pos + len > content_.size())
      throw IndexOutOfBoundsException("SymbolList::deleteElements. Invalid position.", pos + len, 0, size() - 1);
    content_.erase(content_.begin() + static_cast<std::ptrdiff_t>(pos), content_.begin() + static_cast<std::ptrdiff_t>(pos + len));
  }

  void addElement(const T& v) override
  {
    content_.push_back(v);
  }

  void addElement(size_t pos, const T& v) override
  {
    // test:
    if (pos >= content_.size())
      throw IndexOutOfBoundsException("SymbolList::addElement. Invalid position.", pos, 0, size() - 1);
    content_.insert(content_.begin() + static_cast<std::ptrdiff_t>(pos), v);
  }

  void setElement(size_t pos, const T& v) override
  {
    // test:
    if (pos >= content_.size())
      throw IndexOutOfBoundsException("SymbolList::setElement. Invalid position.", pos, 0, size() - 1);
    content_[pos] = v;
  }

  const T& getElement(size_t pos) const override
  {
    if (pos >= content_.size())
      throw IndexOutOfBoundsException("SymbolList::getElement. Invalid position.", pos, 0, size() - 1);
    return content_[pos];
  }

  const T& getValue(size_t pos) const override
  {
    if (pos >= content_.size())
      throw IndexOutOfBoundsException("SymbolList::getValue. Invalid position.", pos, 0, size() - 1);
    return content_[pos];
  }

  const T& operator[](size_t pos) const override { return content_[pos]; }

  T& operator[](size_t pos) override { return content_[pos]; }

  void shuffle() override
  {
    std::shuffle(content_.begin(), content_.end(), RandomTools::DEFAULT_GENERATOR);
  }
};


/**
 * @brief A partial implementation of a EventDrivenSymbolList object.
 *
 * This class implements most of the CoreSymbolList interface, with the exception of the getStateValueAt function, which depend on the template type.
 *
 * @see Alphabet
 */
template<class T>
class AbstractTemplateEventDrivenSymbolList :
  public virtual AbstractTemplateSymbolList<T>,
  // Note: this needs to be virtual because of diamond inheritance
  public virtual TemplateEventDrivenCoreSymbolListInterface<T>
{
private:
  bool propagateEvents_;

protected:
  /**
   * @brief Contains the listeners.
   */
  std::vector<std::shared_ptr<CoreSymbolListListener<T>>> listeners_;

  /**
   * @brief Build a new void EventDrivenSymbolList object with the specified alphabet.
   *
   * @param alpha The alphabet to use.
   */
  AbstractTemplateEventDrivenSymbolList(std::shared_ptr<const Alphabet> alpha) :
    AbstractTemplateSymbolList<T>(alpha), propagateEvents_(true), listeners_() {}

  /**
   * @brief Build a new EventDrivenSymbolList object with the specified alphabet.
   * The content of the site is initialized from a vector of integers.
   *
   * @param list     The content of the site.
   * @param alpha    The alphabet to use.
   * @throw BadIntException If the content does not match the specified alphabet.
   */
  AbstractTemplateEventDrivenSymbolList(const std::vector<T>& list, std::shared_ptr<const Alphabet> alpha) :
    AbstractTemplateSymbolList<T>(list, alpha), propagateEvents_(true), listeners_()
  {}

  /**
   * @brief The generic copy constructor.
   */
  AbstractTemplateEventDrivenSymbolList(const TemplateCoreSymbolListInterface<T>& list) :
    AbstractTemplateSymbolList<T>(list),
    propagateEvents_(true),
    listeners_()
  {}


  /**
   * @brief The copy constructor.
   */
  AbstractTemplateEventDrivenSymbolList(const AbstractTemplateEventDrivenSymbolList<T>& list) :
    AbstractTemplateSymbolList<T>(list),
    propagateEvents_(list.propagateEvents_),
    listeners_(list.listeners_.size())
  {
    for (size_t i = 0; i < list.listeners_.size(); ++i)
    {
      if (list.listeners_[i]->isShared())
      {
        listeners_[i] = list.listeners_[i];
      }
      else
      {
        listeners_[i] = std::shared_ptr<CoreSymbolListListener<T>>(list.listeners_[i]->clone());
      }
    }
  }

  /**
   * @brief The generic assignment operator.
   */
  AbstractTemplateEventDrivenSymbolList<T>& operator=(const TemplateCoreSymbolListInterface<T>& list)
  {
    AbstractTemplateSymbolList<T>::operator=(list);
    propagateEvents_ = true;
    listeners_.clear();
    return *this;
  }


  /**
   * @brief The assignment operator.
   */
  AbstractTemplateEventDrivenSymbolList<T>& operator=(const AbstractTemplateEventDrivenSymbolList& list)
  {
    AbstractTemplateSymbolList<T>::operator=(list);
    propagateEvents_ = list.propagateEvents_;
    listeners_.resize(list.listeners_.size());
    for (size_t i = 0; i < listeners_.size(); ++i)
    {
      if (list.listeners_[i]->isShared())
      {
        listeners_[i] = list.listeners_[i];
      }
      else
      {
        listeners_[i] = std::shared_ptr<CoreSymbolListListener<T>>(list.listeners_[i]->clone());
      }
    }
    return *this;
  }

  // Class destructor
  virtual ~AbstractTemplateEventDrivenSymbolList() = default;

public:
  virtual void setContent(const std::vector<T>& list) override
  {
    CoreSymbolListEditionEvent<T> event(this);
    fireBeforeSequenceChanged(event);

    // Sequence is valid:
    AbstractTemplateSymbolList<T>::setContent(list);
    fireAfterSequenceChanged(event);
  }

  void deleteElement(size_t pos) override
  {
    CoreSymbolListDeletionEvent<T> event(this, pos, 1);
    fireBeforeSequenceDeleted(event);
    AbstractTemplateSymbolList<T>::deleteElement(pos);
    fireAfterSequenceDeleted(event);
  }


  void deleteElements(size_t pos, size_t len) override
  {
    CoreSymbolListDeletionEvent<T> event(this, pos, len);
    fireBeforeSequenceDeleted(event);
    AbstractTemplateSymbolList<T>::deleteElements(pos, len);
    fireAfterSequenceDeleted(event);
  }

  virtual void addElement(const T& v) override
  {
    CoreSymbolListInsertionEvent<T> event(this, AbstractTemplateSymbolList<T>::size(), 1);
    fireBeforeSequenceInserted(event);
    AbstractTemplateSymbolList<T>::addElement(v);
    fireAfterSequenceInserted(event);
  }

  virtual void addElement(size_t pos, const T& v) override
  {
    CoreSymbolListInsertionEvent<T> event(this, pos, 1);
    fireBeforeSequenceInserted(event);
    AbstractTemplateSymbolList<T>::addElement(pos, v);
    fireAfterSequenceInserted(event);
  }

  virtual void setElement(size_t pos, const T& v) override
  {
    CoreSymbolListSubstitutionEvent<T> event(this, pos, pos);
    fireBeforeSequenceSubstituted(event);
    AbstractTemplateSymbolList<T>::setElement(pos, v);
    fireAfterSequenceSubstituted(event);
  }

  /**
   * @name Events handling
   *
   * @{
   */
  virtual size_t getNumberOfListeners() const override { return listeners_.size(); }

  virtual const CoreSymbolListListener<T>& listener(size_t i) const override
  {
    if (!listeners_[i])
      throw Exception("EventDrivenSymbolList::listener. The specified listener is missing.");
    return *listeners_[i];
  }

  virtual std::shared_ptr<const CoreSymbolListListener<T>> getListener(size_t i) const override
  {
    return listeners_[i];
  }

  virtual CoreSymbolListListener<T>& listener(size_t i) override
  {
    if (!listeners_[i])
      throw Exception("EventDrivenSymbolList::listener. The specified listener is missing.");
    return *listeners_[i];
  }

  virtual std::shared_ptr<CoreSymbolListListener<T>> getListener(size_t i) override
  {
    return listeners_[i];
  }

  virtual void addCoreSymbolListListener(std::shared_ptr<CoreSymbolListListener<T>> listener) override
  {
    listeners_.push_back(listener);
  }

  virtual void removeCoreSymbolListListener(std::shared_ptr<CoreSymbolListListener<T>> listener) override
  {
    if (listener->isRemovable())
      listeners_.erase(remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
    else
      throw Exception("EventDrivenSymbolList::removeCoreSymbolListListener. Listener is not removable.");
  }

protected:
  virtual void beforeSequenceChanged(const CoreSymbolListEditionEvent<T>& event) override {}
  virtual void afterSequenceChanged(const CoreSymbolListEditionEvent<T>& event) override {}
  virtual void beforeSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) override {}
  virtual void afterSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) override {}
  virtual void beforeSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) override {}
  virtual void afterSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) override {}
  virtual void beforeSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) override {}
  virtual void afterSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) override {}

  void fireBeforeSequenceChanged(const CoreSymbolListEditionEvent<T>& event) override
  {
    beforeSequenceChanged(event);
    if (propagateEvents_)
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        listeners_[i]->beforeSequenceChanged(event);
      }
  }

  void fireAfterSequenceChanged(const CoreSymbolListEditionEvent<T>& event) override
  {
    afterSequenceChanged(event);
    if (propagateEvents_)
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        listeners_[i]->afterSequenceChanged(event);
      }
  }

  void fireBeforeSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) override
  {
    beforeSequenceInserted(event);
    if (propagateEvents_)
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        listeners_[i]->beforeSequenceInserted(event);
      }
  }

  void fireAfterSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) override
  {
    afterSequenceInserted(event);
    if (propagateEvents_)
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        listeners_[i]->afterSequenceInserted(event);
      }
  }

  void fireBeforeSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) override
  {
    beforeSequenceDeleted(event);
    if (propagateEvents_)
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        listeners_[i]->beforeSequenceDeleted(event);
      }
  }

  void fireAfterSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) override
  {
    afterSequenceDeleted(event);
    if (propagateEvents_)
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        listeners_[i]->afterSequenceDeleted(event);
      }
  }

  void fireBeforeSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) override
  {
    beforeSequenceSubstituted(event);
    if (propagateEvents_)
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        listeners_[i]->beforeSequenceSubstituted(event);
      }
  }

  void fireAfterSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) override
  {
    afterSequenceSubstituted(event);
    if (propagateEvents_)
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        listeners_[i]->afterSequenceSubstituted(event);
      }
  }
  /** @} */

protected:
  void propagateEvents(bool yn) override { propagateEvents_ = yn; }
  bool propagateEvents() const override { return propagateEvents_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_SYMBOLLIST_H
