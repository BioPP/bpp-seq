//
// File: GeneticCode.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Oct 13 15:37:25 2003
//

#ifndef _GENETICCODE_H_
#define _GENETICCODE_H_

#include "Translator.h"
#include "CodonAlphabet.h"
#include "ProteicAlphabet.h"

// From Utils:
#include <Utils/Exceptions.h>

class StopCodonException : public Exception {

	protected:
		string codon;
			
	public:
		// Class constructor
		StopCodonException(const string & text, const string & codon);
	
		// Class destructor
		~StopCodonException() throw ();
	public:
		virtual string getCodon() const;
};

class GeneticCode : public AbstractTranslator
{
	protected:
		const CodonAlphabet * codonAlphabet;
		const ProteicAlphabet * proteicAlphabet;
	
	public:
		GeneticCode();
		virtual ~GeneticCode();
	
	//We implement these functions here:
	public:
		const Alphabet * getSourceAlphabet() const;
		const Alphabet * getTargetAlphabet() const;
		virtual int    translate(      int i     ) const throw (BadIntException, Exception)  = 0;		
		virtual string translate(const string & c) const throw (BadCharException, Exception) = 0;
		virtual Sequence * translate(const Sequence & sequence) const throw (Exception) {
			return AbstractTranslator::translate(sequence);	
		}
		
	public:
		bool areSynonymes(      int i     ,       int j     ) const throw (BadIntException);
		bool areSynonymes(const string & i, const string & j) const throw (BadCharException);
		vector<int>    getSynonymes(      int      aminoacid) const throw (BadIntException);
		vector<string> getSynonymes(const string & aminoacid) const throw (BadCharException);
};


#endif	//_GENETICCODE_H_
