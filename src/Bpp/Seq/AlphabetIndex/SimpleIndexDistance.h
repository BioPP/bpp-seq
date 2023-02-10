//
// File: SimpleIndexDistance.h
// Authors:
//   Julien Dutheil
// Created: 2005-04-21 00:00:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

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
 * Take a one-dimensional index end return the difference between the
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
    if (state1 < 0 || static_cast<unsigned int>(state1) >= size_)
      throw BadIntException(state1, "SimpleIndexDistance::getIndex(). Invalid state1.", getAlphabet().get());
    if (state2 < 0 || static_cast<unsigned int>(state2) >= size_)
      throw BadIntException(state2, "SimpleIndexDistance::getIndex(). Invalid state2.", getAlphabet().get());
    return indexMatrix_(static_cast<size_t>(state1), static_cast<size_t>(state2));
  }

  double getIndex(const std::string& state1, const std::string& state2) const override
  {
    return getIndex(getAlphabet()->charToInt(state1), getAlphabet()->charToInt(state2));
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
        indexMatrix_(i, j) = getIndex(getAlphabet()->getIntCodeAt(i), getAlphabet()->getIntCodeAt(j));
      }
    }
  }

public:
  void setSymmetric(bool yn) { 
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
