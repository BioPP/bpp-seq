// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_ALLELICALPHABET_H
#define BPP_SEQ_ALPHABET_ALLELICALPHABET_H

#include "AbstractAlphabet.h"

// From the STL:
#include <string>
#include <vector>

#include "../Transliterator.h"
#include <Bpp/Seq/ProbabilisticSequence.h>

namespace bpp
{
/**
 * @brief The base class for allelic alphabets.
 *
 * These alphabets are made of unoriented pairs (element from an
 * alphabet, N)^2, such that both numbers sum a given constant @f$N@f$
 * number of individuals. The construction is made from a
 * shared_pointer to an AbstractAlphabet.
 *
 * The strings of the AllelicAlphabet are pairs of the strings of the
 * Alphabets, with the attributed numbers, such that the first element
 * is before the second element in the base Alphabet (such as A3C4,
 * and not C4A3). They are made from the resolved letters of the
 * Alphabets.
 *
 * The letters are numbered as:
 *
 *  -1  : gap
 *
 *  0 -> alphabet size:  alphabet states
 *
 *  (i * (alphabet size) + j) * (nbAlleles-1) + size + k -1 : @f$ a_i k  a_j (N-k)@f$ state
 *
 *  (alphabet size)^2 * (nbAlleles -1) : Unknown "?N?0"
 *
 * For nucleotides with three alleles, the underlying states are for instance:
 *
 * _3_0 (-1), A3_0 (0), C3_0 (1), G3_0 (2), T3_0 (3), A2C1 (6), A1C2 (7), A2G1 (8), A1G2 (9),
 * A2T1 (10), A1T2 (11), C2G1 (16), C1G2 (17), C2T1 (18), C1T2 (19), G2T1 (26), G1T2 (27), ?3_0 (32)
 *
 */
class AllelicAlphabet :
  public AbstractAlphabet
{
protected:
  std::shared_ptr<const Alphabet> alph_;

  /**
   * @brief the number of alleles.
   */
  unsigned int nbAlleles_;

  /**
   * @brief the unknown state number
   */
  int nbUnknown_;

public:
  // Constructor and destructor.
  /**
   * @brief Builds a new word alphabet from an  Alphabet
   */
  AllelicAlphabet(std::shared_ptr<const Alphabet> alph, unsigned int nbAlleles);

  AllelicAlphabet(const AllelicAlphabet& bia) :
    AbstractAlphabet(bia),
    alph_(bia.alph_),
    nbAlleles_(bia.nbAlleles_),
    nbUnknown_(bia.nbUnknown_)
  {}

  AllelicAlphabet* clone() const override
  {
    return new AllelicAlphabet(*this);
  }

  virtual ~AllelicAlphabet() {}

public:
  /**
   * @name Methods redefined from Alphabet
   *
   * @{
   */
  int charToInt(const std::string& state) const override
  {
    if (state.size() != getStateCodingSize())
      throw BadCharException(state, "AllelicAlphabet::charToInt", this);
    else
      return AbstractAlphabet::charToInt(state);
  }

  unsigned int getSize() const override
  {
    return getNumberOfChars() - 2;
  }

  unsigned int getNumberOfTypes() const override
  {
    return getNumberOfChars() - 1;
  }

  /** @} */


  bool isResolvedIn(int state1, int state2) const override;

  /**
   * @brief Returns the number of alleles
   *
   */
  unsigned int getNbAlleles() const
  {
    return nbAlleles_;
  }

  /**
   * @brief Returns Base Alphabet
   */
  std::shared_ptr<const Alphabet> getStateAlphabet() const
  {
    return alph_;
  }

  const Alphabet& stateAlphabet() const
  {
    return *alph_;
  }

  std::string getAlphabetType() const override;

  int getUnknownCharacterCode() const override
  {
    return nbUnknown_;
  }

  bool isUnresolved(int state) const override { return state == getUnknownCharacterCode(); }

  bool isUnresolved(const std::string& state) const override { return charToInt(state) == getUnknownCharacterCode(); }

  std::vector<int> getAlias(int state) const override;

  std::vector<std::string> getAlias(const std::string& state) const override;

  int getGeneric(const std::vector<int>& states) const override
  {
    return states[0];
  }

  std::string getGeneric(const std::vector<std::string>& states) const override
  {
    return states[0];
  }

  class AllelicTransliterator : public AbstractTransliterator
  {
private:
    std::shared_ptr<const AllelicAlphabet> alph_;

public:
    AllelicTransliterator(std::shared_ptr<const AllelicAlphabet> alph) :
      AbstractTransliterator(), alph_(alph)
    {}

    std::shared_ptr<const Alphabet> getSourceAlphabet() const override
    {
      return alph_->getStateAlphabet();
    }
    std::shared_ptr<const Alphabet> getTargetAlphabet() const override { return alph_; }

    std::string translate(const std::string& state) const override
    {
      return getTargetAlphabet()->intToChar(getSourceAlphabet()->charToInt(state));
    }

    /**
     * @brief States of the original alphabet are the first ones of
     * the allelic alphabet.
     */
    int translate(int state) const override
    {
      return state;
    }

    std::unique_ptr<Sequence> translate(const SequenceInterface& sequence) const override
    {
      return AbstractTransliterator::translate(sequence);
    }
  };

  /**
   * @brief Convert a CoreSequence in StateAlphabet to a
   * ProbabilisticSequence of the likelihoods of the di-allelic
   * Alphabet (so if counts are not on only two states, the
   * likelihood is null).
   *
   * @param sequence the CoreSequence to be converted.
   *
   *  Gaps are directly translated in vectors of 1.
   */
  std::unique_ptr<ProbabilisticSequence> convertFromStateAlphabet(const CoreSequenceInterface& sequence) const;

  /**
   * @brief Fills the vector of the likelihoods of a vector of
   * counts in the states alphabet, given the di-alleless.
   *
   */
  void computeLikelihoods(const Vdouble& counts, Vdouble& likelihoods) const;

private:
  /**
   * @name Inner utilitary functions
   *
   * @{
   */
  // bool containsUnresolved(const std::string& state) const;
  // bool containsGap(const std::string& state) const;

  /** @} */

  /**
   * @name Overloaded AbstractAlphabet methods.
   * @{
   */
  unsigned int getStateCodingSize() const override
  {
    auto x = 2 * ((unsigned int)alph_->getStateCodingSize() + (unsigned int)std::to_string(nbAlleles_).size());
    return x;
  }

  /** @} */
};
} // end of namespace bpp.

#endif // BPP_SEQ_ALPHABET_ALLELICALPHABET_H
