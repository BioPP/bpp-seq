//
// File: DNAToRNA.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 14:39:29 2003
//

#include "DNAToRNA.h"


DNAToRNA::DNAToRNA() : ReverseTranslator()
{
	dna = new DNA();
	rna = new RNA();
}


DNAToRNA::~DNAToRNA() {
	delete dna;
	delete rna;
}

// Method to convert DNA sequence to RNA
int    DNAToRNA::translate(int i) const throw (BadIntException) {
	dna -> intToChar(i);
	return i;
}	
string DNAToRNA::translate(const string & c) const throw (BadCharException) {
	int i = dna -> charToInt(c);
	return rna -> intToChar(i);
}

// Method to convert a RNA sequence in DNA sequence
int    DNAToRNA::reverse(int i) const throw (BadIntException) {
	rna -> intToChar(i);
	return i;
}		
string DNAToRNA::reverse(const string & c) const throw (BadCharException) {
	int i = rna -> charToInt(c);
	return dna -> intToChar(i);
}
