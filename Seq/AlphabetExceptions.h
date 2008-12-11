//
// File: AlphabetExceptions.h
// Created by: Julien Dutheil
// Created on: Mon Nov  3 16:41:53 2003
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

#ifndef _ALPHABETEXCEPTIONS_H_
#define _ALPHABETEXCEPTIONS_H_

// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <vector>

using namespace std;

namespace bpp
{

class Alphabet;

/**
 * @brief The alphabet exception base class.
 * 
 * @see Alphabet, Exception
 */
class AlphabetException:
  public Exception
{
	protected:
		const Alphabet * _alphabet;
			
	public:
		/**
		 * @brief Build a new AlphabetException object.
		 * 
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		AlphabetException(const string & text, const Alphabet * alpha = NULL);
	
		virtual ~AlphabetException() throw () {}
        
	public:
		/**
		 * @brief Get the alphabet that threw the exception.
		 * 
		 * @return a const pointer toward the alphabet.
		 */
		virtual const Alphabet * getAlphabet() const;
};

/**
 * @brief An alphabet exception thrown when trying to specify a bad char to the alphabet.
 */
class BadCharException:
  public AlphabetException
{
	protected:
		string _c;
	
	public:
		/**
		 * @brief Build a new BadCharException.
		 * 
		 * @param badChar The faulty character.
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		BadCharException(const string & badChar, const string & text = "", const Alphabet * alpha = NULL);
	
		virtual ~BadCharException() throw() {};
	
	public:
		/**
		 * @brief Get the character that threw the exception.
		 * 
		 * @return the faulty character.
		 */
		virtual string getBadChar() const;
};

/**
 * @brief An alphabet exception thrown when trying to specify a bad int to the alphabet.
 */
class BadIntException:
  public AlphabetException
{
	protected:
		int _i;
	
	public:
		/**
		 * @brief Build a new BadIntException.
		 * @param badInt The faulty integer.
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		BadIntException(int badInt, const string & text = "", const Alphabet * alpha = NULL);
	
		virtual ~BadIntException() throw() {}

	public:
		/**
		 * @brief Get the integer that threw the exception.
		 * 
		 * @return the faulty integer.
		 */
		virtual int getBadInt() const;
};

/**
 * @brief Exception thrown when two alphabets do not match.
 *
 * Typically, this may occur when you try to add a bad sequence to a container,
 * or concatenate two kinds of sequences, and so on.
 */
class AlphabetMismatchException : public Exception
{
	protected:
		const Alphabet * _alphabet1, * _alphabet2;
	
	public:
    
		/**
		 * @brief Build a new AlphabetMismatchException object.
     *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha1 A const pointer toward the first alphabet.
		 * @param alpha2 A const pointer toward the second alphabet, i.e. the one which does not match with the first.
		 */
		AlphabetMismatchException(const char * text = "", const Alphabet * alpha1 = NULL, const Alphabet * alpha2 = NULL);
        
		/**
		 * @brief Build a new AlphabetMismatchException object.
     *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha1 A const pointer toward the first alphabet.
		 * @param alpha2 A const pointer toward the second alphabet, i.e. the one which does not match with the first.
		 */
		AlphabetMismatchException(const string & text = "", const Alphabet * alpha1 = NULL, const Alphabet * alpha2 = NULL);
	
		virtual ~AlphabetMismatchException() throw();

	public:
		/**
		 * @brief Get the alphabets that do not match.
     *
		 * @return a vector of pointers toward the alphabets.
		 */
		vector<const Alphabet *> getAlphabets() const;
};

/**
 * @brief Exception thrown in case no character is available for a certain state in an alphabet.
 */
class CharStateNotSupportedException : public AlphabetException
{
  public:
    /**
     * @brief Build a new CharStateNotSupportedException.
     *
     * @param text A message to be passed to the exception hierarchy.
     * @param alpha A const pointer toward the alphabet that threw the exception.
     */
    CharStateNotSupportedException(const string & text = "", const Alphabet * alpha = NULL);

    virtual ~CharStateNotSupportedException() throw() {};
};

} //end of namespace bpp.

#endif	//_ALPHABETEXCEPTIONS_H_

