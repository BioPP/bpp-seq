/*
 * File SequenceTools.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 21 2003
*/

#ifndef _SEQUENCETOOLS_H_
#define _SEQUENCETOOLS_H_

#include "Alphabet.h"
#include "Sequence.h"
#include "SymbolListTools.h"

// From NumCalc:
#include <NumCalc/RandomTools.h>

// From utils: 
#include <Utils/Exceptions.h>

// From the STL:
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

/* SequenceTools static class
 * Implements methods to manipulate sequences
 *
 * This class can't be instanciated
*/

class SequenceTools: public SymbolListTools
{
    public:	
     	// class destructor
		~SequenceTools();
	
	public:

		// Methods for truncate a sequence into a specified interval
		// Note : all two terminal characters are returned
		static Sequence * subseq(const Sequence & sequence, int begin, int end) throw (Exception) ;

		// Methods to determinate if a sequence is a palindrome
		//WARNING!!! Must check about this!
		static bool isPalindrome(const Sequence & sequence);

		// Method for concatenate two sequences
		// Note : names and alphabets will be checked, only first sequence's commentaries are keeped
		static Sequence * concatenate(const Sequence & seq1, const Sequence & seq2) throw (AlphabetMismatchException, Exception) ;

		// Method to reverse a nucleic sequence
		static Sequence * reverse(const Sequence & sequence) throw (AlphabetException);

		// Method to complement a DNA sequence
		static Sequence * complement(const Sequence & sequence) throw (AlphabetException);
		
		// Method to compute the percent id between 2 seqs:
		static double getPercentIdentity(const Sequence & seq1, const Sequence & seq2) throw (AlphabetMismatchException, SequenceNotAlignedException);

		static unsigned int getNumberOfSites(const Sequence & seq);

		static unsigned int getNumberOfCompleteSites(const Sequence & seq);
};


#endif // _SEQUENCETOOLS_H_
