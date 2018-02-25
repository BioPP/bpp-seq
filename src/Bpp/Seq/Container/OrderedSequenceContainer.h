//
// File OrderedSequenceContainer.h
// Author: Guillaume Deuchst
//         Julien Dutheil
// Last modification : Tuesday August 7 2003
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

#ifndef _ORDEREDSEQUENCECONTAINER_H_
#define _ORDEREDSEQUENCECONTAINER_H_

#include "../Sequence.h"
#include "SequenceContainer.h"
#include "OrderedValuesContainer.h"

namespace bpp
{

/**
 * @brief The OrderedSequenceContainer interface.
 *
 * Interface to manage indexed containers.
 * Sequences may be accessed via their indice, <i>i.e.</i> their position in the container.
 */
  class OrderedSequenceContainer:
    public virtual SequenceContainer,
    public virtual OrderedValuesContainer
  {
  public:
    OrderedSequenceContainer() {}
    virtual ~OrderedSequenceContainer() {}

  public:
		
    /**
     * @brief Retrieve a sequence object from the container.

     *
     * @param sequenceIndex The position of the sequence.
     * @return A reference toward the Sequence object with corresponding name.
     */
    virtual const Sequence& getSequence(size_t sequenceIndex) const = 0;

    /**
     * @brief Replace a sequence in the container.
     *
     * @param sequenceIndex The position of the sequence.
     * @param sequence      The sequence to add.
     * @param checkName     Tell if the container must check if the name of the sequence
     * is already used in the container before adding it.
     */

    virtual void setSequence(size_t sequenceIndex, const Sequence& sequence, bool checkName) = 0;

    /**
     * @brief Extract (and remove) a sequence from the container.
     *
     * @param sequenceIndex The position of the sequence.
     */

    virtual Sequence* removeSequence(size_t sequenceIndex) = 0;

    /**
     * @name Provide direct access to sequences content.
     *
     * @warning These operators allow you to modifiy the content of the sequences.
     * No checking is performed for your modifications, so use with care, or
     * consider using the setContent() methods.
     *
     * @{
     */

    virtual int& valueAt(const std::string& sequenceName, size_t elementIndex) = 0;

    virtual const int& valueAt(const std::string& sequenceName, size_t elementIndex) const = 0;

    /**
     * @brief Element access operator.
     *
     * Allows direct access to the data stored in the container.
     * 
     * @param sequenceIndex The sequence position.
     * @param elementIndex The element position within the sequence.
     */
    virtual int& valueAt(size_t sequenceIndex, size_t elementIndex) = 0;
    
    /**
     * @brief Element access operator.
     *
     * Allows direct access to the data stored in the container.
     * 
     * @param sequenceIndex The sequence position.
     * @param elementIndex The element position within the sequence.
     */
    virtual const int& valueAt(size_t sequenceIndex, size_t elementIndex) const = 0;

    /**
     * @brief Element access operator.
     *
     * Allows direct access to the data stored in the container.
     * This method is faster then the valueAt function, but input
     * parameters are not checked!
     * 
     * @param sequenceIndex The sequence position.
     * @param elementIndex The element position within the sequence.
     */

    virtual int& operator()(size_t sequenceIndex, size_t elementIndex) = 0;
    
    /**
     * @brief Element access operator.
     *
     * Allows direct access to the data stored in the container.
     * This method is faster then the valueAt function, but input
     * parameters are not checked!
     * 
     * @param sequenceIndex The sequence position.
     * @param elementIndex The element position within the sequence.
     */

    virtual const int& operator()(size_t sequenceIndex, size_t elementIndex) const = 0;
    /** @} */
	
  public:
		
    /**
     * @name SequenceContainer methods.
     *
     * @{
     */
    virtual std::string toString(const std::string& name) const = 0;  
    virtual const Sequence& getSequence(const std::string& name) const = 0;
    virtual void setSequence(const std::string& name, const Sequence& sequence, bool checkName) = 0;
    virtual Sequence* removeSequence(const std::string& name) = 0;
    virtual size_t getNumberOfSequences() const = 0;
    virtual std::vector<std::string> getSequencesNames() const = 0;
    virtual void setSequencesNames(const std::vector<std::string> & names, bool checkNames) = 0;
    
    /*
     * @brief For backward compatibility
     *
     */
    
    virtual const Comments& getComments(const std::string& name) const = 0;
    virtual void setComments(const std::string& name, const Comments& comments) = 0;
    /** @} */

  };

} //end of namespace bpp.

#endif	//_ORDEREDSEQUENCECONTAINER_H_

