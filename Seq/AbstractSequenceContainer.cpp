//
// File: AbstractSequenceContainer.cpp
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Wednesday July 30 2003
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

#include "AbstractSequenceContainer.h"

using namespace bpp;

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
throw (IndexOutOfBoundsException)
{
	return getSequence(i) -> getContent();
}

vector<int> AbstractSequenceContainer::getContent(const string & name) const 
throw (SequenceNotFoundException)
{


	return getSequence(name) -> getContent();
}

/**************************************************************************************************/

string AbstractSequenceContainer::toString(unsigned int i)
const throw (IndexOutOfBoundsException)
{
	return getSequence(i) -> toString();
}

string AbstractSequenceContainer::toString(const string & name) const
throw (SequenceNotFoundException)
{
	return getSequence(name) -> toString();
}

/**************************************************************************************************/

Comments AbstractSequenceContainer::getComments(unsigned int i) const
throw (IndexOutOfBoundsException)
{
	return getSequence(i) -> getComments();
}

Comments AbstractSequenceContainer::getComments(const string & name) const
throw (SequenceNotFoundException)
{
	return getSequence(name) -> getComments();
}

/**************************************************************************************************/

void AbstractSequenceContainer::setComments(const string & name, const Comments & comments)
throw (SequenceNotFoundException)
{ 
	int pos = getSequencePosition(name);
	setComments(pos, comments); 
}

/**************************************************************************************************/

Comments AbstractSequenceContainer::getGeneralComments() const
{
	return _comments;
}

void AbstractSequenceContainer::setGeneralComments(const Comments & comments)
{
	_comments = comments; 
}

void AbstractSequenceContainer::deleteGeneralComments()
{
	_comments.clear(); 
}

/**************************************************************************************************/

