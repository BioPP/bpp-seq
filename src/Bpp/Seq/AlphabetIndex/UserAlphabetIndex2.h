//
// File: UserAlphabetIndex2.h
// Created by: Laurent Guéguen
// Created on: vendredi 29 mars 2013, à 13h 05
//

/*
   Copyright or © or Copr. CNRS, (November 17, 2004)

   This software is a computer program whose purpose is to provide classes
   for sequences analysis.

   This software is governed by the CeCILL  license under French law and
   abiding by the rules of distribution of free software.  You can  use,
   modify and/ or redistribute the software under the terms of the CeCILL
   license as circulated by CEA, CNRS and INRIA at the following URL
   "http://www.cecill.info".

   As a counterpart to the access to the source code and  rights to copy,
   modify and redistribute granted by the license, users are provided only
   with a limited warranty  and the software's author,  the holder of the
   economic rights,  and the successive licensors  have only  limited
   liability.

   In this respect, the user's attention is drawn to the risks associated
   with loading,  using,  modifying and/or developing or reproducing the
   software by the user in light of its specific status of free software,
   that may mean  that it is complicated to manipulate,  and  that  also
   therefore means  that it is reserved for developers  and  experienced
   professionals having in-depth computer knowledge. Users are therefore
   encouraged to load and test the software's suitability as regards their
   requirements in conditions enabling the security of their systems and/or
   data to be ensured and,  more generally, to use and operate it in the
   same conditions as regards security.

   The fact that you are presently reading this means that you have had
   knowledge of the CeCILL license and that you accept its terms.
 */

#ifndef _USER_ALPHABET_INDEX2_H_
#define _USER_ALPHABET_INDEX2_H_

#include "AlphabetIndex2.h"
#include <Bpp/Numeric/Matrix/MatrixTools.h>

// From the STL:
#include <vector>

namespace bpp
{
  /**
   * @brief Alphabet index given by user.
   */

  class UserAlphabetIndex2 :
  public virtual AlphabetIndex2
{
private:
  const Alphabet* alph_;
  LinearMatrix<double> index_;
  bool sym_;
  
public:
  UserAlphabetIndex2(const Alphabet* alph, const Matrix<double>& mat) :
  alph_(alph),
  index_(mat),
  sym_(false)
  {
    sym_=MatrixTools::isSymmetric(index_);
  }

  UserAlphabetIndex2(const UserAlphabetIndex2& uAlph) :
    alph_(uAlph.alph_),
    index_(uAlph.index_),
    sym_(uAlph.sym_) 
  {
  }

  UserAlphabetIndex2& operator=(const UserAlphabetIndex2& uAlph)
  {
    alph_  = uAlph.alph_;
    index_ = uAlph.index_;
    sym_ = uAlph.sym_;
    
    return *this;
  }    
  
  virtual ~UserAlphabetIndex2() {}

  UserAlphabetIndex2* clone() const { return new UserAlphabetIndex2(*this); }

public:
  double getIndex(int state1, int state2) const
  {
    return index_(alph_->getStateIndex(state1)-1,alph_->getStateIndex(state2)-1);
  }

  double getIndex(const std::string& state1, const std::string& state2) const
  {
    return index_(alph_->getStateIndex(state1)-1,alph_->getStateIndex(state2)-1);
  }

  void setIndexMatrix(const Matrix<double>& mat)
  {
    index_=mat;
    sym_=MatrixTools::isSymmetric(index_);
  }
  
  LinearMatrix<double>* getIndexMatrix() const { return new LinearMatrix<double>(index_); }

  const Alphabet* getAlphabet() const { return alph_; }

  /**
   * @return True if the index is symatric (that is, index(i,j) == index(j, i)).
   */
  virtual bool isSymmetric() const
  {
    return sym_;
  }
  };
} // end of namespace bpp.

#endif // _USERALPHABETINDEX1_H_

