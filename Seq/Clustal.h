#ifndef CLUSTAL_H
#define CLUSTAL_H

#include "AbstractISequence.h"

class Clustal : public AbstractISequence  {

	public:
		Clustal() {};
		~Clustal() {};

	public:
	
		/**
		 * @name The ISequence interface.
		 *
		 * @{
		 */
		VectorSequenceContainer * read(istream & input, const Alphabet * alpha) const throw (Exception) {
			return AbstractISequence::read(input, alpha);
		}
		VectorSequenceContainer * read(const string & path, const Alphabet * alpha) const throw (Exception) {
			return AbstractISequence::read(path, alpha);
		}
		void read(istream & input, VectorSequenceContainer & sc) const throw (Exception);
		void read(const string & path, VectorSequenceContainer & sc) const throw (Exception) {
			AbstractISequence::read(path, sc);
		}
		/** @} */

		/**
		 * @name The IOSequence interface.
		 *
		 * @{
		 */
		const string getFormatName() const;
		const string getFormatDescription() const;
		/** @} */
};
#endif

