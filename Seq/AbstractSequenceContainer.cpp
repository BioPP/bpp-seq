/*
 * File AbstractSequenceContainer.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Wednesday July 30 2003
*/

#include "AbstractSequenceContainer.h"

/**************************************************************************************************/

AbstractSequenceContainer::AbstractSequenceContainer(const Alphabet * alpha):
_alphabet(alpha) {};

/**************************************************************************************************/

string AbstractSequenceContainer::getName(unsigned int i) const
throw (IndexOutOfBoundsException)
{
	return getSequence(i) -> getName();
}

/**************************************************************************************************/

vector<int> AbstractSequenceContainer::getContent(unsigned int i) const
throw (IndexOutOfBoundsException) {
	return getSequence(i) -> getContent();
}

vector<int> AbstractSequenceContainer::getContent(const string & name) const 
throw (SequenceNotFoundException) {


	return getSequence(name) -> getContent();
}

/**************************************************************************************************/

string AbstractSequenceContainer::toString(unsigned int i)
const throw (IndexOutOfBoundsException) {
	return getSequence(i) -> toString();
}

string AbstractSequenceContainer::toString(const string & name) const
throw (SequenceNotFoundException) {
	return getSequence(name) -> toString();
}

/**************************************************************************************************/

Comments AbstractSequenceContainer::getComments(unsigned int i) const
throw (IndexOutOfBoundsException) {
	return getSequence(i) -> getComments();
}

Comments AbstractSequenceContainer::getComments(const string & name) const
throw (SequenceNotFoundException) {
	return getSequence(name) -> getComments();
}

/**************************************************************************************************/

void AbstractSequenceContainer::setComments(const string & name, const Comments & comments)
throw (SequenceNotFoundException) { 
	int pos = getSequencePosition(name);
	setComments(pos, comments); 
}

/**************************************************************************************************/

Comments AbstractSequenceContainer::getGeneralComments() const {
	return _comments;
}

void AbstractSequenceContainer::setGeneralComments(const Comments & comments) {
	_comments = comments; 
}

void AbstractSequenceContainer::deleteGeneralComments() {
	_comments.clear(); 
}

/**************************************************************************************************/
