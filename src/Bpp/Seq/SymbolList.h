//
// File: SymbolList.h
// Created by: Julien Dutheil
// Created on: Fri Apr 9 2005
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

// From the STL:
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace bpp
{

/**
 * @brief The SymbolList interface.
 *
 * @see Alphabet
 */
class SymbolList: 
  public virtual Clonable
{

	public: 
		/**
		 * @name The Clonable interface
		 *
		 * @{
		 */
#ifndef NO_VIRTUAL_COV
		SymbolList* clone() const = 0;
#endif
		/** @} */

    // Class destructor
		virtual ~SymbolList() {}

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
		virtual unsigned int size() const = 0;

		/**
		 * @name Acting on the content of the list.
		 *
		 * @{
		 */

		/**
		 * @brief Get the whole content of the list as a vector of int.
		 *
		 * @return A reference to the content of the list.
		 */
		virtual const std::vector<int>& getContent() const = 0;

		/**
		 * @brief Set the whole content of the list.
		 *
		 * @param list The new content of the list.
		 * @see The list constructor for information about the way lists are internaly stored.
		 */
		virtual void setContent(const std::vector<int>& list) throw (BadIntException) = 0;

		/**
		 * @brief Set the whole content of the list.
		 *
		 * @param list The new content of the list.
		 * @see The list constructor for information about the way lists are internaly stored.
		 */
		virtual void setContent(const std::vector<std::string>& list) throw (BadCharException) = 0;

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
		virtual void addElement(const std::string& c) throw (BadCharException) = 0;

		/**
		 * @brief Add a character at a certain position in the list.
		 *
		 * @param pos The postion where to insert the element.
		 * @param c   The character to add, given as a string.
		 */
		virtual void addElement(unsigned int pos, const std::string& c) throw (BadCharException, IndexOutOfBoundsException) = 0;

		/**
		 * @brief Set the element at position 'pos' to character 'c'.
		 *
		 * @param pos The position of the character to set.
		 * @param c   The value of the element, given as a string.
		 */
		virtual void setElement(unsigned int pos, const std::string& c) throw (BadCharException, IndexOutOfBoundsException) = 0;

		/**
		 * @brief Delete the element at position 'pos'.
		 *
		 * @param pos The position of the element to delete.
		 */
		virtual void deleteElement(unsigned int pos) throw (IndexOutOfBoundsException) = 0;

    /**
		 * @brief Delete the elements at position 'pos'.
		 *
		 * @param pos The position of the first element to delete.
     * @param len The length of the region to delete.
		 */
		virtual void deleteElements(unsigned int pos, unsigned int len) throw (IndexOutOfBoundsException) = 0;


		/**
		 * @brief Get the element at position 'pos' as a character.
		 *
		 * @param pos The position of the character to retrieve.
		 */
		virtual std::string getChar(unsigned int pos) const throw (IndexOutOfBoundsException) = 0;

		/**
		 * @brief Add a character to the end of the list.
		 *
		 * @param v The character to add, given as an int.
		 */
		virtual void addElement(int v) throw (BadIntException) = 0;

		/**
		 * @brief Add a character at a certain position in the list.
		 *
		 * @param pos The postion where to insert the element.
		 * @param v   The character to add, given as an int.
		 */
		virtual void addElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException) = 0;

		/**
		 * @brief Set the element at position 'pos' to character 'v'.
		 *
		 * @param pos The position of the character to set.
		 * @param v   The value of the element, given as an int.
		 */
		virtual void setElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException) = 0;

		/**
		 * @brief Get the element at position 'pos' as an int.
		 *
		 * @param pos The position of the character to retrieve.
		 */
		virtual int getValue(unsigned int pos) const throw (IndexOutOfBoundsException) = 0;

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
		virtual const int& operator[](unsigned int i) const = 0;
		/**
		 * @brief Operator [] overloaded for quick access to a character in list.
		 *
		 * @param i The position to retrieve.
		 * @return The integer value of character at position i.
		 */
		virtual int& operator[](unsigned int i) = 0;

    /**
     * @brief Randomly shuffle the content of the list, with linear complexity.
     */
    virtual void shuffle() = 0;
    /** @} */
};


/**
 * @brief A basic SymbolList object.
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
class BasicSymbolList: 
  public virtual SymbolList
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
		std::vector<int> content_;

	public: 
		/**
		 * @brief Build a new void BasicSymbolList object with the specified alphabet.
		 *
		 * @param alpha The alphabet to use.
		 */
		BasicSymbolList(const Alphabet* alpha) : alphabet_(alpha), content_() {}

		/**
		 * @brief Build a new BasicSymbolList object with the specified alphabet.
		 * The content of the site is initialized from a vector of characters.
		 *
		 * @param list     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @throw BadCharException If the content does not match the specified alphabet.
		 */
		BasicSymbolList(const std::vector<std::string>& list, const Alphabet* alpha) throw (BadCharException);

		/**
		 * @brief Build a new BasicSymbolList object with the specified alphabet.
		 * The content of the site is initialized from a vector of integers.
		 *
		 * @param list     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @throw BadIntException If the content does not match the specified alphabet.
		 */
		BasicSymbolList(const std::vector<int>& list, const Alphabet* alpha) throw (BadIntException);

		/**
		 * @brief The generic copy constructor.
		 */
		BasicSymbolList(const SymbolList& list);

		/**
		 * @brief The copy constructor.
		 */
		BasicSymbolList(const BasicSymbolList& list);

		/**
		 * @brief The generic assignment operator.
		 */
		BasicSymbolList& operator=(const SymbolList& list);

		/**
		 * @brief The assignment operator.
		 */
		BasicSymbolList& operator=(const BasicSymbolList& list);

		/**
		 * @name The Clonable interface
		 *
		 * @{
		 */
#ifdef NO_VIRTUAL_COV
		Clonable*
#else
		BasicSymbolList*
#endif
		clone() const { return new BasicSymbolList(* this); }
		/** @} */

    // Class destructor
		virtual ~BasicSymbolList() {}

	public:

		virtual const Alphabet* getAlphabet() const { return alphabet_; }

		virtual unsigned int size() const { return content_.size(); }

		virtual const std::vector<int>& getContent() const { return content_; }
		
    virtual void setContent(const std::vector<int>& list) throw (BadIntException);

		virtual void setContent(const std::vector<std::string>& list) throw (BadCharException);

		virtual std::string toString() const;

		virtual void addElement(const std::string& c) throw (BadCharException);

		virtual void addElement(unsigned int pos, const std::string& c) throw (BadCharException, IndexOutOfBoundsException);

		virtual void setElement(unsigned int pos, const std::string& c) throw (BadCharException, IndexOutOfBoundsException);

		virtual void deleteElement(unsigned int pos) throw (IndexOutOfBoundsException);
		
    virtual void deleteElements(unsigned int pos, unsigned int len) throw (IndexOutOfBoundsException);

		virtual std::string getChar(unsigned int pos) const throw (IndexOutOfBoundsException);

		virtual void addElement(int v) throw (BadIntException);

		virtual void addElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException);

		virtual void setElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException);

		virtual int getValue(unsigned int pos) const throw (IndexOutOfBoundsException);

		virtual const int& operator[](unsigned int i) const { return content_[i]; }
		
    virtual int& operator[](unsigned int i) { return content_[i]; }

    virtual void shuffle()
    {
      random_shuffle(content_.begin(), content_.end());
    }
};

class SymbolListEditionEvent
{
  private:
    SymbolList* list_;

  public:
    SymbolListEditionEvent(SymbolList* list):
      list_(list) {}

    SymbolListEditionEvent(const SymbolListEditionEvent& slee): list_(slee.list_) {}
    
    SymbolListEditionEvent& operator=(const SymbolListEditionEvent& slee) { 
      list_ = slee.list_;
      return *this;
    }

    virtual ~SymbolListEditionEvent() {}

  public:
    virtual SymbolList* getSymbolList() { return list_; }
    virtual const SymbolList* getSymbolList() const { return list_; }
};


class SymbolListInsertionEvent:
  public SymbolListEditionEvent
{
  private:
    unsigned int pos_;
    unsigned int len_;

  public:
    SymbolListInsertionEvent(SymbolList* list, unsigned int pos, unsigned int len):
      SymbolListEditionEvent(list), pos_(pos), len_(len) {}

  public:
    virtual unsigned int getPosition() const { return pos_; }
    virtual unsigned int getLength() const { return len_; }
};


class SymbolListDeletionEvent:
  public SymbolListEditionEvent
{
  private:
    unsigned int pos_;
    unsigned int len_;

  public:
    SymbolListDeletionEvent(SymbolList* list, unsigned int pos, unsigned int len):
      SymbolListEditionEvent(list), pos_(pos), len_(len) {}

  public:
    virtual unsigned int getPosition() const { return pos_; }
    virtual unsigned int getLength() const { return len_; }
};


class SymbolListSubstitutionEvent:
  public SymbolListEditionEvent
{
  private:
    unsigned int begin_;
    unsigned int end_;

  public:
    SymbolListSubstitutionEvent(SymbolList* list, unsigned int begin, unsigned int end) :
      SymbolListEditionEvent(list), begin_(begin), end_(end) {}

  public:
    virtual unsigned int getBegin() const { return begin_; }
    virtual unsigned int getEnd() const { return end_; }
};

class SymbolListListener :
  public virtual Clonable
{
  public:
    virtual ~SymbolListListener() {}

#ifndef NO_VIRTUAL_COV
    virtual SymbolListListener* clone() const = 0;
#endif

  public:
    virtual bool isRemovable() const = 0;
    virtual bool isShared() const = 0;
    virtual void beforeSequenceChanged(const SymbolListEditionEvent& event) = 0;
    virtual void afterSequenceChanged(const SymbolListEditionEvent& event) = 0;
    virtual void beforeSequenceInserted(const SymbolListInsertionEvent& event) = 0;
    virtual void afterSequenceInserted(const SymbolListInsertionEvent& event) = 0;
    virtual void beforeSequenceDeleted(const SymbolListDeletionEvent& event) = 0;
    virtual void afterSequenceDeleted(const SymbolListDeletionEvent& event) = 0;
    virtual void beforeSequenceSubstituted(const SymbolListSubstitutionEvent& event) = 0;
    virtual void afterSequenceSubstituted(const SymbolListSubstitutionEvent& event) = 0;
};


/**
 * @brief A event-driven SymbolList object.
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
class EdSymbolList: 
  public virtual SymbolList
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
		std::vector<int> content_;

    /**
     * @brief Contains the listners.
     */
    std::vector<SymbolListListener*> listeners_;


	public: 
		/**
		 * @brief Build a new void BasicSymbolList object with the specified alphabet.
		 *
		 * @param alpha The alphabet to use.
		 */
		EdSymbolList(const Alphabet* alpha) : alphabet_(alpha), propagateEvents_(true), content_(), listeners_() {}

		/**
		 * @brief Build a new BasicSymbolList object with the specified alphabet.
		 * The content of the site is initialized from a vector of characters.
		 *
		 * @param list     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @throw BadCharException If the content does not match the specified alphabet.
		 */
		EdSymbolList(const std::vector<std::string>& list, const Alphabet* alpha) throw (BadCharException);

		/**
		 * @brief Build a new BasicSymbolList object with the specified alphabet.
		 * The content of the site is initialized from a vector of integers.
		 *
		 * @param list     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @throw BadIntException If the content does not match the specified alphabet.
		 */
		EdSymbolList(const std::vector<int>& list, const Alphabet* alpha) throw (BadIntException);

		/**
		 * @brief The generic copy constructor.
		 */
		EdSymbolList(const SymbolList& list);

		/**
		 * @brief The copy constructor.
		 */
		EdSymbolList(const EdSymbolList& list);

		/**
		 * @brief The generic assignment operator.
		 */
		EdSymbolList& operator=(const SymbolList& list);

		/**
		 * @brief The assignment operator.
		 */
		EdSymbolList& operator=(const EdSymbolList& list);

		/**
		 * @name The Clonable interface
		 *
		 * @{
		 */
#ifdef NO_VIRTUAL_COV
		Clonable*
#else
		EdSymbolList*
#endif
		clone() const { return new EdSymbolList(* this); }
		/** @} */

    // Class destructor
		virtual ~EdSymbolList()
    {
      for (unsigned int i = 0; i < listeners_.size(); ++i) {
        if (listeners_[i] && !listeners_[i]->isShared()) {
          delete listeners_[i];
        }
      }
    }

	public:

		virtual const Alphabet* getAlphabet() const { return alphabet_; }

		virtual unsigned int size() const { return content_.size(); }

		virtual const std::vector<int>& getContent() const { return content_; }
		
    virtual void setContent(const std::vector<int>& list) throw (BadIntException);

		virtual void setContent(const std::vector<std::string>& list) throw (BadCharException);

		virtual std::string toString() const;

		virtual void addElement(const std::string& c) throw (BadCharException);

		virtual void addElement(unsigned int pos, const std::string& c) throw (BadCharException, IndexOutOfBoundsException);

		virtual void setElement(unsigned int pos, const std::string& c) throw (BadCharException, IndexOutOfBoundsException);

		virtual void deleteElement(unsigned int pos) throw (IndexOutOfBoundsException);
		
    virtual void deleteElements(unsigned int pos, unsigned int len) throw (IndexOutOfBoundsException);

		virtual std::string getChar(unsigned int pos) const throw (IndexOutOfBoundsException);

		virtual void addElement(int v) throw (BadIntException);

		virtual void addElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException);

		virtual void setElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException);

		virtual int getValue(unsigned int pos) const throw (IndexOutOfBoundsException);

		virtual const int& operator[](unsigned int i) const { return content_[i]; }
		
    virtual int& operator[](unsigned int i) { return content_[i]; }

    virtual void shuffle()
    {
      random_shuffle(content_.begin(), content_.end());
    }

    /**
     * @name Events handling
     *
     * @{
     */
    virtual unsigned int getNumberOfListeners() const { return listeners_.size(); }

    virtual const SymbolListListener& getListener(unsigned int i) const {
      if (listeners_[i] == 0) std::cout << "aie!!!" << std::endl;
      return *listeners_[i];
    }
    
    virtual SymbolListListener& getListener(unsigned int i) { 
      if (listeners_[i] == 0) std::cout << "aie!!!" << std::endl;
      return *listeners_[i];
    }

    virtual void addSymbolListListener(SymbolListListener* listener) { 
      listeners_.push_back(listener);
    }

    virtual void removeSymbolListListener(SymbolListListener* listener) {
      if (listener->isRemovable())
        listeners_.erase(remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
      else
        throw Exception("EdSymbolList::removeSymbolListListener. Listener is not removable.");
    } 
 
  protected:
    virtual void beforeSequenceChanged(const SymbolListEditionEvent& event) {};
    virtual void afterSequenceChanged(const SymbolListEditionEvent& event) {};
    virtual void beforeSequenceInserted(const SymbolListInsertionEvent& event) {};
    virtual void afterSequenceInserted(const SymbolListInsertionEvent& event) {};
    virtual void beforeSequenceDeleted(const SymbolListDeletionEvent& event) {};
    virtual void afterSequenceDeleted(const SymbolListDeletionEvent& event) {};
    virtual void beforeSequenceSubstituted(const SymbolListSubstitutionEvent& event) {};
    virtual void afterSequenceSubstituted(const SymbolListSubstitutionEvent& event) {};

    void fireBeforeSequenceChanged(const SymbolListEditionEvent& event) {
      beforeSequenceChanged(event);
      if (propagateEvents_)
        for (unsigned int i = 0; i < listeners_.size(); ++i)
          listeners_[i]->beforeSequenceChanged(event);
    }

    void fireAfterSequenceChanged(const SymbolListEditionEvent& event) {
      afterSequenceChanged(event);
      if (propagateEvents_)
        for (unsigned int i = 0; i < listeners_.size(); ++i)
          listeners_[i]->afterSequenceChanged(event);
    }
   
    void fireBeforeSequenceInserted(const SymbolListInsertionEvent& event) {
      beforeSequenceInserted(event);
      if (propagateEvents_)
        for (unsigned int i = 0; i < listeners_.size(); ++i)
          listeners_[i]->beforeSequenceInserted(event);
    }

    void fireAfterSequenceInserted(const SymbolListInsertionEvent& event) {
      afterSequenceInserted(event);
      if (propagateEvents_)
        for (unsigned int i = 0; i < listeners_.size(); ++i)
          listeners_[i]->afterSequenceInserted(event);
    }

    void fireBeforeSequenceDeleted(const SymbolListDeletionEvent& event) {
      beforeSequenceDeleted(event);
      if (propagateEvents_)
        for (unsigned int i = 0; i < listeners_.size(); ++i)
          listeners_[i]->beforeSequenceDeleted(event);
    }

    void fireAfterSequenceDeleted(const SymbolListDeletionEvent& event) {
      afterSequenceDeleted(event);
      if (propagateEvents_)
        for (unsigned int i = 0; i < listeners_.size(); ++i)
          listeners_[i]->afterSequenceDeleted(event);
    }

    void fireBeforeSequenceSubstituted(const SymbolListSubstitutionEvent& event) {
      beforeSequenceSubstituted(event);
      if (propagateEvents_)
        for (unsigned int i = 0; i < listeners_.size(); ++i)
          listeners_[i]->beforeSequenceSubstituted(event);
    }

    void fireAfterSequenceSubstituted(const SymbolListSubstitutionEvent& event) {
      afterSequenceSubstituted(event);
      if (propagateEvents_)
        for (unsigned int i = 0; i < listeners_.size(); ++i)
          listeners_[i]->afterSequenceSubstituted(event);
    }
    /** @} */

  protected:
    void propagateEvents(bool yn) { propagateEvents_ = yn; }
    bool propagateEvents() const { return propagateEvents_; }

};


} //end of namespace bpp.

#endif // _SYMBOLLIST_H_

