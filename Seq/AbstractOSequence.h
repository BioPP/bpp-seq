#ifndef _ABSTRACTOSEQUENCE_H_
#define _ABSTRACTOSEQUENCE_H_

#include "VectorSequenceContainer.h"
#include "Alphabet.h"
#include "OSequence.h"

// From the STL:
#include <string>
#include <fstream>
using namespace std;

/**
 * @brief Low level implementation of the ISequence interface.
 */
class AbstractOSequence: public virtual OSequence {

	public: 
		~AbstractOSequence() {}

	public:

		/**
		 * @name OSequence methods:
		 *
		 * @{
		 */ 
		void write(     ostream & output, const SequenceContainer & sc                     ) const throw (Exception) = 0;
		void write(const string & path  , const SequenceContainer & sc, bool overwrite=true) const throw (Exception)
		{
			// Open file in specified mode
			ofstream output(path.c_str(), overwrite ? (ios::out) : (ios::out|ios::app));
			write(output, sc);
			output.close();
		}
		/** @} */
};

#endif //_ABSTRACTOSEQUENCE_H_
