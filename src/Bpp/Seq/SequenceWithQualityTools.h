// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCEWITHQUALITYTOOLS_H
#define BPP_SEQ_SEQUENCEWITHQUALITYTOOLS_H


#include "SequenceTools.h"
#include "SequenceWithQuality.h"

namespace bpp
{
/**
 * @brief The SequenceWithQualityTools static class
 *
 * Implement methods to manipulate SequencesWithQuality
 *
 * @todo
 * - does complement, transcript and reverseTranscript have a meaning on
 *   SequenceWithQuality as the quality is not transferable
 *
 * @author Cahais Vincent
 */

class SequenceWithQualityTools
{
private:
  static DNA DNA_;
  static RNA RNA_;
  static NucleicAcidsReplication DNARep_;
  static NucleicAcidsReplication RNARep_;
  static NucleicAcidsReplication transc_;

public:
  /**
   * @brief Get a sub-sequence.
   *
   * @param sequence The sequence to trunc.
   * @param begin    The first position of the subsequence.
   * @param end      The last position of the subsequence (included).
   * @param output   A sequence object to be appended with the given subsequence.
   */
  static void subseq(const SequenceWithQuality& sequence, size_t begin, size_t end, SequenceWithQuality& output)
  {
    if (end < begin || end >= sequence.size())
      throw Exception("SequenceWithQualityTools::subseq. Invalid coordinates begin=" + TextTools::toString(begin) + ", end=" + TextTools::toString(end) + " for a sequence of size " + TextTools::toString(sequence.size()) + ".");
    std::vector<int> content(end - begin + 1);
    std::vector<int> scores(end - begin + 1);
    for (size_t i = 0; i <= end - begin; ++i)
    {
      content[i] = sequence[begin + i];
      scores[i] = sequence.getQuality(begin + i);
    }
    output.append(content, scores);
  }

  /**
   * @brief Get a sub-sequence.
   *
   * @param sequence The sequence to trunc.
   * @param begin The first position of the subsequence.
   * @param end   The last position of the subsequence.
   * @return A new sequence object with the given subsequence.
   */
  static std::unique_ptr<SequenceWithQuality> subseq(const SequenceWithQuality& sequence, size_t begin, size_t end)
  {
    auto alphaPtr = sequence.getAlphabet();
    auto seq = std::make_unique<SequenceWithQuality>(alphaPtr);
    seq->setName(sequence.getName());
    seq->setComments(sequence.getComments());
    subseq(sequence, begin, end, *seq);
    return seq;
  }


  /**
   * @brief Concatenate two sequences.
   *
   * Sequences must have the same name and alphabets.
   * Only first sequence's commentaries are kept.
   *
   * @param seqwq1 The first SequenceWithQuality.
   * @param seqwq2 The second SequenceWithQuality.
   * @return A new SequenceWithQuality object with the concatenation of the
   * two sequences.
   * @throw AlphabetMismatchException If the two alphabets do not match.
   * @throw Exception If the sequence names do not match.
   */
  static std::unique_ptr<SequenceWithQuality> concatenate(
    const SequenceWithQuality& seqwq1,
    const SequenceWithQuality& seqwq2);

  /**
   * @brief Get the complementary sequence of a nucleotide sequence.
   *
   * @see DNAReplication
   * @return sequence A new SequenceWithQuality object with the
   * complementary sequence.
   * @param sequence The sequence to complement.
   * @throw AlphabetException If the sequence is not a nucleotide sequence.
   */
  static std::unique_ptr<SequenceWithQuality> complement(
    const SequenceWithQuality& sequence);

  /**
   * @brief Get the transcription sequence of a DNA sequence.
   *
   * @see DNAReplication
   * @return sequence A new SequenceWithQuality object with the
   * transcription sequence.
   * @param sequence The sequence to transcript.
   * @throw AlphabetException If the sequence is not a DNA sequence.
   */
  static std::unique_ptr<SequenceWithQuality> transcript(
    const SequenceWithQuality& sequence);

  /**
   * @brief Get the reverse-transcription sequence of a RNA sequence.
   *
   * @see DNAReplication
   * @return sequence A new SequenceWithQuality object with the reverse-
   * transcription sequence.
   * @param sequence The SequenceWithQuality to reverse-transcript.
   * @throw AlphabetException If the sequence is not a RNA sequence.
   */
  static std::unique_ptr<SequenceWithQuality> reverseTranscript(
    const SequenceWithQuality& sequence);

  /**
   * @brief Inverse a sequence from 5'->3' to 3'->5' and vice-versa.
   *
   * ABCDEF becomes FEDCBA, and the sense attribute is changed (may be
   * inhibited).
   *
   * @return A new SequenceWithQuality object containing the inverted
   * sequence.
   * @param sequence The SequenceWithQuality to inverse.
   */
  static std::unique_ptr<SequenceWithQuality> invert(
    const SequenceWithQuality& sequence);

  /**
   * @brief Remove gaps from a SequenceWithQuality.
   *
   * @param seq The sequence to analyse.
   * @return A new SequenceWithQuality object without gaps.
   */
  static std::unique_ptr<SequenceWithQuality> removeGaps(const SequenceWithQuality& seq);

};
}
#endif // BPP_SEQ_SEQUENCEWITHQUALITYTOOLS_H
