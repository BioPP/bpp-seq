// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_DEFAULTNUCLEOTIDESCORE_H
#define BPP_SEQ_ALPHABETINDEX_DEFAULTNUCLEOTIDESCORE_H


// from the STL:
#include <string>

#include "AlphabetIndex2.h"
#include "../Alphabet/NucleicAlphabet.h"
#include "../Alphabet/AlphabetExceptions.h"
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Matrix/Matrix.h>

namespace bpp
{
/**
 * @brief Default Substitution Matrix for nucleotide alignments.
 */
class DefaultNucleotideScore :
  public virtual AlphabetIndex2
{
private:
  LinearMatrix<double> distanceMatrix_;
  std::shared_ptr<const NucleicAlphabet> alpha_;

public:
  /**
   * @brief Build a new DefaultNucleotideScore object.
   *
   * @param alphabet The alphabet to use.
   */
  DefaultNucleotideScore(const NucleicAlphabet* alphabet);

  DefaultNucleotideScore(const DefaultNucleotideScore& dns) :
    distanceMatrix_(dns.distanceMatrix_),
    alpha_(dns.alpha_)
  {}

  DefaultNucleotideScore& operator=(const DefaultNucleotideScore& dns)
  {
    distanceMatrix_ = dns.distanceMatrix_;
    alpha_ = dns.alpha_;
    return *this;
  }

  virtual ~DefaultNucleotideScore()
  {}

public:
  /**
   * @name Methods from the AlphabetIndex2 interface.
   *
   * @{
   */
  /**
   * @copydoc bpp::AlphabetIndex2::getIndex()
   *
   * If states are unresolved, takes the best score of all possible matches
   * and divides it by the number of different states.
   * @author Sylvain Gaillard
   */
  double getIndex(int state1, int state2) const override;
  double getIndex(const std::string& state1, const std::string& state2) const override;
  std::shared_ptr<const Alphabet> getAlphabet() const override
  {
    return alpha_;
  }
  const Alphabet& alphabet() const override
  {
    return *alpha_;
  }
  DefaultNucleotideScore* clone() const override
  {
    return new DefaultNucleotideScore(*this);
  }
  const Matrix<double>& getIndexMatrix() const override;
  bool isSymmetric() const override
  {
    return true;
  }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_DEFAULTNUCLEOTIDESCORE_H
