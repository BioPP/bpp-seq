// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_VECTORMAPPEDCONTAINER_H
#define BPP_SEQ_CONTAINER_VECTORMAPPEDCONTAINER_H


#include "MappedNamedContainer.h"
#include "PositionedNamedContainer.h"
#include "VectorPositionedContainer.h"

// From the STL library:
#include <string>
#include <vector>
#include <iostream>

namespace bpp
{
/**
 * @brief The template VectorMappedContainer class.
 *
 * Objects are stored in a std::vector of shared pointers and in a
 * map of shared pointers.
 *
 * Object access is hence in \f$O(1)\f$ through indexes, and
 * \f$O(log(n))\f$ through names.
 *
 */
template<class T>
class VectorMappedContainer :
  public virtual PositionedNamedContainerInterface<T>,
  public MappedNamedContainer<T>,
  public VectorPositionedContainer<T>
{
private:
  /**
   * @brief vector of the names, in same order as objects
   */
  std::vector<std::string> vNames_;

  /**
   * @brief map <string, size_t> for the positions of the names
   */
  std::map<std::string, size_t> mNames_;

public:
  VectorMappedContainer() :
    MappedNamedContainer<T>(),
    VectorPositionedContainer<T>(),
    vNames_(),
    mNames_()
  {}

  VectorMappedContainer(const VectorMappedContainer& vsc) :
    MappedNamedContainer<T>(vsc),
    VectorPositionedContainer<T>(vsc),
    vNames_(vsc.vNames_),
    mNames_(vsc.mNames_)
  {}

  VectorMappedContainer<T>& operator=(const VectorMappedContainer& vsc)
  {
    MappedNamedContainer<T>::operator=(vsc);
    VectorPositionedContainer<T>::operator=(vsc);
    vNames_ = vsc.vNames_;
    mNames_ = vsc.mNames_;

    return *this;
  }

  virtual ~VectorMappedContainer() {}

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  VectorMappedContainer<T>* clone() const override
  {
    return new VectorMappedContainer(*this);
  }

  /** @} */

  /*
   * @brief size of the position vector
   * !! may be different than the actual number of objects
   *
   */
  size_t getSize() const override
  {
    return VectorPositionedContainer<T>::getSize();
  }

  /*
   * @brief real number of objects
   *
   */
  size_t getNumberOfObjects() const
  {
    return MappedNamedContainer<T>::getSize();
  }

  size_t getObjectPosition(const std::string& name) const override
  {
    auto it = mNames_.find(name);
    if (it == mNames_.end())
      throw Exception("VectorMappedContainer::getObjectPosition : Not found object with name " + name);

    return it->second;
  }

  const std::string& getObjectName(size_t objectIndex) const override
  {
    if (objectIndex >= getSize())
      throw IndexOutOfBoundsException("VectorMappedContainer::getObjectName.", objectIndex, 0, getSize() - 1);

    return vNames_[objectIndex];
  }

  using VectorPositionedContainer<T>::getObject;

  using VectorPositionedContainer<T>::object;

  using MappedNamedContainer<T>::getObject;

  using MappedNamedContainer<T>::object;

  using MappedNamedContainer<T>::hasObject;

  std::vector<std::string> getObjectNames() const override
  {
    return vNames_;
  }

  void setObjectNames(const std::vector<std::string>& names)
  {
    if (names.size() != vNames_.size())
      throw BadSizeException("VectorMappedContainer::setObjectNames: bad number of new names", vNames_.size(), names.size());

    mNames_.clear();

    for (size_t i = 0; i < names.size(); i++)
    {
      MappedNamedContainer<T>::changeName(vNames_[i], names[i]);
      mNames_[names[i]] = i;
    }

    vNames_ = names;
  }

  void setObjectName(size_t pos, const std::string& name)
  {
    MappedNamedContainer<T>::changeName(vNames_[pos], name);
    mNames_[name] = pos;
    vNames_[pos] = name;
  }

  void addObject(std::shared_ptr<T> newObject, size_t objectIndex, const std::string& name, bool check = false) override
  {
    VectorPositionedContainer<T>::addObject(newObject, objectIndex, check);
    MappedNamedContainer<T>::addObject(newObject, name, check);
    vNames_[objectIndex] = name;
    mNames_[name] = objectIndex;
  }

  using VectorPositionedContainer<T>::insertObject;

  void insertObject(std::shared_ptr<T> newObject, size_t objectIndex, const std::string& name) override
  {
    MappedNamedContainer<T>::addObject(newObject, name, true);
    VectorPositionedContainer<T>::insertObject(newObject, objectIndex);
    vNames_.insert(vNames_.begin() + static_cast<std::ptrdiff_t>(objectIndex), name);
    for (auto it : mNames_)
    {
      if (it.second >= objectIndex)
        it.second++;
    }

    mNames_[name] = objectIndex;
  }

  virtual void appendObject(std::shared_ptr<T> newObject, const std::string& name, bool checkNames = true)
  {
    MappedNamedContainer<T>::addObject(newObject, name, checkNames);
    VectorPositionedContainer<T>::appendObject(newObject);
    vNames_.push_back(name);
    mNames_[name] = vNames_.size() - 1;
  }

  std::shared_ptr<T> removeObject(size_t objectIndex) override
  {
    std::shared_ptr<T> obj = VectorPositionedContainer<T>::removeObject(objectIndex);
    MappedNamedContainer<T>::removeObject(vNames_[objectIndex]);
    mNames_.erase(vNames_[objectIndex]);
    for (auto it : mNames_)
    {
      if (it.second > objectIndex)
        it.second--;
    }
    vNames_.erase(vNames_.begin() + static_cast<std::ptrdiff_t>(objectIndex));
    return obj;
  }

  void deleteObject(size_t objectIndex) override
  {
    VectorPositionedContainer<T>::deleteObject(objectIndex);
    MappedNamedContainer<T>::deleteObject(vNames_[objectIndex]);

    mNames_.erase(vNames_[objectIndex]);
    for (auto it : mNames_)
    {
      if (it.second > objectIndex)
        it.second--;
    }
    vNames_.erase(vNames_.begin() + static_cast<std::ptrdiff_t>(objectIndex));
  }


  std::shared_ptr<T> removeObject(const std::string& name) override
  {
    return removeObject(mNames_[name]);
  }

  void deleteObject(const std::string& name) override
  {
    deleteObject(mNames_[name]);
  }

  void addObject_(std::shared_ptr<T> newObject, size_t objectIndex, const std::string& name, bool check = false) const
  {
    VectorPositionedContainer<T>::addObject_(newObject, objectIndex, check);
    MappedNamedContainer<T>::addObject_(newObject, name, check);
    const_cast<std::vector<std::string>& >(vNames_)[objectIndex] = name;
    const_cast<std::map<std::string, size_t>&>(mNames_)[name] = objectIndex;
  }

  void clear() override
  {
    MappedNamedContainer<T>::clear();
    VectorPositionedContainer<T>::clear();
    vNames_.clear();
    mNames_.clear();
  }

  void nullify() override
  {
    MappedNamedContainer<T>::nullify();
    VectorPositionedContainer<T>::nullify();
  }
};
} // end of namespace bpp.

#endif // BPP_SEQ_CONTAINER_VECTORMAPPEDCONTAINER_H
