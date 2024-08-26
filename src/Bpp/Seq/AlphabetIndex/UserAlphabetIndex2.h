// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_USERALPHABETINDEX2_H
#define BPP_SEQ_ALPHABETINDEX_USERALPHABETINDEX2_H

#include <Bpp/Numeric/Matrix/MatrixTools.h>

#include "AlphabetIndex2.h"

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
std::shared_ptr<const Alphabet> alph_;
LinearMatrix<double> index_;
bool sym_;

public:
UserAlphabetIndex2(std::shared_ptr<const Alphabet> alph, const Matrix<double>& mat) :
	alph_(alph),
	index_(mat),
	sym_(false)
{
	sym_ = MatrixTools::isSymmetric(index_);
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

virtual ~UserAlphabetIndex2() {
}

UserAlphabetIndex2* clone() const {
	return new UserAlphabetIndex2(*this);
}

public:
double getIndex(int state1, int state2) const
{
	return index_(alph_->getStateIndex(state1) - 1, alph_->getStateIndex(state2) - 1);
}

double getIndex(const std::string& state1, const std::string& state2) const
{
	return index_(alph_->getStateIndex(state1) - 1, alph_->getStateIndex(state2) - 1);
}

void setIndexMatrix(const Matrix<double>& mat)
{
	index_ = mat;
	sym_ = MatrixTools::isSymmetric(index_);
}

LinearMatrix<double>* getIndexMatrix() const {
	return new LinearMatrix<double>(index_);
}

std::shared_ptr<const Alphabet> getAlphabet() const {
	return alph_;
}

const Alphabet& alphabet() const {
	return *alph_;
}

/**
 * @return True if the index is symatric (that is, index(i,j) == index(j, i)).
 */
virtual bool isSymmetric() const
{
	return sym_;
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_USERALPHABETINDEX2_H
