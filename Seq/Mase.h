/*
 * File Mase.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 21 2003
*/

// Secured inclusion of header's file
#ifndef _MASE_H_
#define _MASE_H_

#include "AbstractISequence.h"
#include "AbstractOSequence.h"
#include "Sequence.h"
#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"

/* Mase Class
 * Define specific methods to use Mase Files
 *
 * This class can be instanciated (i.e. Mase file = new Mase();)
*/
class Mase : public AbstractISequence, public AbstractOSequence
{

	protected:

		/**
		 * @brief The maximum number of chars to be written on a line.
		 */
		unsigned int _charsByLine;

	public :
		// Class constructor
		// Constructor is currently useless to make easier an eventual later addition of parameters, etc.
		Mase(unsigned int charsByLine = 100);

		// Class destructor
		~Mase() { }

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

		/*
		// Methods to read one sequence from a fasta file, by his number or name, and with a known alphabet or not
		Sequence * readSequence(const string & path, unsigned int number, const Alphabet * alpha) const throw (Exception);
		Sequence * readSequence(const string & path, unsigned int number) const throw (Exception);
		Sequence * readSequence(const string & path, const string & name, const Alphabet * alpha) const throw (Exception);
		Sequence * readSequence(const string & path, const string & name) const throw (Exception);

		// Method to get number of sequence characters per line
		// (return the size of first sequence line found in file)
		int getCharsByLine(const string & path) const throw (Exception);

		// Method to get number of sequences contained in specified file
		int getNumberOfSequences(const string & path) const throw (Exception);
		*/

		/**
		 * @name The OSequence interface.
		 *
		 * @{
		 */
		void write(ostream & output, const SequenceContainer & sc) const throw (Exception);
		void write(const string & path, const SequenceContainer & sc, bool overwrite) const throw (Exception) {
			AbstractOSequence::write(path, sc, overwrite);
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

#endif // _MASE_H_
