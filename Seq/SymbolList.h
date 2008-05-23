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

#include "Alphabet.h"

// From Utils:
#include <Utils/Clonable.h>

// From the STL:
#include <string>
#include <vector>

using namespace std;

namespace bpp
{

/**
 * @brief The SymbolList object.
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
class SymbolList: 
  public virtual Clonable
{

  protected:
 		/**
		 * @brief The Alphabet attribute must be initialized in constructor and then can never be changed.
		 * 
		 * To apply another alphabet to a list you'll have to create a new list.
		 */
		const Alphabet * _alphabet;

		/**
		 * @brief The list content.
		 */
		mutable vector<int> _content;

	public: 
		/**
		 * @brief Build a new void SymbolList object with the specified alphabet.
		 *
		 * @param alpha The alphabet to use.
		 */
		SymbolList(const Alphabet * alpha);

		/**
		 * @brief Build a new SymbolList object with the specified alphabet.
		 * The content of the site is initialized from a vector of characters.
		 *
		 * @param list     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @throw BadCharException If the content does not match the specified alphabet.
		 */
		SymbolList(const vector<string> & list, const Alphabet * alpha) throw (BadCharException);

		/**
		 * @brief Build a new SymbolList object with the specified alphabet.
		 * The content of the site is initialized from a vector of integers.
		 *
		 * @param list     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @throw BadIntException If the content does not match the specified alphabet.
		 */
		SymbolList(const vector<int> & list, const Alphabet * alpha) throw (BadIntException);

		/**
		 * @brief The copy constructor.
		 */
		SymbolList(const SymbolList & list);

		/**
		 * @brief The assignment operator.
		 */
		SymbolList & operator = (const SymbolList & list);

		/**
		 * @name The Clonable interface
		 *
		 * @{
		 */
#ifndef NO_VIRTUAL_COV
		SymbolList*
#else
		Clonable*
#endif
		clone() const { return new SymbolList(* this); }
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
		virtual const Alphabet* getAlphabet() const { return _alphabet; }

		/**
		 * @brief Get the number of elements in the list.
		 *
		 * @return The number of sites in the list.
		 */
		virtual unsigned int size() const { return _content.size(); }

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
		virtual const vector<int>& getContent() const { return _content; }

		/**
		 * @brief Set the whole content of the list.
		 *
		 * @param list The new content of the list.
		 * @see The list constructor for information about the way lists are internaly stored.
		 */
		virtual void setContent(const vector<int> & list) throw (BadIntException);

		/**
		 * @brief Set the whole content of the list.
		 *
		 * @param list The new content of the list.
		 * @see The list constructor for information about the way lists are internaly stored.
		 */
		virtual void setContent(const vector<string> & list) throw (BadCharException);

		/** @} */

		/**
		 * @brief Convert the list as a string.
		 *
		 * This method is useful for dumping a list to a file or to the screen for display.
		 *
		 * @return The whole list as a string.
		 */
		virtual string toString() const;

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
		virtual void addElement(string c) throw (BadCharException);

		/**
		 * @brief Add a character at a certain position in the list.
		 *
		 * @param pos The postion where to insert the element.
		 * @param c   The character to add, given as a string.
		 */
		virtual void addElement(unsigned int pos, string c) throw (BadCharException, IndexOutOfBoundsException);

		/**
		 * @brief Set the element at position 'pos' to character 'c'.
		 *
		 * @param pos The position of the character to set.
		 * @param c   The value of the element, given as a string.
		 */
		virtual void setElement(unsigned int pos, string c) throw (BadCharException, IndexOutOfBoundsException);

		/**
		 * @brief Delete the element at postion 'pos'.
		 *
		 * @param pos The position of the element to delete.
		 */
		virtual void deleteElement(unsigned int pos) throw (IndexOutOfBoundsException);

		/**
		 * @brief Get the element at position 'pos' as a character.
		 *
		 * @param pos The position of the character to retrieve.
		 */
		virtual string getChar(unsigned int pos) const throw (IndexOutOfBoundsException);

		/**
		 * @brief Add a character to the end of the list.
		 *
		 * @param v The character to add, given as an int.
		 */
		virtual void addElement(int v) throw (BadIntException);

		/**
		 * @brief Add a character at a certain position in the list.
		 *
		 * @param pos The postion where to insert the element.
		 * @param v   The character to add, given as an int.
		 */
		virtual void addElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException);

		/**
		 * @brief Set the element at position 'pos' to character 'v'.
		 *
		 * @param pos The position of the character to set.
		 * @param v   The value of the element, given as an int.
		 */
		virtual void setElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException);

		/**
		 * @brief Get the element at position 'pos' as an int.
		 *
		 * @param pos The position of the character to retrieve.
		 */
		virtual int getValue(unsigned int pos) const throw (IndexOutOfBoundsException);

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
		virtual const int & operator[](unsigned int i) const { return _content[i]; }
		/**
		 * @brief Operator [] overloaded for quick access to a character in list.
		 *
		 * @param i The position to retrieve.
		 * @return The integer value of character at position i.
		 */
		virtual int & operator[](unsigned int i) { return _content[i]; }
    /** @} */
};

} //end of namespace bpp.

#endif // _SYMBOLLIST_H_

