//
// File: SequenceContainerExceptions.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Nov  3 17:00:05 2003
//

#include "SequenceContainerExceptions.h"


/*******************************************************************************
 *                   Sequence containers exceptions                            *
 *******************************************************************************/

SequenceNotFoundException::SequenceNotFoundException(const char *   text, const char * seqId) :
	Exception("SequenceNotFoundException: " + string(text) + "(" + seqId + ")"),
	id(seqId) {};
		
SequenceNotFoundException::SequenceNotFoundException(const string & text, const string & seqId) :
	Exception("SequenceNotFoundException: " + text + "(" + seqId + ")"),
	id(seqId) {};
		
SequenceNotFoundException::~SequenceNotFoundException() throw() {};
const string SequenceNotFoundException::getSequenceId() const { return id; }

/*******************************************************************************/
