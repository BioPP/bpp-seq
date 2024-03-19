// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_CODONALPHABET_H
#define BPP_SEQ_ALPHABET_CODONALPHABET_H


#include "NucleicAlphabet.h"
#include "WordAlphabet.h"

// From the STL:
#include <string>
#include <memory>

namespace bpp
{
/**
 * @brief Codon alphabet class.
 * @author Laurent Guéguen, Julien Dutheil
 *
 * Since codons are made of 3 nucleic bases (RNA or DNA), this class
 * has a NucleicAlphabet field used to check char description. This
 * nucleic alphabet is passed to the constructor. This class also adds
 * some methods specific to codon manipulation.
 */

class CodonAlphabet :
  public virtual CoreWordAlphabet,
  public AbstractAlphabet
{
protected:
  std::shared_ptr<const NucleicAlphabet> nAlph_;

public:
  // Constructor and destructor.

  /**
   * @brief Builds a new codon alphabet from a nucleic alphabet.
   *
   * @param alpha The nucleic alphabet to be used.
   */
  CodonAlphabet(std::shared_ptr<const NucleicAlphabet> alpha) :
    AbstractAlphabet(),
    nAlph_(alpha)
  {
    build_();
  }

  CodonAlphabet(const CodonAlphabet& bia) :
    AbstractAlphabet(bia),
    nAlph_(bia.nAlph_)
  {}

  CodonAlphabet& operator=(const CodonAlphabet& bia)
  {
    AbstractAlphabet::operator=(bia);
    nAlph_ = bia.nAlph_;

    return *this;
  }

  CodonAlphabet* clone() const override
  {
    return new CodonAlphabet(*this);
  }

  virtual ~CodonAlphabet()
  {}

  std::string getAlphabetType() const override
  {
    return "Codon(letter=" + nAlph_->getAlphabetType() + ")";
  }

private:
  /**
   * @name Inner utilitary functions
   *
   * @{
   */
  bool containsUnresolved(const std::string& state) const override;

  bool containsGap(const std::string& state) const override;

  void build_();

  /** @} */

public:
  /**
   * @name From AbstractAlphabet
   *
   * @{
   */
  unsigned int getNumberOfTypes() const override { return 65; }

  unsigned int getSize() const override
  {
    return 64;
  }

  int getUnknownCharacterCode() const override
  {
    return 64;
  }

  bool isUnresolved(int state) const override
  {
    return state >= 64;
  }

  bool isUnresolved(const std::string& state) const override
  {
    return isUnresolved(charToInt(state));
  }

  bool isResolvedIn(int state1, int state2) const override;

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

  int charToInt(const std::string& state) const override
  {
    if (state.size() != 3)
      throw BadCharException(state, "CodonAlphabet::charToInt", this);
    if (containsUnresolved(state))
      return static_cast<int>(getSize());
    if (containsGap(state))
      return -1;
    else return AbstractAlphabet::charToInt(state);
  }

  /**
   * @name Codon specific methods
   *
   * @{
   */

  /**
   * @brief Get the int code for a codon given the int code of the three underlying positions.
   *
   * The int code of each position must match the nucleic alphabet specified for this alphabet.
   * @param pos1 Int description for position 1.
   * @param pos2 Int description for position 2.
   * @param pos3 Int description for position 3.
   * @return The int code of the codon.
   */
  int getCodon(int pos1, int pos2, int pos3) const
  {
    return (nAlph_->isUnresolved(pos1)
           || nAlph_->isUnresolved(pos2)
           || nAlph_->isUnresolved(pos3)) ? getUnknownCharacterCode()
        : pos3 + 4 * pos2 + 16 * pos1;
  }

  /**
   * @brief Get the char code for a codon given the char code of the
   * three underlying positions.
   *
   * The char code of each position must match the nucleic alphabet
   * specified for this alphabet.
   *
   * NB: This performs pos1 + pos2 + pos3 after checking for each
   * position validity.
   *
   * @param pos1 Char description for position 1.
   * @param pos2 Char description for position 2.
   * @param pos3 Char description for position 3.
   * @return The Char code of the codon.
   */
  std::string getCodon(const std::string& pos1, const std::string& pos2, const std::string& pos3) const
  {
    return pos1 + pos2 + pos3;
  }

  /**
   * @brief Get the int code of the first position of a codon given its int description.
   *
   * @param codon The int description of the codon.
   * @return The int description of the first position of the codon.
   */
  int getFirstPosition(int codon) const
  {
    return isUnresolved(codon) ? nAlph_->charToInt("N") : codon / 16;
  }

  /**
   * @brief Get the int code of the second position of a codon given its int description.
   *
   * @param codon The int description of the codon.
   * @return The int description of the second position of the codon.
   */
  int getSecondPosition(int codon) const
  {
    return isUnresolved(codon) ? nAlph_->charToInt("N") : (codon / 4) % 4;
  }


  /**
   * @brief Get the int code of the third position of a codon given its int description.
   *
   * @param codon The int description of the codon.
   * @return The int description of the third position of the codon.
   */
  int getThirdPosition(int codon) const
  {
    return isUnresolved(codon) ? nAlph_->charToInt("N") : codon % 4;
  }

  /**
   * @brief Get the char code of the first position of a codon given its char description.
   *
   * @param codon The char description of the codon.
   * @return The char description of the first position of the codon.
   */
  std::string getFirstPosition (const std::string& codon) const
  {
    return codon.substr(0, 1);
  }


  /**
   * @brief Get the char code of the second position of a codon given its char description.
   *
   * @param codon The char description of the codon.
   * @return The char description of the second position of the codon.
   */
  std::string getSecondPosition(const std::string& codon) const
  {
    return codon.substr(1, 1);
  }


  /**
   * @brief Get the char code of the third position of a codon given its char description.
   *
   * @param codon The char description of the codon.
   * @return The char description of the third position of the codon.
   */
  std::string getThirdPosition(const std::string& codon) const
  {
    return codon.substr(2, 1);
  }


  /**
   * @name From CoreWordAlphabet
   *
   * @{
   */
  unsigned int getLength() const override
  {
    return 3;
  }

  bool hasUniqueAlphabet() const override
  {
    return true;
  }

  std::shared_ptr<const Alphabet> getNAlphabet(size_t n) const override
  {
    return nAlph_;
  }

  int getWord(const Sequence& seq, size_t pos = 0) const override
  {
    if (seq.size() < pos + 3)
      throw IndexOutOfBoundsException("CodonAlphabet::getWord", pos, 0, seq.size() - 3);
    return getCodon(seq[pos], seq[pos + 1], seq[pos + 2]);
  }

  /**
   * @brief Get the char code for a word given the char code of the
   * underlying positions.
   *
   * The char code of each position must match the corresponding alphabet specified at this position.
   * @param vpos vector description for all the positions.
   * @param pos the start position to match in the vector.
   * @return The string of the word.
   * @throw IndexOutOfBoundsException In case of wrong position.
   */
  std::string getWord(const std::vector<std::string>& vpos, size_t pos = 0) const override
  {
    if (vpos.size() < pos + 3)
      throw IndexOutOfBoundsException("CodonAlphabet::getWord", pos, 0, vpos.size() - 3);

    return getCodon(vpos[pos], vpos[pos + 1], vpos[pos + 2]);
  }

  int getWord(const std::vector<int>& vpos, size_t pos = 0) const override
  {
    if (vpos.size() < pos + 3)
      throw IndexOutOfBoundsException("CodonAlphabet::getWord", pos, 0, vpos.size() - 3);

    return getCodon(vpos[pos], vpos[pos + 1], vpos[pos + 2]);
  }


  int getNPosition(int codon, size_t pos) const override
  {
    if (isUnresolved(codon))
      return nAlph_->getUnknownCharacterCode();
    else
      return pos == 0 ? codon / 16 :
             (pos == 1 ? (codon / 4) % 4
                 : codon % 4);
  }

  /**
   * @brief Get the int codes of each position of a word given its int description.
   *
   * @param word The int description of the word.
   * @return The int description of the positions of the codon.
   */
  std::vector<int> getPositions(int word) const override
  {
    if (isUnresolved(word))
    {
      int n = nAlph_->getUnknownCharacterCode();
      return std::vector<int>{n, n, n};
    }
    else
      return std::vector<int>{word / 16, (word / 4) % 4, word % 4}
    ;
  }


  /**
   * @brief Get the char code of the Nth position of a codon given its char description.
   *
   * @param codon The char description of the codon.
   * @param pos the position in the codon (starting at 0)
   * @return The char description of the position of the codon.
   */
  std::string getNPosition(const std::string& codon, size_t pos) const override
  {
    return codon.substr(pos, 1);
  }

  /**
   * @brief Get the char codes of each position of a word given its char description.
   *
   * @param word The char description of the word.
   * @return The char description of the three positions of the word.
   */
  std::vector<std::string> getPositions(const std::string& word) const override
  {
    return std::vector<std::string>{word.substr(0, 1), word.substr(1, 1), word.substr(2, 1)};
  }

  /**
   * @brief Translate a whole sequence from letters alphabet to words alphabet.
   *
   * @param sequence A sequence in letters alphabet.
   * @param pos the start postion (default 0)
   * @return The corresponding sequence in words alphabet.
   * @throw AlphabetMismatchException If the sequence alphabet do not match the source alphabet.
   * @throw Exception                 Other kind of error, depending on the implementation.
   */
  std::unique_ptr<SequenceInterface> translate(const SequenceInterface& sequence, size_t = 0) const override;

  /**
   * @brief Translate a whole sequence from words alphabet to letters alphabet.
   *
   * @param sequence A sequence in words alphabet.
   * @return The corresponding sequence in letters alphabet.
   * @throw AlphabetMismatchException If the sequence alphabet do not match the target alphabet.
   * @throw Exception                 Other kind of error, depending on the implementation.
   */
  std::unique_ptr<SequenceInterface> reverse(const SequenceInterface& sequence) const override;

  /*
   *
   * @}
   */

  /**
   * @brief Get the number of G+C in codon
   * @param codon The int description of the codon.
   *
   * @return The number of G+C in codon
   */
  int getGCinCodon(int codon) const;

  /**
   * @return The nucleic alphabet associated to this codon alphabet.
   */
  std::shared_ptr<const NucleicAlphabet> getNucleicAlphabet() const
  {
    return nAlph_;
  }

  /**
   * @name Overloaded AbstractAlphabet methods.
   * @{
   */
  unsigned int getStateCodingSize() const override { return 3; }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_CODONALPHABET_H
