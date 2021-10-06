//
// File AbstractValuesContainer.h
// Created by: Laurent Guéguen
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

#ifndef _ABSTRACT_VALUES_CONTAINER_H_
#define _ABSTRACT_VALUES_CONTAINER_H_

#include "../Alphabet/Alphabet.h"
#include "../Commentable.h"
#include "OrderedValuesContainer.h"
#include <Bpp/Exceptions.h>

namespace bpp
{

/**
 * @brief Partial implementation of the OrderedSequenceContainer interface.
 *
 * This abstract class provides an alphabet and comments, with associated methods.
 */
  class AbstractValuesContainer:
    public virtual OrderedValuesContainer,
    public Commentable
  {
  protected:

    /**
     * @brief The container's alphabet.
     */
    const Alphabet* alphabet_;

  public:

    /**
     * @brief This constructor initialize the alphabet pointer.
     *
     * This constructor is to be called by constructors of derived classes.
     *
     * @param alpha The alphabet to be associated to this container.
     */
    AbstractValuesContainer(const Alphabet* alpha):
      Commentable(), alphabet_(alpha) {}
		
    AbstractValuesContainer(const Comments& comments, const Alphabet* alpha):
      Commentable(comments),
      alphabet_(alpha) {}

    AbstractValuesContainer(const AbstractValuesContainer& sc):
      Commentable(sc), alphabet_(sc.alphabet_) {}
    
    AbstractValuesContainer& operator=(const AbstractValuesContainer& sc)
    {
      Commentable::operator=(sc);
      alphabet_ = sc.alphabet_;
      return *this;
    }

    virtual ~AbstractValuesContainer() {}
		
  public:

    /**
     * @name From the SequencedValuesContainer interface
     *
     * @{
     */
    const Alphabet* getAlphabet() const { return alphabet_; }

    const Comments& getGeneralComments() const
    {
      return Commentable::getComments();
    }

    void setGeneralComments(const Comments& comments)
    {
      Commentable::setComments(comments);
    }

    /*
     * @brief For backward compatibility
     *
     */
    
    void deleteGeneralComments()
    {
      Commentable::clearComments();
    }

    void setComments(const std::string& name, const Comments& comments)
    {
      size_t pos = getSequencePosition(name);
      setComments(pos, comments);
    }

    /** @} */

    /**
     * @name From the OrderedSequenceContainer interface
     *
     * @{
     */

    virtual const Comments& getComments(size_t sequenceIndex) const = 0;

    virtual void setComments(size_t sequenceIndex, const Comments& comments) = 0;
    /** @} */

  };

} //end of namespace bpp.

#endif // _ABSTRACT_VALUES_CONTAINER_H_

