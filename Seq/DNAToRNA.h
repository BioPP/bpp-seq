//
// File: DNAToRNA.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 14:39:29 2003
//

#ifndef _DNATORNA_H_
#define _DNATORNA_H_

#include "Translator.h"
#include "DNA.h"
#include "RNA.h"

class DNAToRNA : public ReverseTranslator
{
	protected:
		const Alphabet * dna, * rna;
	
	public:
		DNAToRNA();
		virtual ~DNAToRNA();
	
	public:
		// Method to convert DNA sequence to RNA
		virtual int    translate(      int i     ) const throw (BadIntException);		
		virtual string translate(const string & c) const throw (BadCharException);		
		// Method to convert a RNA sequence in DNA sequence
		virtual int    reverse(      int i     ) const throw (BadIntException);		
		virtual string reverse(const string & c) const throw (BadCharException);			
};


#endif	//_DNATORNA_H_
