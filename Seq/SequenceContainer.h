/*
 * File SequenceContainer.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Friday July 25 2003
 */

#ifndef _SEQUENCECONTAINER_H_
#define _SEQUENCECONTAINER_H_

#include "Alphabet.h"
#include "Sequence.h"
#include "SequenceContainerExceptions.h"

// From Utils:
#include <Utils/Clonable.h>

// From the STL:
#include <cstdlib>

using namespace std;


/**
 * @brief The SequenceContainer interface.
 *
 * Interface for sequence containers manipulation.
 * This interface is the most general one, it supposes no ordering of the sequences in the
 * container. Sequences are retrievable using their names, which must be unic.
 *
 * The container is the SOLE responsible for the allocation/destruction of sequences it
 * contains. This means that any sequence passed to it will be COPIED into the container.
 * The container also provides methods that send pointers towards these sequences
 * (without performing any copy of the underlying objects).
 *
 * Note :
 * <ol>
 * <li> methods for adding sequences to the container are not declared here
 * (so they can't be used throught this interface),
 * because these methods take sequence container's type specific parameters
 * (i.e. a key for map sequence containers);</li>
 * <li> although a sequence can be deleted directly from the sequence container,
 * using specific methods (removeSequence and deleteSequence) to do it is
 * IMPERATIVE (because they achieve	verifications, and they properly
 * delete pointer toward the corresponding sequences).
 * </li>
 * </ol>
 *
 * @see Sequence
 */

class SequenceContainer: public Clonable
{
	public:	// Class destructor:
		virtual ~SequenceContainer() {}

	public:
		/**
		 * @brief Get sequence container's alphabet.
		 *
		 * @return The alphabet associated to this container.
		 */
		virtual const Alphabet * getAlphabet() const = 0;
		
		/**
		 * @brief Get the content of a sequence.
		 *
		 * @param name The name of the sequence.
		 * @return The content of the sequence as a vector of integers.
		 * @throw SequenceNotFoundException If the name does not match any sequence in the container.
		 */
		virtual vector<int> getContent(const string & name) const throw (SequenceNotFoundException) = 0;  
		
		/**
		 * @brief Convert a particular sequence to a string.
		 *
		 * @param name The name of the sequence.
		 * @return A string describing the content of the sequence.
		 * @throw SequenceNotFoundException If the name does not match any sequence in the container.
		 */
		virtual string toString(const string & name) const throw (SequenceNotFoundException) = 0;  

		/**
		 * @brief Retrieve a sequence object from the container.
		 *
		 * @param name The name of the sequence.
		 * @return A pointer toward the Sequence with corresponding name.
		 * @throw SequenceNotFoundException If the name does not match any sequence in the container.
		 */
		virtual const Sequence * getSequence(const string & name) const throw (SequenceNotFoundException) = 0;

		/**
		 * @brief Replace a sequence in the container.


		 *
		 * @param name      The name of the sequence.
		 * @param sequence  The sequence to add.
		 * @param checkName Tell if the container must check if the name of the sequence
		 * is already used in the container before adding it.
		 * @throw SequenceNotFoundException If the name does not match any sequence in
		 * the container.
		 * @throw Exception Any other kind of exception, if the name of the sequence is
		 * already used, are whatever else depending on the implementation.
		 */
		virtual void setSequence(const string & name, const Sequence & sequence, bool checkName) throw (Exception) = 0;

		/**
		 * @brief Extract (and remove) a sequence from the container.
		 *
		 * @param name The name of the sequence.
		 * @throw SequenceNotFoundException If the name does not match any sequence in
		 * the container.
		 */
		virtual Sequence * removeSequence(const string & name) throw (SequenceNotFoundException) = 0;
		
		/**
		 * @brief Delete a sequence of the container.
		 *
		 * @param name The name of the sequence.
		 * @throw SequenceNotFoundException If the name does not match any sequence in
		 * the container.
		 */
		virtual void deleteSequence(const string & name) throw (SequenceNotFoundException) = 0;

		/**
		 * @brief Get the number of sequences in the container.
		 *
		 * @return The number of sequences in the container.
		 */
		virtual unsigned int getNumberOfSequences() const = 0;

		/**
		 * @brief Get all the names of the sequences in the container.
		 *
		 * @return A vector of strings with all sequence names.
		 */
		virtual vector<string> getSequencesNames() const = 0;
		
		/**
		 * @brief Set all sequence names.
		 *
		 * @param names A vector of strings with all sequence names.
		 * Its size must be strictly equal to the the size of the container (the number of
		 * sequences).
		 * @param checkNames Tell if the container must check if the name of the sequence
		 * is already used in the container before adding it.
		 * @throw Exception If there are redundant names in the input vector.
		 */
		virtual void setSequencesNames(const vector<string> & names, bool checkNames) throw (Exception) = 0;

		/**
		 * @brief Get comments of a particular sequence.
		 *
		 * @param name The name of the sequence.
		 * @return The comments associated to sequence with name 'name'.
		 * @throw SequenceNotFoundException If the name does not match any sequence in
		 * the container.
		 */
		virtual Comments getComments(const string & name) const throw (SequenceNotFoundException) = 0;
		
		/**
		 * @brief Set the comments of a particular sequence.
		 *
		 * @param name     The name of the sequence.
		 * @param comments The comments to set to sequence with name 'name'.
		 * @throw SequenceNotFoundException If the name does not match any sequence in
		 * the container.
		 */
		virtual void setComments(const string & name, const Comments & comments) throw (SequenceNotFoundException) = 0;
		
		/**
		 * @brief Get the comments of this container.
		 *
		 * @return The comments associated to this container.
		 */
		virtual Comments getGeneralComments() const = 0;

		/**
		 * @brief Set the comments of this container.
		 *
		 * @param comments The comments to be associated to this container.
		 */
		virtual void setGeneralComments(const Comments & comments) = 0;
		
		/**
		 * @brief Delete the comments associated to this container.
		 */
		virtual void deleteGeneralComments() = 0;

		/**
		 * @brief Delete all sequences in the container.
		 */
		virtual void clear() = 0;
};

#endif	// _SEQUENCECONTAINER_H_
