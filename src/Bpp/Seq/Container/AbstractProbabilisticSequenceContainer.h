//
// File AbstractProbabilisticSequenceContainer.h
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

#ifndef _ABSTRACT_PROB_SEQUENCECONTAINER_H_
#define _ABSTRACT_PROB_SEQUENCECONTAINER_H_

#include "../ProbabilisticSequence.h"
#include "AbstractValuesContainer.h"
#include "ProbabilisticSequenceContainer.h"
#include <Bpp/Exceptions.h>

namespace bpp
{

/**
 * @brief Partial implementation of the OrderedSequenceContainer interface.
 *
 * This abstract class provides an alphabet and comments, with associated methods.
 */
  class AbstractProbabilisticSequenceContainer:
    virtual public AbstractValuesContainer,
    virtual public ProbabilisticSequenceContainer
  {
  public:

    /**
     * @brief This constructor initialize the alphabet pointer.
     *
     * This constructor is to be called by constructors of derived classes.
     *
     * @param alpha The alphabet to be associated to this container.
     */
    AbstractProbabilisticSequenceContainer(const Alphabet* alpha):
      AbstractValuesContainer(alpha) {}
		
    AbstractProbabilisticSequenceContainer(const AbstractProbabilisticSequenceContainer& sc):
      AbstractValuesContainer(sc) {}
    
    AbstractProbabilisticSequenceContainer& operator=(const AbstractProbabilisticSequenceContainer& sc)
    {
      AbstractValuesContainer::operator=(sc);
      return *this;
    }

    virtual ~AbstractProbabilisticSequenceContainer() {}
		
  public:
	
    virtual const std::shared_ptr<BasicProbabilisticSequence> getSequence(std::size_t i) const = 0;

    virtual const std::shared_ptr<BasicProbabilisticSequence> getSequence(const std::string& name) const = 0;

    /**
     * @brief Add a probabilistic sequence to the container.
     *
     * @param sequence  The sequence to add.
     * @param checkName Tell if the container must check if the name of the sequence
     * is already used in the container before adding it.
     * @throw Exception Any other kind of exception, if the name of the sequence is
     * already used, are whatever else depending on the implementation.
     */

    virtual void addSequence(const std::shared_ptr<BasicProbabilisticSequence> sequence, bool checkName = true) = 0;

    /**
     * @brief Add a basic sequence to the container, through
     * conversion to a probabilistic sequence.
     *
     * @param sequence  The sequence to add.
     * @param checkName Tell if the container must check if the name of the sequence
     * is already used in the container before adding it.
     * @throw Exception Any other kind of exception, if the name of the sequence is
     * already used, are whatever else depending on the implementation.
     */
    
    virtual void addSequence(const Sequence& sequence, bool checkName = true) = 0;

    /**
     * @brief Output to string
     *
     **/
    
    std::string toString(const std::string& name) const
    {
      return getSequence(name)->toString();
    }

    const Comments& getComments(const std::string& name) const
    {
      return getSequence(name)->getComments();
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
      return getSequence(sequenceIndex)->getName();
    }
    
    virtual std::string toString(size_t sequenceIndex) const
    {
      return getSequence(sequenceIndex)->toString();
    }

    const Comments& getGeneralComments() const
    {
      return Commentable::getComments();
    }

    void setGeneralComments(const Comments& comments)
    {
      Commentable::setComments(comments);
    }

    void deleteGeneralComments()
    {
      Commentable::clearComments();
    }
    
    virtual const Comments& getComments(size_t sequenceIndex) const
    {
      return getSequence(sequenceIndex)->getComments();
    }

    virtual void setComments(size_t sequenceIndex, const Comments& comments) = 0;
    /** @} */

  };

} //end of namespace bpp.

#endif // _ABSTRACT_PROB_SEQUENCECONTAINER_H_

