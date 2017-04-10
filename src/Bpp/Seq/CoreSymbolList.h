//
// File: CoreSymbolList.h
// Created by: Laurent Guéguen
// Created on: dimanche 2 avril 2017, à 22h 23
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for sequences analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _CORE_SYMBOLLIST_H_
#define _CORE_SYMBOLLIST_H_

#include "Alphabet/Alphabet.h"
#include <Bpp/Clonable.h>

// From the STL:
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace bpp
{

  /**
   * @brief The CoreSymbolList interface.
   *
   */
  template<class T>
  class CoreSymbolList: 
    public virtual Clonable
  {

  public: 
    /**
     * @name The Clonable interface
     *
     * @{
     */
    CoreSymbolList* clone() const = 0;
    /** @} */

    // Class destructor
    virtual ~CoreSymbolList() {}

  public:

    /**
     * @brief Get the alphabet associated to the list.
     *
     * @return A const pointer to the alphabet.
     * @see Alphabet class.
     */
    virtual const Alphabet* getAlphabet() const = 0;

    /**
     * @brief Get the number of elements in the list.
     *
     * @return The number of sites in the list.
     */
    virtual size_t size() const = 0;

    /**
     * @name Acting on the content of the list.
     *
     * @{
     */

    /**
     * @brief Set the whole content of the list.
     *
     * @param list The new content of the list.
     * @see The list constructor for information about the way lists are internaly stored.
     */
    virtual void setContent(const std::vector<T>& list) = 0;

    virtual const std::vector<T>& getContent() const = 0;

    /** @} */

    /**
     * @brief Convert the list as a string.
     *
     * This method is useful for dumping a list to a file or to the screen for display.
     *
     * @return The whole list as a string.
     */

    virtual std::string toString() const = 0;

    /**
     * @name Edition methods.
     *
     * @{
     */

    /**
     * @brief Add a character to the end of the list.
     *
     * @param c The character to add, given as a string.
     */
    virtual void addElement(const T& c) = 0;

    /**
     * @brief Add a character at a certain position in the list.
     *
     * @param pos The postion where to insert the element.
     * @param c   The character to add, given as a string.
     */
    virtual void addElement(size_t pos, const T& c) = 0;

    /**
     * @brief Set the element at position 'pos' to character 'c'.
     *
     * @param pos The position of the character to set.
     * @param c   The value of the element, given as a string.
     */
    virtual void setElement(size_t pos, const T& c) = 0;

    /**
     * @brief Delete the element at position 'pos'.
     *
     * @param pos The position of the element to delete.
     */
    virtual void deleteElement(size_t pos) = 0;

    /**
     * @brief Delete the elements at position 'pos'.
     *
     * @param pos The position of the first element to delete.
     * @param len The length of the region to delete.
     */
    virtual void deleteElements(size_t pos, size_t len) = 0;


    /**
     * @brief Get the element at position 'pos' as a character.
     *
     * @param pos The position of the character to retrieve.
     */
    virtual const T& getElement(size_t pos) const = 0;

    /** @} */

    /**
     * @name Provide direct access to the list content.
     *
     * @warning These operators allow you to modifiy the list content.
     * No alphabet checking is performed for your modifications, so use with care, or
     * consider using the setContent() method.
     *
     * @{
     */

    /**
     * @brief Operator [] overloaded for quick access to a character in list.
     *
     * @param i The position to retrieve.
     * @return The integer value of character at position i.
     */
    virtual const T& operator[](size_t i) const = 0;
    /**
     * @brief Operator [] overloaded for quick access to a character in list.
     *
     * @param i The position to retrieve.
     * @return The integer value of character at position i.
     */
    virtual T& operator[](size_t i) = 0;

    /**
     * @brief Randomly shuffle the content of the list, with linear complexity.
     */
    virtual void shuffle() = 0;
    /** @} */
  };


  template<class T>
  class CoreSymbolListEditionEvent
  {
  private:
    CoreSymbolList<T>* list_;

  public:
    CoreSymbolListEditionEvent(CoreSymbolList<T>* list):
      list_(list) {}

    CoreSymbolListEditionEvent(const CoreSymbolListEditionEvent<T>& slee): list_(slee.list_) {}
    
    CoreSymbolListEditionEvent<T>& operator=(const CoreSymbolListEditionEvent<T>& slee) { 
      list_ = slee.list_;
      return *this;
    }
    
    virtual ~CoreSymbolListEditionEvent() {}

  public:
    virtual CoreSymbolList<T>* getCoreSymbolList() { return list_; }
    virtual const CoreSymbolList<T>* getCoreSymbolList() const { return list_; }
  };


  template<class T>
  class CoreSymbolListInsertionEvent:
    public CoreSymbolListEditionEvent<T>
  {
  private:
    size_t pos_;
    size_t len_;

  public:
    CoreSymbolListInsertionEvent(CoreSymbolList<T>* list, size_t pos, size_t len):
      CoreSymbolListEditionEvent<T>(list), pos_(pos), len_(len) {}

  public:
    virtual size_t getPosition() const { return pos_; }
    virtual size_t getLength() const { return len_; }
  };


  template<class T>
  class CoreSymbolListDeletionEvent:
    public CoreSymbolListEditionEvent<T>
  {
  private:
    size_t pos_;
    size_t len_;

  public:
    CoreSymbolListDeletionEvent(CoreSymbolList<T>* list, size_t pos, size_t len):
      CoreSymbolListEditionEvent<T>(list), pos_(pos), len_(len) {}

  public:
    virtual size_t getPosition() const { return pos_; }
    virtual size_t getLength() const { return len_; }
  };


  template<class T>
  class CoreSymbolListSubstitutionEvent:
    public CoreSymbolListEditionEvent<T>
  {
  private:
    size_t begin_;
    size_t end_;

  public:
    CoreSymbolListSubstitutionEvent(CoreSymbolList<T>* list, size_t begin, size_t end) :
      CoreSymbolListEditionEvent<T>(list), begin_(begin), end_(end) {}

  public:
    virtual size_t getBegin() const { return begin_; }
    virtual size_t getEnd() const { return end_; }
  };

  template<class T>
  class CoreSymbolListListener :
    public virtual Clonable
  {
  public:
    virtual ~CoreSymbolListListener() {}

    virtual CoreSymbolListListener* clone() const = 0;

  public:
    virtual bool isRemovable() const = 0;
    virtual bool isShared() const = 0;
    virtual void beforeSequenceChanged(const CoreSymbolListEditionEvent<T>& event) = 0;
    virtual void afterSequenceChanged(const CoreSymbolListEditionEvent<T>& event) = 0;
    virtual void beforeSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) = 0;
    virtual void afterSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) = 0;
    virtual void beforeSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) = 0;
    virtual void afterSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) = 0;
    virtual void beforeSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) = 0;
    virtual void afterSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) = 0;
  };


  /**
   * @brief A event-driven CoreSymbolList object.
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

  template<class T>
  class EdCoreSymbolList: 
    public virtual CoreSymbolList<T>
  {

  private:
    /**
     * @brief The Alphabet attribute must be initialized in constructor and then can never be changed.
     * 
     * To apply another alphabet to a list you'll have to create a new list.
     */
    const Alphabet* alphabet_;

    bool propagateEvents_;

  protected:
    /**
     * @brief The list content.
     */
    std::vector<T> content_;

    /**
     * @brief Contains the listeners.
     */
    std::vector<CoreSymbolListListener<T>*> listeners_;


  public: 
    /**
     * @brief Build a new void CoreSymbolList object with the specified alphabet.
     *
     * @param alpha The alphabet to use.
     */
    EdCoreSymbolList(const Alphabet* alpha) : alphabet_(alpha), propagateEvents_(true), content_(), listeners_() {}

    /**
     * @brief Build a new CoreSymbolList object with the specified alphabet.
     * The content of the site is initialized from a vector of integers.
     *
     * @param list     The content of the site.
     * @param alpha    The alphabet to use.
     * @throw BadIntException If the content does not match the specified alphabet.
     */

    EdCoreSymbolList(const std::vector<T>& list, const Alphabet* alpha) :
	alphabet_(alpha), propagateEvents_(true), content_(), listeners_()
    {
      setContent(list);
    }
    

    /**
     * @brief The generic copy constructor.
     */
    EdCoreSymbolList(const CoreSymbolList<T>& list) :
      alphabet_(list.getAlphabet()), propagateEvents_(true), content_(list.size()), listeners_()
    {
      for (size_t i = 0; i < list.size(); ++i) {
        content_[i] = list[i];
      }
    }
    

    /**
     * @brief The copy constructor.
     */
    EdCoreSymbolList(const EdCoreSymbolList<T>& list) :
      alphabet_(list.getAlphabet()), propagateEvents_(list.propagateEvents_), content_(list.content_), listeners_(list.listeners_)
    {
      for (size_t i = 0; i < listeners_.size(); ++i)
        if (!list.listeners_[i]->isShared())
          listeners_[i] = dynamic_cast<CoreSymbolListListener<T>*>(list.listeners_[i]->clone());
    }

    /**
     * @brief The generic assignment operator.
     */
    EdCoreSymbolList<T>& operator=(const CoreSymbolList<T>& list)
    {
      content_.resize(list.size());
      for (size_t i = 0; i < list.size(); ++i) {
        content_[i] = list[i];
      }
      alphabet_        = list.getAlphabet();
      propagateEvents_ = true;
      for (size_t i = 0; i < listeners_.size(); ++i)
        if (!listeners_[i]->isShared())
          delete listeners_[i];
      listeners_.clear();
      return *this;
    }


    /**
     * @brief The assignment operator.
     */
    
    EdCoreSymbolList<T>& operator=(const EdCoreSymbolList& list)
    {
      content_         = list.getContent();
      alphabet_        = list.getAlphabet();
      propagateEvents_ = list.propagateEvents_;
      for (size_t i = 0; i < listeners_.size(); ++i)
        delete listeners_[i];
      listeners_ = list.listeners_;
      for (size_t i = 0; i < listeners_.size(); ++i)
        if (!list.listeners_[i]->isShared())
          listeners_[i] = dynamic_cast<CoreSymbolListListener<T>*>(list.listeners_[i]->clone());
      return *this;
    }

    /**
     * @name The Clonable interface
     *
     * @{
     */
    EdCoreSymbolList* clone() const { return new EdCoreSymbolList(* this); }
    /** @} */

    // Class destructor
    virtual ~EdCoreSymbolList()
    {
      for (size_t i = 0; i < listeners_.size(); ++i) {
        if (listeners_[i] && !listeners_[i]->isShared()) {
          delete listeners_[i];
        }
      }
    }

  public:

    const Alphabet* getAlphabet() const { return alphabet_; }

    size_t size() const { return static_cast<size_t>(content_.size()); }

    const std::vector<T>& getContent() const { return content_; }

    std::vector<T>& getContent() { return content_; }

    virtual void setContent(const std::vector<T>& list)
    {
      CoreSymbolListEditionEvent<T> event(this);
      fireBeforeSequenceChanged(event);
      
      //Sequence is valid:
      content_ = list;
      fireAfterSequenceChanged(event);
    }
    
    void deleteElement(size_t pos)
    {
      if (pos >= content_.size())
        throw IndexOutOfBoundsException("EdCoreSymbolList::deleteElement. Invalid position.", pos, 0, size() - 1);
      CoreSymbolListDeletionEvent<T> event(this, pos, 1);
      fireBeforeSequenceDeleted(event);
      content_.erase(content_.begin() + std::ptrdiff_t(pos));
      fireAfterSequenceDeleted(event);
    }

		
    void deleteElements(size_t pos, size_t len)
    {
      if(pos + len > content_.size())
        throw IndexOutOfBoundsException("EdCoreSymbolList::deleteElements. Invalid position.", pos + len, 0, size() - 1);
      CoreSymbolListDeletionEvent<T> event(this, pos, len);
      fireBeforeSequenceDeleted(event);
      content_.erase(content_.begin() + std::ptrdiff_t(pos), content_.begin() + std::ptrdiff_t(pos + len));
      fireAfterSequenceDeleted(event);
    }

    const T& getElement(size_t pos) const
    {
      if (pos >= content_.size())
        throw IndexOutOfBoundsException("EdCoreSymbolList::getChar. Invalid position.", pos, 0, size() - 1);
      std::string c = "";
      try {
        c = alphabet_->intToChar(content_[pos]);
      } catch(BadIntException bie) {
        //This should never happen!
      }
      return c;
    }

    virtual void addElement(int v)
    {
      CoreSymbolListInsertionEvent<T> event(this, size(), 1);
      fireBeforeSequenceInserted(event);
      content_.push_back(v);
      fireAfterSequenceInserted(event);
    }

    virtual void addElement(size_t pos, int v)
    {
      //test:
      if (pos >= content_.size())
        throw IndexOutOfBoundsException("EdCoreSymbolList::addElement. Invalid position.", pos, 0, size() - 1);
      CoreSymbolListInsertionEvent<T> event(this, pos, 1);
      fireBeforeSequenceInserted(event);
      content_.insert(content_.begin() + std::ptrdiff_t(pos), v);
      fireAfterSequenceInserted(event);
    }


    virtual void setElement(size_t pos, int v)
    {
      //test:
      if (pos >= content_.size())
        throw IndexOutOfBoundsException("EdCoreSymbolList::setElement. Invalid position.", pos, 0, size() - 1);
      CoreSymbolListSubstitutionEvent<T> event(this, pos, pos);
      fireBeforeSequenceSubstituted(event);
      content_[pos] = v;
      fireAfterSequenceSubstituted(event);
    }
    
    int getValue(size_t pos) const
    {
      if (pos >= content_.size())
        throw IndexOutOfBoundsException("EdCoreSymbolList::getValue. Invalid position.", pos, 0, size() - 1);
      return content_[pos];
    }

    const T& operator[](size_t i) const { return content_[i]; }
		
    T& operator[](size_t i) { return content_[i]; }

    void shuffle()
    {
      random_shuffle(content_.begin(), content_.end());
    }

    /**
     * @name Events handling
     *
     * @{
     */
    virtual size_t getNumberOfListeners() const { return listeners_.size(); }

    virtual const CoreSymbolListListener<T>& getListener(size_t i) const {
      if (listeners_[i] == 0)
        std::cout << "EdCoreSymbolList::getListener: aie!!!" << std::endl;
      return *listeners_[i];
    }
    
    virtual CoreSymbolListListener<T>& getListener(size_t i) { 
      if (listeners_[i] == 0)
        std::cout << "EdCoreSymbolList::getListener: aie!!!" << std::endl;
      return *listeners_[i];
    }

    virtual void addCoreSymbolListListener(CoreSymbolListListener<T>* listener) { 
      listeners_.push_back(listener);
    }

    virtual void removeCoreSymbolListListener(CoreSymbolListListener<T>* listener) {
      if (listener->isRemovable())
        listeners_.erase(remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
      else
        throw Exception("EdCoreSymbolList::removeCoreSymbolListListener. Listener is not removable.");
    } 
 
  protected:
    virtual void beforeSequenceChanged(const CoreSymbolListEditionEvent<T>& event) {};
    virtual void afterSequenceChanged(const CoreSymbolListEditionEvent<T>& event) {};
    virtual void beforeSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) {};
    virtual void afterSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) {};
    virtual void beforeSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) {};
    virtual void afterSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) {};
    virtual void beforeSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) {};
    virtual void afterSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) {};

    void fireBeforeSequenceChanged(const CoreSymbolListEditionEvent<T>& event) {
      beforeSequenceChanged(event);
      if (propagateEvents_)
        for (size_t i = 0; i < listeners_.size(); ++i)
          listeners_[i]->beforeSequenceChanged(event);
    }

    void fireAfterSequenceChanged(const CoreSymbolListEditionEvent<T>& event) {
      afterSequenceChanged(event);
      if (propagateEvents_)
        for (size_t i = 0; i < listeners_.size(); ++i)
          listeners_[i]->afterSequenceChanged(event);
    }
   
    void fireBeforeSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) {
      beforeSequenceInserted(event);
      if (propagateEvents_)
        for (size_t i = 0; i < listeners_.size(); ++i)
          listeners_[i]->beforeSequenceInserted(event);
    }

    void fireAfterSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) {
      afterSequenceInserted(event);
      if (propagateEvents_)
        for (size_t i = 0; i < listeners_.size(); ++i)
          listeners_[i]->afterSequenceInserted(event);
    }

    void fireBeforeSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) {
      beforeSequenceDeleted(event);
      if (propagateEvents_)
        for (size_t i = 0; i < listeners_.size(); ++i)
          listeners_[i]->beforeSequenceDeleted(event);
    }

    void fireAfterSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) {
      afterSequenceDeleted(event);
      if (propagateEvents_)
        for (size_t i = 0; i < listeners_.size(); ++i)
          listeners_[i]->afterSequenceDeleted(event);
    }

    void fireBeforeSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) {
      beforeSequenceSubstituted(event);
      if (propagateEvents_)
        for (size_t i = 0; i < listeners_.size(); ++i)
          listeners_[i]->beforeSequenceSubstituted(event);
    }

    void fireAfterSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) {
      afterSequenceSubstituted(event);
      if (propagateEvents_)
        for (size_t i = 0; i < listeners_.size(); ++i)
          listeners_[i]->afterSequenceSubstituted(event);
    }
    /** @} */

  protected:
    void propagateEvents(bool yn) { propagateEvents_ = yn; }
    bool propagateEvents() const { return propagateEvents_; }

  };


} //end of namespace bpp.

#endif // _CORE_SYMBOLLIST_H_

