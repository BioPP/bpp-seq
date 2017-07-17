//
// File: VectorPositionedContainer.h
// Created by: Julien Dutheil
//             Laurent Guéguen (for template feature)
// Created on: Mon Oct  6 11:50:40 2003
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

#ifndef VECTOR_POSITIONED_CONTAINER_H_
#define VECTOR_POSITIONED_CONTAINER_H_

#include "PositionedContainer.h"
#include <Bpp/Numeric/VectorTools.h>
#include <Bpp/Clonable.h>
#include <Bpp/Exceptions.h>

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
 *
 */

  template<class T>
  class VectorPositionedContainer :
    public virtual PositionedContainer<T>,
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
     * @param alpha The alphabet for this container.
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
     *
     */
    
    VectorPositionedContainer<T>& operator=(const VectorPositionedContainer<T>& vsc)
    {
      positions_=vsc.positions_;
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
    
    VectorPositionedContainer<T>* clone() const
    {
      return new VectorPositionedContainer<T>(*this);
    }
    
    /** @} */

    size_t getSize() const
    {
      return positions_.size();
    }
    
    const std::shared_ptr<T> getObject(size_t objectIndex) const
    {
      if (objectIndex >= getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::getSite.", objectIndex, 0, getSize() - 1);
      return positions_[objectIndex];
    }

    std::shared_ptr<T> getObject(size_t objectIndex)
    {
      if (objectIndex >= getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::getSite.", objectIndex, 0, getSize() - 1);
      return positions_[objectIndex];
    }
    
    /*
     * @brief Fill container
     *
     */
    
    void addObject(std::shared_ptr<T> object, size_t objectIndex, bool checkPosition = false)
    {
      if (objectIndex >= getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::addSite.", objectIndex, 0, getSize() - 1);

      if (checkPosition && positions_[objectIndex]!=nullptr)
        throw BadIntegerException("VectorPositionedContainer::setObject: object position already occupied in container ", (int)objectIndex);

      positions_[objectIndex] = object;
    }

    void insertObject(std::shared_ptr<T> object, size_t objectIndex)
    {
      if (objectIndex > getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::insertObject.", objectIndex, 0, getSize());

      positions_.insert(positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex), object);
    }


    bool isAvailablePosition(size_t objectIndex) const
    {
      return (objectIndex < getSize() && positions_[objectIndex]==nullptr);
    }

    std::shared_ptr<T> removeObject(size_t objectIndex)
    {
      if (objectIndex >= getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::removeObject.", objectIndex, 0, getSize() - 1);

      std::shared_ptr<T> ret = positions_[objectIndex];

      positions_[objectIndex]=nullptr;

      return ret;
    }
    
    std::shared_ptr<T> deleteObject(size_t objectIndex)
    {
      if (objectIndex >= getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::deleteObject.", objectIndex, 0, getSize() - 1);

      std::shared_ptr<T> ret = positions_[objectIndex];

      positions_.erase(positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex));

      return ret;
    }

    void deleteObjects(size_t objectIndex, size_t length)
    {
      if (objectIndex + length > getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::deleteObjects.", objectIndex + length, 0, getSize() - 1);

      positions_.erase(positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex), positions_.begin() + static_cast<std::ptrdiff_t>(objectIndex+length));
    }

    void appendObject(std::shared_ptr<T> object)
    {
      positions_.push_back(object);
    }

    void setSize(size_t size)
    {
      if (positions_.size()>size)
        throw Exception("VectorPositionedContainer::setSize : not possible to shroten the vector.");

      positions_.resize(size);
    }

    void clear()
    {
      positions_.clear();
    }

  protected:
  
    std::shared_ptr<T> getObject_(size_t objectIndex) const
    {
      if (objectIndex >= VectorPositionedContainer<T>::getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::getObject.", objectIndex, 0, VectorPositionedContainer<T>::getSize() - 1);
      return positions_[objectIndex];
    }

    void addObject_(std::shared_ptr<T> object, size_t objectIndex, bool checkPosition = false) const
    {
      if (objectIndex >= VectorPositionedContainer<T>::getSize())
        throw IndexOutOfBoundsException("VectorPositionedContainer::addObject.", objectIndex, 0, (int)VectorPositionedContainer<T>::getSize() - 1);

      if (checkPosition && positions_[objectIndex]!=nullptr)
        throw BadIntegerException("VectorPositionedContainer::setObject: object position already occupied in container ", (int)objectIndex);

      const_cast<std::vector<std::shared_ptr<T> >& >(positions_)[objectIndex] = object;
    }
  };

} // end of namespace bpp.

#endif  // VECTOR_ORDERED_CONTAINER_H_

