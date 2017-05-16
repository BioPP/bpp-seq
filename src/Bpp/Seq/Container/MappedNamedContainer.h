//
// File MappedNamedContainer.h
// Authors : Guillaume Deuchst
//           Julien Dutheil
//           Sylvain Gaillard
//           Laurent Guéguen (for template feature)
// Last modification : lundi 27 mars 2017, à 10h 15
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

#ifndef _MAPPED_NAMED_CONTAINER_H_
#define _MAPPED_NAMED_CONTAINER_H_

#include <string>
#include <memory>
#include <map>

#include "NamedContainer.h"
#include <Bpp/Clonable.h>

namespace bpp
{

/**
 * @brief MappedNamedContainer class
 *
 * Objects are stored using a key std::string, in a map object.
 * 
 */

  template <class T>
  class MappedNamedContainer :
    virtual public NamedContainer<T>,
    virtual public Clonable
  {
  private:
    std::map<std::string, std::shared_ptr<T> > mObjects_;
    
  public:
    MappedNamedContainer() :
      mObjects_() 
    {
    }

    MappedNamedContainer(const std::map<std::string, std::shared_ptr<T> >& ms) :
      mObjects_(ms) 
    {
    }
    
    MappedNamedContainer(const MappedNamedContainer& msc) :
      mObjects_(msc.mObjects_) 
    {
    }
    
    /**
     * @name The clonable interface
     * 
     * @{
     */
    MappedNamedContainer* clone() const { return new MappedNamedContainer(*this); }
    /**
     * @}
     */


    MappedNamedContainer& operator=(const MappedNamedContainer& msc)
    {
      mObjects_=msc.mObjects_;
      return *this;
    }

    virtual ~MappedNamedContainer() 
    {
    }
    

    /**
     * @brief Get a object.
     *
     * @param name The key of the object to retrieve.
     * @return The object associated to the given key.
     */

    std::shared_ptr<T> getObject(const std::string& name) const
    {
      const auto it=mObjects_.find(name);
      if (it==mObjects_.end())
        throw Exception("MappedNamedContainer::getObject : unknown name " + name);
      
      return it->second;
    }

    std::shared_ptr<T> getObject(const std::string& name) 
    {
      auto it=mObjects_.find(name);
      if (it==mObjects_.end())
        throw Exception("MappedNamedContainer::getObject : unknown name " + name);
        
      return it->second;
    }

    bool hasObject(const std::string& name) const
    {
      return mObjects_.find(name)!=mObjects_.end();
    }
    

    /**
     * @brief Set a object.
     *
     * @param name The key of the object.
     * @param object The new object that will be associated to the key.
     * @param checkName Tell is the object name must be checked.
     */
    
    void addObject(std::shared_ptr<T> object, const std::string& name, bool checkName = false)
    {
      if (mObjects_.find(name)!=mObjects_.end())
        throw Exception("MappedNamedContainer::setObject : Object's name already exists in container : " + name);

      mObjects_[name] = object;
    }

    /**
     * @brief Remove a object.
     * 
     * @param name The key of the object.
     * @return The object previously associated to the given key.
     */
    
    std::shared_ptr<T> removeObject(const std::string& name)
    {
      if (mObjects_.find(name)!=mObjects_.end())
        throw Exception("MappedNamedContainer::removeObject : Object's name does bot exist in container : " + name);
      
      std::shared_ptr<T> obj = mObjects_[name];
      mObjects_.erase(name);
      return obj;
      
    }

    /**
     * @return All objects keys.
     */

    std::vector<std::string> getObjectsNames() const
    {
      std::vector<std::string> vNames;
      
      for (auto it : mObjects_)
        vNames.push_back(it.first);

      return vNames;
    }
    
    size_t getSize() const { return mObjects_.size(); }
    
    void clear()
    {
      mObjects_.clear();
    }
    

  }; 
} //end of namespace bpp.

#endif // _MAPPEDNAMEDCONTAINER_H_

