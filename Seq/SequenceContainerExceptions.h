//
// File: SequenceContainerExceptions.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Nov  3 17:00:05 2003
//

#ifndef _SEQUENCECONTAINEREXCEPTIONS_H_
#define _SEQUENCECONTAINEREXCEPTIONS_H_

// From Utils:
#include <Utils/Exceptions.h>

/*******************************************************************************
 *                    Sequence containers exceptions                           *
 *******************************************************************************/

/**
 * @brief The sequence not found exception base class.
 *
 * @see Exception
 */
class SequenceNotFoundException : public Exception {

	protected:

		/**
		 * @brief The id of the sequence that was to be found.
		 */
		const string id;
	
	public:

		/**
		 * @brief Build a new SequenceNotFoundException object.
		 *
		 * @param text  A message to be passed to the exception hierarchy.
		 * @param seqId A the id of the sequence that was to be found.
		 */
		SequenceNotFoundException(const char * text, const char * seqId = "");

		/**
		 * @brief Build a new SequenceNotFoundException object.
		 *
		 * @param text  A message to be passed to the exception hierarchy.
		 * @param seqId A the id of the sequence that was to be found.
		 */
		SequenceNotFoundException(const string & text, const string & seqId = "");
	
		// Class destructor
		~SequenceNotFoundException() throw();
	
	public:

		/**
		 * @brief Get the id of the sequence that was to be found.
		 *
		 * @return The id of the sequence that was to be found.
		 */
		virtual const string getSequenceId() const;
};


#endif	//_SEQUENCECONTAINEREXCEPTIONS_H_
