//
// File: GeneticCode.cpp
// Created by: Julien Dutheil
// Created on: Mon Oct 13 15:37:25 2003
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

#include "GeneticCode.h"
#include "SequenceTools.h"
#include "AlphabetTools.h"

using namespace bpp;

/**********************************************************************************************/

StopCodonException::StopCodonException(const string & text, const string & codon) :
	Exception("StopCodonException: " + text + "(" + codon + ")"),
	codon(codon) {};
StopCodonException::~StopCodonException() throw() {};
string StopCodonException::getCodon() const { return codon; }

/**********************************************************************************************/

vector<int> GeneticCode::getSynonymous(int aminoacid) const throw (BadIntException)
{
	//test:
	_proteicAlphabet -> intToChar(aminoacid);
	
	vector<int> synonymes;
	for(unsigned int i = 0; i < _codonAlphabet -> getSize(); i++) {
		if(translate(i) == aminoacid) synonymes.push_back(i);
	}
	return synonymes;	
}

/**********************************************************************************************/

vector<string> GeneticCode::getSynonymous(const string & aminoacid) const throw (BadCharException)
{
	//test:
	int aa = _proteicAlphabet -> charToInt(aminoacid);
	
	vector<string> synonymes;
	for(unsigned int i = 0; i < _codonAlphabet -> getSize(); i++) {
		if(translate(i) == aa) synonymes.push_back(_codonAlphabet -> intToChar(i));
	}
	return synonymes;	
}

/**********************************************************************************************/

Sequence * GeneticCode::getCodingSequence(const Sequence & sequence, bool lookForInitCodon, bool includeInitCodon) const throw (Exception)
{
  unsigned int initPos = 0;
  unsigned int stopPos = sequence.size();
  if(AlphabetTools::isCodonAlphabet(sequence.getAlphabet()))
  {
    // Look for AUG(or ATG) codon:
    if(lookForInitCodon) {
      for(unsigned int i = 0; i < sequence.size(); i++)
      {
        vector<int> pos = _codonAlphabet->getPositions(sequence[i]);
        if(pos[0] == 0 && pos[1] == 3 && pos[2] == 2)
        {
          initPos = includeInitCodon ? i : i+1;
          break;
        }
      }
    }
    // Look for stop codon:
    for(unsigned int i = initPos; i < sequence.size(); i++)
    {
      if(_codonAlphabet->isStop(sequence[i]))
      {
        stopPos = i;
        break;
      }
    }
  } else if(AlphabetTools::isNucleicAlphabet(sequence.getAlphabet()))
  {
    // Look for AUG(or ATG) codon:
    if(lookForInitCodon) {
      for(unsigned int i = 0; i < sequence.size() - 2; i++)
      {
        if(sequence[i] == 0 && sequence[i+1] == 3 && sequence[i+2] == 2)
        {
          initPos = includeInitCodon ? i : i+3;
          break;
        }
      }
    }
    // Look for stop codon:
    const NucleicAlphabet * nucAlpha = _codonAlphabet->getNucleicAlphabet();
    for(unsigned int i = initPos; i < sequence.size() - 2; i+=3)
    {
      string codon = nucAlpha -> intToChar(sequence[i])
                   + nucAlpha -> intToChar(sequence[i+1])
                   + nucAlpha -> intToChar(sequence[i+2]);
      if(_codonAlphabet->isStop(codon))
      {
        stopPos = i;
        break;
      }
    }
  } else throw AlphabetMismatchException("Sequence must have alphabet of type nucleic or codon in GeneticCode::getCodingSequence.", NULL, sequence.getAlphabet());
  
  return SequenceTools::subseq(sequence, initPos, stopPos - 1);
}
	
/**********************************************************************************************/

