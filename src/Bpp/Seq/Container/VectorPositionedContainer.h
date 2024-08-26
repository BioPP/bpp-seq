// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_VECTORPOSITIONEDCONTAINER_H
#define BPP_SEQ_CONTAINER_VECTORPOSITIONEDCONTAINER_H

#include <Bpp/Clonable.h>
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/VectorTools.h>

#include "PositionedContainer.h"

// From the STL library:
#include <string>
#include <vector>
#include <iostream>
#include <iterator>

namespace bpp
{
/**
 * @brief The template VectorPositionedContainer class.
 *
 * Objects are stored in a std::vector of shared pointers.
 * Object access is hence in \f$O(1)\f$.
 */
template<class T>
class VectorPositionedContainer :
	public virtual PositionedContainerInterface<T>,
	public virtual Clonable
{
protected:
std::vector<std::shared_ptr<T> > positions_;

public:
/**
 * @brief Build a new container from a set of positions.
 *
 * @param vs A std::vector of objects.
 */
VectorPositionedContainer(const std::vector<std::shared_ptr<T> >& vs) :
	positions_(vs)
{
}

/**
 * @brief Build a new empty container with specified size.
 *
 * @param size Number of sequences in the container.
 */
VectorPositionedContainer(size_t size = 0) :
	positions_(size)
{
}


VectorPositionedContainer(const VectorPositionedContainer<T>& vsc) :
	positions_(vsc.positions_)
{
}

/**
 * @brief copy where shared_ptr elements are shared
 */
VectorPositionedContainer<T>& operator=(const VectorPositionedContainer<T>& vsc)
{
	positions_ = vsc.positions_;
	return *this;
}

virtual ~VectorPositionedContainer()
{
	positions_.clear();
}

public:
/**
 * @name The Clonable interface.
 *
 * @{
 */
VectorPositionedContainer<T>* clone() const override
{
	return new VectorPositionedContainer<T>(*this);
}

/** @} */

size_t getSize() const override
{
	return positions_.size();
}

bool isAvailablePosition(size_t objectIndex) const
{
	return objectIndex < getSize() && (positions_[objectIndex] == nullptr || positions_[objectIndex]->size() == 0);
}

bool hasObjectWithPosition(size_t objectIndex) const
{
	return objectIndex < getSize() && (positions_[objectIndex] != nullptr && positions_[objectIndex]->size() > 0);
}

void setSize(size_t size) override
{
	if (positions_.size() > size)
		throw Exception("VectorPositionedContainer::setSize : not possible to shorten the vector.");

	positions_.resize(size);
}

/**
 * @brief Destroys  the vector
 */
void clear() override
{
	positions_.clear();
}


/**
 * @brief Nullify all elements
 */
virtual void nullify()
{
	std::fill(positions_.begin(), positions_.end(), nullptr);
}

const std::shared_ptr<T> getObject(size_t objectIndex) const override
{
	if (objectIndex >= getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::getObject.", objectIndex, 0, getSize());
	return positions_[objectIndex];
}

std::shared_ptr<T> getObject(size_t objectIndex) override
{
	if (objectIndex >= getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::getObject.", objectIndex, 0, getSize());
	return positions_[objectIndex];
}

const T& object(size_t objectIndex) const override
{
	if (objectIndex >= getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::object.", objectIndex, 0, getSize());
	return *positions_[objectIndex];
}

T& object(size_t objectIndex) override
{
	if (objectIndex >= getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::object.", objectIndex, 0, getSize());
	return *positions_[objectIndex];
}

/**
 * @brief Add an object
 *
 * @param object  The object to add.
 * @param objectIndex The new position of the object
 * @param checkPosition Look if the position is empty.
 */
void addObject(std::shared_ptr<T> object, size_t objectIndex, bool checkPosition = false)
{
	if (objectIndex >= getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::addObject.", objectIndex, 0, getSize());

	if (checkPosition && positions_[objectIndex] != nullptr)
		throw BadIntegerException("VectorPositionedContainer::addObject: object position already occupied in container ", (int)objectIndex);

	positions_[objectIndex] = object;
}

/**
 * @brief Insert an object
 *
 * @param object          The object to add.
 * @param objectIndex The new position of the object
 */
void insertObject(std::shared_ptr<T> object, size_t objectIndex)
{
	if (objectIndex > getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::insertObject.", objectIndex, 0, getSize());

	positions_.insert(positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex), object);
}


std::shared_ptr<T> removeObject(size_t objectIndex) override
{
	if (objectIndex >= getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::removeObject.", objectIndex, 0, getSize());

	std::shared_ptr<T> ret = positions_[objectIndex];

	positions_.erase(positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex));

	return ret;
}

void deleteObject(size_t objectIndex) override
{
	if (objectIndex >= getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::deleteObject.", objectIndex, 0, getSize());

	positions_.erase(positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex));
}

void deleteObjects(size_t objectIndex, size_t length)
{
	if (objectIndex + length > getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::deleteObjects.", objectIndex + length, 0, getSize());

	positions_.erase(positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex), positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex + length));
}

void appendObject(std::shared_ptr<T> object)
{
	positions_.push_back(object);
}

std::shared_ptr<T> getObject_(size_t objectIndex) const
{
	if (objectIndex >= VectorPositionedContainer<T>::getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::getObject.", objectIndex, 0, VectorPositionedContainer<T>::getSize() - 1);
	return positions_[objectIndex];
}

void addObject_(std::shared_ptr<T> object, size_t objectIndex, bool checkPosition = false) const
{
	if (objectIndex >= VectorPositionedContainer<T>::getSize())
		throw IndexOutOfBoundsException("VectorPositionedContainer::addObject.", objectIndex, 0, VectorPositionedContainer<T>::getSize() - 1);

	if (checkPosition && positions_[objectIndex] != nullptr)
		throw BadIntegerException("VectorPositionedContainer::setObject: object position already occupied in container ", (int)objectIndex);

	const_cast<std::vector<std::shared_ptr<T> >&>(positions_)[objectIndex] = object;
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_VECTORPOSITIONEDCONTAINER_H
