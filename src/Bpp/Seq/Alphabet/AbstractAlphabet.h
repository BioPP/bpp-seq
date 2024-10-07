// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_ABSTRACTALPHABET_H
#define BPP_SEQ_ALPHABET_ABSTRACTALPHABET_H

#include <Bpp/Exceptions.h>

#include "Alphabet.h"
#include "AlphabetState.h"

// From the STL:
#include <string>
#include <vector>
#include <map>

namespace bpp
{
/**
 * @brief A partial implementation of the Alphabet interface.
 *
 * It contains a vector of AlphabetState.
 * All methods are based upon this vector
 * but do not provide any method to initialize it.
 * This is up to each constructor of the derived classes.
 *
 * @see Alphabet
 */
class AbstractAlphabet :
  public Alphabet
{
private:
  /**
   * @brief Alphabet: vector of AlphabetState.
   */
  std::vector<AlphabetState*> alphabet_;

  /**
   * @name maps used to quick search for letter and num.
   * @{
   */
  std::map<std::string, size_t> letters_;
  std::map<int, size_t> nums_;
  /** @} */

  /**
   * @brief Update the private maps letters_ and nums_ when adding a state.
   *
   * @param pos The index of the state in the alphabet_ vector.
   * @param st The state that has been added or modified
   */
  void updateMaps_(size_t pos, const AlphabetState& st);

protected:
  /**
   * @name Available codes
   *
   * These vectors will be computed the first time you call the getAvailableInts or getAvailableChars method.
   *
   * @{
   */
  mutable std::vector<std::string> charList_;
  mutable std::vector<int> intList_;
  /** @} */

public:
  AbstractAlphabet() : alphabet_(), letters_(), nums_(), charList_(), intList_()
  {}

  AbstractAlphabet(const AbstractAlphabet& alph) : alphabet_(), letters_(alph.letters_), nums_(alph.nums_), charList_(alph.charList_), intList_(alph.intList_)
  {
    for (size_t i = 0; i < alph.alphabet_.size(); ++i)
    {
      alphabet_.push_back(alph.alphabet_[i]->clone());
    }
  }

  AbstractAlphabet& operator=(const AbstractAlphabet& alph)
  {
    for (size_t i = 0; i < alphabet_.size(); ++i)
    {
      delete alphabet_[i];
    }

    for (size_t i = 0; i < alph.alphabet_.size(); ++i)
    {
      alphabet_.push_back(alph.alphabet_[i]->clone());
    }

    letters_  = alph.letters_;
    nums_     = alph.nums_;
    charList_ = alph.charList_;
    intList_  = alph.intList_;

    return *this;
  }

  virtual AbstractAlphabet* clone() const = 0;

  virtual ~AbstractAlphabet()
  {
    for (size_t i = 0; i < alphabet_.size(); ++i)
    {
      delete alphabet_[i];
    }
  }

public:
  /**
   * @name Implement these methods from the Alphabet interface.
   *
   * @{
   */
  size_t getNumberOfStates() const
  {
    return alphabet_.size();
  }
  unsigned int getNumberOfChars() const
  {
    return static_cast<unsigned int>(alphabet_.size());
  }
  std::string getName(const std::string& state) const;
  std::string getName(int state) const;
  int charToInt(const std::string& state) const;
  std::string intToChar(int state) const;
  bool isIntInAlphabet(int state) const;
  bool isCharInAlphabet(const std::string& state) const;
  bool isResolvedIn(int state1, int state2) const;
  std::vector<int> getAlias(int state) const;
  std::vector<std::string> getAlias(const std::string& state) const;
  int getGeneric(const std::vector<int>& states) const;
  std::string getGeneric(const std::vector<std::string>& states) const;
  const std::vector<int>& getSupportedInts() const;
  const std::vector<std::string>& getSupportedChars() const;
  const std::vector<std::string>& getResolvedChars() const;
  int getGapCharacterCode() const
  {
    return -1;
  }
  bool isGap(int state) const
  {
    return state == -1;
  }
  bool isGap(const std::string& state) const
  {
    return charToInt(state) == -1;
  }
  /** @} */

  /**
   * @name Specific methods to access AlphabetState
   * @{
   */
  /**
   * @brief Get a state at a position in the alphabet_ vector.
   *
   * This method must be overloaded in specialized classes to send back
   * a reference of the correct type.
   *
   * @param stateIndex The index of the state in the alphabet_ vector.
   * @throw IndexOutOfBoundsException If the index is invalid.
   */
  virtual AlphabetState& getStateAt(size_t stateIndex);

  /**
   * @brief Get a state at a position in the alphabet_ vector.
   *
   * This method must be overloaded in specialized classes to send back
   * a reference of the correct type.
   *
   * @param stateIndex The index of the state in the alphabet_ vector.
   * @throw IndexOutOfBoundsException If the index is invalid.
   */
  virtual const AlphabetState& getStateAt(size_t stateIndex) const;

  /**
   * @brief Get a state by its letter.
   *
   * This method must be overloaded in specialized classes to send back
   * a reference of the correct type.
   *
   * @param letter The letter of the state to find.
   * @throw BadCharException If the letter is not in the Alphabet.
   */
  const AlphabetState& getState(const std::string& letter) const;

  AlphabetState& getState(const std::string& letter);

  /**
   * @brief Get a state by its num.
   *
   * This method must be overloaded in specialized classes to send back
   * a reference of the correct type.
   *
   * @param num The num of the state to find.
   * @throw BadIntException If the num is not in the Alphabet.
   */
  const AlphabetState& getState(int num) const;

  AlphabetState& getState(int num);

  int getIntCodeAt(size_t stateIndex) const
  {
    return getStateAt(stateIndex).getNum();
  }

  const std::string& getCharCodeAt(size_t stateIndex) const
  {
    return getStateAt(stateIndex).getLetter();
  }

  size_t getStateIndex(int state) const;

  size_t getStateIndex(const std::string& state) const;
  /** @} */

protected:
  /**
   * @brief Add a state to the Alphabet.
   *
   * @param st The state to add.
   * @throw Exception If a wrong alphabet state is provided.
   */
  virtual void registerState(AlphabetState* st);

  /**
   * @brief Set a state in the Alphabet.
   *
   * @param pos The index of the state in the alphabet_ vector.
   * @param st The new state to put in the Alphabet.
   * @throw Exception If a wrong alphabet state is provided.
   * @throw IndexOutOfBoundsException If an incorrect index is provided.
   */
  virtual void setState(size_t pos, AlphabetState* st);

  /**
   * @brief Resize the private alphabet_ vector.
   *
   * @param size The new size of the Alphabet.
   */
  void resize(size_t size)
  {
    alphabet_.resize(size);
  }

  /**
   * @brief Re-update the maps using the alphabet_ vector content.
   */
  void remap()
  {
    letters_.clear();
    nums_.clear();
    for (size_t i = 0; i < alphabet_.size(); ++i)
    {
      updateMaps_(i, *alphabet_[i]);
    }
  }

  unsigned int getStateCodingSize() const
  {
    return 1;
  }

  bool equals(const Alphabet& alphabet) const
  {
    return getAlphabetType() == alphabet.getAlphabetType();
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_ABSTRACTALPHABET_H
