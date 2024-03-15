// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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
