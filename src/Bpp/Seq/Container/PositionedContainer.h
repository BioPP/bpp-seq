// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_POSITIONEDCONTAINER_H
#define BPP_SEQ_CONTAINER_POSITIONEDCONTAINER_H

#include <Bpp/Clonable.h>

// From the STL:

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
 */
template<class T>
class PositionedContainerInterface :
  public virtual Clonable
{
public:
  PositionedContainerInterface() {}
  virtual ~PositionedContainerInterface() {}

public:
  /**
   * @brief the size
   */
  virtual size_t getSize() const = 0;

  virtual void setSize(size_t size) {}

  virtual void clear() = 0;

  /**
   * @brief Retrieve an object from the container.
   *
   * @param objectIndex The position of the object.
   * @return A shared pointer toward the Object object with corresponding name.
   */
  virtual const std::shared_ptr<T> getObject(size_t objectIndex) const = 0;

  /**
   * @brief Retrieve an object from the container.
   *
   * @param objectIndex The position of the object.
   * @return A shared pointer toward the Object object with corresponding name.
   */
  virtual std::shared_ptr<T> getObject(size_t objectIndex) = 0;

  /**
   * @brief Get a reference toward an object from the container.
   *
   * @param objectIndex The position of the object.
   * @return A reference toward the Object object with corresponding name.
   */
  virtual const T& object(size_t objectIndex) const = 0;

  /**
   * @brief Get a reference toward an object from the container.
   *
   * @param objectIndex The position of the object.
   * @return A reference toward the Object object with corresponding name.
   */
  virtual T& object(size_t objectIndex) = 0;

  /**
   * @brief Extract and remove a object from the container.
   *
   * @param objectIndex The position of the object.
   * @return A smart pointer toward the removed object.
   */
  virtual std::shared_ptr<T> removeObject(size_t objectIndex) = 0;

  /**
   * @brief Delete an object from the container.
   *
   * @param objectIndex The index of the object in the container.
   */
  virtual void deleteObject(size_t objectIndex) = 0;
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_POSITIONEDCONTAINER_H
