//
// File: VectorProbabilisticSequenceContainer.h
// Authors:
//   Laurent Guéguen
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

#ifndef BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H
#define BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H

#include <Bpp/Exceptions.h>

#include "../Alphabet/Alphabet.h"
#include "../ProbabilisticSequence.h"
#include "AbstractValuesContainer.h"
#include "VectorMappedContainer.h"

// From the STL:
#include <algorithm>
#include <vector>

namespace bpp
{
/**
 * @brief The VectorProbabilisticSequenceContainer class.
 *
 * This is the simplest implementation of the OrderedValuesContainer interface for ProbabilisticSequences
 * ProbabilisticSequences are stored in a std::vector of shared pointers.
 */

class VectorProbabilisticSequenceContainer :
    public AbstractValuesContainer,
    virtual public VectorMappedContainer<ProbabilisticSequence>
{
public:

  /**
   * @brief Build a container with shared Sequences.
   *
   */
  
  VectorProbabilisticSequenceContainer(
    const std::vector<std::shared_ptr<ProbabilisticSequence>>& vs, const Alphabet* alpha);

  /**
   * @brief Build an empty container that will contain sequences of a particular alphabet.
   *
   * @param alpha The alphabet of the container.
   */
  VectorProbabilisticSequenceContainer(const Alphabet* alpha) : VectorMappedContainer<ProbabilisticSequence>(), AbstractValuesContainer(alpha) {}

  /**
   * @name Copy contructors:
   *
   * @{
   */

  /**
   * @brief Copy from a VectorProbabilisticSequenceContainer.
   *
   * @param vsc The VectorProbabilisticSequenceContainer to copy into this container.
   */
  VectorProbabilisticSequenceContainer(const VectorProbabilisticSequenceContainer& vsc);

  /** @} */

  /**
   * @brief Assign from a VectorProbabilisticSequenceContainer.
   *
   * @param vsc The VectorProbabilisticSequenceContainer to copy into this container.
   */
  VectorProbabilisticSequenceContainer& operator=(const VectorProbabilisticSequenceContainer& vsc);


  void clear()
  {
    VectorMappedContainer<ProbabilisticSequence>::clear();
  }

public:

  std::string toString(const std::string& name) const
  {
    return getSequence(name).toString();
  }

  std::string toString(size_t sequenceIndex) const
  {
    return getSequence(sequenceIndex).toString();
  }

  const Comments& getComments(const std::string& name) const
  {
    return getSequence(name).getComments();
  }

  virtual const Comments& getComments(size_t sequenceIndex) const
  {
    return getSequence(sequenceIndex).getComments();
  }

  void setComments(const std::string& name, const Comments& comments)
  {
    size_t pos = getSequencePosition(name);
    setComments(pos, comments);
  }

  const std::string& getName(size_t sequenceIndex) const
  {
    return getSequence(sequenceIndex).getName();
  }

  size_t getSequencePosition(const std::string& name) const
  {
    return getObjectPosition(name);
  }

  /**
   * @name The Clonable interface.
   *
   * @{
   */
  VectorProbabilisticSequenceContainer* clone() const
  {
    return new VectorProbabilisticSequenceContainer(*this);
  }

  /** @} */

  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */
  bool hasSequence(const std::string& name) const
  {
    return hasObject(name);
  }

  const ProbabilisticSequence& getSequence(const std::string& name) const
  {
    return *getObject(name);
  }

  void setSequence(const std::string& name, const ProbabilisticSequence& sequence, bool checkName = true)
  {
    setSequence(getSequencePosition(name), sequence, checkName);
  }

  std::shared_ptr<ProbabilisticSequence> removeSequence(const std::string& name)
  {
    return removeSequence(getSequencePosition(name));
  }

  size_t getNumberOfSequences() const { return getSize(); }

  std::vector<std::string> getSequenceNames() const
  {
    return getObjectNames();
  }

  void setSequenceNames(const std::vector<std::string>& names, bool checkNames = true);

  VectorProbabilisticSequenceContainer* createEmptyContainer() const;

  /** @} */


  const ProbabilisticSequence& getSequence(size_t sequenceIndex) const
  {
    return *getObject(sequenceIndex);
  }

  void  setSequence(size_t sequenceIndex, const ProbabilisticSequence& sequence, bool checkName = true)
  {
    if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorProbabilisticSequenceContainer::setSequence : Alphabets don't match", getAlphabet(), sequence.getAlphabet());

    addObject(std::shared_ptr<ProbabilisticSequence>(sequence.clone()), sequenceIndex, sequence.getName(), checkName);
  }

  std::shared_ptr<ProbabilisticSequence> removeSequence(size_t sequenceIndex)
  {
    return removeObject(sequenceIndex);
  }

  void setComments(size_t sequenceIndex, const Comments& comments)
  {
    getSequence_(sequenceIndex).setComments(comments);
  }

  /** @} */

  /**
   * @name Add sequence to this container.
   *
   * @{
   */

  /**
   * @brief Add a sequence at the end of the container.
   *
   * The sequence is copied into the container.
   * If checkNames is set to true, the method check if the name of the
   * sequence is already used in the container, and sends an exception if it
   * is the case. Otherwise, do not check the name: the method is hence faster,
   * but use it at your own risks!
   *
   * @param sequence The sequence to add.
   * @param checkName Tell if the method must check the name of the sequence
   * before adding it.
   */

  virtual void addSequence(const ProbabilisticSequence& sequence, bool checkName = true)
  {
    if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorProbabilisticSequenceContainer::addSequence : Alphabets don't match", getAlphabet(), sequence.getAlphabet());

    appendObject(std::shared_ptr<ProbabilisticSequence>(sequence.clone()), sequence.getName(), checkName);
  }

  /**
  * @brief Add a sequence at the end of the container.
  *
  * The sequence is shared with the container.
  * If checkNames is set to true, the method check if the name of the
  * sequence is already used in the container, and sends an exception if it
  * is the case. Otherwise, do not check the name: the method is hence faster,
  * but use it at your own risks!
  *
  * @param sequence The sequence to add.
  * @param checkName Tell if the method must check the name of the sequence
  * before adding it.
  * @throw Exception If the sequence couldn't be added to the container.
  */

  virtual void addSequence(const std::shared_ptr<ProbabilisticSequence> sequence, bool checkName = true)
  {
    if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorProbabilisticSequenceContainer::addSequence : Alphabets don't match", getAlphabet(), sequence->getAlphabet());

    appendObject(sequence, sequence->getName(), checkName);
  }

  /**
   * @brief Add a sequence to the container at a particular position.
   *
   * The sequence is copied into the container.
   * If checkName is set to true, the method check if the name of the
   * sequence is already used in the container, and sends an exception if it
   * is the case. Otherwise, do not check the name: the method is hence faster,
   * but use it at your own risks!
   *
   * @param sequence The sequence to add.
   * @param sequenceIndex The position where to insert the new sequence.
   * All the following sequences will be pushed.
   * @param checkName Tell if the method must check the name of the sequence
   * before adding it.
   * @throw Exception If the sequence couldn't be added to the container.
   */
  virtual void addSequence(const ProbabilisticSequence& sequence, size_t sequenceIndex, bool checkName = true)
  {
    addObject(std::shared_ptr<ProbabilisticSequence>(sequence.clone()), sequenceIndex, sequence.getName(), checkName);
  }

protected:
  /**
   * @name AbstractSequenceContainer methods.
   *
   * @{
   */
  ProbabilisticSequence& getSequence_(size_t i)
  {
    return *getObject(i);
  }


  ProbabilisticSequence& getSequence_(const std::string& name)
  {
    return *getObject(name);
  }

  double getStateValueAt(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    return getSequence(sequenceName).getStateValueAt(siteIndex, state);
  }

  double operator()(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    return getSequence(sequenceName)(siteIndex, state);
  }

  double getStateValueAt(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorProbabilisticSequenceContainer::getStateValueAt.", sequenceIndex, 0, getNumberOfSequences() - 1);
    const ProbabilisticSequence& seq = getSequence(sequenceIndex);

    if (siteIndex >= seq.size())
      throw IndexOutOfBoundsException("VectorProbabilisticSequenceContainer::getStateValueAt.", siteIndex, 0, seq.size() - 1);

    return (getSequence(sequenceIndex))[siteIndex][(size_t)state];
  }

  double operator()(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    return (getSequence(sequenceIndex))[siteIndex][(size_t)state];
  }

};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H
