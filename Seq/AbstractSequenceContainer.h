/*
 * File AbstractSequenceContainer.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Wednesday July 30 2003
 */

// Secured inclusion of header's file
#ifndef _ABSTRACTSEQUENCECONTAINER_H_
#define _ABSTRACTSEQUENCECONTAINER_H_

#include "Sequence.h"
#include "SequenceContainer.h"
#include "OrderedSequenceContainer.h"
#include "Alphabet.h"

// From Utils:
#include <Utils/Exceptions.h>

/**
 * @brief Low level implementation of the OrderedSequenceContainer class.
 *
 * This abstract class provides an alphabet and comments, with associated methods.
 * It also implements many manipulation on sequences methods.
 */
class AbstractSequenceContainer : public OrderedSequenceContainer
{
	protected:

		/**
		 * @brief The container's alphabet.
		 */
		const Alphabet * _alphabet;

    /**
     * @brief The container's comments.
     */
		Comments _comments;

	public:

		/**
		 * @brief This constructor initialize the alphabet pointer.
		 *
		 * This constructor is to be called by constructors of derived classes.
		 *
		 * @param alpha The alphabet to be associated to this container.
		 */
		AbstractSequenceContainer(const Alphabet * alpha);

		~AbstractSequenceContainer() {}
		
	public:
	
		/**
		 * @name From the SequenceContainer interface
		 *
		 * @{
		 */
		const Alphabet * getAlphabet() const { return _alphabet; }
		vector<int> getContent(const string & name) const throw (SequenceNotFoundException);
		string toString(const string & name) const throw (SequenceNotFoundException);
		Comments getComments(const string & name) const throw (SequenceNotFoundException);
		void setComments(const string & name, const Comments & comments) throw (SequenceNotFoundException);
		Comments getGeneralComments() const;
		void setGeneralComments(const Comments & comments);
		void deleteGeneralComments();

		/** @} */

		/**
		 * @name From the OrderedSequenceContainer interface
		 *
		 * @{
		 */
		virtual string getName(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		virtual vector<int> getContent(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		virtual string toString(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		virtual Comments getComments(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		virtual void     setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException) = 0;
		/** @} */

};

#endif // _ABSTRACTSEQUENCECONTAINER_H_
