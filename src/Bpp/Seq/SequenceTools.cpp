//
// File: SequenceTools.cpp
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
//   Sylvain Gaillard
// Created: 2003-08-21 00:00:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#include <Bpp/Numeric/Matrix/Matrix.h>
#include <Bpp/Numeric/VectorTools.h>

#include "Alphabet/AlphabetTools.h"
#include "SequenceTools.h"
#include "StringSequenceTools.h"

using namespace bpp;

// From the STL:
#include <ctype.h>
#include <cmath>
#include <list>
#include <iostream>

using namespace std;

shared_ptr<RNY> SequenceTools::RNY_(new RNY(AlphabetTools::DNA_ALPHABET));
NucleicAcidsReplication SequenceTools::DNARep_(AlphabetTools::DNA_ALPHABET, AlphabetTools::DNA_ALPHABET);
NucleicAcidsReplication SequenceTools::RNARep_(AlphabetTools::RNA_ALPHABET, AlphabetTools::RNA_ALPHABET);
NucleicAcidsReplication SequenceTools::transc_(AlphabetTools::DNA_ALPHABET, AlphabetTools::RNA_ALPHABET);

/******************************************************************************/

bool SequenceTools::areSequencesIdentical(const Sequence& seq1, const Sequence& seq2)
{
  // Site's size and content checking
  if (seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    return false;
  if (seq1.size() != seq2.size())
    return false;
  else
  {
    for (size_t i = 0; i < seq1.size(); i++)
    {
      if (seq1[i] != seq2[i])
        return false;
    }
    return true;
  }
}

/******************************************************************************/

Sequence* SequenceTools::concatenate(const Sequence& seq1, const Sequence& seq2)
{
  // Sequence's alphabets matching verification
  if ((seq1.getAlphabet()->getAlphabetType()) != (seq2.getAlphabet()->getAlphabetType()))
    throw AlphabetMismatchException("SequenceTools::concatenate : Sequence's alphabets don't match ", seq1.getAlphabet(), seq2.getAlphabet());

  // Sequence's names matching verification
  if (seq1.getName() != seq2.getName())
    throw Exception ("SequenceTools::concatenate : Sequence's names don't match");

  // Concatenate sequences and send result
  Sequence* concat = seq1.clone();
  concat->setToSizeR(seq1.size() + seq2.size());
  for (size_t i = 0; i < seq2.size(); ++i)
  {
    (*concat)[seq1.size() + i] = seq2[i];
  }
  return concat;
}

/******************************************************************************/

void SequenceTools::complement(Sequence& seq)
{
  // Alphabet type checking
  NucleicAcidsReplication* NAR;
  if (AlphabetTools::isDNAAlphabet(seq.getAlphabet().get()))
  {
    NAR = &DNARep_;
  }
  else if (AlphabetTools::isRNAAlphabet(seq.getAlphabet().get()))
  {
    NAR = &RNARep_;
  }
  else
  {
    throw AlphabetException("SequenceTools::complement: Sequence must be nucleic.", seq.getAlphabet());
  }
  for (size_t i = 0; i < seq.size(); ++i)
  {
    seq.setElement(i, NAR->translate(seq.getValue(i)));
  }
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::getComplement(const Sequence& sequence)
{
  // Alphabet type checking
  NucleicAcidsReplication* NAR;
  if (AlphabetTools::isDNAAlphabet(sequence.getAlphabet().get()))
  {
    NAR = &DNARep_;
  }
  else if (AlphabetTools::isRNAAlphabet(sequence.getAlphabet().get()))
  {
    NAR = &RNARep_;
  }
  else
  {
    throw AlphabetException ("SequenceTools::getComplement: Sequence must be nucleic.", sequence.getAlphabet());
  }

  return NAR->translate(sequence);
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::transcript(const Sequence& sequence)
{
  // Alphabet type checking
  if (!AlphabetTools::isDNAAlphabet(sequence.getAlphabet().get()))
  {
    throw AlphabetException ("SequenceTools::transcript : Sequence must be DNA", sequence.getAlphabet());
  }

  return transc_.translate(sequence);
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::reverseTranscript(const Sequence& sequence)
{
  // Alphabet type checking
  if (!AlphabetTools::isRNAAlphabet(sequence.getAlphabet().get()))
  {
    throw AlphabetException ("SequenceTools::reverseTranscript : Sequence must be RNA", sequence.getAlphabet());
  }

  return transc_.reverse(sequence);
}

/******************************************************************************/

void SequenceTools::invert(Sequence& seq)
{
  size_t seq_size = seq.size(); // store seq size for efficiency
  int tmp_state = 0; // to store one state when swapping positions
  size_t j = seq_size; // symetric position iterator from sequence end
  for (size_t i = 0; i < seq_size / 2; ++i)
  {
    j = seq_size - 1 - i;
    tmp_state = seq.getValue(i);
    seq.setElement(i, seq.getValue(j));
    seq.setElement(j, tmp_state);
  }
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::getInvert(const Sequence& sequence)
{
  auto iSeq = unique_ptr<Sequence>(sequence.clone());
  invert(*iSeq);
  return iSeq;
}

/******************************************************************************/

void SequenceTools::invertComplement(Sequence& seq)
{
  // Alphabet type checking
  NucleicAcidsReplication* NAR;
  if (AlphabetTools::isDNAAlphabet(seq.getAlphabet().get()))
  {
    NAR = &DNARep_;
  }
  else if (AlphabetTools::isRNAAlphabet(seq.getAlphabet().get()))
  {
    NAR = &RNARep_;
  }
  else
  {
    throw AlphabetException("SequenceTools::invertComplement: Sequence must be nucleic.", seq.getAlphabet());
  }
  // for (size_t i = 0 ; i < seq.size() ; i++) {
  //  seq.setElement(i, NAR->translate(seq.getValue(i)));
  // }
  size_t seq_size = seq.size(); // store seq size for efficiency
  int tmp_state = 0; // to store one state when swapping positions
  size_t j = seq_size; // symetric position iterator from sequence end
  for (size_t i = 0; i < seq_size / 2; ++i)
  {
    j = seq_size - 1 - i;
    tmp_state = seq.getValue(i);
    seq.setElement(i, NAR->translate(seq.getValue(j)));
    seq.setElement(j, NAR->translate(tmp_state));
  }
  if (seq_size % 2)   // treate the state in the middle of odd sequences
  {
    seq.setElement(seq_size / 2, NAR->translate(seq.getValue(seq_size / 2)));
  }
}

/******************************************************************************/

double SequenceTools::getPercentIdentity(const Sequence& seq1, const Sequence& seq2, bool ignoreGaps)
{
  if (seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SequenceTools::getPercentIdentity", seq1.getAlphabet(), seq2.getAlphabet());
  if (seq1.size() != seq2.size())
    throw SequenceNotAlignedException("SequenceTools::getPercentIdentity", &seq2);
  int gap = seq1.getAlphabet()->getGapCharacterCode();
  size_t id = 0;
  size_t tot = 0;
  for (size_t i = 0; i < seq1.size(); i++)
  {
    int x = seq1.getValue(i);
    int y = seq2.getValue(i);
    if (ignoreGaps)
    {
      if (x != gap && y != gap)
      {
        tot++;
        if (x == y)
          id++;
      }
    }
    else
    {
      tot++;
      if (x == y)
        id++;
    }
  }
  return static_cast<double>(id) / static_cast<double>(tot) * 100.;
}

/******************************************************************************/

size_t SequenceTools::getNumberOfSites(const Sequence& seq)
{
  size_t count = 0;
  auto alphaPtr = seq.getAlphabet();
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (!alphaPtr->isGap(seq[i]))
      count++;
  }
  return count;
}

/******************************************************************************/

size_t SequenceTools::getNumberOfCompleteSites(const Sequence& seq)
{
  size_t count = 0;
  auto alphaPtr = seq.getAlphabet();
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (!alphaPtr->isGap(seq[i]) && !alphaPtr->isUnresolved(seq[i]))
      count++;
  }
  return count;
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::getSequenceWithCompleteSites(const Sequence& seq)
{
  auto alphaPtr = seq.getAlphabet();
  vector<int> content;
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (!(alphaPtr->isGap(seq[i]) || alphaPtr->isUnresolved(seq[i])))
      content.push_back(seq[i]);
  }
  auto newSeq = unique_ptr<Sequence>(seq.clone());
  newSeq->setContent(content);
  return newSeq;
}

/******************************************************************************/

size_t SequenceTools::getNumberOfUnresolvedSites(const Sequence& seq)
{
  size_t count = 0;
  auto alphaPtr = seq.getAlphabet();
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (alphaPtr->isUnresolved(seq[i]))
      count++;
  }
  return count;
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::getSequenceWithoutGaps(const Sequence& seq)
{
  auto alphaPtr = seq.getAlphabet();
  vector<int> content;
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (!alphaPtr->isGap(seq[i]))
      content.push_back(seq[i]);
  }
  auto newSeq = unique_ptr<Sequence>(seq.clone());
  newSeq->setContent(content);
  return newSeq;
}

/******************************************************************************/

void SequenceTools::removeGaps(Sequence& seq)
{
  auto alphaPtr = seq.getAlphabet();
  for (size_t i = seq.size(); i > 0; --i)
  {
    if (alphaPtr->isGap(seq[i - 1]))
      seq.deleteElement(i - 1);
  }
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::getSequenceWithoutStops(const Sequence& seq, const GeneticCode& gCode)
{
  auto calpha = dynamic_pointer_cast<const CodonAlphabet>(seq.getAlphabet());
  if (!calpha)
    throw Exception("SequenceTools::getSequenceWithoutStops. Input sequence should have a codon alphabet.");
  vector<int> content;
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (!gCode.isStop(seq[i]))
      content.push_back(seq[i]);
  }
  auto newSeq = unique_ptr<Sequence>(seq.clone());
  newSeq->setContent(content);
  return newSeq;
}

/******************************************************************************/

void SequenceTools::removeStops(Sequence& seq, const GeneticCode& gCode)
{
  auto calpha = dynamic_pointer_cast<const CodonAlphabet>(seq.getAlphabet());
  if (!calpha)
    throw Exception("SequenceTools::removeStops. Input sequence should have a codon alphabet.");
  for (size_t i = seq.size(); i > 0; --i)
  {
    if (gCode.isStop(seq[i - 1]))
      seq.deleteElement(i - 1);
  }
}

/******************************************************************************/

void SequenceTools::replaceStopsWithGaps(Sequence& seq, const GeneticCode& gCode)
{
  auto calpha = dynamic_pointer_cast<const CodonAlphabet>(seq.getAlphabet());
  if (!calpha)
    throw Exception("SequenceTools::replaceStopsWithGaps. Input sequence should have a codon alphabet.");
  int gap = calpha->getGapCharacterCode();
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (gCode.isStop(seq[i]))
      seq.setElement(i, gap);
  }
}

/******************************************************************************/

unique_ptr<BowkerTest> SequenceTools::bowkerTest(const Sequence& seq1, const Sequence& seq2)
{
  if (seq1.size() != seq2.size())
    throw SequenceNotAlignedException("SequenceTools::bowkerTest.", &seq2);
  size_t n = seq1.size();
  auto alphaPtr = seq1.getAlphabet();
  unsigned int r = alphaPtr->getSize();

  // Compute contingency table:
  RowMatrix<double> array(r, r);
  int x, y;
  for (size_t i = 0; i < n; i++)
  {
    x = seq1[i];
    y = seq2[i];
    if (!alphaPtr->isGap(x) && !alphaPtr->isUnresolved(x)
        && !alphaPtr->isGap(y) && !alphaPtr->isUnresolved(y))
    {
      array(static_cast<size_t>(x), static_cast<size_t>(y))++;
    }
  }

  // Compute Bowker's statistic:
  double sb2 = 0, nij, nji;
  for (unsigned int i = 1; i < r; ++i)
  {
    for (unsigned int j = 0; j < i; ++j)
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

  // Compute p-value:
  double pvalue = 1. - RandomTools::pChisq(sb2, (r - 1) * r / 2);

  // Return results:
  auto test = make_unique<BowkerTest>();
  test->setStatistic(sb2);
  test->setPValue(pvalue);
  return test;
}

/******************************************************************************/

void SequenceTools::getPutativeHaplotypes(const Sequence& seq, std::vector< unique_ptr<Sequence> >& hap, unsigned int level)
{
  vector< vector< int > > states(seq.size());
  list< unique_ptr<Sequence> > t_hap;
  auto alphaPtr = seq.getAlphabet();
  unsigned int hap_count = 1;
  // Vector of available states at each position
  for (size_t i = 0; i < seq.size(); ++i)
  {
    vector<int> st = alphaPtr->getAlias(seq[i]);
    if (!st.size())
    {
      st.push_back(alphaPtr->getGapCharacterCode());
    }
    if (st.size() <= level)
    {
      states[i] = st;
    }
    else
    {
      states[i] = vector<int>(1, seq[i]);
    }
  }
  // Combinatorial haplotypes building (the use of tree may be more accurate)
  t_hap.push_back(make_unique<Sequence>(seq.getName() + "_hap" + TextTools::toString(hap_count++), "", alphaPtr));
  for (size_t i = 0; i < states.size(); ++i)
  {
    for (auto it = t_hap.begin(); it != t_hap.end(); it++)
    {
      for (size_t j = 0; j < states[i].size(); ++j)
      {
        auto tmp_seq = unique_ptr<Sequence>((**it).clone());
        tmp_seq->setName(seq.getName() + "_hap");
        if (j < states[i].size() - 1)
        {
          tmp_seq->setName(tmp_seq->getName() + TextTools::toString(hap_count++));
          tmp_seq->addElement(states[i][j]);
          t_hap.insert(it, std::move(tmp_seq));
        }
        else
        {
          (**it).addElement(states[i][j]);
        }
      }
    }
  }
  for (auto it = t_hap.rbegin(); it != t_hap.rend(); it++)
  {
    hap.push_back(std::move(*it));
  }
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::combineSequences(const Sequence& s1, const Sequence& s2)
{
  if (s1.getAlphabet()->getAlphabetType() != s2.getAlphabet()->getAlphabetType())
  {
    throw AlphabetMismatchException("SequenceTools::combineSequences(const Sequence& s1, const Sequence& s2): s1 and s2 don't have same Alphabet.", s1.getAlphabet(), s2.getAlphabet());
  }
  auto alphaPtr = s1.getAlphabet();
  vector<int> st;
  vector<int> seq;
  size_t length = NumTools::max(s1.size(), s2.size());
  for (size_t i = 0; i < length; ++i)
  {
    if (i < s1.size())
      st.push_back(s1.getValue(i));
    if (i < s2.size())
      st.push_back(s2.getValue(i));
    seq.push_back(alphaPtr->getGeneric(st));
    st.clear();
  }
  auto s = make_unique<Sequence>(s1.getName() + "+" + s2.getName(), seq, alphaPtr);
  return s;
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::subtractHaplotype(const Sequence& s, const Sequence& h, string name, unsigned int level)
{
  auto alphaPtr = s.getAlphabet();
  if (name.size() == 0)
    name = s.getName() + "_haplotype";
  string seq;
  if (s.size() != h.size())
    throw SequenceNotAlignedException("SequenceTools::subtractHaplotype: haplotype must be aligned with the sequence.", &h);
  for (unsigned int i = 0; i < s.size(); ++i)
  {
    string c;
    vector<int> nucs = alphaPtr->getAlias(s.getValue(i));
    if (nucs.size() > 1)
    {
      remove(nucs.begin(), nucs.end(), h.getValue(i));
      nucs = vector<int>(nucs.begin(), nucs.end() - 1);
    }
    else
    {
      nucs = vector<int>(nucs.begin(), nucs.end());
    }
    c = alphaPtr->intToChar(alphaPtr->getGeneric(nucs));
    if (level <= nucs.size() && (alphaPtr->isUnresolved(s.getValue(i)) || alphaPtr->isUnresolved(h.getValue(i))))
    {
      c = alphaPtr->intToChar(alphaPtr->getUnknownCharacterCode());
    }
    seq += c;
  }
  auto hap = make_unique<Sequence>(name, seq, alphaPtr);
  return hap;
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::RNYslice(const Sequence& seq, int ph)
{
  // Alphabet type checking
  if (AlphabetTools::isDNAAlphabet(seq.getAlphabet().get()))
  {
    throw AlphabetException ("SequenceTools::transcript : Sequence must be DNA", seq.getAlphabet());
  }

  if ((ph < 1) || (ph > 3))
    throw Exception("Bad phase for RNYSlice: " + TextTools::toString(ph) + ". Should be between 1 and 3.");

  size_t s = seq.size();
  size_t n = (s - static_cast<size_t>(ph) + 3) / 3;

  vector<int> content(n);

  int tir = seq.getAlphabet()->getGapCharacterCode();
  size_t j;

  for (size_t i = 0; i < n; i++)
  {
    j = i * 3 + static_cast<size_t>(ph) - 1;

    if (j == 0)
      content[i] = RNY_->getRNY(tir, seq[0], seq[1], *seq.getAlphabet());
    else
    {
      if (j == s - 1)
        content[i] = RNY_->getRNY(seq[j - 1], seq[j], tir, *seq.getAlphabet());
      else
        content[i] = RNY_->getRNY(seq[j - 1], seq[j], seq[j + 1], *seq.getAlphabet());
    }
  }

  // New sequence creating, and sense reversing
  auto alphaPtr = dynamic_pointer_cast<const Alphabet>(RNY_);
  auto seqPtr = make_unique<Sequence>(seq.getName(), content, seq.getComments(), alphaPtr);

  // Send result
  return seqPtr;
}

unique_ptr<Sequence> SequenceTools::RNYslice(const Sequence& seq)
{
  // Alphabet type checking
  if (AlphabetTools::isDNAAlphabet(seq.getAlphabet().get()))
  {
    throw AlphabetException ("SequenceTools::transcript : Sequence must be DNA", seq.getAlphabet());
  }

  size_t n = seq.size();

  vector<int> content(n);

  int tir = seq.getAlphabet()->getGapCharacterCode();

  if (seq.size() >= 2)
  {
    content[0] = RNY_->getRNY(tir, seq[0], seq[1], *seq.getAlphabet());

    for (unsigned int i = 1; i < n - 1; i++)
    {
      content[i] = RNY_->getRNY(seq[i - 1], seq[i], seq[i + 1],
                               *seq.getAlphabet());
    }

    content[n - 1] = RNY_->getRNY(seq[n - 2], seq[n - 1], tir, *seq.getAlphabet());
  }

  // New sequence creating, and sense reversing
  auto alphaPtr = dynamic_pointer_cast<const Alphabet>(RNY_);
  auto seqPtr = make_unique<Sequence>(seq.getName(), content, seq.getComments(), alphaPtr);

  // Send result
  return seqPtr;
}

/******************************************************************************/


void SequenceTools::getCDS(Sequence& sequence, const GeneticCode& gCode, bool checkInit, bool checkStop, bool includeInit, bool includeStop)
{
  auto alphabet = dynamic_pointer_cast<const CodonAlphabet>(sequence.getAlphabet());
  if (!alphabet)
    throw AlphabetException("SequenceTools::getCDS. Sequence is not a codon sequence.", sequence.getAlphabet());
  if (checkInit)
  {
    size_t i;
    for (i = 0; i < sequence.size() && !gCode.isStart(sequence[i]); ++i)
    {}
    for (size_t j = 0; includeInit ? j < i : j <= i; ++j)
    {
      sequence.deleteElement(j);
    }
  }
  if (checkStop)
  {
    size_t i;
    for (i = 0; i < sequence.size() && !gCode.isStop(sequence[i]); ++i)
    {}
    for (size_t j = includeStop ? i + 1 : i; j < sequence.size(); ++j)
    {
      sequence.deleteElement(j);
    }
  }
}

/******************************************************************************/

size_t SequenceTools::findFirstOf(const Sequence& seq, const Sequence& motif, bool strict)
{
  if (motif.size() > seq.size())
    return seq.size();
  for (size_t seqi = 0; seqi < seq.size() - motif.size() + 1; seqi++)
  {
    bool match = false;
    for (size_t moti = 0; moti < motif.size(); moti++)
    {
      if (strict)
      {
        match = seq.getValue(seqi + moti) == motif.getValue(moti);
      }
      else
      {
        match = AlphabetTools::match(seq.getAlphabet(), seq.getValue(seqi + moti), motif.getValue(moti));
      }
      if (!match)
      {
        break;
      }
    }
    if (match)
    {
      return seqi;
    }
  }
  return seq.size();
}

/******************************************************************************/

unique_ptr<Sequence> SequenceTools::getRandomSequence(shared_ptr<const Alphabet>& alphabet, size_t length)
{
  int s = static_cast<int>(alphabet->getSize());
  vector<int> content(length);
  for (size_t i = 0; i < length; ++i)
  {
    content[i] = RandomTools::giveIntRandomNumberBetweenZeroAndEntry(s);
  }
  return make_unique<Sequence>("random", content, alphabet);
}

/******************************************************************************/
