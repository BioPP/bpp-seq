//
// File: SymbolList.h
// Created by: Julien Dutheil
//             Laurent Guéguen
// Created on: Fri Apr 9 2005
//             mercredi 12 avril 2017, à 12h 02
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

#ifndef _SYMBOLLIST_H_
#define _SYMBOLLIST_H_

#include "Alphabet/Alphabet.h"
#include <Bpp/Clonable.h>
#include "CoreSymbolList.h"

// From the STL:
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace bpp
{

  /**
   * @brief A SymbolList object.
   *
   * This is a general purpose container, containing an ordered list
   * of T states. The states that allowed to be present in the list
   * are defined by an alphabet object, which is passed to the list
   * constructor by a pointer.
   *
   * @see Alphabet
   */

  template<class T>
  class SymbolList: 
    public virtual CoreSymbolList<T>
  {
  private:
    /**
     * @brief The Alphabet attribute must be initialized in constructor and then can never be changed.
     * 
     * To apply another alphabet to a list you'll have to create a new list.
     */
    const Alphabet* alphabet_;

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
    
    SymbolList(const Alphabet* alpha) : alphabet_(alpha), content_() {}

    /**
     * @brief Build a new SymbolList object with the specified alphabet.
     * The content of the site is initialized from a vector of T objects.
     *
     * @param list     The content of the site.
     * @param alpha    The alphabet to use.
     */
    
    SymbolList(const std::vector<T>& list, const Alphabet* alpha) :
      alphabet_(alpha), content_()
    {
      setContent(list);
    }

    /**
     * @brief The generic copy constructor.
    */
    
    SymbolList(const SymbolList<T>& list) :
      alphabet_(list.alphabet_), content_(list.content_)
    {}


     /**
     * @brief The copy constructor.
     */
    SymbolList(const CoreSymbolList<T>& list) : 
      alphabet_(list.getAlphabet()), content_(list.size())
    {
      for (size_t i = 0; i < list.size(); ++i)
        content_[i] = list[i];
    }


    /**
     * @brief The generic assignment operator.
     */
    
    SymbolList<T>& operator=(const CoreSymbolList<T>& list)
    {
      content_.resize(list.size());
      for (size_t i = 0; i < list.size(); ++i)
        content_[i] = list[i];
      alphabet_ = list.getAlphabet();
      return *this;
    }

    /**
     * @brief The assignment operator.
     */
    
    SymbolList<T>& operator=(const SymbolList<T>& list)
    {
      content_  = list.content_;
      alphabet_ = list.alphabet_;
      return *this;
    }
    
    /**
     * @name The Clonable interface
     *
     * @{
     */

    virtual SymbolList<T>* clone() const = 0;// return new SymbolList<T>(* this); }
    /** @} */

    // Class destructor
    virtual ~SymbolList() {}
    
  public:

    const Alphabet* getAlphabet() const { return alphabet_; }

    size_t size() const { return static_cast<size_t>(content_.size()); }

    virtual void setContent(const std::vector<T>& list)
    {
      content_ = list;
    };

    virtual const std::vector<T>& getContent() const { return content_; }
		
    virtual std::string toString() const
    {
      return "";
    }

    void deleteElement(size_t pos)
    {
      if(pos >= content_.size())
        throw IndexOutOfBoundsException("SymbolList::deleteElement. Invalid position.", pos, 0, size() - 1);
      content_.erase(content_.begin() + static_cast<std::ptrdiff_t>(pos));
    }
    
    void deleteElements(size_t pos, size_t len)
    {
      if (pos + len > content_.size())
        throw IndexOutOfBoundsException("SymbolList::deleteElements. Invalid position.", pos + len, 0, size() - 1);
      content_.erase(content_.begin() + static_cast<std::ptrdiff_t>(pos), content_.begin() + static_cast<std::ptrdiff_t>(pos + len));
    }
    
    void addElement(const T& v)
    {
      content_.push_back(v);
    }

    void addElement(size_t pos, const T& v)
    {
      //test:
      if(pos >= content_.size())
        throw IndexOutOfBoundsException("SymbolList::addElement. Invalid position.", pos, 0, size() - 1);
      content_.insert(content_.begin() + static_cast<std::ptrdiff_t>(pos), v);
    }

    void setElement(size_t pos, const T& v)
    {
      //test:
      if(pos >= content_.size())
        throw IndexOutOfBoundsException("SymbolList::setElement. Invalid position.", pos, 0, size() - 1);
      content_[pos] = v;
    }

    virtual const T& getElement(size_t pos) const
    {
      if(pos >= content_.size())
        throw IndexOutOfBoundsException("SymbolList::getElement. Invalid position.", pos, 0, size() - 1);
      return content_[pos];
    }

    virtual const T& getValue(size_t pos) const
    {
      if (pos >= content_.size())
        throw IndexOutOfBoundsException("SymbolList::getValue. Invalid position.", pos, 0, size() - 1);
      return content_[pos];
    }

    virtual const T& operator[](size_t i) const { return content_[i]; }
		
    virtual T& operator[](size_t i) { return content_[i]; }

    virtual void shuffle()
    {
      random_shuffle(content_.begin(), content_.end());
    }
  };

  
  template<class T>
  class EdSymbolList:
    public SymbolList<T>,
    public virtual EdCoreSymbolList<T>
  {
  private:
    /**
     * @brief The Alphabet attribute must be initialized in constructor and then can never be changed.
     * 
     * To apply another alphabet to a list you'll have to create a new list.
     */
    bool propagateEvents_;

  protected:
    /**
     * @brief Contains the listeners.
     */
    std::vector<CoreSymbolListListener<T>* > listeners_;
    
    /**
     * @brief Build a new void CoreSymbolList object with the specified alphabet.
     *
     * @param alpha The alphabet to use.
     */
    
    EdSymbolList(const Alphabet* alpha) : SymbolList<T>(alpha), propagateEvents_(true), listeners_() {}

    /**
     * @brief Build a new CoreSymbolList object with the specified alphabet.
     * The content of the site is initialized from a vector of integers.
     *
     * @param list     The content of the site.
     * @param alpha    The alphabet to use.
     * @throw BadIntException If the content does not match the specified alphabet.
     */

    EdSymbolList(const std::vector<T>& list, const Alphabet* alpha) :
      SymbolList<T>(list,alpha), propagateEvents_(true), listeners_()
    {}
    
    /**
     * @brief The generic copy constructor.
     */
    EdSymbolList(const CoreSymbolList<T>& list) :
      SymbolList<T>(list), propagateEvents_(true), listeners_()
    {}
    

    /**
     * @brief The copy constructor.
     */
    EdSymbolList(const EdSymbolList<T>& list) :
      SymbolList<T>(list), propagateEvents_(list.propagateEvents_), listeners_(list.listeners_)
    {
      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        if (!list.listeners_[i]->isShared())
          throw Exception("EdSymbolList(EdSymbolList) :  mgmt of non shared listeners to be implemented");

        listeners_[i] = (dynamic_cast<CoreSymbolListListener<T>*>(list.listeners_[i]->clone()));
      }
      
    }

    /**
     * @brief The generic assignment operator.
     */
    EdSymbolList<T>& operator=(const CoreSymbolList<T>& list)
    {
      SymbolList<T>::operator=(list);
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
    
    EdSymbolList<T>& operator=(const EdSymbolList& list)
    {
      SymbolList<T>::operator=(list);
      propagateEvents_ = list.propagateEvents_;
      listeners_ = list.listeners_;
      for (size_t i = 0; i < listeners_.size(); ++i)
        delete listeners_[i];

      for (size_t i = 0; i < listeners_.size(); ++i)
      {
        if (!list.listeners_[i]->isShared())
          throw Exception("EdSymbolList::operator= :  mgmt of non shared listeners to be implemented");
        
        listeners_[i] = dynamic_cast<CoreSymbolListListener<T>*>(list.listeners_[i]->clone());
      }
      
      return *this;
    }

    /**
     * @name The Clonable interface
     *
     * @{
     */
    
    EdSymbolList* clone() const { return new EdSymbolList(* this); }
    /** @} */

    // Class destructor
    virtual ~EdSymbolList()
    {
      for (size_t i = 0; i < listeners_.size(); ++i) {
        if (listeners_[i] && !listeners_[i]->isShared()) {
          delete listeners_[i];
        }
      }
    }

  public:
    virtual void setContent(const std::vector<T>& list)
    {
      CoreSymbolListEditionEvent<T> event(this);
      fireBeforeSequenceChanged(event);
      
      //Sequence is valid:
      SymbolList<T>::setContent(list);
      fireAfterSequenceChanged(event);
    }

    void deleteElement(size_t pos)
    {
      CoreSymbolListDeletionEvent<T> event(this, pos, 1);
      fireBeforeSequenceDeleted(event);
      SymbolList<T>::deleteElement(pos);
      fireAfterSequenceDeleted(event);
    }

		
    void deleteElements(size_t pos, size_t len)
    {
      CoreSymbolListDeletionEvent<T> event(this, pos, len);
      fireBeforeSequenceDeleted(event);
      SymbolList<T>::deleteElements(pos,len);
      fireAfterSequenceDeleted(event);
    }

    virtual void addElement(const T& v)
    {
      CoreSymbolListInsertionEvent<T> event(this, SymbolList<T>::size(), 1);
      fireBeforeSequenceInserted(event);
      SymbolList<T>::addElement(v);
      fireAfterSequenceInserted(event);
    }

    virtual void addElement(size_t pos, const T& v)
    {
      CoreSymbolListInsertionEvent<T> event(this, pos, 1);
      fireBeforeSequenceInserted(event);
      SymbolList<T>::addElement(pos,v);
      fireAfterSequenceInserted(event);
    }

    virtual void setElement(size_t pos, const T& v)
    {
      CoreSymbolListSubstitutionEvent<T> event(this, pos, pos);
      fireBeforeSequenceSubstituted(event);
      SymbolList<T>::setElement(pos, v);
      fireAfterSequenceSubstituted(event);
    }

    /**
     * @name Events handling
     *
     * @{
     */
    virtual size_t getNumberOfListeners() const { return listeners_.size(); }

    virtual const CoreSymbolListListener<T>& getListener(size_t i) const {
      if (listeners_[i] == 0)
        std::cout << "EdSymbolList::getListener: aie!!!" << std::endl;
      return *listeners_[i];
    }
    
    virtual CoreSymbolListListener<T>& getListener(size_t i) { 
      if (listeners_[i] == 0)
        std::cout << "EdSymbolList::getListener: aie!!!" << std::endl;
      return *listeners_[i];
    }

    virtual void addCoreSymbolListListener(CoreSymbolListListener<T>* listener) { 
      listeners_.push_back(listener);
    }

    virtual void removeCoreSymbolListListener(CoreSymbolListListener<T>* listener) {
      if (listener->isRemovable())
        listeners_.erase(remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
      else
        throw Exception("EdSymbolList::removeCoreSymbolListListener. Listener is not removable.");
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

#endif // _SYMBOLLIST_H_

