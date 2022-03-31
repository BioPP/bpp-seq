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

#include <Bpp/Clonable.h>

#include "../Alphabet/Alphabet.h"
#include "../Commentable.h"
#include "SequenceContainerExceptions.h"

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
class SequenceContainer :
  public virtual Clonable,
  public virtual Commentable
{
public:
  SequenceContainer() {}
  virtual ~SequenceContainer() {}

public:
  /**
   * @brief Get container's alphabet.
   *
   * @return The alphabet associated to this container.
   */
  virtual std::shared_ptr<const Alphabet> getAlphabet() const = 0;

  /**
   * @brief Get the number of sequences in the container.
   *
   * @return The number of sequences in the container.
   */
  virtual size_t getNumberOfSequences() const = 0;

  /**
   * @brief Get the value of a state at a given position
   *
   * @param siteIndex  index of the site
   * @param sequenceKey key of the sequence in the container
   * @param state  state in the alphabet
   */
  virtual double getStateValueAt(size_t siteIndex, const HashType& sequenceKey, int state) const = 0;

  virtual double operator()(size_t siteIndex, const HashType& sequenceKey, int state) const = 0;

  /**
   * @brief Element access function.
   *
   * Allows direct access to the data stored in the container.
   *
   * @param sequenceKey key of the sequence in the container
   * @param elementIndex The element position within the sequence.
   */
  //Note (Julien): I think this function is very dangerous, as the underlying sequence will not be notified of the change! Let's try to do without... 
  //virtual int& valueAt(const HashType& sequenceName, size_t elementIndex) = 0;

  /**
   * @brief Element access function.
   *
   * Allows direct access to the data stored in the container.
   *
   * @param sequenceKey key of the sequence in the container
   * @param elementIndex The element position within the sequence.
   */
  virtual const int& valueAt(const HashType& sequenceKey, size_t elementIndex) const = 0;

  /**
   * @brief Element access operator.
   *
   * Allows direct access to the data stored in the container.
   *
   * @param sequenceIndex The sequence position.
   * @param elementIndex The element position within the sequence.
   */
  virtual const int& valueAt(size_t sequenceIndex, size_t elementIndex) const = 0;


  /**
   * @brief Element access operator.
   *
   * Allows direct access to the data stored in the container.
   * This method is faster then the valueAt function, but input
   * parameters are not checked!
   *
   * @param sequenceKey key of the sequence in the container
   * @param elementIndex The element position within the sequence.
   */
  //Note (Julien): I think this function is very dangerous, as the underlying sequence will not be notified of the change! Let's try to do without... 
  //virtual int& operator()(const HashType& sequenceKey, size_t elementIndex) = 0;

  /**
   * @brief Element access operator.
   *
   * Allows direct access to the data stored in the container.
   * This method is faster then the valueAt function, but input
   * parameters are not checked!
   *
   * @param sequenceKey key of the sequence in the container
   * @param elementIndex The element position within the sequence.
   */
  virtual const int& operator()(const HashType& sequenceKey, size_t elementIndex) const = 0;

  /**
   * @brief Element access operator.
   *
   * Allows direct access to the data stored in the container.
   * This method is faster then the valueAt function, but input
   * parameters are not checked!
   *
   * @param sequenceIndex The sequence position.
   * @param elementIndex The element position within the sequence.
   */
  virtual const int& operator()(size_t sequenceIndex, size_t elementIndex) const = 0;
 
  /**
   * @brief Delete all data in the container.
   */
  virtual void clear() = 0;

  /**
   * @brief Return a copy of this container, but with no data inside.
   *
   * This method creates a new SequencedContainer objet.
   * The class of this container depends on the class implementing this interface.
   *
   * @return A new empty container, with the same alphabet as this one.
   */

  virtual std::shared_ptr< SequencedContainer<SequenceType, HashType> > createEmptyContainer() const = 0;








  /**
   * @name Access by key
   *
   * @{
   */

  /**
   * @brief Check if a certain key is associated to a sequence in the container.
   *
   * @param name The key to which the sequence is associated.
   * @return True if the key was found in the contained.
   */
  virtual bool hasSequence(const HashType& key) const = 0;

  /**
   * @brief Retrieve a sequence object from the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @return A reference toward the Sequence with corresponding name.
   */
  virtual const SequenceType& getSequence(const HashType& sequenceKey) const = 0;

  /**
   * @brief Replace a sequence in the container.
   *
   * If a sequence is found with the given key, it will be udated with the new one.
   * If no sequence with the given key is found, the new sequence will be added to the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @param sequence  The sequence to set.
   */
  virtual void setSequence(const HashType& sequenceKey, const SequenceType& sequence) = 0;

  /**
   * @brief Remove a sequence from the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   */
  virtual std::shared_ptr<SequenceType> removeSequence(const HashType& sequenceKey) = 0;

  /**
   * @brief Add a sequence to the container.
   *
   * Similar to setSequence, with the exception that an Exception will be raised in case of an existing key.
   * Existing sequences will not be updated, only new ones will be appended.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @param sequence  The sequence to add.
   */
  virtual void addSequence(const HashType& sequenceKey, const SequenceType& sequence) = 0;

  /** @} */





  /**
   * @name Access by position
   *
   * @{
   */
  
  /**
   * @brief Retrieve a sequence object from the container.
   *
   * @param sequenceIndex The position of the sequence.
   * @return A reference toward the Sequence object with corresponding name.
   */
  virtual const SequenceType& getSequence(size_t sequenceIndex) const = 0;

  /**
   * @brief Replace a sequence in the container.
   *
   * The underlying key may be updated, depending on the actual implementation of this interface.
   *
   * @param sequenceIndex The position of the sequence.
   * @param sequence      The sequence to add.
   */
  virtual void setSequence(size_t sequenceIndex, const SequenceType& sequence) = 0;

  /**
   * @brief Remove a sequence from the container.
   *
   * @param sequenceIndex The position of the sequence.
   */
  virtual std::shared_ptr<SequenceType> removeSequence(size_t sequenceIndex) = 0;

  /**@} */


  /**
   * @name Index management
   *
   * Position to key and key to position.
   *
   * @{
   */

  /**
   * @brief Get the key associated to a given sequence.
   *
   * @param sequenceIndex The position of the sequence in the container.
   * @return The key to which the sequence is associated.
   */
  virtual const HashType& getKey(size_t sequenceIndex) const = 0;

  /**
   * @brief Get the position of a sequence with a given key in the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @return The position of the sequence with name 'name', if it exists.
   */
  virtual size_t getSequencePosition(const HashType& sequenceKey) const = 0;

  /** @} */
  
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCECONTAINER_H
