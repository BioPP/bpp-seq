// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_SIMPLESCORE_H
#define BPP_SEQ_ALPHABETINDEX_SIMPLESCORE_H



// from the STL:
#include <string>

#include "AlphabetIndex2.h"
#include "../Alphabet/Alphabet.h"
#include "../Alphabet/AlphabetExceptions.h"
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Matrix/Matrix.h>

namespace bpp
{
/**
 * @brief Simple Substitution Matrix, with match and mismatch penalties.
 */
class SimpleScore :
  public virtual AlphabetIndex2
{
private:
  LinearMatrix<double> distanceMatrix_;
  std::shared_ptr<const Alphabet> alphabet_;

public:
  /**
   * @brief Build a new simpleScore object.
   *
   * @param alphabet The alphabet to use.
   * @param match Matching score.
   * @param mismatch Mismatching penalty.
   */
  SimpleScore(const Alphabet* alphabet, double match, double mismatch);

  SimpleScore(const SimpleScore& sc) :
    distanceMatrix_(sc.distanceMatrix_),
    alphabet_(sc.alphabet_)
  {}

  SimpleScore& operator=(const SimpleScore& sc)
  {
    distanceMatrix_ = sc.distanceMatrix_;
    alphabet_ = sc.alphabet_;
    return *this;
  }

  virtual ~SimpleScore() {}

  SimpleScore* clone() const override { return new SimpleScore(*this); }

public:
  /**
   * @name Methods from the AlphabetIndex2 interface.
   *
   * @{
   */
  double getIndex(int state1, int state2) const override;
  double getIndex(const std::string& state1, const std::string& state2) const override;
  std::shared_ptr<const Alphabet> getAlphabet() const override { return alphabet_; }
  const Matrix<double>& getIndexMatrix() const override { return distanceMatrix_; }
  bool isSymmetric() const override { return true; }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_SIMPLESCORE_H
