/*
 * File OSequence.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 21 2003
*/

// Secured inclusion of header's file
#ifndef _OSEQUENCE_H_
#define _OSEQUENCE_H_

#include "IOSequence.h"

// From Utils:
#include <Utils/Exceptions.h>

/**
 * @brief The OSequence interface.
 */
class OSequence : public IOSequence
{
	public:
		// Class destructor
		virtual ~OSequence() {}

	public:

		/**
		 * @brief Write a container to a file.
		 *
		 * @param path      The path to the file to write.
		 * @param sc        The container to write.
		 * @param overwrite If true the sequences are written at the beginning of the file instead of being appended.
		 *                  Any previous content will be lost.
		 * @throw Exception If the file is not in the specified format.
		 */
		virtual void write(const string & path, const SequenceContainer & sc, bool overwrite) const throw (Exception) = 0;

		/**
		 * @brief Write a sequence to a file.
		 *
		 * @param path      The path to the file to write.
		 * @param sequence  The sequence to write.
		 * @param overwrite If true the sequences are written at the beginning of the file instead of being appended.
		 *                  Any previous content will be lost.
		 * @throw Exception If the file is not in the specified format.
		 */
		//virtual void write(const string & path, const Sequence * sequence, bool overwrite) const throw (Exception) = 0;
};

#endif	// _OSEQUENCE_H_
