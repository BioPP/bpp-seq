//
// File: Commentable.h
// Authors:
//   Laurent Guéguen
// Created: lundi 15 mai 2017, Ã  00h 03
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

#ifndef BPP_SEQ_COMMENTABLE_H
#define BPP_SEQ_COMMENTABLE_H



// From the STL:
#include <string>
#include <iostream>

namespace bpp
{
/**
 * @brief Declaration of Comments type.
 *
 * Comments are defined as a std::vector of std::strings to allow the later creation of a
 * full Comments class.
 */
typedef std::vector<std::string> Comments;

class Commentable
{
public:
  Commentable() {}
  virtual ~Commentable() {}

  /**
   * @brief Get the comments.
   *
   * @return The comments.
   */
  virtual const Comments& getComments() const = 0;

  /**
   * @brief Set the comments.
   *
   * @param comments The new comments.
   */
  virtual void setComments(const Comments& comments) = 0;

  virtual void clearComments() = 0;
};

class SimpleCommentable:
  public virtual Commentable
{
protected:
  Comments comments_;

public:
  SimpleCommentable() :
    comments_()
  {}

  SimpleCommentable(const Comments& comments) :
    comments_(comments)
  {}

  SimpleCommentable(const SimpleCommentable& com) :
    comments_(com.comments_)
  {
  }

  SimpleCommentable& operator=(const SimpleCommentable& com)
  {
    comments_ = com.comments_;
    return *this;
  }

  virtual ~SimpleCommentable()
  {}

  /**
   * @brief Get the comments.
   *
   * @return The comments.
   */
  const Comments& getComments() const override { return comments_; }

  /**
   * @brief Set the comments.
   *
   * @param comments The new comments.
   */
  void setComments(const Comments& comments) override { comments_ = comments; }

  void clearComments() override { comments_.clear(); }
};

} // end of namespace bpp.
#endif // BPP_SEQ_COMMENTABLE_H
