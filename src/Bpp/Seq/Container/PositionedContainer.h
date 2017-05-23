//
// File PositionedContainer.h
// Author: Laurent Guéguen
// Last modification : mardi 21 mars 2017, à 15h 02
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

  This software is a computer program whose purpose is to provide classes
  for objects analysis.

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

#ifndef _POSITIONED_CONTAINER_H_
#define _POSITIONED_CONTAINER_H_

#include <memory>

namespace bpp
{

/**
 * @brief The PositionedContainer interface.
 *
 * Interface to manage indexed containers, where Objects may be
 * accessed via their indice, <i>i.e.</i> their position in the
 * container.
 *
 * All Objects must have a position, accessible through getPosition
 * and setPosition methods (see Site class).
 *
 */

  template<class T>
  class PositionedContainer
  {
  public:
    PositionedContainer() {}
    virtual ~PositionedContainer() {}

  public:

    /**
     * @brief the size
     *
     **/

    virtual size_t getSize() const = 0;
         
    virtual void setSize(size_t size) {};

    virtual void clear() = 0;
    
    /**
     * @brief Retrieve an object from the container.

     * @param objectIndex The position of the object.
     * @return A reference toward the Object object with corresponding name.
     */
    
    virtual const std::shared_ptr<T> getObject(size_t objectIndex) const = 0;

    virtual std::shared_ptr<T> getObject(size_t objectIndex) = 0;

    /**
     * @brief Add  an object in the container.
     *
     * @param object          The object to add.
     * @param position The new position of the object
     * @param checkPosition Look if the position is empty.
     */

    virtual void addObject(std::shared_ptr<T> object, size_t objectIndex, bool checkPosition = false) {};

    /**
     * @brief Insert an object in the container.
     *
     * @param object          The object to add.
     * @param position The new position of the object
     */

    virtual void insertObject(std::shared_ptr<T> object, size_t objectIndex) {};

    /**
     * @brief Extract and remove a object from the container.
     * The container size is unchanged, the positioned is set to null.
     *
     * @param objectIndex The position of the object.
     */
    
    virtual std::shared_ptr<T> removeObject(size_t objectIndex) = 0;

    /**
     * @brief Extract and remove a object from the container.
     * The container is then shortened
     *
     * @param name The name of the object.
     */
    
    virtual std::shared_ptr<T> deleteObject(const std::string& name) {return 0;};


    /**
     * @brief Add a object at the end of the container.
     *
     * @param object          The object to add.
     */

    virtual void appendObject(std::shared_ptr<T> object) {};

  };

} //end of namespace bpp.

#endif	//_POSITIONED_CONTAINER_H_

