//
// File: InvertebrateMitochondrialCodonAlphabet.h
// Created by: bazin <bazin@univ-montp2.fr>
// Created on: thu mar  1 14:25:09 CET 2005
//

#ifndef _INVERTEBRATEMITOCHONDRIALCODONALPHABET_H_
#define _INVERTEBRATEMITOCHONDRIALCODONALPHABET_H_


#include "CodonAlphabet.h"
/**
 * @brief This class implements the invertebrate mitochondrial codon alphabet.
 */
class InvertebrateMitochondrialCodonAlphabet : public CodonAlphabet
{
	public:
		InvertebrateMitochondrialCodonAlphabet(const NucleicAlphabet * alpha);
		virtual ~InvertebrateMitochondrialCodonAlphabet();
	
	public:

		unsigned int getSize() const;
		unsigned int getNumberOfTypes() const;
	
		string getAlphabetType() const;

		bool isStop(      int      codon) const;
		bool isStop(const string & codon) const;
};


#endif	//_INVERTEBRATEMITOCHONDRIALCODONALPHABET_H_
