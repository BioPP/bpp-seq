//
// File: Phylip.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Oct 27 12:22:56 2003
//

#ifndef _PHYLIP_H_
#define _PHYLIP_H_

#include "AbstractISequence.h"
#include "AbstractOSequence.h"
#include "Sequence.h"
#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"

// From the STL:
#include <iostream>

using namespace std;

class Phylip: public AbstractISequence, public AbstractOSequence
{
	protected:

		/**
		 * @brief The maximum number of chars to be written on a line.
		 */
		unsigned int _charsByLine;

		/* this class allows two kind of Phylip format:
		 * traditional, with names limited to 10 chars,
		 * and 'extended', defined by PAML, with names separated from sequences by at least 6 white spaces.
		 */
		bool _extended;
		/* tells if sequences are in the seuqential or the interleave format/
		 */
		bool _sequential;
	
	public:
		// Class constructor
		// Constructor is currently useless to make easier an eventual later addition of parameters, etc.
		Phylip(bool extended = true, bool sequential = true, unsigned int charsByLine = 100);

		// Class destructor
		virtual ~Phylip();

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

		// Method to get number of sequences contained in specified file
		int getNumberOfSequences(const string & path) const throw (Exception);

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
		
	protected:
		//Reading tools:
		static string getNextNonEmptyLine(istream & in); 
		const vector<string> splitNameAndSequence(const string & s) const; 
		void readSequential (istream & in, VectorSequenceContainer & vsc) const throw (Exception);
		void readInterleaved(istream & in, VectorSequenceContainer & vsc) const throw (Exception);
		//Writing tools:
		vector<string> getSizedNames(const vector<string> & names) const;
		void writeSequential (ostream & out, const SequenceContainer & sc, int charsByLine) const;
		void writeInterleaved(ostream & out, const SequenceContainer & sc, int charsByLine) const;
};


#endif	//_PHYLIP_H_
