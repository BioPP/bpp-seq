//
// File: InvertebrateMitochondrialGeneticCode.h
// Created by: bazin <bazin@univ-montp2.fr>
// Created on: wen mar  2 15:22:46 CET 2005
//

#ifndef _INVERTEBRATEMITOCHONDRIALGENETICCODE_H_
#define _INVERTEBRATEMITOCHONDRIALGENETICCODE_H_

#include "GeneticCode.h"
#include "NucleicAlphabet.h"

class InvertebrateMitochondrialGeneticCode : public GeneticCode
{
	public:
		InvertebrateMitochondrialGeneticCode(const NucleicAlphabet * alpha);
		virtual ~InvertebrateMitochondrialGeneticCode();
	
	public:
		int    translate(      int i     ) const throw (BadIntException);
		string translate(const string & c) const throw (BadCharException);
};


#endif	//_INVERTEBRATEMITOCHONDRIALGENETICCODE_H_
