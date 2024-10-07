// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_NAMEDCONTAINER_H
#define BPP_SEQ_CONTAINER_NAMEDCONTAINER_H

#include <Bpp/Clonable.h>

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief A Container template for objects that are accessible
 * through names.
 *
 * The objects are all stored as shared pointers.
 */
template<class T>
class NamedContainerInterface :
  public virtual Clonable
{
public:
  NamedContainerInterface()
  {}
  virtual ~NamedContainerInterface()
  {}

protected:
  /**
   * @brief Retrieve an object from the container.
   *
   * @param name The name of the object.
   * @return A reference toward the Object with corresponding name.
   */
  virtual const std::shared_ptr<T> getObject(const std::string& name) const = 0;

  virtual std::shared_ptr<T> getObject(const std::string& name) = 0;

  virtual const T& object(const std::string& name) const = 0;

  virtual T& object(const std::string& name) = 0;

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
   * @brief Extract and remove a object from the container.
   *
   * @param name The name of the object.

   */
  virtual std::shared_ptr<T> removeObject(const std::string& name) = 0;

  /**
   * @brief Remove a object from the container.
   *
   * @param name The name of the object.
   */

  virtual void deleteObject(const std::string& name) = 0;

  /**
   * @brief Get all the names of the objects in the container.
   *
   * @return A vector of strings with all object names.
   */

  virtual std::vector<std::string> getObjectNames() const = 0;

public:
  /**
   * @brief Get the number of objects in the container.
   *
   * @return The number of objects in the container.
   */

  virtual size_t getSize() const = 0;

  /**
   * @brief Delete all objects in the container.
   */

  virtual void clear() = 0;
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_NAMEDCONTAINER_H
