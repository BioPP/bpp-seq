// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_BLOSUM50_H
#define BPP_SEQ_ALPHABETINDEX_BLOSUM50_H


// from the STL:
#include <string>

#include "AlphabetIndex2.h"
#include "../Alphabet/AlphabetExceptions.h"
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Matrix/Matrix.h>

namespace bpp
{
/**
 * @brief BLOSUM 50 Substitution Matrix.
 *
 * Reference:
 * Henikoff, S. and Henikoff, J.G.
 * Amino acid substitution matrices from protein blocks
 * Proc. Natl. Acad. Sci. USA 89, 10915-10919 (1992)
 *
 * Data from AAIndex2 database, Accession Number HENS920104.
 */
class BLOSUM50 :
  public ProteicAlphabetIndex2
{
private:
  LinearMatrix<double> distanceMatrix_;

public:
  BLOSUM50();

  BLOSUM50(const BLOSUM50& blosum) :
    ProteicAlphabetIndex2(blosum),
    distanceMatrix_(blosum.distanceMatrix_)
  {}

  BLOSUM50& operator=(const BLOSUM50& blosum)
  {
    ProteicAlphabetIndex2::operator=(*this);
    distanceMatrix_ = blosum.distanceMatrix_;
    return *this;
  }

  BLOSUM50* clone() const override
  {
    return new BLOSUM50();
  }

  virtual ~BLOSUM50()
  {}

public:
  /**
   * @name Methods from the AlphabetIndex2 interface.
   *
   * @{
   */
  double getIndex(int state1, int state2) const override;
  double getIndex(const std::string& state1, const std::string& state2) const override;
  const Matrix<double>& getIndexMatrix() const override;
  bool isSymmetric() const override
  {
    return true;
  }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_BLOSUM50_H
