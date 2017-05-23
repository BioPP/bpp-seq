//
// File: NamedContainer.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
//             Laurent Guéguen
// Created on: Fri Jul 25 2003 (template in mardi 21 mars 2017, à 15h 04)
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#ifndef _NAMEDCONTAINER_H_
#define _NAMEDCONTAINER_H_

// From the STL:
#include <string>

namespace bpp
{

/**
 * @brief A Container template for objects that are accessible
 * through names.
 *
 * The objects are all stored as shared pointers.
 *
 */

  template<class T>
  class NamedContainer
  {
  public:
    NamedContainer() {}
    virtual ~NamedContainer() {}

  public:
    /**
     * @brief Retrieve an object from the container.
     *
     * @param name The name of the object.
     * @return A reference toward the Object with corresponding name.
     */

    virtual const std::shared_ptr<T> getObject(const std::string& name) const = 0;
    virtual std::shared_ptr<T> getObject(const std::string& name) = 0;
    
    /**
     * @brief Check if a object with a given name is present in the
     * container.
     *
     * @param name The name of the object.
     * @return True if a object with the given name is present in the
     * container.
     */

    virtual bool hasObject(const std::string& name) const = 0;

    /**
     * @brief Add a object to the container.
     *
     * @param object  The object to add.
     * @param name the name of the object.
     * @param checkName Tell if the container must check if the name
     * of the object is already used in the container before adding
     * it.
     */
    
    virtual void addObject(std::shared_ptr<T> object, const std::string& name, bool checkName = false) {};

    /**
     * @brief Extract and remove a object from the container.
     * The container size is preserved.
     *
     * @param name The name of the object.
     */
    
    virtual std::shared_ptr<T> removeObject(const std::string& name) {return 0;};

    /**
     * @brief Get the number of objects in the container.
     *
     * @return The number of objects in the container.
     */

    virtual size_t getSize() const = 0;

    /**
     * @brief Get all the names of the objects in the container.
     *
     * @return A vector of strings with all object names.
     */

    virtual std::vector<std::string> getObjectsNames() const = 0;
		
    /**
     * @brief Delete all objects in the container.
     */

    virtual void clear() = 0;

  };

} //end of namespace bpp.

#endif	// _NAMEDCONTAINER_H_

