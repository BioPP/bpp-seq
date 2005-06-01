#ifndef _ABSTRACTOSEQUENCE_H_
#define _ABSTRACTOSEQUENCE_H_

#include "VectorSequenceContainer.h"
#include "Alphabet.h"
#include "OSequence.h"

// From the STL:
#include <string>
using namespace std;

/**
 * @brief Low level implementation of the ISequence interface.
 */
class AbstractOSequence: public OSequence {

	public: 
		~AbstractOSequence() {}

	public:

		/**
		 * @name OSequence methods:
		 *
		 * @{
		 */ 
		void write(     ostream & output, const SequenceContainer & sc                     ) const throw (Exception) = 0;
		void write(const string & path  , const SequenceContainer & sc, bool overwrite=true) const throw (Exception);
		/** @} */
};

#endif //_ABSTRACTOSEQUENCE_H_
