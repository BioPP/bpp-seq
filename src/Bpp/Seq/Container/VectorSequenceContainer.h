//
// File: VectorSequenceContainer.h
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

#ifndef BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H
#define BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H

#include <Bpp/Exceptions.h>

#include "../Alphabet/Alphabet.h"
#include "../Sequence.h"
#include "AbstractSequenceContainer.h"
#include "VectorMappedContainer.h"

// From the STL:
#include <algorithm>
#include <vector>

namespace bpp
{
/**
 * @brief The VectorSequenceContainer class.
 *
 * This is the simplest implementation of the OrderedSequenceContainer interface.
 * std::shared_ptr<Sequence> are stored in a std::vector, as well as in a std::map.
 *
 * 
 * 
 */

class VectorSequenceContainer :
  virtual public AbstractSequenceContainer,
  virtual public VectorMappedContainer<Sequence>
{
public:

  /**
   * @brief Build a container with shared Sequences.
   *
   * The keys of the map are the names of the Sequences
   *
   */
  
  VectorSequenceContainer(
    const std::vector<std::shared_ptr<Sequence>>& vs, const Alphabet* alpha);

  /**
   * @brief Build an empty container that will contain sequences of a particular alphabet.
   *
   * @param alpha The alphabet of the container.
   */
  VectorSequenceContainer(const Alphabet* alpha) : AbstractSequenceContainer(alpha), VectorMappedContainer<Sequence>() {}

  /**
   * @name Copy contructors:
   *
   * @{
   */

  /**
   * @brief Copy from a VectorSequenceContainer.
   *
   * @param vsc The VectorSequenceContainer to copy into this container.
   *
   */
  
  VectorSequenceContainer(const VectorSequenceContainer& vsc);

  /**
   * @brief Copy from an OrderedSequenceContainer.
   *
   * @param osc The OrderedSequenceContainer to copy into this container.
   *
   * The keys of the map are the names of the Sequences
   *
   */

  VectorSequenceContainer(const OrderedSequenceContainer& osc);

  /**
   * @brief Copy from a SequenceContainer.
   *
   * @param osc The SequenceContainer to copy into this container.
   *
   * The keys of the map are the names of the Sequences
   *
   */

  VectorSequenceContainer(const SequenceContainer& osc);

  /** @} */

  /**
   * @brief Assign from a VectorSequenceContainer.
   *
   * @param vsc The VectorSequenceContainer to copy into this container.
   */
  VectorSequenceContainer& operator=(const VectorSequenceContainer& vsc);

  /**
   * @brief Copy from an OrderedSequenceContainer.
   *
   * @param osc The OrderedSequenceContainer to copy into this container.
   *
   * The keys of the map are the names of the Sequences
   *
   */
  
  VectorSequenceContainer& operator=(const OrderedSequenceContainer& osc);

  /**
   * @brief Copy from a SequenceContainer.
   *
   * @param osc The SequenceContainer to copy into this container.
   *
   * The keys of the map are the names of the Sequences
   *
   */
  
  VectorSequenceContainer& operator=(const SequenceContainer& osc);

  void clear()
  {
    VectorMappedContainer<Sequence>::clear();
  }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  VectorSequenceContainer* clone() const { return new VectorSequenceContainer(*this); }
  /** @} */

  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */

  /**
   * @brief check if there is a Sequence with this name in the map
   * (same as hasSequenceByKey).
   *
   */
  
  bool hasSequence(const std::string& name) const
  {
    return hasSequenceByKey(name);
  }

  bool hasSequenceByName(const std::string& name) const;

  /**
   * @brief check if there is a Sequence with this key in the map.
   *
   */
  
  bool hasSequenceByKey(const std::string& name) const
  {
    return hasObject(name);
  }

  /**
   * @brief get the Sequence with this name in the map (same as
   * getSequenceByKey).
   *
   */

  const Sequence& getSequence(const std::string& name) const
  {
    return getSequenceByKey(name);
  }

  const Sequence& getSequenceByName(const std::string& name) const;

  /**
   * @brief check if there is a Sequence with this key in the map.
   *
   */
  
  const Sequence& getSequenceByKey(const std::string& name) const
  {
    return *getObject(name);
  }

  /**
   * @brief Copy a Sequence to the given key in the map.
   *
   */
  
  void setSequence(const std::string& name, const Sequence& sequence, bool checkName = true)
  {
    setSequence(getSequencePosition(name), sequence, checkName);
  }

  /**
   * @brief get the Sequence with this name in the map (same as
   * removeSequenceByKey).
   *
   */

  std::shared_ptr<Sequence> removeSequence(const std::string& name)
  {
    return removeSequenceByKey(name);
  }

  /**
   * @brief remove & return a Sequence with this key from the map.
   *
   */
  
  std::shared_ptr<Sequence> removeSequenceByKey(const std::string& name)
  {
    return removeSequence(getSequencePosition(name));
  }

  /**
   * @brief remove & return a Sequence with this name.
   *
   */
  
  std::shared_ptr<Sequence> removeSequenceByName(const std::string& name);

  /**
   * @brief delete the Sequence with this name in the map (same as
   * deleteSequenceByKey).
   *
   */

  void deleteSequence(const std::string& name)
  {
    deleteSequenceByKey(name);
  }

  void deleteSequence(size_t pos)
  {
    deleteObject(pos);
  }

  /**
   * @brief remove & return a Sequence with this key from the map.
   *
   */
  
  void deleteSequenceByKey(const std::string& name)
  {
    deleteSequence(getSequencePosition(name));
  }

  /**
   * @brief remove & return a Sequence with this name.
   *
   */
  
  void deleteSequenceByName(const std::string& name);


  size_t getNumberOfSequences() const { return getSize(); }

  
  /**
   * @brief get Sequences proper names (may be different from the keys
   * used to store them in the map), in the order of the vector.
   *
   */
  
  std::vector<std::string> getSequencesNames() const;

  /**
   * @brief get Sequences keys (ie the strings used to store them in
   * the map, may be different from their proper names), in the order of the vector.
   *
   */
  
  std::vector<std::string> getSequencesKeys() const
  {
    return getObjectsNames();
  }

  /**
   * @brief set the proper names of the Sequences, in the order of the
   * vector.
   *
   */
  
  void setSequencesNames(const std::vector<std::string>& names, bool checkNames = true);

  VectorSequenceContainer* createEmptyContainer() const;

  /**
   * @brief Get the value of an element, given sequenceName in the map
   * and the elementIndex position.
   *
   */
  
  int& valueAt(const std::string& sequenceName, size_t elementIndex)
  {
    return getSequence_(sequenceName)[elementIndex];
  }

  /**
   * @brief Get the value at a Position in a Sequence with given
   * sequenceName in the map (may be not the actual name of the
   * Sequence..
   *
   */
  
  const int& valueAt(const std::string& key, size_t elementIndex) const
  {
    return getSequenceByKey(key)[elementIndex];
  }

  int& operator()(const std::string& sequenceName, size_t elementIndex)
  {
    return getSequence_(sequenceName)[elementIndex];
  }

  const int& operator()(const std::string& sequenceName, size_t elementIndex) const
  {
    return getSequence(sequenceName)[elementIndex];
  }

  int& valueAt(size_t sequenceIndex, size_t elementIndex)
  {
    return getSequence_(sequenceIndex)[elementIndex];
  }

  const int& valueAt(size_t sequenceIndex, size_t elementIndex) const
  {
    return getSequence(sequenceIndex)[elementIndex];
  }

  int& operator()(size_t sequenceIndex, size_t elementIndex)
  {
    return getSequence_(sequenceIndex)[elementIndex];
  }
  const int& operator()(size_t sequenceIndex, size_t elementIndex) const
  {
    return getSequence(sequenceIndex)[elementIndex];
  }
  /** @} */


  /**
   * @name The OrderedSequenceContainer interface.
   *
   * @{
   */

  
  size_t getSequencePosition(const std::string& name) const
  {
    return getObjectPosition(name);
  }

  const Sequence& getSequence(size_t sequenceIndex) const
  {
    return *getObject(sequenceIndex);
  }

  void  setSequence(size_t sequenceIndex, const Sequence& sequence, bool checkName = true)
  {
    if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", getAlphabet(), sequence.getAlphabet());

    addObject(std::shared_ptr<Sequence>(sequence.clone()), sequenceIndex, sequence.getName(), checkName);
  }

  std::shared_ptr<Sequence> removeSequence(size_t sequenceIndex)
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

  virtual void addSequence(const Sequence& sequence, bool checkName = true)
  {
    if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::addSequence : Alphabets don't match", getAlphabet(), sequence.getAlphabet());

    appendObject(std::shared_ptr<Sequence>(sequence.clone()), sequence.getName(), checkName);
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

  virtual void addSequence(const std::shared_ptr<Sequence> sequence, bool checkName = true)
  {
    if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::addSequence : Alphabets don't match", getAlphabet(), sequence->getAlphabet());

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
  virtual void addSequence(const Sequence& sequence, size_t sequenceIndex, bool checkName = true)
  {
    addObject(std::shared_ptr<Sequence>(sequence.clone()), sequenceIndex, sequence.getName(), checkName);
  }

protected:
  /**
   * @name AbstractSequenceContainer methods.
   *
   * @{
   */
  Sequence& getSequence_(size_t i)
  {
    return *getObject(i);
  }


  /**
   * @brief getSequence with given key
   *
   */
  
  Sequence& getSequence_(const std::string& key)
  {
    return *getObject(key);
  }

  /** @} */

  /**
   * @name SequencedValuesContainer methods.
   *
   * @{
   */

  /**
   * @brief get Value at given state with given key in the Sequence Map
   *
   */
  
  double getStateValueAt(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    return getSequence(sequenceName).getStateValueAt(siteIndex, state);
  }

  double operator()(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    return getSequence(sequenceName)(siteIndex, state);
  }

  /*
   *
   * @}
   *
   */

  /**
   * @name OrderedValuesContainer methods.
   *
   * @{
   */
  double getStateValueAt(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSequenceContainer::getStateValueAt.", sequenceIndex, 0, getNumberOfSequences() - 1);
    const Sequence& seq = getSequence(sequenceIndex);

    if (siteIndex >= seq.size())
      throw IndexOutOfBoundsException("VectorSequenceContainer::getStateValueAt.", siteIndex, 0, seq.size() - 1);

    return getAlphabet()->isResolvedIn(seq[siteIndex], state) ? 1. : 0.;
  }

  double operator()(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    return getAlphabet()->isResolvedIn(getSequence(sequenceIndex)[siteIndex], state) ? 1. : 0.;
  }

  /*
   *
   * @}
   *
   */
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H
