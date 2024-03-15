// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_ALPHABETSTATE_H
#define BPP_SEQ_ALPHABET_ALPHABETSTATE_H

#include <Bpp/Clonable.h>


// From the STL
#include <string>

namespace bpp
{
/**
 * @brief This is the base class to describe states in an Alphabet.
 *
 * @author Sylvain Gaillard
 */
class AlphabetState : public virtual Clonable
{
private:
  int num_;
  std::string letter_;
  std::string name_;

public:
  AlphabetState(int num, const std::string& letter, const std::string& name) : num_(num), letter_(letter), name_(name) {}

  // Class destructor
  virtual ~AlphabetState() {}

public:
  /**
   * @name The Clonable interface.
   * @{
   */
  AlphabetState* clone() const { return new AlphabetState(*this); }
  /** @} */

  /**
   * @brief Get the state's number.
   *
   * @return The state's number (i.e. -1 for gap (-)).
   */
  int getNum() const { return num_; }
  /**
   * @brief Set the state's number.
   *
   * @param num The state's number.
   */
  void setNum(int num) { num_ = num; }

  /**
   * @brief Get the letter(s) corresponding to the state.
   *
   * The letter is a string because it may more than one char
   * (for instance: codon).
   *
   * @return The state's letter.
   */
  const std::string& getLetter() const { return letter_; }
  /**
   * @brief Set the letter(s) of the state.
   *
   * @param letter The state's letter.
   */
  void setLetter(const std::string& letter) { letter_ = letter; }

  /**
   * @brief Get the name of the state.
   *
   * @return The full name of the state (i.e. Adenine).
   */
  const std::string& getName() const { return name_; }
  /**
   * @brief Set the name of the state.
   *
   * @param name The state's name
   */
  void setName(const std::string& name) { name_ = name; }

  /**
   * @brief operator ==
   *
   * Comparison is done on state num
   */
  bool operator==(AlphabetState& l2)
  {
    return getNum() == l2.getNum();
  }
};
}
#endif // BPP_SEQ_ALPHABET_ALPHABETSTATE_H
