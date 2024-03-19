// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_SIMPLEINDEXDISTANCE_H
#define BPP_SEQ_ALPHABETINDEX_SIMPLEINDEXDISTANCE_H


// from the STL:
#include <string>

#include "AlphabetIndex1.h"
#include "AlphabetIndex2.h"
#include "../Alphabet/AlphabetExceptions.h"
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Matrix/Matrix.h>

namespace bpp
{
/**
 * @brief Simple dissimilarity distance.
 *
 * Take a one-dimensional index and return the difference between the
 * indexes of two states.
 */

class SimpleIndexDistance :
  public virtual AlphabetIndex2
{
private:
  std::shared_ptr<AlphabetIndex1> index_;
  size_t size_;
  RowMatrix<double> indexMatrix_;
  bool sym_;

public:
  SimpleIndexDistance(std::shared_ptr<AlphabetIndex1> index) :
    index_(index),
    size_(index->getAlphabet()->getSize()),
    indexMatrix_(index->getAlphabet()->getSize(), index->getAlphabet()->getSize()),
    sym_(false)
  {
    computeIndexMatrix_();
  }

  SimpleIndexDistance(const SimpleIndexDistance& sid) :
    index_(sid.index_),
    size_(sid.size_),
    indexMatrix_(sid.indexMatrix_),
    sym_(sid.sym_)
  {}

  SimpleIndexDistance& operator=(const SimpleIndexDistance& sid)
  {
    index_ = sid.index_;
    size_ = sid.size_;
    indexMatrix_ = sid.indexMatrix_;
    sym_ = sid.sym_;
    return *this;
  }

  virtual ~SimpleIndexDistance() {}

public:
  double getIndex(int state1, int state2) const override
  {
    return indexMatrix_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
  }

  double getIndex(const std::string& state1, const std::string& state2) const override
  {
    return indexMatrix_(getAlphabet()->getStateIndex(state1) - 1, getAlphabet()->getStateIndex(state2) - 1);
  }

  std::shared_ptr<const Alphabet> getAlphabet() const override { return index_->getAlphabet(); }

  SimpleIndexDistance* clone() const override { return new SimpleIndexDistance(*this); }

  const Matrix<double>& getIndexMatrix() const override { return indexMatrix_; }

protected:
  void computeIndexMatrix_()
  {
    for (size_t i = 0; i < size_; ++i)
    {
      for (size_t j = 0; j < size_; ++j)
      {
        auto d = index_->getIndex(getAlphabet()->getIntCodeAt(i + 1)) - index_->getIndex(getAlphabet()->getIntCodeAt(j + 1));
        if (sym_) d = NumTools::abs<double>(d);
        indexMatrix_(i, j) = d;
      }
    }
  }

public:
  void setSymmetric(bool yn)
  {
    sym_ = yn;
    computeIndexMatrix_();
  }

  bool isSymmetric() const override { return sym_; }

  /**
   * @return The AlphabetIndex1 object associated to this object.
   */
  std::shared_ptr<const AlphabetIndex1> getAlphabetIndex1() const { return index_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_SIMPLEINDEXDISTANCE_H
