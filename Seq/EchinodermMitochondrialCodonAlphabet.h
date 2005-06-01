//
// File: EchinodermMitochondrialCodonAlphabet.h
// Created by: eric bazin <bazin@univ-montp2.fr>
// Created on: 14 11:31:27 CET 2005
//

#ifndef _ECHINODERMMITOCHONDRIALCODONALPHABET_H_
#define _ECHINODERMMITOCHONDRIALCODONALPHABET_H_


#include "CodonAlphabet.h"
/**
 * @brief <p>This class implements the Echinoderm and Faltworms
 * Mitochondrial codon alphabet.</p>
 */
class EchinodermMitochondrialCodonAlphabet : public CodonAlphabet
{
	public:
		EchinodermMitochondrialCodonAlphabet(const NucleicAlphabet * alpha);
		virtual ~EchinodermMitochondrialCodonAlphabet();
	
	public:
		unsigned int getSize() const;
		unsigned int getNumberOfTypes() const;
	
		string getAlphabetType() const;

		bool isStop(      int      codon) const;
		bool isStop(const string & codon) const;
};


#endif	//_ECHINODERMMITOCHONDRIALCODONALPHABET_H_
