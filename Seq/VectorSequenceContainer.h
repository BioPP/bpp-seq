/*
 * File VectorSequenceContainer.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Wednesday July 30 2003
*/

// Secured inclusion of header's file
#ifndef _VECTORSEQUENCECONTAINER_H_
#define _VECTORSEQUENCECONTAINER_H_

#include "Sequence.h"
#include "AbstractSequenceContainer.h"
#include "Alphabet.h"

// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <algorithm>
#include <vector>

using namespace std;

/**
 * @brief The VectorSequenceContainer class.
 *
 * This is the simplest implementation of the OrderedSequenceContainer interface.
 * Sequences are stored in a vector of pointers.
 * The container is responsible for the creation and destruction of the sequence
 * objects it contains.
 */
class VectorSequenceContainer : public AbstractSequenceContainer
{
	protected:

		/**
		 * @brief A vector of pointers toward the sequences stored in the container.
		 */
		vector<Sequence *> _sequences;
        
	public:	// Constructors and destructor:
		
		/**
		 * @brief Build a new container from a vector of pointers toward sequence objects.
		 *
		 * The addSequence() method is called uppon each Sequence object, hence each sequence is
		 * <i>copied</i> into the container.
		 *
		 * @param vs    The vector of pointers toward sequence objects.
		 * @param alpha The alphabet to all sequences.
		 * @throw AlphabetMismatchException if one sequence does not match the specified alphabet.
		 */
		VectorSequenceContainer(
			const vector<const Sequence *> & vs, const Alphabet * alpha)
			throw (AlphabetMismatchException);
	
		/**
		 * @brief Build a void container that will contain sequences of a particular alphabet.
		 *
		 * @param alpha The alphabet of the container.
		 */
		VectorSequenceContainer(const Alphabet * alpha);
		
		/**
		 * @name Copy contructors:
		 *
		 * @{
		 */
		
		/**
		 * @brief Copy from a VectorSequenceContainer.
		 *
		 * @param vsc The VectorSequenceContainer to copy into this container.
		 */
		VectorSequenceContainer(const VectorSequenceContainer & vsc);
		
		/**
		 * @brief Copy from an OrderedSequenceContainer.
		 *
		 * @param osc The OrderedSequenceContainer to copy into this container.
		 */
		VectorSequenceContainer(const OrderedSequenceContainer & osc);

		/** @} */

		/**
		 * @brief Assign from a VectorSequenceContainer.
		 *
		 * @param vsc The VectorSequenceContainer to copy into this container.
		 */
		VectorSequenceContainer & operator = (const VectorSequenceContainer & vsc);

		/**
		 * @brief Copy from an OrderedSequenceContainer.
		 *
		 * @param osc The OrderedSequenceContainer to copy into this container.
		 */
		VectorSequenceContainer & operator = (const OrderedSequenceContainer & osc);
	
		/**
		 * @brief Container destructor: delete all sequences in the container.
		 */
		~VectorSequenceContainer();

	public:
		
		/**
		 * @name The Clonable interface.
		 *
		 * @{
		 */
		Clonable * clone() const;
		/** @} */

		/**
		 * @name The SequenceContainer interface.
		 *
		 * @{
		 */
		const Sequence * getSequence(const string & name) const throw (SequenceNotFoundException);
		void             setSequence(const string & name, const Sequence & sequence, bool checkName = true) throw (Exception);
		      Sequence * removeSequence(const string & name) throw (SequenceNotFoundException);
		void             deleteSequence(const string & name) throw (SequenceNotFoundException);
		unsigned int getNumberOfSequences() const;
		vector<string> getSequencesNames() const;
		void setSequencesNames(const vector<string> & names, bool checkNames = true) throw (Exception);
		void clear();
		SequenceContainer * getEmptyContainer() const;
		/** @} */


		/**
		 * @name The OrderedSequenceContainer interface.
		 *
		 * @{
		 */
		void setComments(const string & name, const Comments & comments) throw (SequenceNotFoundException) {
			AbstractSequenceContainer::setComments(name, comments);
		}
		void setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException);
		unsigned int        getSequencePosition(const string & name) const throw (SequenceNotFoundException);
		const Sequence *    getSequence(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		void                setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName = true) throw (Exception);
		      Sequence * removeSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		void             deleteSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
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
		 * @throw Exception If the sequence couldn't be added to the container.
		 */
		void addSequence(const Sequence & sequence, bool checkName = true) throw (Exception);

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
		void addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName = true) throw (Exception);

	protected:

		/**
		 * @name AbstractSequenceContainer methods.
		 *
		 * @{
		 */
		Sequence * getSequence(unsigned int i)      throw (IndexOutOfBoundsException);
		Sequence * getSequence(const string & name) throw (SequenceNotFoundException);
		/** @} */
};

#endif // _VECTORSEQUENCECONTAINER_H_
