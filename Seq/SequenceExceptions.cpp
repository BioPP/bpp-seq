//
// File: SequenceExceptions.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Nov  3 16:35:30 2003
//

#include "SequenceExceptions.h"
#include "Sequence.h"

	
/*******************************************************************************
 *                            Sequence exceptions                              *
 *******************************************************************************/

SequenceException::SequenceException(const char *   text, const Sequence * seq) :
	Exception("SequenceException: " + string(text) + (seq != NULL ? "(" + seq -> getName() + ")" : string(""))),
	sequence(seq) {};
		
SequenceException::SequenceException(const string & text, const Sequence * seq) :
	Exception("SequenceException: " + text + (seq != NULL ? "(" + seq -> getName() + ")" : string(""))),
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
	
AlphabetMismatchException::AlphabetMismatchException(const char *   text, const Alphabet * alpha1, const Alphabet * alpha2) :
	Exception("AlphabetMismatchException: " + string(text) + (alpha1 != NULL && alpha2 != NULL ? "(" + alpha1 -> getAlphabetType() + ", " + alpha2 -> getAlphabetType() + ")" : string(""))),
	alphabet1(alpha1),
	alphabet2(alpha2) {};
		
AlphabetMismatchException::AlphabetMismatchException(const string & text, const Alphabet * alpha1, const Alphabet * alpha2) :
	Exception("AlphabetMismatchException: " + text + (alpha1 != NULL && alpha2 != NULL ? "(" + alpha1 -> getAlphabetType() + ", " + alpha2 -> getAlphabetType() + ")" : string(""))),
	alphabet1(alpha1),
	alphabet2(alpha2) {};
		
AlphabetMismatchException::~AlphabetMismatchException() throw () {};
vector<const Alphabet *> AlphabetMismatchException::getAlphabets() const {
	vector<const Alphabet *> v(2);
	v[0] = alphabet1;
	v[1] = alphabet2;
	return v;
}

/******************************************************************************/

SequenceNotAlignedException::SequenceNotAlignedException(const char *   text, const Sequence * seq) :
	SequenceException("SequenceNotALignedException: " + string(text), seq) {};
		
SequenceNotAlignedException::SequenceNotAlignedException(const string & text, const Sequence * seq) :
	SequenceException("SequenceNotAlignedException: " + text, seq) {};
		
SequenceNotAlignedException::~SequenceNotAlignedException() throw () {};

/******************************************************************************/
