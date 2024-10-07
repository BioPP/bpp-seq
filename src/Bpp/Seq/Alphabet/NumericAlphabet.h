// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_NUMERICALPHABET_H
#define BPP_SEQ_ALPHABET_NUMERICALPHABET_H

#include <Bpp/Numeric/Prob/UniformDiscreteDistribution.h>
#include <string>

#include "AbstractAlphabet.h"
#include "AlphabetNumericState.h"

/**
 * @brief This alphabet is used to deal NumericAlphabet
 *
 * From a UniformDiscreteDistribution, sets one AlphabetNumericState per catagory of the distribution,
 * with description & value the category value.
 *
 * An gap state is registered first, with value 0.
 */

namespace bpp
{
class NumericAlphabet : public AbstractAlphabet
{
private:
  const UniformDiscreteDistribution* pdd_;

  std::map<double, size_t> values_;

public:
  NumericAlphabet(const UniformDiscreteDistribution&);

  virtual ~NumericAlphabet()
  {
    delete pdd_;
  }

  NumericAlphabet(const NumericAlphabet&);

  NumericAlphabet& operator=(const NumericAlphabet&);

  NumericAlphabet* clone() const
  {
    return new NumericAlphabet(*this);
  }

public:
  void setState(size_t pos, AlphabetState* st);
  void registerState(AlphabetState* st);

  unsigned int getSize() const;
  unsigned int getNumberOfTypes() const;
  int getUnknownCharacterCode() const
  {
    return -1;
  }
  std::vector<int> getAlias(int state) const;
  std::vector<std::string> getAlias(const std::string& state) const;
  bool isUnresolved(int state) const;
  bool isUnresolved(const std::string& state) const;

  std::string getAlphabetType() const
  {
    return "Numeric";
  }

  AlphabetNumericState& getStateAt(size_t stateIndex);
  const AlphabetNumericState& getStateAt(size_t stateIndex) const;

  /**
   * @ brief Specific methods
   *
   */

  /**
   * @brief Returns the difference between successive values
   *
   */
  double getDelta() const;

  /**
   * @brief Returns the value for the character number
   *
   */
  double intToValue(int state) const;

  /**
   * @brief Returns the CategoryIndex of the category to which the value belongs.
   *
   */
  size_t getValueIndex(double value) const;

  /**
   * @brief Re-update the maps.
   */
  void remap();
};
}
#endif // BPP_SEQ_ALPHABET_NUMERICALPHABET_H
