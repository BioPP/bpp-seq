//
// File: StandardCodonAlphabet.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 17:51:36 2003
//

#ifndef _STANDARDCODONALPHABET_H_
#define _STANDARDCODONALPHABET_H_


#include "CodonAlphabet.h"
/**
 * @brief <p>This class implements the standard codon alphabet.</p>
 */
class StandardCodonAlphabet : public CodonAlphabet
{
	public:
		StandardCodonAlphabet(const NucleicAlphabet * alpha);
		virtual ~StandardCodonAlphabet();
	
	public:
		/* Get the number of codons (without stops), send 61.
		 */
		unsigned int getSize() const;
		// return 62 : gap and stops are not included, generic unresolved bases (NNN) counts for one
		unsigned int getNumberOfTypes() const;
	
		string getAlphabetType() const;

		bool isStop(      int      codon) const;
		bool isStop(const string & codon) const;
};


#endif	//_STANDARDCODONALPHABET_H_
