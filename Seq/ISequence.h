/*
 * File ISequence.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Wednesday July 30 2003
*/

// Secured inclusion of header's file
#ifndef _ISEQUENCE_H_
#define _ISEQUENCE_H_

#include "IOSequence.h"
#include "Sequence.h"
#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"

// From Utils:
#include <Utils/Exceptions.h>

//From the STL:
#include <iostream>
#include <string>
using namespace std;

/**
 * @brief The ISequence interface.
 */
class ISequence : public IOSequence
{
	public:
		// Class destructor
		virtual ~ISequence() {}

	public:
	
		/**
		 * @brief Create a new container from a stream.
		 *
		 * @param input  The input stream to read.
		 * @param alpha The alphabet to be associated to the container.
		 * @throw Exception If the file is not in the specified format.
		 */
		virtual VectorSequenceContainer * read(istream & input, const Alphabet * alpha) const throw (Exception) = 0;
		/**
		 * @brief Create a new container from a file.
		 *
		 * @param path  The path to the file to read.
		 * @param alpha The alphabet to be associated to the container.
		 * @throw Exception If the file is not in the specified format.
		 */
		virtual VectorSequenceContainer * read(const string & path, const Alphabet * alpha) const throw (Exception) = 0;

		/**
		 * @brief Add sequences to a container from a stream.
		 *
		 * @param input  The input stream to read.
		 * @param alpha The alphabet to be associated to the container.
		 * @throw Exception If the file is not in the specified format.
		 */
		virtual void read(istream & path, VectorSequenceContainer & sc) const throw (Exception) = 0;

/**
		 * @brief Add sequences to a container from a file.
		 *
		 * @param path  The path to the file to read.
		 * @param alpha The alphabet to be associated to the container.
		 * @throw Exception If the file is not in the specified format.
		 */
		virtual void read(const string & path, VectorSequenceContainer & sc) const throw (Exception) = 0;
};

#endif	// ISEQUENCE_H
