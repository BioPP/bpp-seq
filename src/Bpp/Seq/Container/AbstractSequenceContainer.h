//
// File AbstractSequenceContainer.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
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

#ifndef _ABSTRACTSEQUENCECONTAINER_H_
#define _ABSTRACTSEQUENCECONTAINER_H_

#include "../Sequence.h"
#include "OrderedSequenceContainer.h"
#include "AbstractValuesContainer.h"

namespace bpp
{

/**
 * @brief Partial implementation of the OrderedSequenceContainer interface.
 *
 * This abstract class provides an alphabet and comments, with associated methods.
 */
  class AbstractSequenceContainer:
    public virtual OrderedSequenceContainer,
    public AbstractValuesContainer
  {
  public:

    /**
     * @brief This constructor initialize the alphabet pointer.
     *
     * This constructor is to be called by constructors of derived classes.
     *
     * @param alpha The alphabet to be associated to this container.
     */
    AbstractSequenceContainer(const Alphabet* alpha):
      AbstractValuesContainer(alpha) {}
		
    AbstractSequenceContainer(const AbstractSequenceContainer& sc):
      AbstractValuesContainer(sc)
    {
    }
    
    AbstractSequenceContainer& operator=(const AbstractSequenceContainer& sc)
    {
      AbstractValuesContainer::operator=(sc);
      return *this;
    }

    /**
     * @brief Copy constructor from any SequenceContainer object.
     *
     * @param sc Another sequence container.
     */
    AbstractSequenceContainer(const SequenceContainer& sc):
      AbstractValuesContainer(sc.getGeneralComments(), sc.getAlphabet())
    {
    }

    /**
     * @brief Assignation operator from any SequenceContainer object.
     *
     * @param sc Another sequence container.
     */

    AbstractSequenceContainer& operator=(const SequenceContainer& sc)
    {
      alphabet_ = sc.getAlphabet();
      setGeneralComments(sc.getGeneralComments());
      
      return *this;
    }

    virtual ~AbstractSequenceContainer() {}
		
  public:
	
    std::string toString(const std::string& name) const
    {
      return getSequence(name).toString();
    }

    const Comments& getComments(const std::string& name) const
    {
      return getSequence(name).getComments();
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
    virtual const std::string& getName(size_t sequenceIndex) const
    {
      return getSequence(sequenceIndex).getName();
    }
    
    virtual std::string toString(size_t sequenceIndex) const
    {
      return getSequence(sequenceIndex).toString();
    }

    virtual const Comments& getComments(size_t sequenceIndex) const
    {
      return getSequence(sequenceIndex).getComments();
    }

    virtual void setComments(size_t sequenceIndex, const Comments& comments) = 0;
    /** @} */

  };

} //end of namespace bpp.

#endif // _ABSTRACTSEQUENCECONTAINER_H_

