// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_ALPHABETNUMERICSTATE_H
#define BPP_SEQ_ALPHABET_ALPHABETNUMERICSTATE_H


// From the STL
#include <string>

// From bpp-core
#include <Bpp/Clonable.h>

// From bpp-seq

#include "AlphabetState.h"

namespace bpp
{
/**
 * @brief States that have a double value in addition to letter & name
 *
 */

class AlphabetNumericState : public AlphabetState
{
private:
  double value_;

public:
  AlphabetNumericState(int num, double value, const std::string& letter, const std::string& name) : AlphabetState(num, letter, name), value_(value)
  {}

  // Class destructor
  virtual ~AlphabetNumericState()
  {}

public:
  /**
   * @name The Clonable interface.
   * @{
   */
  AlphabetNumericState* clone() const
  {
    return new AlphabetNumericState(*this);
  }
  /** @} */

  /**
   * @brief Get the state value.
   *
   * @return The state value
   */
  double getValue() const
  {
    return value_;
  }

  /**
   * @brief Set the state value.
   * @param value The given state value.
   */
  void setValue(double value)
  {
    value_ = value;
  }
};
}
#endif // BPP_SEQ_ALPHABET_ALPHABETNUMERICSTATE_H
