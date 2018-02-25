//
// File: SequenceContainer.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Fri Jul 25 2003
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

#ifndef _SEQUENCECONTAINER_H_
#define _SEQUENCECONTAINER_H_

#include "../Sequence.h"
#include "SequencedValuesContainer.h"

// From the STL:
#include <string>

namespace bpp
{

/**
 * @brief The SequenceContainer interface.
 *
 * This interface is the most general one in the container hierarchy
 * for sequences. No assumption is made on the sequences in the
 * container (no ordering, no alignment). Sequences may be retrieved
 * using their names, which must be unique.
 *
 * The container is the only one responsible for the
 * allocation/destruction of sequences it contains. This means that
 * any sequence passed to it will be <strong>copied</strong> into the
 * container. The container also provides methods that send const
 * pointers towards these sequences (without performing any copy of
 * the underlying objects).
 *
 * Notes :
 * 1. methods for adding sequences to the container are not declared here
 * (so they can't be used throught this interface),
 * because these methods take sequence container's type specific parameters
 * (i.e. a key for map sequence containers);
 * 2. to delete a sequence from a container, one must use the appropriate method
 * (removeSequence()).
 * These methods performs a few check, and properly update pointers.
 * You should never delete a sequence from a container by yourself.
 *
 * @see Sequence
 */

  class SequenceContainer:
    public virtual SequencedValuesContainer
  {
  public:
    SequenceContainer() {}
    virtual ~SequenceContainer() {}

  public:
		
    /**
     * @brief Retrieve a sequence object from the container.
     *
     * @param name The name of the sequence.
     * @return A reference toward the Sequence with corresponding name.
     */
    virtual const Sequence& getSequence(const std::string& name) const = 0;
 
    /**
     * @brief Replace a sequence in the container.
     *
     * @param name      The name of the sequence.
     * @param sequence  The sequence to add.
     * @param checkName Tell if the container must check if the name of the sequence
     * is already used in the container before adding it.
     */
    virtual void setSequence(const std::string& name, const Sequence& sequence, bool checkName) = 0;

    /**
     * @brief Extract (and remove) a sequence from the container.
     *
     * @param name The name of the sequence.
     */
    virtual Sequence* removeSequence(const std::string& name) = 0;
		
    /**
     * @name Provide direct access to sequences content.
     *
     * @warning These operators allow you to modifiy the content of the sequences.
     * No checking is performed for your modifications, so use with care, or
     * consider using the setContent() methods.
     *
     * @{
     */
    
    /**
     * @brief Element access function.
     *
     * Allows direct access to the data stored in the container.
     * 
     * @param sequenceName The sequence name.
     * @param elementIndex The element position within the sequence.
     */
    virtual int& valueAt(const std::string& sequenceName, size_t elementIndex) = 0;

    /**
     * @brief Element access function.
     *
     * Allows direct access to the data stored in the container.
     * 
     * @param sequenceName The sequence name.
     * @param elementIndex The element position within the sequence.
     */
    virtual const int& valueAt(const std::string& sequenceName, size_t elementIndex) const = 0;

    /**
     * @brief Element access operator.
     *
     * Allows direct access to the data stored in the container.
     * This method is faster then the valueAt function, but input
     * parameters are not checked!
     * 
     * @param sequenceName The sequence name.
     * @param elementIndex The element position within the sequence.
     */
    virtual int& operator()(const std::string& sequenceName, size_t elementIndex) = 0;

    /**
     * @brief Element access operator.
     *
     * Allows direct access to the data stored in the container.
     * This method is faster then the valueAt function, but input
     * parameters are not checked!
     * 
     * @param sequenceName The sequence name.
     * @param elementIndex The element position within the sequence.
     */
    virtual const int& operator()(const std::string& sequenceName, size_t elementIndex) const = 0;
    /** @} */
  };

} //end of namespace bpp.

#endif	// _SEQUENCECONTAINER_H_

