//
// File: EchinodermMitochondrialGeneticCode.h
// Created by: eric bazin <bazin@univ-montp2.fr>
// Created on: 14 11:31:27 CET 2005
//

#ifndef _ECHINODERMMITOCHONDRIALGENETICCODE_H_
#define _ECHINODERMMITOCHONDRIALGENETICCODE_H_

#include "GeneticCode.h"
#include "NucleicAlphabet.h"

class EchinodermMitochondrialGeneticCode : public GeneticCode
{
	public:
		EchinodermMitochondrialGeneticCode(const NucleicAlphabet * alpha);
		virtual ~EchinodermMitochondrialGeneticCode();
	
	public:
		int    translate(           int state) const throw (BadIntException);
		string translate(const string & state) const throw (BadCharException);
};


#endif	//_ECHINODERMMITOCHONDRIALGENETICCODE_H_
