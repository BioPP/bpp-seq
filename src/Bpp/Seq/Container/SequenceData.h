//
// File: SequenceData.h
// Authors:
//   Laurent Guéguen
// Created: mardi 30 mai 2017, Ã  21h 25
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

#ifndef BPP_SEQ_CONTAINER_SEQUENCEDATA_H
#define BPP_SEQ_CONTAINER_SEQUENCEDATA_H

#include <Bpp/Clonable.h>

#include "../Alphabet/Alphabet.h"
#include "../Commentable.h"
#include "../CoreSequence.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief The SequenceData interface.
 *
 * This interface is the most general one in the container hierarchy.
 * No assumption is made on the data in the container (no ordering, no
 * alignment, no type).
 *
 */
template<class HashType = std::string>
class SequenceDataInterface :
  public virtual Clonable,
  public virtual Commentable
{
public:
  SequenceDataInterface() {}
  virtual ~SequenceDataInterface() {}

  SequenceDataInterface<HashType>* clone() const override = 0;

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
   * @return a vector with all sequence keys.
   */
  virtual std::vector<HashType> getSequenceKeys() const = 0;

  /**
   * @brief Reset all sequence keys.
   */
  virtual void setSequenceKeys(const std::vector<HashType>& sequenceKeys) = 0;
 
  /**
   * @brief Get the value of a state at a given position
   *
   * @param sitePosition  index of the site
   * @param sequenceKey key of the sequence in the container
   * @param state  state in the alphabet
   */
  virtual double getStateValueAt(size_t sitePosition, const HashType& sequenceKey, int state) const = 0;

  /**
   * @brief Get the value of a state at a given position
   *
   * Same as getValueAt.
   * 
   * @param sitePosition  index of the site
   * @param sequenceKey key of the sequence in the container
   * @param state state in the alphabet
   */
  virtual double operator()(size_t sitePosition, const HashType& sequenceKey, int state) const = 0;

  /**
   * @brief Get value of a state at a given position
   *
   * @param sitePosition  index of the site
   * @param sequencePosition index of the sequence in the container
   * @param state state in the alphabet
   */
  virtual double getStateValueAt(size_t sitePosition, size_t sequencePosition, int state) const = 0;

  /**
   * @brief Get the value of a state at a given position
   *
   * Same as getValueAt.
   * 
   * @param sitePosition  index of the site
   * @param sequencePosition index of the sequence in the container
   * @param state state in the alphabet
   */
  virtual double operator()(size_t sitePosition, size_t sequencePosition, int state) const = 0;

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
  virtual SequenceDataInterface<HashType>* createEmptyContainer() const = 0;

  /**
   * @brief Check if a certain key is associated to a sequence in the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @return True if the key was found in the contained.
   */
  virtual bool hasSequence(const HashType& sequenceKey) const = 0;


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
  virtual const CoreSequenceInterface& getSequence(const HashType& sequenceKey) const = 0;

  /**
   * @brief Remove and delete a sequence from the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   */
  virtual void deleteSequence(const HashType& sequenceKey) = 0;

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
  virtual const CoreSequenceInterface& getSequence(size_t sequencePosition) const = 0;

  /**
   * @brief Remove and delete a sequence from the container.
   *
   * @param sequencePosition The position of the sequence.
   */
  virtual void deleteSequence(size_t sequencePosition) = 0;
  
  /** @} */

  
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
   * @param sequencePosition The position of the sequence in the container.
   * @return The key to which the sequence is associated.
   */
  virtual const HashType& getSequenceKey(size_t sequencePosition) const = 0;

  /**
   * @brief Get the position of a sequence with a given key in the container.
   *
   * @param sequenceKey The key to which the sequence is associated.
   * @return The position of the sequence with name 'name', if it exists.
   */
  virtual size_t getSequencePosition(const HashType& sequenceKey) const = 0;

  /** @} */

  /**
   * @name Access sequence names
   *
   * @{
   */

  /**
   * @return a vector with all sequence names.
   */
  virtual std::vector<std::string> getSequenceNames() const = 0;
  
  /**
   * @brief Batch-set all sequence names.
   *
   * @param names A vector of sequence names. Must be the same size as the number of sequences in the container, otherwise a DimensionException is thrown.
   * @param updateKeys Tell whether the keys of the sequences should be updated with the new sequence names.
   */
  virtual void setSequenceNames(const std::vector<std::string>& names, bool updateKeys) = 0;

  /**
   * @return a vector with all sequence comments.
   */
  virtual std::vector<Comments> getSequenceComments() const = 0;
   /** @} */

};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCEDATA_H
