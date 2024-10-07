// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_NUCLEICALPHABET_H
#define BPP_SEQ_ALPHABET_NUCLEICALPHABET_H

#include <iostream>
#include <map>
#include <typeinfo>

#include "LetterAlphabet.h"
#include "NucleicAlphabetState.h"

namespace bpp
{
/**
 * @brief The abstract base class for nucleic alphabets.
 *
 * This class only implements a few methods, it is mainly designed for methods/classes
 * that will require to work with both RNA and DNA.
 */
class NucleicAlphabet :
  public LetterAlphabet
{
private:
  std::map<int, size_t> binCodes_;
  void updateBinMaps_(size_t pos, const NucleicAlphabetState& st)
  {
    if (binCodes_.find(st.getBinaryCode()) == binCodes_.end())
      binCodes_[st.getBinaryCode()] = pos;
  }

public:
  NucleicAlphabet() : LetterAlphabet(), binCodes_()
  {}

  NucleicAlphabet(const NucleicAlphabet& bia) : LetterAlphabet(bia), binCodes_(bia.binCodes_)
  {}

  NucleicAlphabet& operator=(const NucleicAlphabet& bia)
  {
    LetterAlphabet::operator=(bia);
    binCodes_ = bia.binCodes_;
    return *this;
  }

  virtual NucleicAlphabet* clone() const = 0;

  virtual ~NucleicAlphabet()
  {}

protected:
  /**
   * @name Overloaded methods from AbstractAlphabet
   * @{
   */
  void registerState(AlphabetState* st)
  {
    NucleicAlphabetState* nst = dynamic_cast<NucleicAlphabetState*>(st);
    if (!nst)
      throw Exception("NucleicAlphabet::registerState. Incorrect alphabet type.");
    LetterAlphabet::registerState(nst);
    updateBinMaps_(getNumberOfChars(), *nst);
  }

  void setState(size_t pos, AlphabetState* st)
  {
    NucleicAlphabetState* nst = dynamic_cast<NucleicAlphabetState*>(st);
    if (!nst)
      throw Exception("NucleicAlphabet::setState. Incorrect alphabet type.");
    LetterAlphabet::setState(pos, nst);
    updateBinMaps_(pos, *nst);
  }

  /** @} */

public:
  /**
   * @name Overloaded methods from AbstractAlphabet
   * @{
   */
  const NucleicAlphabetState& getStateAt(size_t stateIndex) const
  {
    return dynamic_cast<const NucleicAlphabetState&>(
      AbstractAlphabet::getStateAt(stateIndex)
      );
  }
  NucleicAlphabetState& getStateAt(size_t stateIndex)
  {
    return dynamic_cast<NucleicAlphabetState&>(
      AbstractAlphabet::getStateAt(stateIndex)
      );
  }
  const NucleicAlphabetState& getState(const std::string& letter) const
  {
    return dynamic_cast<const NucleicAlphabetState&>(
      AbstractAlphabet::getState(letter)
      );
  }
  const NucleicAlphabetState& getState(int num) const
  {
    return dynamic_cast<const NucleicAlphabetState&>(
      AbstractAlphabet::getState(num)
      );
  }
  /** @} */

  /**
   * @name Specific methods
   * @{
   */

  /**
   * @brief Get a state by its binary representation.
   *
   * @param code The binary representation as an unsigned char.
   * @return The NucleicAlphabetState.
   * @throw BadIntException If the code is not a valid state.
   * @author Sylvain Gaillard
   */
  const NucleicAlphabetState& getStateByBinCode(int code) const
  {
    std::map<int, size_t>::const_iterator it = binCodes_.find(code);
    if (it == binCodes_.end())
      throw BadIntException(code, "NucleicAlphabet::getState(unsigned char): Binary code not in alphabet", this);
    return getStateAt(it->second);
  }

  /**
   * @brief Subtract states
   *
   * Get the remaining state when subtracting one state to another.
   *
   * @code
   * int a = alpha->charToInt("A");
   * int m = alpha->charToInt("M");
   * int c = alpha->subtract(m, a);
   *
   * cout << alpha->intToChar(c) << endl;
   *
   * // should print C because M - A = C
   * @endcode
   *
   * @param s1 the first state as an int
   * @param s2 the second state as an int
   * @throw BadIntException if one of the states is not valid.
   * @return The remaining state as an int
   * @author Sylvain Gaillard
   */
  int subtract(int s1, int s2) const
  {
    return getStateByBinCode(getState(s1).getBinaryCode() & ~getState(s2).getBinaryCode()).getNum();
  }

  /**
   * @brief Subtract states
   *
   * Get the remaining state when subtracting one state to another.
   *
   * @code
   * string a = "A";
   * string m = "M";
   *
   * cout << alpha->subtract(m, a) << endl;
   *
   * // should print C because M - A = C
   * @endcode
   *
   * @param s1 the first state as a string
   * @param s2 the second state as a string
   * @throw BadCharException if one of the states is not valid.
   * @return The remaining state as a string
   * @author Sylvain Gaillard
   */
  std::string subtract(const std::string& s1, const std::string& s2) const
  {
    return intToChar(subtract(charToInt(s1), charToInt(s2)));
  }

  /**
   * @brief Get the overlap between to states
   *
   * Get the overlapping states between two steps.
   *
   * @code
   * int m = alpha->charToInt("M");
   * int r = alpha->charToInt("R");
   * int a = alpha->getOverlap(m, r);
   *
   * cout << alpha->intToChar(a) << endl;
   *
   * // should print A because M = A/C and R = A/G
   * @endcode
   *
   * @param s1 the first state as an int
   * @param s2 the second state as an int
   * @throw BadIntException if one of the states is not valid
   * @return The overlapping state
   * @author Sylvain Gaillard
   */
  int getOverlap(int s1, int s2) const
  {
    return getStateByBinCode(getState(s1).getBinaryCode() & getState(s2).getBinaryCode()).getNum();
  }

  /**
   * @brief Get the overlap between to states
   *
   * Get the overlapping states between two steps.
   *
   * @code
   * string m = "M";
   * string r = R;
   *
   * cout << alpha->getOverlap(m, r) << endl;
   *
   * // should print A because M = A/C and R = A/G
   * @endcode
   *
   * @param s1 the first state as a string
   * @param s2 the second state as a string
   * @throw BadCharException if one of the states is not valid
   * @return The overlapping state
   * @author Sylvain Gaillard
   */
  std::string getOverlap(const std::string& s1, const std::string& s2) const
  {
    return intToChar(getOverlap(charToInt(s1), charToInt(s2)));
  }

  /** @} */

public:
  // return 4 : A, C, G, T (or U)
  unsigned int getSize() const
  {
    return 4;
  }

  // return 15 : gap isn't included, generic unresolved bases (N, X, ?, O, 0) count for one
  unsigned int getNumberOfTypes() const
  {
    return 15;
  }

  int getUnknownCharacterCode() const
  {
    return 14;
  }

  bool isUnresolved(int state) const
  {
    return state > 3;
  }
  bool isUnresolved(const std::string& state) const
  {
    return charToInt(state) > 3;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_NUCLEICALPHABET_H
