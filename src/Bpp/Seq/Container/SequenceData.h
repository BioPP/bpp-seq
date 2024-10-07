// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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
 */
template<class HashType = std::string>
class TemplateSequenceDataInterface :
  public virtual Clonable,
  public virtual Commentable
{
public:
  TemplateSequenceDataInterface()
  {}
  virtual ~TemplateSequenceDataInterface()
  {}

  TemplateSequenceDataInterface<HashType>* clone() const override = 0;

public:
  /**
   * @brief Get the container's alphabet.
   *
   * @return The alphabet associated to this container.
   */
  virtual const Alphabet& alphabet() const = 0;

  /**
   * @brief Get a pointer toward the container's alphabet.
   *
   * @return A pointer toward the alphabet associated to this container.
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
   * This method creates a new SequencedContainer object.
   * The class of this container depends on the class implementing this interface.
   *
   * @return A new empty container, with the same alphabet as this one.
   */
  virtual TemplateSequenceDataInterface<HashType>* createEmptyContainer() const = 0;

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
  virtual const CoreSequenceInterface& sequence(const HashType& sequenceKey) const = 0;

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
  virtual const CoreSequenceInterface& sequence(size_t sequencePosition) const = 0;

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
  virtual const HashType& sequenceKey(size_t sequencePosition) const = 0;

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

using SequenceDataInterface = TemplateSequenceDataInterface<std::string>;
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCEDATA_H
