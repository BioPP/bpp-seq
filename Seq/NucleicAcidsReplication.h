//
// File: NucleicAcidsReplication.h
// Created by: Julien Dutheil
// Created on: Fri May 20 14:20 2005
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

#ifndef _NUCLEICACIDSREPLICATION_H_
#define _NUCLEICACIDSREPLICATION_H_

#include "Translator.h"
#include "NucleicAlphabet.h"

// From the STL:
#include <map>

using namespace std;

namespace bpp
{

/**
 * @brief Replication between to nucleic acids.
 *
 * Example of use:
 * - DNA -> DNA: DNA Replication
 * - RNA -> RNA: RNA Replication
 * - DNA -> RNA: Transcription
 * - RNA -> DNA: Reverse transcriptio
 *
 * Since this is an instance of the ReverseIterator interface, transcription and
 * reverse transcription may be achieved from the some instance of the object by
 * using the translate and reverse methods.
 */
class NucleicAcidsReplication : public ReverseTranslator
{
  protected:
    const NucleicAlphabet * _nuc1, *_nuc2;
    mutable map<int, int> _trans;
  
  public:
    NucleicAcidsReplication(const NucleicAlphabet * nuc1, const NucleicAlphabet * nuc2);
    virtual ~NucleicAcidsReplication() {}
  
  public:
    const Alphabet * getSourceAlphabet() const { return _nuc1; }
    const Alphabet * getTargetAlphabet() const { return _nuc2; }
           int translate(int state) const throw (BadIntException);
        string translate(const string & state) const throw (BadCharException);    
    Sequence * translate(const Sequence & sequence) const throw (AlphabetMismatchException);
           int reverse(int state) const throw (BadIntException);    
        string reverse(const string & state) const throw (BadCharException);      
    Sequence * reverse(const Sequence & sequence) const throw (AlphabetMismatchException, Exception);

};

} //end of namespace bpp.

#endif  //_NUCLEICACIDSREPLICATION_H_

