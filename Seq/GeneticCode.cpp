//
// File: GeneticCode.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Oct 13 15:37:25 2003
//

#include "GeneticCode.h"

StopCodonException::StopCodonException(const string & text, const string & codon) :
	Exception("StopCodonException: " + text + "(" + codon + ")"),
	codon(codon) {};
StopCodonException::~StopCodonException() throw() {};
string StopCodonException::getCodon() const { return codon; }




GeneticCode::GeneticCode() : AbstractTranslator() {}

GeneticCode::~GeneticCode() {}

const Alphabet * GeneticCode::getSourceAlphabet() const { return codonAlphabet; }

const Alphabet * GeneticCode::getTargetAlphabet() const { return proteicAlphabet; }
	
bool GeneticCode::areSynonymes(      int i     ,       int j     ) const throw (BadIntException) {
	return (translate(i) == translate(j));
}

bool GeneticCode::areSynonymes(const string & i, const string & j) const throw (BadCharException) {
	return (translate(i) == translate(j));
}

vector<int>    GeneticCode::getSynonymes(      int      aminoacid) const throw (BadIntException) {
	//test:
	proteicAlphabet -> intToChar(aminoacid);
	
	vector<int> synonymes;
	for(unsigned int i = 0; i < codonAlphabet -> getSize(); i++) {
		if(translate(i) == aminoacid) synonymes.push_back(i);
	}
	return synonymes;	
}

vector<string> GeneticCode::getSynonymes(const string & aminoacid) const throw (BadCharException) {
	//test:
	int aa = proteicAlphabet -> charToInt(aminoacid);
	
	vector<string> synonymes;
	for(unsigned int i = 0; i < codonAlphabet -> getSize(); i++) {
		if(translate(i) == aa) synonymes.push_back(codonAlphabet -> intToChar(i));
	}
	return synonymes;	
}
