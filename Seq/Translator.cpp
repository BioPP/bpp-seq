//
// File: Translator.h
// Created by: Julien Dutheil
// Created on: Sun Oct 12 14:25:25 2003
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

#include "Translator.h"

using namespace bpp;

Sequence * AbstractTranslator::translate(const Sequence & sequence) const throw (AlphabetMismatchException, Exception)
{
	if(sequence.getAlphabet()->getAlphabetType() != getSourceAlphabet()->getAlphabetType())
		throw AlphabetMismatchException("AbstractTranslator::translate", getSourceAlphabet(), getTargetAlphabet());
	Sequence * tSeq = new Sequence(sequence.getName(), "", sequence.getComments(), getTargetAlphabet());
	for(unsigned int i = 0; i < sequence.size(); i++)
  {
		tSeq->addElement(translate(sequence.getValue(i)));
	}
	return tSeq;
}

Sequence * AbstractReverseTranslator::reverse(const Sequence & sequence) const throw (AlphabetMismatchException, Exception)
{
	if(sequence.getAlphabet()->getAlphabetType() != getTargetAlphabet()->getAlphabetType())
		throw AlphabetMismatchException("AbstractReverseTranslator::reverse", getSourceAlphabet(), getTargetAlphabet());
	Sequence * rSeq = new Sequence(sequence.getName(), "", sequence.getComments(), getSourceAlphabet());
	for(unsigned int i = 0; i < sequence.size(); i++)
  {
		rSeq->addElement(reverse(sequence.getValue(i)));
	}
	return rSeq;
}

