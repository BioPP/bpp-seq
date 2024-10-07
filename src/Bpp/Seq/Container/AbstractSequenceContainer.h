// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_ABSTRACTSEQUENCECONTAINER_H
#define BPP_SEQ_CONTAINER_ABSTRACTSEQUENCECONTAINER_H

#include <Bpp/Exceptions.h>

#include "../Alphabet/Alphabet.h"
#include "../Sequence.h"
#include "../Commentable.h"
#include "SequenceContainer.h"

namespace bpp
{
/**
 * @brief Partial implementation of the SequenceContainer interface.
 *
 * This abstract class provides an alphabet and comments, with associated methods.
 */
template<class SequenceType, class HashType = std::string>
class AbstractTemplateSequenceContainer :
  public virtual TemplateSequenceContainerInterface<SequenceType, HashType>,
  public SimpleCommentable
{
protected:
  /**
   * @brief The container's alphabet.
   */
  std::shared_ptr<const Alphabet> alphabet_;

public:
  /**
   * @brief This constructor initializes the alphabet pointer.
   *
   * This constructor is to be called by constructors of derived classes.
   *
   * @param alphabet The alphabet to be associated to this container.
   */
  AbstractTemplateSequenceContainer(std::shared_ptr<const Alphabet> alphabet) :
    SimpleCommentable(), alphabet_(alphabet)
  {}

  /**
   * @brief This constructor initializes the alphabet pointer and the comments field.
   *
   * This constructor is to be called by constructors of derived classes.
   *
   * @param alphabet The alphabet to be associated to this container.
   * @param comments General comments to be associated to this container.
   */
  AbstractTemplateSequenceContainer(std::shared_ptr<const Alphabet> alphabet, const Comments& comments) :
    SimpleCommentable(comments),
    alphabet_(alphabet)
  {}

  AbstractTemplateSequenceContainer(const AbstractTemplateSequenceContainer<SequenceType, HashType>& sc) :
    SimpleCommentable(sc), alphabet_(sc.alphabet_)
  {}

  AbstractTemplateSequenceContainer& operator=(const AbstractTemplateSequenceContainer<SequenceType, HashType>& sc)
  {
    SimpleCommentable::operator=(sc);
    alphabet_ = sc.alphabet_;
    return *this;
  }

  /**
   * @brief Copy constructor from any SequenceContainer object.
   *
   * @param sc Another sequence container.
   */
  AbstractTemplateSequenceContainer(const TemplateSequenceContainerInterface<SequenceType, HashType>& sc) :
    SimpleCommentable(sc.getComments()),
    alphabet_(sc.getAlphabet())
  {}

  /**
   * @brief Assignation operator from any SequenceContainer object.
   *
   * @param sc Another sequence container.
   */
  AbstractTemplateSequenceContainer& operator=(const TemplateSequenceContainerInterface<SequenceType, HashType>& sc)
  {
    alphabet_ = sc.getAlphabet();
    setComments(sc.getComments());

    return *this;
  }

  virtual ~AbstractTemplateSequenceContainer()
  {}

public:
  const Alphabet& alphabet() const override
  {
    if (!alphabet_) throw Exception("AbstractSequenceContainer::alphabet() : no associated alphabet.");
    return *alphabet_;
  }

  std::shared_ptr<const Alphabet> getAlphabet() const override
  {
    return alphabet_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_ABSTRACTSEQUENCECONTAINER_H
