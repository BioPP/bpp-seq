//
// File: AlphabetExceptions.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Nov  3 16:41:53 2003
//

#ifndef _ALPHABETEXCEPTIONS_H_
#define _ALPHABETEXCEPTIONS_H_

// From Utils:
#include <Utils/Exceptions.h>

class Alphabet;

/******************************************************************************
 *                         Alphabet exceptions:                               *
 ******************************************************************************/

/**
 * @brief The alphabet exception base class.
 * @see Exception
 */
class AlphabetException : public Exception {

	protected:
		const Alphabet * alphabet;
			
	public: // Class constructors.
		//AlphabetException(const char *   text, const Alphabet * alpha = NULL);
		/**
		 * @brief Build a new AlphabetException.
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		AlphabetException(const string & text, const Alphabet * alpha = NULL);
	
		// Class destructor
		~AlphabetException() throw () {}
        
	public:
		/**
		 * @brief Get the alphabet that threw the exception.
		 * @return a const pointer toward the alphabet.
		 */
		virtual const Alphabet * getAlphabet() const;
};

/**
 * @brief An alphabet exception throw when trying to specify a bad char to the alphabet.
 */
class BadCharException : public AlphabetException {

	protected:
		string c;
	
	public:	// Class constructor
		//BadCharException(const string badChar, const char *   text = "", const Alphabet * alpha = NULL);
		/**
		 * @brief Build a new BadCharException.
		 * @param badChar The faulty character.
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		BadCharException(const string badChar, const string & text = "", const Alphabet * alpha = NULL);
	
		// Class destructor
		~BadCharException() throw() {};
	
	public:
		/**
		 * @brief Get the character that threw the exception.
		 * @return the faulty character.
		 */
		virtual string getBadChar() const;
};

/**
 * @brief An alphabet exception throw when trying to specify a bad int to the alphabet.
 */
class BadIntException : public AlphabetException {

	protected:
		int i;
	
	public:	// Class constructor
		//BadIntException(int badInt, const char *   text = "", const Alphabet * alpha = NULL);
		/**
		 * @brief Build a new BadIntException.
		 * @param adInt The faulty integer.
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		BadIntException(int badInt, const string & text = "", const Alphabet * alpha = NULL);
	
		// Class destructor
		~BadIntException() throw() {}

	public:
		/**
		 * @brief Get the integer that threw the exception.
		 * @return the faulty integer.
		 */
		virtual int getBadInt() const;
};

#endif	//_ALPHABETEXCEPTIONS_H_
