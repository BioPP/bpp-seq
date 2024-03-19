// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX2_H
#define BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX2_H

#include <Bpp/Clonable.h>
#include <Bpp/Numeric/Matrix/Matrix.h>

#include "../Alphabet/Alphabet.h"
#include "../Alphabet/AlphabetTools.h"
#include "../Alphabet/ProteicAlphabet.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief Two dimensionnal alphabet index interface.
 *
 * Derivatives of this interface implement distances between two states.
 */
class AlphabetIndex2 : public Clonable
{
public:
  AlphabetIndex2() {}
  virtual ~AlphabetIndex2() {}

public:
  virtual AlphabetIndex2* clone() const = 0;

  /**
   * @brief Get the index associated to a pair of states.
   *
   * @param state1 First state to consider, as a int value in the Alphabet
   * @param state2 Second state to consider, as a int value in the Alphabet
   * @return The index associated to the pair of states.
   */

  virtual double getIndex(int state1, int state2) const = 0;

  /**
   * @brief Get the index associated to a pair of states.
   *
   * @param state1 First state to consider, as a string value.
   * @param state2 Second state to consider, as a string value.
   * @return The index associated to the pair of states.
   */

  virtual double getIndex(const std::string& state1, const std::string& state2) const = 0;

  /**
   * @brief Get the alphabet associated to this index.
   *
   * @return Alphabet The alphabet associated to this index.
   */
  virtual std::shared_ptr<const Alphabet> getAlphabet() const = 0;

  /**
   * @return A matrix object with all indices.
   */
  virtual const Matrix<double>& getIndexMatrix() const = 0;

  /**
   * @return True if the index is symatric (that is, index(i,j) == index(j, i)).
   */
  virtual bool isSymmetric() const = 0;
};

/**
 * @brief Virtual AlphabetIndex2 for proteic alphabet
 */
class ProteicAlphabetIndex2 :
  virtual public AlphabetIndex2
{
private:
  std::shared_ptr<const ProteicAlphabet> alpha_;

public:
  ProteicAlphabetIndex2() : alpha_(AlphabetTools::PROTEIN_ALPHABET) {}
  virtual ~ProteicAlphabetIndex2() {}

  virtual ProteicAlphabetIndex2* clone() const override = 0;

  ProteicAlphabetIndex2(const ProteicAlphabetIndex2& pt) :
    alpha_(pt.alpha_)
  {}

  ProteicAlphabetIndex2& operator=(const ProteicAlphabetIndex2& pt)
  {
    alpha_ = pt.alpha_;
    return *this;
  }

  std::shared_ptr<const Alphabet> getAlphabet() const override { return alpha_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX2_H
