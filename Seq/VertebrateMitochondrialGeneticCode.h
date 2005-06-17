//
// File: VertebrateMitochondrialGeneticCode.h
// Created by: bazin <bazin@univ-montp2.fr>
// Created on: wen mar  2 16:01:59 CET 2005
//

#ifndef _VERTEBRATEMITOCHONDRIALGENETICCODE_H_
#define _VERTEBRATEMITOCHONDRIALGENETICCODE_H_

#include "GeneticCode.h"
#include "NucleicAlphabet.h"

class VertebrateMitochondrialGeneticCode : public GeneticCode
{
	public:
		VertebrateMitochondrialGeneticCode(const NucleicAlphabet * alpha);
		virtual ~VertebrateMitochondrialGeneticCode();
	
	public:
		int    translate(           int state) const throw (BadIntException);
		string translate(const string & state) const throw (BadCharException);
};


#endif	//_VERTEBRATEMITOCHONDRIALGENETICCODE_H_
