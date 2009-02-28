//
// File VectorSequenceContainer.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for sequences analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

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

namespace bpp
{

/**
 * @brief The VectorSequenceContainer class.
 *
 * This is the simplest implementation of the OrderedSequenceContainer interface.
 * Sequences are stored in a vector of pointers.
 * The container is responsible for the creation and destruction of the sequence
 * objects it contains.
 */
class VectorSequenceContainer:
  public AbstractSequenceContainer
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
		 * @brief Build an empty container that will contain sequences of a particular alphabet.
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

		/**
		 * @brief Copy from a SequenceContainer.
		 *
		 * @param osc The SequenceContainer to copy into this container.
		 */
		VectorSequenceContainer(const SequenceContainer & osc);

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
		 * @brief Copy from a SequenceContainer.
		 *
		 * @param osc The SequenceContainer to copy into this container.
		 */
		VectorSequenceContainer & operator = (const SequenceContainer & osc);

    /**
		 * @brief Container destructor: delete all sequences in the container.
		 */
		virtual ~VectorSequenceContainer() { clear(); }

	public:
		
		/**
		 * @name The Clonable interface.
		 *
		 * @{
		 */
		Clonable * clone() const { return new VectorSequenceContainer(*this); }
		/** @} */

		/**
		 * @name The SequenceContainer interface.
		 *
		 * @{
		 */
		const Sequence * getSequence(const string & name) const throw (SequenceNotFoundException);
		void setSequence(const string & name, const Sequence & sequence, bool checkName = true) throw (Exception)
    {
      setSequence(getSequencePosition(name), sequence, checkName);
    }
		Sequence * removeSequence(const string & name) throw (SequenceNotFoundException)
    {
      return removeSequence(getSequencePosition(name));
    }
		void deleteSequence(const string & name) throw (SequenceNotFoundException)
    {
      deleteSequence(getSequencePosition(name));
    }
		unsigned int getNumberOfSequences() const;
		vector<string> getSequencesNames() const;
		void setSequencesNames(const vector<string> & names, bool checkNames = true) throw (Exception);
		void clear();
		SequenceContainer * createEmptyContainer() const;
    
    int & valueAt(const string & sequenceName, unsigned int elementIndex) throw (SequenceNotFoundException, IndexOutOfBoundsException)
    {
      return (* getSequenceInner(sequenceName))[elementIndex];
    }
    const int & valueAt(const string & sequenceName, unsigned int elementIndex) const throw (SequenceNotFoundException, IndexOutOfBoundsException)
    {
      return (* getSequence(sequenceName))[elementIndex]; 
    }
    int & operator()(const string & sequenceName, unsigned int elementIndex)
    {
      return (* getSequenceInner(sequenceName))[elementIndex];
    }
    const int & operator()(const string & sequenceName, unsigned int elementIndex) const
    {
      return (* getSequence(sequenceName))[elementIndex]; 
    }
    
    int & valueAt(unsigned int sequenceIndex, unsigned int elementIndex) throw (IndexOutOfBoundsException)
    {
      return (* getSequenceInner(sequenceIndex))[elementIndex];
    }
    const int & valueAt(unsigned int sequenceIndex, unsigned int elementIndex) const throw (IndexOutOfBoundsException)
    {
      return (* getSequence(sequenceIndex))[elementIndex];
    } 
    int & operator()(unsigned int sequenceIndex, unsigned int elementIndex)
    {
      return (* getSequenceInner(sequenceIndex))[elementIndex];
    }
    const int & operator()(unsigned int sequenceIndex, unsigned int elementIndex) const
    {
      return (* getSequence(sequenceIndex))[elementIndex];
    } 
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
		virtual void addSequence(const Sequence & sequence, bool checkName = true) throw (Exception);

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
		virtual void addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName = true) throw (Exception);

	protected:

		/**
		 * @name AbstractSequenceContainer methods.
		 *
		 * @{
		 */
		Sequence * getSequenceInner(unsigned int i)      throw (IndexOutOfBoundsException);
		Sequence * getSequenceInner(const string & name) throw (SequenceNotFoundException);
		/** @} */
};

} //end of namespace bpp.

#endif // _VECTORSEQUENCECONTAINER_H_

