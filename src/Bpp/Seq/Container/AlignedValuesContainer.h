//
// File AlignedValuesContainer.h
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

#ifndef _ALIGNED_VALUES_CONTAINER_H_
#define _ALIGNED_VALUES_CONTAINER_H_

#include "OrderedValuesContainer.h"
#include "../CruxSymbolListSite.h"

#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/VectorTools.h>
#include "../Sequence.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief The Container of Aligned Values interface.
 *
 * Container implementing the AlignedValuesContainer interface deal
 * with <em>aligned</em> data.
 */

  class AlignedValuesContainer :
    virtual public OrderedValuesContainer
  {

  public:
    AlignedValuesContainer() {}
    virtual ~AlignedValuesContainer() {}

    AlignedValuesContainer* clone() const = 0;

    
    /**
     * @brief Get a CruxSymbolListSite from a given position.
     *
     * @param siteIndex The position 
     */
    
    virtual const CruxSymbolListSite& getSymbolListSite(size_t siteIndex) const = 0;

    virtual CruxSymbolListSite& getSymbolListSite(size_t siteIndex) = 0;

    /**
     * @brief Remove a continuous range of sites in the container.
     *
     * @param siteIndex The position of the first site in the container.
     * @param length The length of the region to delete, starting at pposition siteIndex.
     * @throw IndexOutOfBoundsException If the specified range is not valid.
     */
    virtual void deleteSites(size_t siteIndex, size_t length) = 0;

    /**
     * @brief Get the number of aligned positions in the container.
     *
     * @return The number of sites in the container.
     */
    
    virtual size_t getNumberOfSites() const = 0;

    /**
     * @brief Set all positions attributes.
     */
    
    virtual void reindexSites() = 0;

    /**
     * @brief Get all position attributes of sites.
     *
     * @return A vector with all site positions.
     */

    virtual Vint getSitePositions() const = 0;

    /**
     * @brief Set all position attributes of sites.
     *
     * @param A vector with all site positions.
     */
    
    virtual void setSitePositions(Vint vPositions) = 0;

  };
} // end of namespace bpp.

#endif  // _ALIGNED_VALUES_CONTAINER_H_

