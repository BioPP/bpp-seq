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

namespace bpp
{

/**
 * @brief Exception thrown when a stop codon is found.
 */
class StopCodonException:
  public Exception
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

/**
 * @brief Partial implementation of the Translator interface for genetic code object.
 *
 * A genetic code object if a translator from a codon alphabet to a proteic alphabet.
 * Depending on the codon alphabet used, several genetic code can be implemented.
 *
 * @see CodonAlphabet, ProteicAlphabet
 */
class GeneticCode:
  public AbstractTranslator
{
	protected:
		const CodonAlphabet * _codonAlphabet;
		const ProteicAlphabet * _proteicAlphabet;
	
	public:
		GeneticCode(): AbstractTranslator() {}
		virtual ~GeneticCode() {}
	
	public:
		/**
		 * @name Methods form the Translator interface.
		 *
		 * @{
		 */
		const Alphabet * getSourceAlphabet() const { return _codonAlphabet; }
		const Alphabet * getTargetAlphabet() const { return _proteicAlphabet; }
		virtual int translate(int state) const throw (BadIntException, Exception)  = 0;		
		virtual string translate(const string & state) const throw (BadCharException, Exception) = 0;
		virtual Sequence * translate(const Sequence & sequence) const throw (Exception)
    {
			return AbstractTranslator::translate(sequence);	
		}
		/** @} */
		
	public:
		/**
		 * @name Specific methods.
		 *
		 * @{
		 */
		bool areSynonymous(int i, int j) const throw (BadIntException)
    {
    	return (translate(i) == translate(j));
    }
		bool areSynonymous(const string & i, const string & j) const throw (BadCharException)
    {
      return (translate(i) == translate(j));
    }
		vector<int> getSynonymous(int aminoacid) const throw (BadIntException);
		vector<string> getSynonymous(const string & aminoacid) const throw (BadCharException);
    /**
     * @brief Get the subsequence corresponding to the coding part of a given sequence.
     *
     * If lookForInitCodon if set to 'true', the subsequence will start at the first AUG motif,
     * otherwise the subsequence will start at the begining of the sequence.
     * The subsequence ends at the first stop codon (excluded) found, or the end of the sequence.
     *
     * The sequence may have a nucleotide or codon alphabet.
     * The subsequence has the same alphabet, name and comments of the input sequence.
     * In case of nucleotide sequence and if the lookForInitCodon option is checked, the phase
     * will be determined from the sequence.
     *
     * @param sequence The sequence to parse.
     * @param lookForInitCodon Tell if the AUG codon must be found.
     * @param includeInitCodon (if lookForInitCodon is true) tell if the init codon must be included in the subsequence.
     * @return A nucleotide/codon subsequence.
     */
    Sequence * getCodingSequence(const Sequence & sequence, bool lookForInitCodon = false, bool includeInitCodon = false) const throw (Exception);
		/** @} */
};

} //end of namespace bpp.

#endif	//_GENETICCODE_H_

