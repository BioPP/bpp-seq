//
// File: AbstractSequenceContainer.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
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
 * @brief Partial implementation of the OrderedSequenceContainer interface.
 *
 * This abstract class provides an alphabet and comments, with associated methods.
 */
class AbstractSequenceContainer :
  public virtual SequenceContainer,
  public Commentable
{

protected:
  /**
   * @brief The container's alphabet.
   */
  const std::shared_ptr<const Alphabet> alphabet_;


public:
  /**
   * @brief This constructor initializes the alphabet pointer.
   *
   * This constructor is to be called by constructors of derived classes.
   *
   * @param alphabet The alphabet to be associated to this container.
   */
  AbstractSequenceContainer(std::shared_ptr<const Alphabet> alphabet) :
    Commentable(), alphabet_(alphabet)
  {}

  /**
   * @brief This constructor initializes the alphabet pointer and the comments field.
   *
   * This constructor is to be called by constructors of derived classes.
   *
   * @param alphabet The alphabet to be associated to this container.
   * @param comments General comments to be associated to this container.
   */
  AbstractSequenceContainer(std::shared_ptr<const Alphabet> alphabet, const Comments& comments) :
    Commentable(comments),
    alphabet_(alphabet)
  {}

  AbstractSequenceContainer(const AbstractSequenceContainer& sc) :
    Commentable(sc), alphabet_(sc.alphabet_)
  {}

  AbstractSequenceContainer& operator=(const AbstractSequenceContainer& sc)
  {
    Commentable::operator=(sc);
    alphabet_ = sc.alphabet_;
    return *this;
  }

  /**
   * @brief Copy constructor from any SequenceContainer object.
   *
   * @param sc Another sequence container.
   */
  AbstractSequenceContainer(const SequenceContainer& sc) :
    Commentable(sc.getGeneralComments()),
    alphabet_(sc.getAlphabet())
  {}

  /**
   * @brief Assignation operator from any SequenceContainer object.
   *
   * @param sc Another sequence container.
   */
  AbstractSequenceContainer& operator=(const SequenceContainer& sc)
  {
    alphabet_ = sc.getAlphabet();
    setGeneralComments(sc.getGeneralComments());

    return *this;
  }

  virtual ~AbstractSequenceContainer() {}

public:
  const std::shared_ptr<const Alphabet> getAlphabet() const { return alphabet_; }
  
  virtual const std::string& getKey(size_t sequenceIndex) const
  {
    return getSequence(sequenceIndex).getName();
  }

  virtual std::string toString(size_t sequenceIndex) const
  {
    return getSequence(sequenceIndex).toString();
  }

  virtual const Comments& getComments(size_t sequenceIndex) const
  {
    return getSequence(sequenceIndex).getComments();
  }

  virtual void setComments(size_t sequenceIndex, const Comments& comments) = 0;
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_ABSTRACTSEQUENCECONTAINER_H
