//
// File: StandardGeneticCode.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Oct 13 15:39:17 2003
//

#ifndef _STANDARDGENETICCODE_H_
#define _STANDARDGENETICCODE_H_

#include "GeneticCode.h"
#include "NucleicAlphabet.h"

class StandardGeneticCode : public GeneticCode
{
	public:
		StandardGeneticCode(const NucleicAlphabet * alpha);
		virtual ~StandardGeneticCode();
	
	public:
		int    translate(      int i     ) const throw (BadIntException);
		string translate(const string & c) const throw (BadCharException);
};


#endif	//_STANDARDGENETICCODE_H_
