//
// File: SequenceContainer.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
//   Laurent Guéguen
// Created: 2003-07-25 00:00:00
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

#ifndef BPP_SEQ_CONTAINER_SEQUENCECONTAINER_H
#define BPP_SEQ_CONTAINER_SEQUENCECONTAINER_H

#include "SequenceData.h"
#include "SequenceContainerExceptions.h"
#include "../Sequence.h"
#include "../ProbabilisticSequence.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief The SequenceContainer interface.
 *
 * This is the base interface of the container hierarchy.
 * Sequence objects are stored internally and indexed.
 * The interface implements two access methods:
 * - Per key,
 * - Per position in the container. 
 *
 * @see Sequence
 */
template<class SequenceType, class HashType = std::string>
class TemplateSequenceContainerInterface :
  public virtual TemplateSequenceDataInterface<HashType>
{
public:
  TemplateSequenceContainerInterface() {}
  virtual ~TemplateSequenceContainerInterface() {}

public:

  /**
   * @brief Return a copy of this container, but with no data inside.
   *
   * This method creates a new SequencedContainer objet.
   * The class of this container depends on the class implementing this interface.
   *
   * @return A new empty container, with the same alphabet as this one.
   */
  virtual TemplateSequenceContainerInterface<SequenceType, HashType>*
  createEmptyContainer() const override = 0;

  /**
   * @name Access by key
   *
   * @{
   */

  /**
   * @brief Retrieve a sequence object from the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @return A reference toward the Sequence with corresponding name.
   */
  virtual const SequenceType& sequence(const HashType& sequenceKey) const override = 0;

  /**
   * @brief Replace a sequence in the container.
   *
   * If a sequence is found with the given key, it will be udated with the new one.
   * If no sequence with the given key is found, the new sequence will be added to the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @param sequencePtr The sequence to set.
   */
  virtual void setSequence(const HashType& sequenceKey, std::unique_ptr<SequenceType>& sequencePtr) = 0;

  /**
   * @brief Add a sequence to the container.
   *
   * Similar to setSequence, with the exception that an Exception will be raised in case of an existing key.
   * Existing sequences will not be updated, only new ones will be appended.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @param sequencePtr The sequence to add.
   */
  virtual void addSequence(const HashType& sequenceKey, std::unique_ptr<SequenceType>& sequencePtr) = 0;

  /**
   * @brief Remove a sequence from the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   */
  virtual std::unique_ptr<SequenceType> removeSequence(const HashType& sequenceKey) = 0;

  /**
   * @brief Get the content of the dataset at a specific position (sequence key, site postion).
   *
   * @param sequenceKey key of the sequence in the container
   * @param sitePosition  index of the site
   * @return The element at the given position.
   */
  virtual const typename SequenceType::ElementType& valueAt(const HashType& sequenceKey, size_t sitePosition) const = 0;
  
  /**
   * @brief Get the content of the dataset at a specific position (sequence key, site postion).
   *
   * @param sequenceKey key of the sequence in the container
   * @param sitePosition  index of the site
   * @return The element at the given position.
   */
  virtual typename SequenceType::ElementType& valueAt(const HashType& sequenceKey, size_t sitePosition) = 0;

  /** @} */


  /**
   * @name Access by position
   *
   * @{
   */
  
  /**
   * @brief Retrieve a sequence object from the container.
   *
   * @param sequencePosition The position of the sequence.
   * @return A reference toward the Sequence object with corresponding name.
   */
  virtual const SequenceType& sequence(size_t sequencePosition) const override = 0;

  /**
   * @brief Replace a sequence in the container.
   *
   * The original key associated to the sequence will be kept.
   * @param sequencePosition The position of the sequence.
   * @param sequencePtr      The sequence to add.
   */
  virtual void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr) = 0;

  /**
   * @brief Replace a sequence in the container.
   *
   * The original key associated to the sequence will be kept.
   * @param sequencePosition The position of the sequence.
   * @param sequencePtr      The sequence to add.
   * @param sequenceKey      The new key of the sequence.
   */
  virtual void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr, const HashType& sequenceKey) = 0;

  /**
   * @brief Insert a sequence in the container.
   *
   * @param sequencePosition The position of the sequence.
   * @param sequencePtr      The sequence to add.
   * @param sequenceKey      The new key of the sequence.
   */
  virtual void insertSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr, const HashType& sequenceKey) = 0;

  /**
   * @brief Remove a sequence from the container.
   *
   * @param sequencePosition The position of the sequence.
   */
  virtual std::unique_ptr<SequenceType> removeSequence(size_t sequencePosition) = 0;

  /**
   * @brief Get the content of the dataset at a specific position (sequence position, site postion).
   *
   * @param sequencePosition index of the sequence in the container
   * @param sitePosition  index of the site
   * @return The element at the given position.
   */
  virtual const typename SequenceType::ElementType& valueAt(size_t sequencePosition, size_t sitePosition) const = 0;
 
  /**
   * @brief Get the content of the dataset at a specific position (sequence position, site postion).
   *
   * @param sequencePosition index of the sequence in the container
   * @param sitePosition  index of the site
   * @return The element at the given position.
   */
  virtual typename SequenceType::ElementType& valueAt(size_t sequencePosition, size_t sitePosition) = 0;
  /**@} */

};

//Aliases:
using SequenceContainerInterface = TemplateSequenceContainerInterface<Sequence>;
using ProbabilisticSequenceContainerInterface = TemplateSequenceContainerInterface<ProbabilisticSequence>;

template<class T>
class SwitchDeleter
{
private:
  bool doDelete_;

public:
  SwitchDeleter(): doDelete_(true) {}

public:
  void operator()(T* ptr) const {
    if (doDelete_) delete ptr;
  }

  void on() { doDelete_ = true; }
  void off() { doDelete_ = false; }

  bool isOn() const { return doDelete_; }
};



} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCECONTAINER_H
