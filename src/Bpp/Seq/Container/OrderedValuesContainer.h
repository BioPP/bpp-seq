//
// File OrderedValuesContainer.h
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

#ifndef _ORDERED_VALUES_CONTAINER_H_
#define _ORDERED_VALUES_CONTAINER_H_

#include "SequencedValuesContainer.h"
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/VectorTools.h>

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief The Container of Ordered Values interface.
 *
 * Container implementing the OrderedValuesContainer interface deal
 * with named and indexed.
 * This interface provides access methods to the doubles stored in the
 * data.
 */

  class OrderedValuesContainer :
    virtual public SequencedValuesContainer
  {
    
  public:
    OrderedValuesContainer() {}
    virtual ~OrderedValuesContainer() {}

    OrderedValuesContainer* clone() const = 0;

    /**
     * @brief Convert a particular sequence to a string.
     *
     * @param sequenceIndex The position of the sequence.
     * @return A string describing the content of the sequence.
     * @throw IndexOutOfBoundsException If the position does not match any sequence in the container.
     */

    virtual std::string toString(size_t sequenceIndex) const = 0;

    /**
     * @brief Get the name of a particular row of the alignement (aka sequence).
     *
     * @param sequenceIndex The position of the sequence.
     * @return The name of the sequence at position 'sequenceIndex'.
     * @throw IndexOutOfBoundsException If the position does not match any sequence in
     * the container.
     */
    
    virtual const std::string& getName(size_t sequenceIndex) const = 0;

    /**
     * @brief Get the position of a sequence in sequence container from its name.
     *
     * @param name The name of the sequence.
     * @return The position of the sequence with name 'name', if it exists.
     * @throw SequenceNotFoundException If no sequence with name 'name' could be found.
     */

    virtual size_t getSequencePosition(const std::string & name) const = 0;

    /*
     * @brief get value of a state in a position
     * @param siteIndex  index of the site 
     * @param sequenceIndex index of the sequence in the container
     * @param state  state in the alphabet
     */
  
    virtual double getStateValueAt(size_t siteIndex, size_t sequenceIndex, int state) const  = 0;
  
    virtual double operator()(size_t siteIndex, size_t sequenceIndex, int state) const = 0;

    /**
     * @brief Get comments of a particular sequence.
     *
     * @param sequenceIndex The position of the sequence.
     * @return The comments associated to sequence at position 'sequenceIndex'.
     * @throw IndexOutOfBoundsException If the position does not match any sequence in
     * the container.
     */

    virtual const Comments& getComments(size_t sequenceIndex) const = 0;

    /**
     * @brief Set the comments of a particular sequence
     *
     * @param sequenceIndex The position of the sequence.
     * @param comments      The comments to set to sequence with position 'i'.
     * @throw IndexOutOfBoundsException If the position does not match any sequence in
     * the container.
     */

    virtual void setComments(size_t sequenceIndex, const Comments & comments) = 0;

  };
} // end of namespace bpp.

#endif  // _ORDERED_VALUES_CONTAINER_H_

