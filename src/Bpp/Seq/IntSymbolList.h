//
// File: IntSymbolList.h
// Created by: Julien Dutheil
// Created on: Fri Apr 9 2005
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

#ifndef _INTSYMBOLLIST_H_
#define _INTSYMBOLLIST_H_

#include "Alphabet/Alphabet.h"
#include <Bpp/Clonable.h>
#include "SymbolList.h"

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
  class IntSymbolList :
    virtual public SymbolList<int>
  {
  public:
    IntSymbolList() :
      SymbolList<int>(0)
    {
    }
    
    
    // Class destructor
    virtual ~IntSymbolList() {}

  public:

    /**
     * @name Acting on the content of the list.
     *
     */

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
  
  class BasicIntSymbolList: 
    public virtual IntSymbolList
  {
  public:
    BasicIntSymbolList(const Alphabet* alpha) : SymbolList<int>(alpha)
    {
    }
    

    /**
     * @brief Build a new BasicIntSymbolList object with the specified alphabet.
     * The content of the site is initialized from a vector of characters.
     *
     * @param list     The content of the site.
     * @param alpha    The alphabet to use.
     */
    BasicIntSymbolList(const std::vector<std::string>& list, const Alphabet* alpha);

    /**
     * @brief Build a new BasicIntSymbolList object with the specified alphabet.
     * The content of the site is initialized from a vector of integers.
     *
     * @param list     The content of the site.
     * @param alpha    The alphabet to use.
     */
    BasicIntSymbolList(const std::vector<int>& list, const Alphabet* alpha);

    /**
     * @brief The copy constructor.
     */
    BasicIntSymbolList(const BasicIntSymbolList& list);

    /**
     * @brief The generic assignment operator.
     */
    BasicIntSymbolList& operator=(const IntSymbolList& list);

    /**
     * @brief The assignment operator.
     */
    BasicIntSymbolList& operator=(const BasicIntSymbolList& list);

    /**
     * @name The Clonable interface
     *
     * @{
     */
    BasicIntSymbolList* clone() const { return new BasicIntSymbolList(* this); }
    /** @} */

    // Class destructor
    virtual ~BasicIntSymbolList() {}
    
  public:

    void setContent(const std::vector<int>& list);
    
    void setContent(const std::vector<std::string>& list);

    std::string toString() const;

    virtual void addElement(const std::string& c);

    virtual void addElement(size_t pos, const std::string& c);

    virtual void setElement(size_t pos, const std::string& c);

    virtual std::string getChar(size_t pos) const;

    virtual double getStateValueAt(size_t siteIndex, int state) const
    {
      if (siteIndex >= content_.size())
        throw IndexOutOfBoundsException("IntSymbolList::getStateValueAt.", siteIndex, 0, content_.size() - 1);
      
      return getAlphabet()->isResolvedIn(content_[siteIndex],state)?1.:0.;
    }
    
    virtual double operator()(size_t siteIndex, int state) const 
    {
      return getAlphabet()->isResolvedIn(content_[siteIndex],state)?1.:0.;
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

  class EdIntSymbolList: 
    virtual public EdSymbolList<int>
  {
  public: 
    /**
     * @brief Build a new void BasicIntSymbolList object with the specified alphabet.
     *
     * @param alpha The alphabet to use.
     */
    EdIntSymbolList(const Alphabet* alpha) : EdSymbolList<int>(alpha) 
    {
    }
    

    /**
     * @brief Build a new BasicIntSymbolList object with the specified alphabet.
     * The content of the site is initialized from a vector of characters.
     *
     * @param list     The content of the site.
     * @param alpha    The alphabet to use.
     */
    EdIntSymbolList(const std::vector<std::string>& list, const Alphabet* alpha);

    /**
     * @brief Build a new BasicIntSymbolList object with the specified alphabet.
     * The content of the site is initialized from a vector of integers.
     *
     * @param list     The content of the site.
     * @param alpha    The alphabet to use.
     */
    EdIntSymbolList(const std::vector<int>& list, const Alphabet* alpha);

    /**
     * @brief The generic copy constructor.
     */
    EdIntSymbolList(const IntSymbolList& list);

    /**
     * @brief The copy constructor.
     */
    EdIntSymbolList(const EdIntSymbolList& list);

    /**
     * @brief The generic assignment operator.
     */
    EdIntSymbolList& operator=(const IntSymbolList& list);

    /**
     * @brief The assignment operator.
     */
    EdIntSymbolList& operator=(const EdIntSymbolList& list);

    /**
     * @name The Clonable interface
     *
     * @{
     */
    EdIntSymbolList* clone() const { return new EdIntSymbolList(* this); }
    /** @} */

    ~EdIntSymbolList() 
    {
    }
    
  public:

    virtual void setContent(const std::vector<std::string>& list);

    void setContent(const std::vector<int>& list);

    virtual std::string toString() const;

    virtual void addElement(const std::string& c);

    virtual void addElement(size_t pos, const std::string& c);

    virtual void setElement(size_t pos, const std::string& c);

    virtual std::string getChar(size_t pos) const;

    void addIntSymbolListListener(IntSymbolListListener* listener)
    {
      addCoreSymbolListListener(listener);
    }

  protected:
    virtual void beforeSequenceChanged(const IntSymbolListEditionEvent& event) {};
    virtual void afterSequenceChanged(const IntSymbolListEditionEvent& event) {};
    virtual void beforeSequenceInserted(const IntSymbolListInsertionEvent& event) {};
    virtual void afterSequenceInserted(const IntSymbolListInsertionEvent& event) {};
    virtual void beforeSequenceDeleted(const IntSymbolListDeletionEvent& event) {};
    virtual void afterSequenceDeleted(const IntSymbolListDeletionEvent& event) {};
    virtual void beforeSequenceSubstituted(const IntSymbolListSubstitutionEvent& event) {};
    virtual void afterSequenceSubstituted(const IntSymbolListSubstitutionEvent& event) {};

  };


} //end of namespace bpp.

#endif // _INTSYMBOLLIST_H_

