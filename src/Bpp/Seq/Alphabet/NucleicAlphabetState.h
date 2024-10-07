// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_NUCLEICALPHABETSTATE_H
#define BPP_SEQ_ALPHABET_NUCLEICALPHABETSTATE_H


// From the STL
#include <string>
#include <bitset>

namespace bpp
{
/**
 * @brief This is the base class to describe states in a NucleicAlphabet.
 *
 * This class store a binary code for each state.
 * This binary code is of length 4, one bit per nucleotid.
 * For DNA alphabet, this code looks like this:
 * <table>
 * <tr><th>Letter</th><th>Binary code</th><th>int value</th></tr>
 * <tr><td>-</td><td>0000</td><td>0</td></tr>
 * <tr><td>A</td><td>0001</td><td>1</td></tr>
 * <tr><td>C</td><td>0010</td><td>2</td></tr>
 * <tr><td>G</td><td>0100</td><td>4</td></tr>
 * <tr><td>T</td><td>1000</td><td>8</td></tr>
 * <tr><td>N</td><td>1111</td><td>15</td></tr>
 * <tr><td>...</td><td></td><td></td></tr>
 * <tr><td>M</td><td>0011</td><td>3</td></tr>
 * <tr><td>W</td><td>1001</td><td>9</td></tr>
 * <tr><td>...</td><td></td><td></td></tr>
 * <tr><td>V</td><td>0111</td><td>7</td></tr>
 * </table>
 * This notation allows the use of bitwize operations like:
 *   - build a generic state from to states
 * @code
 * A | G = R  <=>  0001 | 0100 = 0101
 * @endcode
 *   - extract a state from an unresolved one by subtraction of an other state
 * @code
 * S & ~ C = G  <=>  0110 & ~ 0010 = 0100
 * @endcode
 *
 * The binary code is stored as a char because it's the smallest memory word
 * that can be allocated. A char is 8 bits long allowing the use of this
 * class with Alphabet of at least 8 resolved states (enough for known
 * nucleic alphabets!).
 *
 * @author Sylvain Gaillard
 */
class NucleicAlphabetState : public AlphabetState
{
private:
  int binCode_;

public:
  NucleicAlphabetState(int num, const std::string& letter, unsigned char code, const std::string& name) :
    AlphabetState(num, letter, name), binCode_(code)
  {}

  // Class destructor
  virtual ~NucleicAlphabetState()
  {}

public:
  NucleicAlphabetState* clone() const
  {
    return new NucleicAlphabetState(*this);
  }
  /**
   * @brief Get the state's binary representation.
   *
   * @return The state's binary representation.
   */
  int getBinaryCode() const
  {
    return binCode_;
  }
  /**
   * @brief Set the state's binary representation.
   *
   * @param code The state's binary representation.
   */
  void setBinaryCode(int code)
  {
    binCode_ = code;
  }
};
}
#endif // BPP_SEQ_ALPHABET_NUCLEICALPHABETSTATE_H
