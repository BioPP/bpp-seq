//
// File: Translator.h
// Created by: jdutheil <jdutheil@Deedlit>
// Created on: Sun Oct 12 14:25:25 2003
//

#include "Translator.h"


Translator::Translator() {}
Translator::~Translator() {}

ReverseTranslator::ReverseTranslator() {}
ReverseTranslator::~ReverseTranslator() {}

AbstractTranslator::AbstractTranslator() {}
AbstractTranslator::~AbstractTranslator() {}

Sequence * AbstractTranslator::translate(const Sequence & sequence) const throw (Exception) {
	if(sequence.getAlphabet() -> getAlphabetType() != getSourceAlphabet() -> getAlphabetType())
		throw AlphabetMismatchException("AbstractTranslator::translate", getSourceAlphabet(), getTargetAlphabet());
	Sequence * tSeq = new Sequence(sequence.getName(), "", sequence.getComments(), getTargetAlphabet());
	for(unsigned int i = 0; i < sequence.size(); i++) {
		tSeq -> addElement(translate(sequence.getValue(i)));
	}
	return tSeq;
}

AbstractReverseTranslator::AbstractReverseTranslator() {}
AbstractReverseTranslator::~AbstractReverseTranslator() {}

Sequence * AbstractReverseTranslator::reverse(const Sequence & sequence) const throw (Exception) {
	if(sequence.getAlphabet() -> getAlphabetType() != getTargetAlphabet() -> getAlphabetType())
		throw AlphabetMismatchException("AbstractReverseTranslator::reverse", getSourceAlphabet(), getTargetAlphabet());
	Sequence * rSeq = new Sequence(sequence.getName(), "", sequence.getComments(), getSourceAlphabet());
	for(unsigned int i = 0; i < sequence.size(); i++) {
		rSeq -> addElement(reverse(sequence.getValue(i)));
	}
	return rSeq;
}
