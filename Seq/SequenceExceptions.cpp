//
// File: SequenceExceptions.cpp
// Created by: Julien Dutheil
// Created on: Mon Nov  3 16:35:30 2003
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

#include "SequenceExceptions.h"
#include "Sequence.h"

using namespace bpp;

/******************************************************************************/

SequenceException::SequenceException(const char *   text, const Sequence * seq) :
	Exception("SequenceException: " + string(text) + (seq != NULL ? "(" + seq->getName() + ")" : string(""))),
	sequence(seq) {};
		
SequenceException::SequenceException(const string & text, const Sequence * seq) :
	Exception("SequenceException: " + text + (seq != NULL ? "(" + seq->getName() + ")" : string(""))),
	sequence(seq) {};
		
SequenceException::~SequenceException() throw() {};
	
const Sequence * SequenceException::getSequence() const { return sequence; }

/******************************************************************************/

EmptySequenceException::EmptySequenceException(const char *   text, const Sequence * seq) :
	SequenceException("EmptySequenceException: " + string(text), seq) {};
		
EmptySequenceException::EmptySequenceException(const string & text, const Sequence * seq) :
	SequenceException("EmptySequenceException: " + text, seq) {};
		
EmptySequenceException::~EmptySequenceException() throw () {};

/******************************************************************************/
	
SequenceWithGapException::SequenceWithGapException(const char *   text, const Sequence * seq) :
	SequenceException("SequenceWithGapException: " + string(text), seq) {};
		
SequenceWithGapException::SequenceWithGapException(const string & text, const Sequence * seq) :
	SequenceException("SequenceWithGapException: " + text, seq) {};
		
SequenceWithGapException::~SequenceWithGapException() throw () {};

/******************************************************************************/

SequenceNotAlignedException::SequenceNotAlignedException(const char *   text, const Sequence * seq) :
	SequenceException("SequenceNotAlignedException: " + string(text), seq) {};
		
SequenceNotAlignedException::SequenceNotAlignedException(const string & text, const Sequence * seq) :
	SequenceException("SequenceNotAlignedException: " + text, seq) {};
		
SequenceNotAlignedException::~SequenceNotAlignedException() throw () {};

/******************************************************************************/

