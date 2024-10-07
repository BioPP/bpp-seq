// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_AAINDEX2ENTRY_H
#define BPP_SEQ_ALPHABETINDEX_AAINDEX2ENTRY_H

#include <Bpp/Numeric/Matrix/Matrix.h>

#include "AlphabetIndex2.h"

namespace bpp
{
/**
 * @brief Create a AlphabetIndex2 object from an AAIndex2 entry.
 */
class AAIndex2Entry :
  public virtual ProteicAlphabetIndex2
{
private:
  LinearMatrix<double> property_;
  bool sym_;

public:
  /**
   * @brief Create a new AAIndex2Entry from an input stream.
   *
   * @param input The input stream to use.
   * @param sym Tell if the matrix is symmetric.
   * This option as an effect only if the matrix is specified as a triangle in the entry.
   * If sym==true, the other triangle will be built by symmetry.
   * If sym==false, the other triangle will be set to (-) the given triangle.
   * If the input matrix is square, it will be considered non-symetric.
   * @throw IOException if the stream content does not follow the AAIndex2 database entry format.
   */
  AAIndex2Entry(std::istream& input, bool sym = true);

  AAIndex2Entry(const AAIndex2Entry& index) :
    ProteicAlphabetIndex2(*this),
    property_(index.property_),
    sym_(index.sym_)
  {}

  AAIndex2Entry& operator=(const AAIndex2Entry& index)
  {
    ProteicAlphabetIndex2::operator=(*this);

    property_ = index.property_;
    sym_ = index.sym_;
    return *this;
  }

  virtual ~AAIndex2Entry()
  {}

public:
  AAIndex2Entry* clone() const override
  {
    return new AAIndex2Entry(*this);
  }

  double getIndex(int state1, int state2) const override
  {
    return property_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
  }

  double getIndex(const std::string& state1, const std::string& state2) const override
  {
    return property_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
  }

  const Matrix<double>& getIndexMatrix() const override
  {
    return property_;
  }

  bool isSymmetric() const override
  {
    return sym_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_AAINDEX2ENTRY_H
