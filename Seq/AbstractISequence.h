
#ifndef ABSTRACTISEQUENCE_H
#define ABSTRACTISEQUENCE_H

#include "VectorSequenceContainer.h"
#include "Alphabet.h"
#include "ISequence.h"

// From the STL:
#include <string>
using namespace std;

/**
 * @brief Low level implementation of the ISequence interface.
 */
class AbstractISequence: public ISequence {

	public: 
		~AbstractISequence() {}

	public:

		/**
		 * @name ISequence methods:
		 *
		 * @{
		 */ 
		virtual void read(     istream & input, VectorSequenceContainer & sc) const throw (Exception) = 0;
		virtual void read(const string & path , VectorSequenceContainer & sc) const throw (Exception);
		virtual VectorSequenceContainer * read(     istream & input, const Alphabet * alpha) const throw (Exception);
		virtual VectorSequenceContainer * read(const string & path , const Alphabet * alpha) const throw (Exception);
		/** @} */
};

#endif
