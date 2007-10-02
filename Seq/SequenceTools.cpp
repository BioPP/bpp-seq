//
// File: SequenceTools.cpp
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Tue Aug 21 2003
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

#include "SequenceTools.h"

#include "AlphabetTools.h"
#include "StringSequenceTools.h"

// From the STL:
#include <ctype.h>
#include <cmath>
#include <iostream>

using namespace std;

DNA SequenceTools::_DNA;
RNA SequenceTools::_RNA;
NucleicAcidsReplication SequenceTools::_DNARep(& _DNA, & _DNA);
NucleicAcidsReplication SequenceTools::_RNARep(& _RNA, & _RNA);
NucleicAcidsReplication SequenceTools::_transc(& _DNA, & _RNA);

/****************************************************************************************/

Sequence * SequenceTools::subseq(const Sequence & sequence, unsigned int begin, unsigned int end) throw (IndexOutOfBoundsException, Exception)
{
	// Checking interval
	if (end >= sequence.size()) throw IndexOutOfBoundsException ("SequenceTools::subseq : Invalid upper bound", end, 0, sequence.size());
	if (end < begin) throw Exception ("SequenceTools::subseq : Invalid interval");

	// Copy sequence
	vector<int> temp(sequence.getContent());

	// Truncate sequence
	temp.erase(temp.begin() + end + 1, temp.end());
	temp.erase(temp.begin(), temp.begin() + begin);

	// New sequence creation
	return new Sequence(sequence.getName(), temp, sequence.getComments(), sequence.getAlphabet());
}
	
/****************************************************************************************/

Sequence * SequenceTools::concatenate(const Sequence & seq1, const Sequence & seq2) throw (AlphabetMismatchException, Exception)
{
	// Sequence's alphabets matching verification
	if ((seq1.getAlphabet()->getAlphabetType()) != (seq2.getAlphabet()->getAlphabetType())) 
		throw AlphabetMismatchException("SequenceTools::concatenate : Sequence's alphabets don't match ", seq1.getAlphabet(), seq2.getAlphabet());
	
	// Sequence's names matching verification
	if (seq1.getName() != seq2.getName())
    throw Exception ("SequenceTools::concatenate : Sequence's names don't match");

	// Concatenate sequences and send result
	vector<int> sequence  = seq1.getContent();
	vector<int> sequence2 = seq2.getContent();
	sequence.insert(sequence.end(), sequence2.begin(), sequence2.end());
	return new Sequence(seq1.getName(), sequence, seq1.getComments(), seq1.getAlphabet());
}
		
/****************************************************************************************/

Sequence * SequenceTools::reverse(const Sequence & sequence)
{
	unsigned int n = sequence.size();
	vector<int> content(n);
	for(unsigned int i = 0; i < n; i++)
  {
		content[i] = sequence[n - i - 1];
	}
	// New sequence creating, and sense reversing
	Sequence * s = new Sequence(sequence.getName(), content, sequence.getComments(), sequence.getAlphabet());
	s->setSense(! sequence.getSense());

	// Send result
	return s;
}

/****************************************************************************************/

Sequence * SequenceTools::complement(const Sequence & sequence) throw (AlphabetException)
{
	// Alphabet type checking
	NucleicAcidsReplication * NAR;
	if (sequence.getAlphabet()->getAlphabetType() == "DNA alphabet")
  {
		NAR = &_DNARep;
	}
  else if(sequence.getAlphabet()->getAlphabetType() == "RNA alphabet")
  {
		NAR = &_RNARep;
	}
  else
  {
		throw AlphabetException ("SequenceTools::complement : Sequence must be nucleic.", sequence.getAlphabet());
	}
	
	return NAR->translate(sequence);
}

/****************************************************************************************/

Sequence * SequenceTools::transcript(const Sequence & sequence) throw (AlphabetException)
{
	// Alphabet type checking
	if (sequence.getAlphabet()->getAlphabetType() != "DNA alphabet")
  {
		throw AlphabetException ("SequenceTools::transcript : Sequence must be DNA", sequence.getAlphabet());
	}

	return _transc.translate(sequence);
}

/****************************************************************************************/

Sequence * SequenceTools::reverseTranscript(const Sequence & sequence) throw (AlphabetException)
{
	// Alphabet type checking
	if (sequence.getAlphabet()->getAlphabetType() != "RNA alphabet")
  {
		throw AlphabetException ("SequenceTools::reverseTranscript : Sequence must be RNA", sequence.getAlphabet());
	}

	return _transc.reverse(sequence);
}

/****************************************************************************************/
		
double SequenceTools::getPercentIdentity(const Sequence & seq1, const Sequence & seq2) throw (AlphabetMismatchException, SequenceNotAlignedException)
{
	if(seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
		throw AlphabetMismatchException("SequenceTools::getPercentIdentity", seq1.getAlphabet(), seq2.getAlphabet());
	if(seq1.size() != seq2.size())
		throw SequenceNotAlignedException("SequenceTools::getPercentIdentity", &seq2);
	int id = 0;
	for(unsigned int i = 0; i < seq1.size(); i++)
		if(seq1.getValue(i) == seq2.getValue(i)) id++;
	return (double)id / (double)seq1.size() * 100.; 			
}

/****************************************************************************************/

unsigned int SequenceTools::getNumberOfSites(const Sequence & seq)
{
	unsigned int count = 0;
	for(unsigned int i = 0; i < seq.size(); i++)
  {
		if(seq[i] > -1) count++;
	}
	return count;
}

/****************************************************************************************/

unsigned int SequenceTools::getNumberOfCompleteSites(const Sequence & seq)
{
	unsigned int count = 0;
	for(unsigned int i = 0; i < seq.size(); i++)
  {
		if(seq[i] > -1 && seq[i] < (int)seq.getAlphabet()->getSize()) count++;
	}
	return count;
}

/****************************************************************************************/

Sequence * SequenceTools::removeGaps(const Sequence & seq)
{
	vector<int> content;
  for(unsigned int i = 0; i < seq.size(); i++)
  {
		if(seq[i] > -1) content.push_back(seq[i]);
	}
  Sequence * newSeq = seq.clone();
  newSeq->setContent(content);
	return newSeq;
}

/****************************************************************************************/

