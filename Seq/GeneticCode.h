//
// File: GeneticCode.h
// Created by: Julien Dutheil
// Created on: Mon Oct 13 15:37:25 2003
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

Julien.Dutheil@univ-montp2.fr

This software is a computer program whose purpose is to provide classes
for sequences analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _GENETICCODE_H_
#define _GENETICCODE_H_

#include "Translator.h"
#include "CodonAlphabet.h"
#include "ProteicAlphabet.h"

// From Utils:
#include <Utils/Exceptions.h>

class StopCodonException : public Exception
{
	protected:
		string codon;
			
	public:
		// Class constructor
		StopCodonException(const string & text, const string & codon);
	
		// Class destructor
		virtual ~StopCodonException() throw ();
		
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
	
	public:
		/**
		 * @name Methods form the Translator interface.
		 *
		 * @{
		 */
		const Alphabet * getSourceAlphabet() const;
		const Alphabet * getTargetAlphabet() const;
		virtual int translate(int state) const throw (BadIntException, Exception)  = 0;		
		virtual string translate(const string & state) const throw (BadCharException, Exception) = 0;
		virtual Sequence * translate(const Sequence & sequence) const throw (Exception) {
			return AbstractTranslator::translate(sequence);	
		}
		/** @} */
		
	public:
		/**
		 * @name Specific methods.
		 *
		 * @{
		 */
		bool areSynonymous(int i, int j) const throw (BadIntException);
		bool areSynonymous(const string & i, const string & j) const throw (BadCharException);
		vector<int> getSynonymous(int aminoacid) const throw (BadIntException);
		vector<string> getSynonymous(const string & aminoacid) const throw (BadCharException);
		/** @} */
};


#endif	//_GENETICCODE_H_

