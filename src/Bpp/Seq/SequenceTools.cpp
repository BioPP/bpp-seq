//
// File: SequenceTools.cpp
// Authors: Guillaume Deuchst
//          Julien Dutheil
//          Sylvain Gaillard
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

#include "Alphabet/AlphabetTools.h"
#include "StringSequenceTools.h"
#include <Bpp/Numeric/Matrix/Matrix.h>
#include <Bpp/Numeric/VectorTools.h>

using namespace bpp;

// From the STL:
#include <ctype.h>
#include <cmath>
#include <list>
#include <iostream>

using namespace std;

DNA SequenceTools::_DNA;
RNA SequenceTools::_RNA;
NucleicAcidsReplication SequenceTools::_DNARep(& _DNA, & _DNA);
NucleicAcidsReplication SequenceTools::_RNARep(& _RNA, & _RNA);
NucleicAcidsReplication SequenceTools::_transc(& _DNA, & _RNA);

/******************************************************************************/

Sequence* SequenceTools::subseq(const Sequence& sequence, unsigned int begin, unsigned int end) throw (IndexOutOfBoundsException, Exception)
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
	return new BasicSequence(sequence.getName(), temp, sequence.getComments(), sequence.getAlphabet());
}
	
/******************************************************************************/

Sequence* SequenceTools::concatenate(const Sequence& seq1, const Sequence& seq2) throw (AlphabetMismatchException, Exception)
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
	return new BasicSequence(seq1.getName(), sequence, seq1.getComments(), seq1.getAlphabet());
}
		
/******************************************************************************/

Sequence& SequenceTools::complement(Sequence& seq) throw (AlphabetException) {
	// Alphabet type checking
	NucleicAcidsReplication* NAR;
	if (seq.getAlphabet()->getAlphabetType() == "DNA alphabet") {
		NAR = &_DNARep;
	} else if (seq.getAlphabet()->getAlphabetType() == "RNA alphabet") {
		NAR = &_RNARep;
	} else {
		throw AlphabetException("SequenceTools::complement: Sequence must be nucleic.", seq.getAlphabet());
	}
  for (unsigned int i = 0 ; i < seq.size() ; i++) {
    seq.setElement(i, NAR->translate(seq.getValue(i)));
  }
  return seq;
}
		
/******************************************************************************/

Sequence* SequenceTools::getComplement(const Sequence& sequence) throw (AlphabetException) {
	// Alphabet type checking
	NucleicAcidsReplication* NAR;
	if (sequence.getAlphabet()->getAlphabetType() == "DNA alphabet") {
		NAR = &_DNARep;
	} else if (sequence.getAlphabet()->getAlphabetType() == "RNA alphabet") {
		NAR = &_RNARep;
	} else {
		throw AlphabetException ("SequenceTools::getComplement: Sequence must be nucleic.", sequence.getAlphabet());
	}
	
	return NAR->translate(sequence);
}

/******************************************************************************/

Sequence* SequenceTools::transcript(const Sequence& sequence) throw (AlphabetException)
{
	// Alphabet type checking
	if (sequence.getAlphabet()->getAlphabetType() != "DNA alphabet")
  {
		throw AlphabetException ("SequenceTools::transcript : Sequence must be DNA", sequence.getAlphabet());
	}

	return _transc.translate(sequence);
}

/******************************************************************************/

Sequence* SequenceTools::reverseTranscript(const Sequence& sequence) throw (AlphabetException)
{
	// Alphabet type checking
	if (sequence.getAlphabet()->getAlphabetType() != "RNA alphabet")
  {
		throw AlphabetException ("SequenceTools::reverseTranscript : Sequence must be RNA", sequence.getAlphabet());
	}

	return _transc.reverse(sequence);
}

/******************************************************************************/

Sequence& SequenceTools::invert(Sequence& seq) {
  unsigned int seq_size = seq.size(); // store seq size for efficiency
  unsigned int tmp_state = 0; // to store one state when swapping positions
  unsigned int j = seq_size; // symetric position iterator from sequence end
  for (unsigned int i = 0 ; i < seq_size / 2 ; i++) {
    j = seq_size - 1 - i;
    tmp_state = seq.getValue(i);
    seq.setElement(i, seq.getValue(j));
    seq.setElement(j, tmp_state);
  }
  return seq;
}

/******************************************************************************/

Sequence* SequenceTools::getInvert(const Sequence& sequence) {
  Sequence* iSeq = sequence.clone();
  invert(*iSeq);
  return iSeq;
}

/******************************************************************************/

Sequence& SequenceTools::invertComplement(Sequence& seq) {
	// Alphabet type checking
	NucleicAcidsReplication* NAR;
	if (seq.getAlphabet()->getAlphabetType() == "DNA alphabet") {
		NAR = &_DNARep;
	} else if (seq.getAlphabet()->getAlphabetType() == "RNA alphabet") {
		NAR = &_RNARep;
	} else {
		throw AlphabetException("SequenceTools::complement: Sequence must be nucleic.", seq.getAlphabet());
	}
  for (unsigned int i = 0 ; i < seq.size() ; i++) {
    seq.setElement(i, NAR->translate(seq.getValue(i)));
  }
  unsigned int seq_size = seq.size(); // store seq size for efficiency
  unsigned int tmp_state = 0; // to store one state when swapping positions
  unsigned int j = seq_size; // symetric position iterator from sequence end
  for (unsigned int i = 0 ; i < seq_size / 2 ; i++) {
    j = seq_size - 1 - i;
    tmp_state = seq.getValue(i);
    seq.setElement(i, NAR->translate(seq.getValue(j)));
    seq.setElement(j, NAR->translate(tmp_state));
  }
  if (seq_size % 2) { // treate the state in the middle of odd sequences
    seq.setElement(seq_size / 2, NAR->translate(seq.getValue(seq_size / 2)));
  }
  return seq;
}

/******************************************************************************/

double SequenceTools::getPercentIdentity(const Sequence& seq1, const Sequence& seq2, bool ignoreGaps) throw (AlphabetMismatchException, SequenceNotAlignedException)
{
	if (seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
		throw AlphabetMismatchException("SequenceTools::getPercentIdentity", seq1.getAlphabet(), seq2.getAlphabet());
	if (seq1.size() != seq2.size())
		throw SequenceNotAlignedException("SequenceTools::getPercentIdentity", &seq2);
  int gap = seq1.getAlphabet()->getGapCharacterCode();
	unsigned int id = 0;
	unsigned int tot = 0;
	for (unsigned int i = 0; i < seq1.size(); i++) {
    int x = seq1.getValue(i);
    int y = seq2.getValue(i);
    if (ignoreGaps) {
      if (x != gap && y != gap) {
        tot++;
		    if (x == y) id++;
      }
    } else {
      tot++;
		  if (x == y) id++;
    }
  }
	return static_cast<double>(id) / static_cast<double>(tot) * 100.; 			
}

/******************************************************************************/

unsigned int SequenceTools::getNumberOfSites(const Sequence& seq)
{
	unsigned int count = 0;
  const Alphabet* alpha = seq.getAlphabet();
	for(unsigned int i = 0; i < seq.size(); i++)
  {
		if(! alpha->isGap(seq[i])) count++;
	}
	return count;
}

/******************************************************************************/

unsigned int SequenceTools::getNumberOfCompleteSites(const Sequence & seq)
{
	unsigned int count = 0;
  const Alphabet * alpha = seq.getAlphabet();
	for(unsigned int i = 0; i < seq.size(); i++)
  {
		if(! alpha->isGap(seq[i]) && ! alpha->isUnresolved(seq[i])) count++;
	}
	return count;
}

/******************************************************************************/

unsigned int SequenceTools::getNumberOfUnresolvedSites(const Sequence & seq)
{
  unsigned int count = 0;
  const Alphabet * alpha = seq.getAlphabet();
  for (unsigned int i = 0 ; i < seq.size() ; i++) {
    if (alpha->isUnresolved(seq[i]))
      count++;
  }
  return count;
}

/******************************************************************************/

Sequence * SequenceTools::removeGaps(const Sequence & seq)
{
	vector<int> content;
  const Alphabet * alpha = seq.getAlphabet();
  for(unsigned int i = 0; i < seq.size(); i++)
  {
		if(! alpha->isGap(seq[i])) content.push_back(seq[i]);
	}
  Sequence * newSeq = dynamic_cast<Sequence *>(seq.clone());
  newSeq->setContent(content);
	return newSeq;
}

/******************************************************************************/

BowkerTest* SequenceTools::bowkerTest(const Sequence& seq1, const Sequence& seq2)
  throw (SequenceNotAlignedException)
{
  if(seq1.size() != seq2.size())
    throw SequenceNotAlignedException("SequenceTools::bowkerTest.", &seq2);
  unsigned int n = seq1.size();
  const Alphabet* alpha = seq1.getAlphabet();
  unsigned int r = alpha->getSize();

  //Compute contingency table:
  RowMatrix<double> array(r, r);
  int x, y;
  for(unsigned int i = 0; i < n; i++)
  {
    x = seq1[i];
    y = seq2[i];
    if(!alpha->isGap(x) && !alpha->isUnresolved(x)
    && !alpha->isGap(y) && !alpha->isUnresolved(y))
    {
      array(static_cast<unsigned int>(x), static_cast<unsigned int>(y))++;
    }
  }
  
  //Compute Bowker's statistic:
  double sb2 = 0, nij, nji;
  for(unsigned int i = 1; i < r; ++i)
  {
    for(unsigned int j = 0; j < i; ++j)
    {
      nij = array(i, j);
      nji = array(j, i);
      if (nij != 0 || nji != 0)
      {
        sb2 += pow(nij - nji, 2) / (nij + nji);
      }
      // Else: we should display a warning there.
    }
  }
  
  //Compute p-value:
  double pvalue = 1. - RandomTools::pChisq(sb2, (r - 1) * r / 2);

  //Return results:
  BowkerTest* test = new BowkerTest();
  test->setStatistic(sb2);
  test->setPValue(pvalue);
  return test;
}

/******************************************************************************/

void SequenceTools::getPutativeHaplotypes(const Sequence & seq, std::vector<Sequence *> & hap, unsigned int level)
{
  vector< vector < int > > states(seq.size());
  list<Sequence *> t_hap;
  const Alphabet * alpha = seq.getAlphabet();
  unsigned int hap_count = 1;
  // Vector of available states at each position
  for (unsigned int i = 0 ; i < seq.size() ; i++) {
    vector<int> st = alpha->getAlias(seq[i]);
    if (!st.size()) {
      st.push_back(alpha->getGapCharacterCode());
    }
    if (st.size() <= level) {
      states[i] = st;
    } else {
      states[i] = vector <int> (1, seq[i]);
    }
  }
  // Combinatorial haplotypes building (the use of tree may be more accurate)
  t_hap.push_back(new BasicSequence(seq.getName() + "_hap" + TextTools::toString(hap_count++), "", alpha));
  for (unsigned int i = 0 ; i < states.size() ; i++) {
    for (list<Sequence *>::iterator it = t_hap.begin() ; it != t_hap.end() ; it++) {
      for (unsigned int j = 0 ; j < states[i].size() ; j++) {
        Sequence * tmp_seq = new BasicSequence(seq.getName() + "_hap", (** it).getContent(), alpha);
        if (j < states[i].size() - 1) {
          tmp_seq->setName(tmp_seq->getName() + TextTools::toString(hap_count++));
          tmp_seq->addElement(states[i][j]);
          t_hap.insert(it, tmp_seq);
        }
        else {
          (** it).addElement(states[i][j]);
        }
      }
    }
  }
  for (list<Sequence *>::reverse_iterator it = t_hap.rbegin() ; it != t_hap.rend() ; it++) {
    hap.push_back(* it);
  }
}

/******************************************************************************/

Sequence* SequenceTools::combineSequences(const Sequence& s1, const Sequence& s2) throw (AlphabetMismatchException) {
  if (s1.getAlphabet()->getAlphabetType() != s2.getAlphabet()->getAlphabetType()) {
    throw AlphabetMismatchException("SequenceTools::combineSequences(const Sequence& s1, const Sequence& s2): s1 and s2 don't have same Alphabet.", s1.getAlphabet(), s2.getAlphabet());
  }
  const Alphabet* alpha = s1.getAlphabet();
  vector<int> st;
  vector<int> seq;
  unsigned int length = NumTools::max(s1.size(), s2.size());
  for (unsigned int i = 0 ; i < length ; i++) {
    if (i < s1.size())
      st.push_back(s1.getValue(i));
    if (i < s2.size())
      st.push_back(s2.getValue(i));
    seq.push_back(alpha->getGeneric(st));
    st.clear();
  }
  Sequence* s = new BasicSequence(s1.getName() + "+" + s2.getName(), seq, alpha);
  return s;
}

/******************************************************************************/

Sequence* SequenceTools::subtractHaplotype(const Sequence& s, const Sequence& h, string name, unsigned int level) throw (SequenceNotAlignedException) {
  const Alphabet * alpha = s.getAlphabet();
  if (name.size() == 0) 
    name = s.getName() + "_haplotype";
  string seq;
  if (s.size() != h.size()) throw SequenceNotAlignedException("SequenceTools::subtractHaplotype: haplotype must be aligned with the sequence.", &h);
  for (unsigned int i = 0 ; i < s.size() ; ++i) {
    string c; 
    vector<int> nucs = alpha->getAlias(s.getValue(i));
    if (nucs.size() > 1) {
      remove(nucs.begin(), nucs.end(), h.getValue(i));
      nucs = vector<int>(nucs.begin(), nucs.end() - 1);
    } else {
      nucs = vector<int>(nucs.begin(), nucs.end());
    }  
    c = alpha->intToChar(alpha->getGeneric(nucs));
    if (level <= nucs.size() && (alpha->isUnresolved(s.getValue(i)) || alpha->isUnresolved(h.getValue(i)))) {
      c = alpha->intToChar(alpha->getUnknownCharacterCode());
    }  
    seq += c; 
  }  
  Sequence* hap = new BasicSequence(name, seq, alpha);
  return hap;
}

/******************************************************************************/

void SequenceTools::getCDS(Sequence& sequence, bool checkInit, bool checkStop, bool includeInit, bool includeStop)
{
  const CodonAlphabet* alphabet = dynamic_cast<const CodonAlphabet*>(sequence.getAlphabet());
  if (!alphabet)
    throw AlphabetException("SequenceTools::getCDS. Sequence is not a codon sequence.");
  if (checkInit) {
    unsigned int i;
    for (i = 0; i < sequence.size() && !alphabet->isInit(sequence[i]); ++i) {}
    for (unsigned int j = 0; includeInit ? j < i : j <= i; ++j)
      sequence.deleteElement(j);
  }
  if (checkStop) {
    unsigned int i;
    for (i = 0; i < sequence.size() && !alphabet->isStop(sequence[i]); ++i) {}
    for (unsigned int j = includeStop ? i + 1 : i; j < sequence.size(); ++j)
      sequence.deleteElement(j);
  }
}

/******************************************************************************/

