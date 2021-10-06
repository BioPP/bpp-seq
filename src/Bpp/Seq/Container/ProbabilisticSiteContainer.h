//
// File ProbabilisticSiteContainer.h
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

#ifndef _PROBABILISTIC_SITECONTAINER_H_
#define _PROBABILISTIC_SITECONTAINER_H_

#include "../ProbabilisticSite.h"
#include "AlignedValuesContainer.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief The ProbabilisticSiteContainer interface.
 *
 * Container implementing the ProbabilisticSiteContainer interface
 * deal with <em>aligned</em> probabilistic sequences.
 * This interface provides methods to retrieve, add or set sites in the alignment.
 * All site objects are cloned befored being added and retrieved.
 */

  class ProbabilisticSiteContainer :
    public virtual AlignedValuesContainer
  {
  public:
    ProbabilisticSiteContainer() {}
    virtual ~ProbabilisticSiteContainer() {}

    ProbabilisticSiteContainer* clone() const = 0;

  public:
    /**
     * @brief Get a site from the container.
     *
     * @param siteIndex The position of the site in the container.
     * @return A site objet corresponding to site i in the alignment.
     * @throw IndexOutOfBoundsException If the specified site does not exists.
     */
  
    virtual const std::shared_ptr<ProbabilisticSite> getSite(size_t siteIndex) const = 0;
    virtual std::shared_ptr<ProbabilisticSite> getSite(size_t siteIndex) = 0;

    const CruxSymbolListSite& getSymbolListSite(size_t siteIndex) const
    {
      return *getSite(siteIndex);
    }

    CruxSymbolListSite& getSymbolListSite(size_t siteIndex)
    {
      return *getSite(siteIndex);
    }

    /**
     * @brief Add a site at the end of the container.
     *
     * @param site          The site to add.
     * @param checkPosition Look if the position of the new site match a position attribute in the container.
     */
    
    virtual void appendSite(std::shared_ptr<ProbabilisticSite> site, bool checkPosition = false) = 0;

  };
} // end of namespace bpp.

#endif  // _PROBABILISTIC_SITECONTAINER_H_

