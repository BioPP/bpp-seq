#ifndef CLUSTAL_H
#define CLUSTAL_H

#include "AbstractISequence.h"

class Clustal : public AbstractISequence  {

	public:
		Clustal() {};
		~Clustal() {};

	public:
	
		/**
		 * @name The ISequence interface
		 *
		 * @{
		 */
		VectorSequenceContainer * read(const string & path, const Alphabet * alpha) const throw (Exception) {
			return AbstractISequence::read(path, alpha);
		}
		void read(const string & path, VectorSequenceContainer & sc) const throw (Exception);
		Sequence * readSequence(const string & path, unsigned int number, const Alphabet * alpha) const throw (Exception);
		Sequence * readSequence(const string & path, unsigned int number) const throw (Exception);
		Sequence * readSequence(const string & path, const string & name, const Alphabet * alpha) const throw (Exception);
		Sequence * readSequence(const string & path, const string & name) const throw (Exception);
		int getNumberOfSequences(const string & path) const throw (Exception);
		/** @} */

		const string getFormatName() const;
		const string getFormatDescription() const;
};
#endif

