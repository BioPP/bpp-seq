//
// File: NucleicAcidsReplication.cpp
// Created by: Julien Dutheil
// Created on: Fri May 20 14:40 2005
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

#include "NucleicAcidsReplication.h"

using namespace bpp;

using namespace std;

NucleicAcidsReplication::NucleicAcidsReplication(const NucleicAlphabet * nuc1, const NucleicAlphabet * nuc2) : _nuc1(nuc1), _nuc2(nuc2)
{
  _trans[-1] = -1;
  _trans[0] = 3;
  _trans[1] = 2;
  _trans[2] = 1;
  _trans[3] = 0;

  _trans[4] = 9;
  _trans[5] = 8;
  _trans[6] = 6;
  _trans[7] = 7;
  _trans[8] = 5;
  _trans[9] = 4;

  _trans[10] = 13;
  _trans[11] = 12;
  _trans[12] = 11;
  _trans[13] = 10;

  _trans[14] = 14;
}

int NucleicAcidsReplication::translate(int state) const throw (BadIntException)
{
  _nuc1->intToChar(state);
  return _trans[state];
}

string NucleicAcidsReplication::translate(const string & state) const throw (BadCharException)
{
  int i = _nuc1->charToInt(state);
  return _nuc2->intToChar(_trans[i]);
}

Sequence * NucleicAcidsReplication::translate(const Sequence & sequence) const throw (AlphabetMismatchException)
{
  if(sequence.getAlphabet()->getAlphabetType() != getSourceAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("NucleicAcidsReplication::translate", getSourceAlphabet(), getTargetAlphabet());
  Sequence * tSeq = new Sequence(sequence.getName(), "", sequence.getComments(), getTargetAlphabet());
  for(unsigned int i = 0; i < sequence.size(); i++)
  {
    tSeq->addElement(translate(sequence.getValue(i)));
  }
  tSeq->setSense(! tSeq->getSense());
  return tSeq;
}


int NucleicAcidsReplication::reverse(int state) const throw (BadIntException) 
{
  _nuc2->intToChar(state);
  return _trans[state];
}

string NucleicAcidsReplication::reverse(const string & state) const throw (BadCharException)
{
  int i = _nuc2->charToInt(state);
  return _nuc1->intToChar(_trans[i]);
}

Sequence * NucleicAcidsReplication::reverse(const Sequence & sequence) const throw (AlphabetMismatchException, Exception)
{
  if(sequence.getAlphabet()->getAlphabetType() != getTargetAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("NucleicAcidsReplication::reverse", getSourceAlphabet(), getTargetAlphabet());
  Sequence * rSeq = new Sequence(sequence.getName(), "", sequence.getComments(), getSourceAlphabet());
  for(unsigned int i = 0; i < sequence.size(); i++)
  {
    rSeq->addElement(reverse(sequence.getValue(i)));
  }
  rSeq->setSense(! rSeq->getSense());
  return rSeq;
}

