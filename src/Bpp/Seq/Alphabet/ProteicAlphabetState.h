// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_PROTEICALPHABETSTATE_H
#define BPP_SEQ_ALPHABET_PROTEICALPHABETSTATE_H


// From the STL
#include <string>

namespace bpp
{
/**
 * @brief This is the base class to describe states in a ProteicAlphabet.
 *
 * @author Sylvain Gaillard
 */
class ProteicAlphabetState : public AlphabetState
{
private:
  std::string abbr_;

public:
  ProteicAlphabetState(int num, const std::string& letter, const std::string& abbr, const std::string& name) : AlphabetState(num, letter, name), abbr_(abbr) {}

  // Class destructor
  virtual ~ProteicAlphabetState() {}

public:
  ProteicAlphabetState* clone() const
  {
    return new ProteicAlphabetState(*this);
  }
  /**
   * @brief Get the state's abbreviation.
   *
   * @return The state's abbreviation.
   */
  const std::string& getAbbreviation() const { return abbr_; }
  /**
   * @brief Set the state's abbreviation.
   *
   * @param abbr The state's abbreviation.
   */
  void setAbbreviation(const std::string& abbr) { abbr_ = abbr; }
};
}
#endif // BPP_SEQ_ALPHABET_PROTEICALPHABETSTATE_H
