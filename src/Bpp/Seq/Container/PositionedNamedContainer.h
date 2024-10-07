// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_POSITIONEDNAMEDCONTAINER_H
#define BPP_SEQ_CONTAINER_POSITIONEDNAMEDCONTAINER_H


#include "NamedContainer.h"
#include "PositionedContainer.h"

// From the STL library:
#include <string>
#include <vector>
#include <iostream>

namespace bpp
{
/**
 * @brief The template PositionedNamedContainer interface, that links
 * position & name containers.
 *
 * This interface also includes addObject and insertObject methods.
 */
template<class T>
class PositionedNamedContainerInterface :
  public virtual PositionedContainerInterface<T>,
  public virtual NamedContainerInterface<T>
{
public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  virtual PositionedNamedContainerInterface<T>* clone() const = 0;

  /** @} */

  virtual ~PositionedNamedContainerInterface()
  {}

public:
  virtual void addObject(std::shared_ptr<T> object, size_t objectIndex, const std::string& name, bool check = false) = 0;

  virtual void insertObject(std::shared_ptr<T> object, size_t objectIndex, const std::string& name) = 0;

  /**
   * @brief Link between position & name
   *
   */

  virtual size_t getObjectPosition(const std::string& name) const = 0;

  virtual const std::string& getObjectName(size_t objectIndex) const = 0;
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_POSITIONEDNAMEDCONTAINER_H
