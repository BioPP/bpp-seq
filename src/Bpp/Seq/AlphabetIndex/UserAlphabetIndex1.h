// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_USERALPHABETINDEX1_H
#define BPP_SEQ_ALPHABETINDEX_USERALPHABETINDEX1_H


#include "AlphabetIndex1.h"

// From the STL:
#include <vector>

namespace bpp
{
/**
 * @brief Alphabet index given by user.
 */
class UserAlphabetIndex1 :
  public virtual AlphabetIndex1
{
private:
  std::shared_ptr<const Alphabet> alph_;
  std::vector<double> index_;

public:
  UserAlphabetIndex1(std::shared_ptr<const Alphabet> alph) :
    alph_(alph),
    index_(alph->getSize(), 0)
  {}

  UserAlphabetIndex1(const UserAlphabetIndex1& uAlph) :
    alph_(uAlph.alph_),
    index_(uAlph.index_)
  {}

  UserAlphabetIndex1& operator=(const UserAlphabetIndex1& uAlph)
  {
    alph_  = uAlph.alph_;
    index_ = uAlph.index_;
    return *this;
  }

  virtual ~UserAlphabetIndex1() {}

  UserAlphabetIndex1* clone() const override { return new UserAlphabetIndex1(*this); }

public:
  double getIndex(int state) const
  {
    return index_[alph_->getStateIndex(state) - 1];
  }

  void setIndex(int state, double val)
  {
    index_[alph_->getStateIndex(state) - 1] = val;
  }

  double getIndex(const std::string& state) const
  {
    return index_[alph_->getStateIndex(state) - 1];
  }

  void setIndex(const std::string& state, double val)
  {
    index_[alph_->getStateIndex(state) - 1] = val;
  }

  const std::vector<double>& indexVector() const { return index_; }

  std::shared_ptr<const Alphabet> getAlphabet() const { return alph_; }
  
  const Alphabet& alphabet() const { return *alph_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_USERALPHABETINDEX1_H
