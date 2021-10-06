//
// File: CoreSequence.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Tue Sep 29 2015
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

#ifndef _CORESEQUENCE_H_
#define _CORESEQUENCE_H_

// From the STL:
#include <string>
#include <vector>

#include <Bpp/Clonable.h>
#include "Commentable.h"

namespace bpp
{

/**
 * @brief The core sequence interface. 
 *
 * This interface specifies the 'core' functionality of a sequence, that is,
 * an ordered set of states, a name and some meta data (comments).
 */
  
  class CoreSequence:
    public virtual Clonable
  {
  public:
    CoreSequence() {}

    virtual ~CoreSequence() {}

  public:
  
#ifndef NO_VIRTUAL_COV
    CoreSequence* clone() const = 0;
#endif
  
    /**
     * @name Setting/getting the name of the sequence.
     *
     * @{
     */
     
    /**
     * @brief Get the name of this sequence.
     *
     * @return This sequence name.
     */
    virtual const std::string& getName() const = 0;
    
    /**
     * @brief Set the name of this sequence.
     *
     * @param name The new name of the sequence.
     */
    virtual void setName(const std::string& name) = 0;    
    /** @} */
    
    
    /**
     * @brief Get the comments.
     *
     * @return The comments.
     */
     
    virtual const Comments& getComments() const = 0;

    /**
     * @brief Set the comments.
     *
     * @param comments The new comments.
     */

    virtual void setComments(const Comments& comments) = 0;
  
    /**
     * @name Adjusting the size of the sequence.
     *
     * @{
     */
     
    /**
     * @brief Set up the size of a sequence from the right side.
     *
     * All new characters are filled with gaps.
     * If the specified size is < to the sequence size, the sequence will be truncated.
     *
     * @param newSize The new size of the sequence.
     */
    virtual void setToSizeR(size_t newSize) = 0;
    
    /**
     * @brief Set up the size of a sequence from the left side.
     *
     * All new characters are filled with gaps.
     * If the specified size is < to the sequence size, the sequence will be truncated.
     *
     * @param newSize The new size of the sequence.
     */
    virtual void setToSizeL(size_t newSize) = 0;

    /** @} */

  };


/**
 * @brief A partial implementation of the CoreSequence interface. 
 */
  class AbstractCoreSequence :
    public virtual CoreSequence,
    public virtual Commentable
  {
  private:

    /**
     * @brief The sequence name.
     */
    std::string name_;

  public:

    /**
     * @brief Constructor of the AbstractCoreSequence object.
     * 
     * @param name     The sequence name.
     */
    AbstractCoreSequence(const std::string& name):
      Commentable(),
      name_(name) {}

    /**
     * @brief Constructor of the AbstractCoreSequence object.
     * 
     * @param name     The sequence name.
     * @param comments Comments to add to the sequence.
     */
    AbstractCoreSequence(const std::string& name, const Comments& comments):
      Commentable(comments),
      name_(name) {}

    AbstractCoreSequence():
      Commentable(),
      name_() {}

    AbstractCoreSequence(const AbstractCoreSequence& s):
      Commentable(s),
      name_(s.name_) {}
    
    AbstractCoreSequence& operator=(const AbstractCoreSequence& s)
    {
      Commentable::operator=(s);
      name_ = s.name_;
      return *this;
    }
  
    AbstractCoreSequence(const CoreSequence& s):
      Commentable(s.getComments()),
      name_(s.getName())
    {
    }
    
    AbstractCoreSequence& operator=(const CoreSequence& s)
    {
      Commentable::setComments(s.getComments());
      name_ = s.getName();
      return *this;
    }
   
    virtual ~AbstractCoreSequence() {}
        
    
  public:
  
    /**
     * @name The Clonable interface
     *
     * @{
     */
    virtual AbstractCoreSequence* clone() const = 0;
    /** @} */
        
    
    /**
     * @name Setting/getting the name of the sequence.
     *
     * @{
     */
     
    const std::string& getName() const { return name_; }
    
    void setName(const std::string& name) { name_ = name; }

    const Comments& getComments() const
    {
      return Commentable::getComments();
    }

    void setComments(const Comments& comments)
    {
      Commentable::setComments(comments);
    }    

    /** @} */
    
  };

} //end of namespace bpp.

#endif // _CORESEQUENCE_H_

