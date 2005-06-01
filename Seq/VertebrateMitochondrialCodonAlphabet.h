//
// File: VertebrateMitochondrialCodonAlphabet.h
// Created by: bazin <bazin@univ-montp2.fr>
// Created on: wen mar  2 16:01:59 CET 2005
///

#ifndef _VERTEBRATEMITOCHONDRIALCODONALPHABET_H_
#define _VERTEBRATEMITOCHONDRIALCODONALPHABET_H_


#include "CodonAlphabet.h"
/**
 * @brief <p>This class implements the vertebrate mitochondrial alphabet.</p>
 */
class VertebrateMitochondrialCodonAlphabet : public CodonAlphabet
{
	public:
		VertebrateMitochondrialCodonAlphabet(const NucleicAlphabet * alpha);
		virtual ~VertebrateMitochondrialCodonAlphabet();
	
	public:
		/* Get the number of codons (without stops).
		 */
		unsigned int getSize() const;
		unsigned int getNumberOfTypes() const;
	
		string getAlphabetType() const;

		bool isStop(      int      codon) const;
		bool isStop(const string & codon) const;
};


#endif	//_VERTEBRATEMITOCHONDRIALCODONALPHABET_H_
