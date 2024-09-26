// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "SequencePositionIterators.h"

using namespace bpp;
using namespace std; // for the STL

/******************************************************************************/

bool AbstractSequencePositionIterator::operator==(const SequencePositionIterator& it) const
{
	return this->getPosition() == it.getPosition();
}

/******************************************************************************/

bool AbstractSequencePositionIterator::operator!=(const SequencePositionIterator& it) const
{
	return this->getPosition() != it.getPosition();
}

/******************************************************************************/

void AbstractSequencePositionIterator::setPosition(unsigned int pos)
{
	this->currentPosition_ = pos;
}

/******************************************************************************/

const Sequence& AbstractSequencePositionIterator::getSequence() const
{
	return *(this->sequence_);
}

/******************************************************************************/

unsigned int AbstractSequencePositionIterator::getPosition() const
{
	return this->currentPosition_;
}

/******************************************************************************/

int AbstractSequencePositionIterator::getValue() const
{
	return this->sequence_->getValue(this->currentPosition_);
}

/******************************************************************************/

string AbstractSequencePositionIterator::getChar() const
{
	return this->sequence_->getChar(this->currentPosition_);
}


// ===============================
// SimpleSequencePositionIterator
// ===============================
/******************************************************************************/

SimpleSequencePositionIterator::SimpleSequencePositionIterator(const SequencePositionIterator& it) :
	AbstractSequencePositionIterator(it.getSequence(), it.getPosition()) {
}

/******************************************************************************/

SimpleSequencePositionIterator& SimpleSequencePositionIterator::operator++()
{
	this->setPosition(this->getPosition() + 1);
	return *this;
}

/******************************************************************************/

SimpleSequencePositionIterator SimpleSequencePositionIterator::operator++(int i)
{
	SimpleSequencePositionIterator ans = *this;
	++(*this);
	return ans;
}

/******************************************************************************/

SimpleSequencePositionIterator& SimpleSequencePositionIterator::operator+=(int i)
{
	if (i > 0)
		this->setPosition(this->getPosition() + static_cast<unsigned int>(i));
	else if (i < 0)
	{
		unsigned int d = static_cast<unsigned int>(-i);
		if (d > this->getPosition())
			throw Exception("SimpleSequencePositionIterator::operator+=. Negative increment too large.");
		else
			this->setPosition(this->getPosition() - d);
	}
	return *this;
}

/******************************************************************************/

SimpleSequencePositionIterator& SimpleSequencePositionIterator::operator-=(int i)
{
	return (*this) += -i;
}

/******************************************************************************/

SimpleSequencePositionIterator SimpleSequencePositionIterator::operator+(int i) const
{
	SimpleSequencePositionIterator res(*this);
	res += i;
	return res;
}

/******************************************************************************/

SimpleSequencePositionIterator SimpleSequencePositionIterator::operator-(int i) const
{
	return (*this) + (-i);
}

/******************************************************************************/

bool SimpleSequencePositionIterator::hasMorePositions() const
{
	return this->getPosition() < this->getSequence().size();
}
/******************************************************************************/
