// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "SequenceWithQualityTools.h"

using namespace bpp;
using namespace std;

NucleicAcidsReplication SequenceWithQualityTools::DNARep_(AlphabetTools::DNA_ALPHABET, AlphabetTools::DNA_ALPHABET);
NucleicAcidsReplication SequenceWithQualityTools::RNARep_(AlphabetTools::RNA_ALPHABET, AlphabetTools::RNA_ALPHABET);
NucleicAcidsReplication SequenceWithQualityTools::transc_(AlphabetTools::DNA_ALPHABET, AlphabetTools::RNA_ALPHABET);

/******************************************************************************/

unique_ptr<SequenceWithQuality> SequenceWithQualityTools::concatenate(
    const SequenceWithQuality& seqwq1,
    const SequenceWithQuality& seqwq2)
{
  // Sequence's alphabets matching verification
  if ((seqwq1.getAlphabet()->getAlphabetType()) != (seqwq2.getAlphabet()->getAlphabetType()))
    throw AlphabetMismatchException("SequenceTools::concatenate : Sequence's alphabets don't match ", seqwq1.getAlphabet(), seqwq2.getAlphabet());

  // Sequence's names matching verification
  if (seqwq1.getName() != seqwq2.getName())
    throw Exception ("SequenceTools::concatenate : Sequence's names don't match");

  // Concatenate sequences and send result
  vector<int> sequence  = seqwq1.getContent();
  vector<int> sequence2 = seqwq2.getContent();
  vector<int> qualities = seqwq1.getQualities();
  vector<int> qualities2 = seqwq2.getQualities();

  sequence.insert(sequence.end(), sequence2.begin(), sequence2.end());
  qualities.insert(qualities.end(), qualities2.begin(), qualities2.end());
  auto alphaPtr = seqwq1.getAlphabet();
  return make_unique<SequenceWithQuality>(seqwq1.getName(), sequence, qualities, seqwq1.getComments(), alphaPtr);
}

/******************************************************************************/

unique_ptr<SequenceWithQuality> SequenceWithQualityTools::complement(
    const SequenceWithQuality& sequence)
{
  // Alphabet type checking
  NucleicAcidsReplication* NAR;
  if (sequence.getAlphabet()->getAlphabetType() == "DNA alphabet")
  {
    NAR = &DNARep_;
  }
  else if (sequence.getAlphabet()->getAlphabetType() == "RNA alphabet")
  {
    NAR = &RNARep_;
  }
  else
  {
    throw AlphabetException ("SequenceTools::complement : Sequence must be nucleic.", sequence.getAlphabet());
  }
  auto seq = NAR->translate(sequence);
  auto seqwq = make_unique<SequenceWithQuality>(*seq, sequence.getQualities());
  return seqwq;
}

/******************************************************************************/

unique_ptr<SequenceWithQuality> SequenceWithQualityTools::transcript(
    const SequenceWithQuality& sequence)
{
  // Alphabet type checking
  if (sequence.getAlphabet()->getAlphabetType() != "DNA alphabet")
  {
    throw AlphabetException ("SequenceTools::transcript : Sequence must be DNA", sequence.getAlphabet());
  }
  auto seq = transc_.translate(sequence);
  auto seqwq = make_unique<SequenceWithQuality>(*seq, sequence.getQualities());
  return seqwq;
}

/******************************************************************************/

unique_ptr<SequenceWithQuality> SequenceWithQualityTools::reverseTranscript(
    const SequenceWithQuality& sequence)
{
  // Alphabet type checking
  if (sequence.getAlphabet()->getAlphabetType() != "RNA alphabet")
  {
    throw AlphabetException ("SequenceTools::reverseTranscript : Sequence must be RNA", sequence.getAlphabet());
  }

  auto seq = transc_.reverse(sequence);
  // Here we must also reverse the scores:
  vector<int> scores(sequence.getQualities().rbegin(), sequence.getQualities().rend());
  auto seqwq = make_unique<SequenceWithQuality>(*seq, scores);
  return seqwq;
}

/******************************************************************************/

unique_ptr<SequenceWithQuality> SequenceWithQualityTools::invert(
    const SequenceWithQuality& sequence)
{
  vector<int> iContent(sequence.getContent().rbegin(), sequence.getContent().rend());
  vector<int> iQualities(sequence.getQualities().rbegin(), sequence.getQualities().rend());
  auto iSeq = unique_ptr<SequenceWithQuality>(sequence.clone());
  iSeq->setContent(iContent);
  iSeq->setQualities(iQualities);

  return iSeq;
}

/******************************************************************************/

unique_ptr<SequenceWithQuality> SequenceWithQualityTools::removeGaps(const SequenceWithQuality& seq)
{
  vector<int> content;
  vector<int> qualities;
  auto alpha = seq.getAlphabet();
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (!alpha->isGap(seq[i]))
    {
      content.push_back(seq[i]);
      qualities.push_back(seq.getQualities()[i]);
    }
  }
  auto newSeq = unique_ptr<SequenceWithQuality>(seq.clone());
  newSeq->setContent(content);
  newSeq->setQualities(qualities);
  return newSeq;
}

/******************************************************************************/

