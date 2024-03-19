// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_MIYATAAACHEMICALDISTANCE_H
#define BPP_SEQ_ALPHABETINDEX_MIYATAAACHEMICALDISTANCE_H


// from the STL:
#include <string>

#include "AlphabetIndex2.h"
#include "../Alphabet/AlphabetExceptions.h"
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Matrix/Matrix.h>

namespace bpp
{
/**
 * @brief Miyata et al. (1979) Amino-Acid chemical distance.
 *
 * Two kinds of matrix can be built:
 * - a symmetric one, with \f$I_{i,j} = I_{i,j}\f$,
 * - or a non-symmetric one, with \f$I_{i,j} = -I_{i,j}\f$.
 *
 * Reference:
 * Miyata, T., Miyazawa, S. and Yasunaga, T.
 * Two types of amino acid substitutions in protein evolution
 * J. Mol. Evol. 12, 219-236 (1979)
 *
 * Data from AAIndex2 database, Accession Number MIYT790101.
 */
class MiyataAAChemicalDistance :
  public ProteicAlphabetIndex2
{
private:
  LinearMatrix<double> distanceMatrix_;
  LinearMatrix<double> indexMatrix_;
  bool sym_;

public:
  MiyataAAChemicalDistance();

  MiyataAAChemicalDistance(const MiyataAAChemicalDistance& md) :
    ProteicAlphabetIndex2(md),
    distanceMatrix_(md.distanceMatrix_),
    indexMatrix_(md.indexMatrix_),
    sym_(md.sym_)
  {}

  MiyataAAChemicalDistance& operator=(const MiyataAAChemicalDistance& md)
  {
    ProteicAlphabetIndex2::operator=(*this);

    distanceMatrix_ = md.distanceMatrix_;
    indexMatrix_ = md.indexMatrix_;
    sym_ = md.sym_;
    return *this;
  }

  virtual ~MiyataAAChemicalDistance() {}

  MiyataAAChemicalDistance* clone() const override { return new MiyataAAChemicalDistance(); }

public:
  /**
   * @name Methods from the AlphabetIndex2 interface.
   *
   * @{
   */
  double getIndex(int state1, int state2) const override;
  double getIndex(const std::string& state1, const std::string& state2) const override;
  const Matrix<double>& getIndexMatrix() const override { return indexMatrix_; }
  /** @} */

protected:
  void computeIndexMatrix_();

public:
  void setSymmetric(bool yn)
  {
    sym_ = yn;
    computeIndexMatrix_();
  }
  bool isSymmetric() const override { return sym_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_MIYATAAACHEMICALDISTANCE_H
