/*
 * File SequenceTools.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 21 2003
*/

#include "SequenceTools.h" // class's header file

#include "StringSequenceTools.h"

// From the STL:
#include <ctype.h>
#include <cmath>
#include <iostream>

using namespace std;

SequenceTools::~SequenceTools() {}

Sequence * SequenceTools::subseq(const Sequence & sequence, int begin, int end) throw (Exception) {
	// Checking interval
	if (end < begin) throw Exception ("SequenceTools::truncate : Invalid interval");

	// Copy sequence
	vector<int> temp(sequence.getContent());

	// Truncate sequence
	temp.erase(temp.begin() + end + 1, temp.end());
	temp.erase(temp.begin(), temp.begin() + begin);

	// New sequence creation
	return new Sequence(sequence.getName(), temp, sequence.getComments(), sequence.getAlphabet());
}
	
// Method for concatenate two sequences
// Note : names and alphabets will be checked, only first sequence's commentaries are keeped
Sequence * SequenceTools::concatenate(const Sequence & seq1, const Sequence & seq2) throw (AlphabetMismatchException, Exception) {
	// Sequence's alphabets matching verification
	if ((seq1.getAlphabet() -> getAlphabetType()) != (seq2.getAlphabet() -> getAlphabetType())) 
		throw AlphabetMismatchException("SequenceTools::concatenate : Sequence's alphabets don't match ", seq1.getAlphabet(), seq2.getAlphabet());
	
	// Sequence's names matching verification
	if (seq1.getName() != seq2.getName()) throw Exception ("SequenceTools::concatenate : Sequence's names don't match");

	// Concatenate sequences and send result
	vector<int> sequence  = seq1.getContent();
	vector<int> sequence2 = seq2.getContent();
	sequence.insert(sequence.end(), sequence2.begin(), sequence2.end());
	return new Sequence(seq1.getName(), sequence, seq1.getComments(), seq1.getAlphabet());
}
		

// Methods to determinate if a sequence is a palindrome
// (sequence is equals when readed from right or from left)
bool SequenceTools::isPalindrome(const Sequence & sequence) {
	return StringSequenceTools::isPalindrome(sequence.toString());
}

// Method to reverse a nucleic sequence
// @return Sequence * a new Sequence object containing the reversed sequence.
Sequence * SequenceTools::reverse(const Sequence & sequence) throw (AlphabetException) {
	// Alphabet type checking
	if ((sequence.getAlphabet() -> getAlphabetType() != "DNA alphabet") && (sequence.getAlphabet() ->getAlphabetType() != "RNA alphabet")) {
		throw AlphabetException ("SequenceTools::reverse : Sequence must be DNA or RNA", sequence.getAlphabet());
	}

	// Initializing
	string * content = StringSequenceTools::reverse(sequence.toString());

	// New sequence creating, and sense reversing
	Sequence * s = new Sequence(sequence.getName(), * content, sequence.getComments(), sequence.getAlphabet());
	s -> setSense(! sequence.getSense());

	// Send result
	return s;
}

// Method to complement a DNA sequence
// @return Sequence * a new Sequence object containing the reversed sequence.
Sequence * SequenceTools::complement(const Sequence & sequence) throw (AlphabetException) {
	// Alphabet type checking
	if (sequence.getAlphabet() -> getAlphabetType() != "DNA alphabet") {
		throw AlphabetException ("SequenceTools::complement : Sequence must be DNA", sequence.getAlphabet());
	}

	// Initializing
	string * content = StringSequenceTools::complement(sequence.toString());
	
	// Send new sequence
	return new Sequence (sequence.getName(), * content, sequence.getComments(), sequence.getAlphabet());
}

double SequenceTools::getPercentIdentity(const Sequence & seq1, const Sequence & seq2) throw (AlphabetMismatchException, SequenceNotAlignedException) {
	if(seq1.getAlphabet() -> getAlphabetType() != seq2.getAlphabet() -> getAlphabetType())
		throw AlphabetMismatchException("Sequence::getPercentIdentity", seq1.getAlphabet(), seq2.getAlphabet());
	if(seq1.size() != seq2.size())
		throw SequenceNotAlignedException("Sequence::getPercentIdentity", &seq2);
	int id = 0;
	for(unsigned int i = 0; i < seq1.size(); i++)
		if(seq1.getValue(i) == seq2.getValue(i)) id++;
	return (double)id / (double)seq1.size() * 100.; 			
}

unsigned int SequenceTools::getNumberOfSites(const Sequence & seq)
{
	unsigned int count = 0;
	for(unsigned int i = 0; i < seq.size(); i++) {
		if(seq[i] > -1) count++;
	}
	return count;
}

unsigned int SequenceTools::getNumberOfCompleteSites(const Sequence & seq) {
	unsigned int count = 0;
	for(unsigned int i = 0; i < seq.size(); i++) {
		if(seq[i] > -1 && seq[i] < (int)seq.getAlphabet() -> getSize()) count++;
	}
	return count;
}
