//
// File: SequenceExceptions.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Nov  3 16:35:30 2003
//

#ifndef _SEQUENCEEXCEPTIONS_H_
#define _SEQUENCEEXCEPTIONS_H_

#include "Alphabet.h"

// From Utils:
#include <Utils/Exceptions.h>

class Sequence;

/*******************************************************************************
 *                            Sequence exceptions                              *
 *******************************************************************************/

/**
 * @brief The sequence exception base class.
 *
 * @see Exception
 */
class SequenceException : public Exception {

	protected:

		/**
		 * @brief A pointer toward a sequence object.
		 */
		const Sequence * sequence;
	
	public:
    
		/**
		 * @brief Build a new SequenceException object.
		 *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param seq A const pointer toward the sequence that threw the exception.
		 */
		SequenceException(const char * text, const Sequence * seq = NULL);
        
		/**
		 * @brief Build a new SequenceException object.
		 *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param seq A const pointer toward the sequence that threw the exception.
		 */
		SequenceException(const string & text, const Sequence * seq = NULL);
	
		~SequenceException() throw();
	
	public:
    
		/**
		 * @brief Get the sequence that threw the exception.
		 *
		 * @return A const pointer toward the sequence.
		 */
		virtual const Sequence * getSequence() const;
};

/******************************************************************************/

/**
 * @brief Exception thrown when a sequence is found to be empty and it should not.
 */
class EmptySequenceException : public SequenceException {

	public:
    
		/**
		 * @brief Build a new EmptySequenceException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param seq  A const pointer toward the sequence that threw the exception.
		 */
		EmptySequenceException(const char * text, const Sequence * seq = NULL);
        
		/**
		 * @brief Build a new EmptySequenceException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param seq A const pointer toward the sequence that threw the exception.
		 */
		EmptySequenceException(const string & text, const Sequence * seq = NULL);
	
		~EmptySequenceException() throw();
};

/******************************************************************************/

/**
 * @brief Exception thrown when a sequence is found to have gap and it should not.
 */
class SequenceWithGapException : public SequenceException {

	public:
    
		/**
		 * @brief Build a new SequenceWithGapException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param seq A const pointer toward the sequence that threw the exception.
		 */
		SequenceWithGapException(const char *   text, const Sequence * seq = NULL);
        
		/**
		 * @brief Build a new SequenceWithGapException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param seq A const pointer toward the sequence that threw the exception.
		 */
		SequenceWithGapException(const string & text, const Sequence * seq = NULL);
	
		~SequenceWithGapException() throw();
};

/******************************************************************************/

/**
 * @brief Exception thrown when two alphabet don't match.
 *
 * Typically, this may occur when you try to add a bad sequence to a container,
 * or concatenate two kinds of sequences, and so on.</p>
 */
class AlphabetMismatchException : public Exception {

	protected:
		const Alphabet * alphabet1, * alphabet2;
	
	public:
    
		/**
		 * @brief Build a new AlphabetMismatchException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha1 A const pointer toward the first alphabet.
		 * @param alpha2 A const pointer toward the second alphabet, i.e. the one which does not match with the first.
		 */
		AlphabetMismatchException(const char *   text = "", const Alphabet * alpha1 = NULL, const Alphabet * alpha2 = NULL);
        
		/**
		 * @brief Build a new AlphabetMismatchException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha1 A const pointer toward the first alphabet.
		 * @param alpha2 A const pointer toward the second alphabet, i.e. the one which does not match with the first.
		 */
		AlphabetMismatchException(const string & text = "", const Alphabet * alpha1 = NULL, const Alphabet * alpha2 = NULL);
	
		~AlphabetMismatchException() throw();

	public:
		/**
		 * @brief Get the alphabets that do not match.
         *
		 * @return a vector of pointers toward the alphabets.
		 */
		vector<const Alphabet *> getAlphabets() const;
};

/******************************************************************************/

/**
 * @brief Exception thrown when a sequence is not align with others.
 *
 * Typically, this may occur when you try to add a bad sequence to a site container.
 */
class SequenceNotAlignedException : public SequenceException {

	public:
    
		/**
         * @brief Build a new SequenceNotAlignedException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param seq  A const pointer toward the sequence that threw the exception.
         */
		SequenceNotAlignedException(const char *   text, const Sequence * seq);

		/**
         * @brief Build a new SequenceNotAlignedException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param seq  A const pointer toward the sequence that threw the exception.
         */
        SequenceNotAlignedException(const string & text, const Sequence * seq);
	
		~SequenceNotAlignedException() throw();
};

/******************************************************************************/

#endif	//_SEQUENCEEXCEPTIONS_H_
