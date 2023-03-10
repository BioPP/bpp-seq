//
// File: SequenceTools.h
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

#ifndef BPP_SEQ_SEQUENCETOOLS_H
#define BPP_SEQ_SEQUENCETOOLS_H

#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Random/RandomTools.h>
#include <Bpp/Numeric/Stat/StatTest.h>

#include "Alphabet/Alphabet.h"
#include "Alphabet/DNA.h"
#include "Alphabet/RNA.h"
#include "Alphabet/RNY.h"
#include "GeneticCode/GeneticCode.h"
#include "NucleicAcidsReplication.h"
#include "Sequence.h"
#include "SymbolListTools.h"

// From the STL:
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>

namespace bpp
{
/**
 * @brief Bowker's homogeneity test results class.
 */
class BowkerTest :
  public StatTest
{
private:
  double pvalue_;
  double stat_;

public:
  BowkerTest() : pvalue_(1.),
    stat_(0.) {}

  virtual ~BowkerTest() {}

  BowkerTest* clone() const { return new BowkerTest(*this); }

public:
  std::string getName() const { return "Bowker's test for homogeneity."; }
  double getStatistic() const { return stat_; }
  double getPValue() const { return pvalue_; }

  void setStatistic(double stat) { stat_ = stat; }
  void setPValue(double pvalue) { pvalue_ = pvalue; }
};

/**
 * @brief SequenceTools static class
 *
 * Implement methods to manipulate sequences
 */
class SequenceTools :
  public SymbolListTools
{
private:
  static std::shared_ptr<RNY> RNY_;
  static NucleicAcidsReplication DNARep_;
  static NucleicAcidsReplication RNARep_;
  static NucleicAcidsReplication transc_;

public:
  SequenceTools() {}
  virtual ~SequenceTools() {}

public:
  /**
   * @param seq1 The first sequence.
   * @param seq2 The second sequence.
   * @return True if the two sequences have the same content (and, of course, alphabet).
   */
  static bool areSequencesIdentical(const SequenceInterface& seq1, const SequenceInterface& seq2);

  /**
   * @brief Get a sub-sequence.
   *
   * @param sequence The sequence to trunc.
   * @param begin    The first position of the subsequence.
   * @param end      The last position of the subsequence (included).
   * @param output   A sequence object to be appended with the given subsequence.
   */
  static void subseq(const SequenceInterface& sequence, size_t begin, size_t end, SequenceInterface& output)
  {
    if (end < begin || end >= sequence.size())
      throw Exception("SequenceTools::subseq. Invalid coordinates begin=" + TextTools::toString(begin) + ", end=" + TextTools::toString(end) + " for a sequence of size " + TextTools::toString(sequence.size()) + ".");
    std::vector<int> content(end - begin + 1);
    for (size_t i = 0; i <= end - begin; ++i)
    {
      content[i] = sequence[begin + i];
    }
    output.append(content);
  }

  /**
   * @brief Get a sub-sequence.
   *
   * @param sequence The sequence to trunc.
   * @param begin The first position of the subsequence.
   * @param end   The last position of the subsequence.
   * @return A new sequence object with the given subsequence.
   */
  template<class SequenceTypeOut>
  static std::unique_ptr<SequenceTypeOut> subseq(const SequenceInterface& sequence, size_t begin, size_t end)
  {
    auto alphaPtr = sequence.getAlphabet();
    auto seq = std::make_unique<SequenceTypeOut>(alphaPtr);
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
   * @param seq1 The first sequence.
   * @param seq2 The second sequence.
   * @return A new sequence object with the concatenation of the two sequences.
   * The type of the output sequence is the same as the one of seq1, as it is created using the clone method.
   * @throw AlphabetMismatchException If the two alphabets do not match.
   * @throw Exception If the sequence names do not match.
   */
  template<class SequenceTypeOut>
  static std::unique_ptr<SequenceTypeOut> concatenate(const SequenceInterface& seq1, const SequenceInterface& seq2)
  {
    // Sequence's alphabets matching verification
    if ((seq1.alphabet().getAlphabetType()) != (seq2.alphabet().getAlphabetType()))
      throw AlphabetMismatchException("SequenceTools::concatenate : Sequence's alphabets don't match ", seq1.getAlphabet(), seq2.getAlphabet());

    // Sequence's names matching verification
    if (seq1.getName() != seq2.getName())
      throw Exception ("SequenceTools::concatenate : Sequence's names don't match");

    // Concatenate sequences and send result
    auto concat = std::make_unique<SequenceTypeOut>(seq1);
    concat->setToSizeR(seq1.size() + seq2.size());
    for (size_t i = 0; i < seq2.size(); ++i)
    {
      (*concat)[seq1.size() + i] = seq2[i];
    }
    return concat;
  }

  /**
   * @brief Complement the nucleotide sequence itself
   *
   * @param seq The sequence to be complemented.
   * @throw AlphabetException if the sequence is not a nucleotide sequence.
   * @author Sylvain Gaillard
   */
  static void complement(SequenceInterface& seq);

  /**
   * @brief Get the complementary sequence of a nucleotide sequence.
   *
   * @see DNAReplication
   * @return A new sequence object with the complementary sequence.
   * @param sequence The sequence to complement.
   * @throw AlphabetException If the sequence is not a nucleotide sequence.
   */
  static std::unique_ptr<Sequence> getComplement(const SequenceInterface& sequence);
 
  /**
   * @brief Get the transcription sequence of a DNA sequence.
   *
   * Translate DNA sequence into RNA sequence.
   *
   * @see DNAReplication
   * @return sequence A new sequence object with the transcription sequence.
   * @param sequence The sequence to transcript.
   * @throw AlphabetException If the sequence is not a DNA sequence.
   */
  static std::unique_ptr<Sequence> transcript(const Sequence& sequence);

  /**
   * @brief Get the reverse-transcription sequence of a RNA sequence.
   *
   * Translate RNA sequence into DNA sequence.
   *
   * @see DNAReplication
   * @return sequence A new sequence object with the reverse-transcription sequence.
   * @param sequence The sequence to reverse-transcript.
   * @throw AlphabetException If the sequence is not a RNA sequence.
   */
  static std::unique_ptr<Sequence> reverseTranscript(const Sequence& sequence);

  /**
   * @brief Inverse a sequence from 5'->3' to 3'->5' and vice-versa.
   *
   * ABCDEF becomes FEDCBA, and the sense attribute is changed (may be
   * inhibited).
   *
   * @param seq The sequence to inverse.
   * @author Sylvain Gaillard
   */
  template<class SequenceType>
  static void invert(SequenceType& seq)
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

  /**
   * @brief Inverse a sequence from 5'->3' to 3'->5' and vice-versa.
   *
   * ABCDEF becomes FEDCBA, and the sense attribute is changed (may be
   * inhibited).
   *
   * @param sequence The sequence to inverse.
   * @return A new sequence object containing the inverted sequence.
   * @author Sylvain Gaillard
   */
  static std::unique_ptr<Sequence> getInvert(const Sequence& sequence);

  /**
   * @brief Inverse and complement a sequence.
   *
   * This methode is more accurate than calling invert and complement
   * separatly.
   *
   * @param seq The sequence to inverse and complement.
   * @author Sylvain Gaillard
   */
  static void invertComplement(Sequence& seq);

  /**
   * @return The identity percent of 2 sequence.
   * One match is counted if the two sequences have identical states.
   * @param seq1 The first sequence.
   * @param seq2 The second sequence.
   * @param ignoreGaps If true, only positions without gaps will be used for the counting.
   * @throw AlphabetMismatchException If the two sequences do not have the same alphabet.
   * @throw SequenceNotAlignedException If the two sequences do not have the same length.
   */
  static double getPercentIdentity(const Sequence& seq1, const Sequence& seq2, bool ignoreGaps = false);

  /**
   * @return The number of sites in the sequences, <i>i.e.</i> all positions without gaps.
   *
   * @param seq The sequence to analyse.
   */
  static size_t getNumberOfSites(const Sequence& seq);

  /**
   * @return The number of complete sites in the sequences, <i>i.e.</i> all positions without gaps and unresolved states (generic characters).
   *
   * @param seq The sequence to analyse.
   */
  static size_t getNumberOfCompleteSites(const Sequence& seq);

  /**
   * @brief keep only complete sites in a sequence.
   *
   * The deleteElement method of the Sequence object will be used where appropriate.
   * @param seq The sequence to analyse.
   */

  static std::unique_ptr<Sequence> getSequenceWithCompleteSites(const Sequence& seq);

  /**
   * @return The number of unresolved sites in the sequence.
   *
   * @param seq The sequence to analyse.
   *
   * @author Sylvain Gaillard
   */

  static size_t getNumberOfUnresolvedSites(const Sequence& seq);


  /**
   * @brief Remove gaps from a sequence.
   *
   * The deleteElement method of the Sequence object will be used where appropriate.
   * @param seq The sequence to analyse.
   */
  static void removeGaps(Sequence& seq);

  /**
   * @brief Get a copy of the sequence without gaps.
   *
   * A whole new sequence will be created by adding all non-gap positions.
   * The original sequence will be cloned to serve as a template.
   *
   * @param seq The sequence to analyse.
   * @return A new sequence object without gaps.
   */
  static std::unique_ptr<Sequence> getSequenceWithoutGaps(const Sequence& seq);

  /**
   * @brief Remove stops from a codon sequence.
   *
   * The deleteElement method of the Sequence object will be used where appropriate.
   * @param seq The sequence to analyse.
   * @param gCode The genetic code according to which stop codons are specified.
   * @throw Exception if the input sequence does not have a codon alphabet.
   */
  static void removeStops(Sequence& seq, const GeneticCode& gCode);

  /**
   * @brief Get a copy of the codon sequence without stops.
   *
   * A whole new sequence will be created by adding all non-stop positions.
   * The original sequence will be cloned to serve as a template.
   *
   * @param seq The sequence to analyse.
   * @param gCode The genetic code according to which stop codons are specified.
   * @return A new sequence object without stops.
   * @throw Exception if the input sequence does not have a codon alphabet.
   */
  static std::unique_ptr<Sequence> getSequenceWithoutStops(const Sequence& seq, const GeneticCode& gCode);

  /**
   * @brief Replace stop codons by gaps.
   *
   * The setElement method of the Sequence object will be used where appropriate.
   * @param seq The sequence to analyse.
   * @param gCode The genetic code according to which stop codons are specified.
   * @throw Exception if the input sequence does not have a codon alphabet.
   */
  static void replaceStopsWithGaps(Sequence& seq, const GeneticCode& gCode);

  /**
   * @brief Bowker's test for homogeneity.
   *
   * Computes the contingency table of occurrence of all pairs of states and test its symmetry using Bowker's (1948) test.
   *
   * Reference:<br>
   * @code
   * Ababneh F. Bioinformatics 2006 22(10) 1225-1231
   * @endcode
   *
   * @param seq1 The first sequence.
   * @param seq2 The second sequence.
   * @return A BowkerTest object with the computed statistic and p-value (computed from a chi square distribution).
   * @throw SequenceNotAlignedException If the two sequences do not have the same length.
   */
  static std::unique_ptr<BowkerTest> bowkerTest(const Sequence& seq1, const Sequence& seq2);

  /**
   * @brief Get all putatives haplotypes from an heterozygous sequence.
   *
   * @param seq The sequence to resolve
   * @param hap The vector to fill with the new sequences
   * @param level The maximum number of states that a generic char must code
   * (if this number is higher than level, the state will not be resolved).
   * For instance if level = 3 and Alphabet is DNA, all generic char will be
   * resolved but N.
   *
   * @author Sylvain Gaillard
   */
  static void getPutativeHaplotypes(const Sequence& seq, std::vector< std::unique_ptr<Sequence> >& hap, unsigned int level = 2);

  /**
   * @brief Combine two sequences.
   *
   * @author Sylvain Gaillard
   */
  static std::unique_ptr<Sequence> combineSequences(const Sequence& s1, const Sequence& s2);

  /**
   * @brief Subtract haplotype from an heterozygous sequence.
   *
   * Subtract an haplotype (i.e. a fully resolved sequence) from an heterozygous
   * sequence to get the other haplotype. The new haplotype could be an unresolved
   * sequence if unresolved characters in the sequence code for more than 2 states.
   *
   * For example:<br>
   * @code
   * >heterozygous sequence
   * ATTCGGGKWTATRYRM
   * >haplotype
   * ATTCGGGTATATGCAA
   * >subtracted haplotype
   * ATTCGGGGTTATATGC
   * @endcode
   *
   * @param s The heterozygous sequence.
   * @param h The haplotype to subtract.
   * @param name The name of the new computed haplotype.
   * @param level The number of states from which the site is set to fully unresolved.
   * @throw SequenceNotAlignedException if s and h don't have the same size.
   *
   * @author Sylvain Gaillard
   */
  static std::unique_ptr<Sequence> subtractHaplotype(const Sequence& s, const Sequence& h, std::string name = "", unsigned int level = 1);

  /**
   * @brief Get the RNY decomposition of a DNA sequence
   *
   * This function gives the decomposition in the given phase.
   * In phase 1, the first triplet is centered on the first character.
   *
   * @return sequence A new sequence object with the transcription sequence.
   * @param sequence The sequence to transcript.
   * @param ph The phase to use (1,2 or 3).
   * @throw AlphabetException If the sequence is not a DNA sequence.
   *
   * @author Laurent Guéguen
   */
  static std::unique_ptr<Sequence> RNYslice(const Sequence& sequence, int ph);

  /**
   * @brief Get the RNY decomposition of a DNA sequence
   *
   * This function gives the alternative succession in
   * phases 1, 2 and 3.
   *
   * @return sequence A new sequence object with the transcription sequence.
   * @param sequence The sequence to transcript.
   * @throw AlphabetException If the sequence is not a DNA sequence.
   *
   * @author Laurent Guéguen
   */
  static std::unique_ptr<Sequence> RNYslice(const Sequence& sequence);

  /**
   * @brief Extract CDS part from a codon sequence. Optionally check for intiator and stop codons, or both.
   *
   * @param sequence The sequence to be reduced to CDS part.
   * @param gCode The genetic code according to which start and stop codons are specified.
   * @param checkInit If true, then everything before the initiator codon will be removed, together with the initiator codon if includeInit is false.
   * @param checkStop If true, then everything after the first stop codon will be removed, together with the stop codon if includeStop is false.
   * @param includeInit Tell if initiator codon should be kept or removed. No effect if checkInit is false.
   * @param includeStop Tell if stop codon should be kept or removed. No effect if checkStop is false.
   */
  static void getCDS(Sequence& sequence, const GeneticCode& gCode, bool checkInit, bool checkStop, bool includeInit = true, bool includeStop = true);

  /**
   * @brief Find the position of a motif in a sequence
   *
   * @param seq The reference sequence
   * @param motif The motif to find
   * @param strict If true (default) find exactly the motif
   *               If false find compatible match
   * @return The position of the first occurence of the motif or the seq
   * length.
   */
  static size_t findFirstOf(const Sequence& seq, const Sequence& motif, bool strict = true);

  /**
   * @brief Get a random sequence of given size and alphabet, with all state with equal probability.
   *
   * @param alphabet The alphabet to use.
   * @param length The length of the sequence to generate.
   * @return A pointer toward a new Sequence object.
   */
  static std::unique_ptr<Sequence> getRandomSequence(std::shared_ptr<const Alphabet>& alphabet, size_t length);
};
} // end of namespace bpp.
#endif // BPP_SEQ_SEQUENCETOOLS_H
