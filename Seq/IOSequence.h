/*
 * File IOSequence.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Wednesday July 30 2003
*/

// Secured inclusion of header's file
#ifndef _IOSEQUENCE_H_
#define _IOSEQUENCE_H_

#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"
#include "Alphabet.h"
#include "Sequence.h"

// From Utils:
#include <Utils/Exceptions.h>

// From STL:
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

/**
 * @brief The IOSequence interface.
 */
class IOSequence
{
	public:
		// Class destructor
		virtual ~IOSequence() {}

	public:

		/**
		 * @brief Get the name of the file format.
		 *
		 * @return The name of the format implemented.
		 */
		virtual const string getFormatName() const = 0;
		
		/**
		 * @brief Get a description of the file format.
		 *
		 * @return A description of the format implemented.
		 */
		virtual const string getFormatDescription() const = 0;
};

#endif	// _IOSEQUENCE_H_
