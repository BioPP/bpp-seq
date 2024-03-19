// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_ALIGNMENTDATA_H
#define BPP_SEQ_CONTAINER_ALIGNMENTDATA_H

#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/VectorTools.h>
#include "SequenceData.h"
#include "../CoreSite.h"

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
template<class HashType = std::string>
class TemplateAlignmentDataInterface :
  public virtual TemplateSequenceDataInterface<HashType>
{
public:
  TemplateAlignmentDataInterface() {}
  virtual ~TemplateAlignmentDataInterface() {}

  TemplateAlignmentDataInterface* clone() const override = 0;


  /**
   * @brief Get a site from a given position.
   *
   * @param siteIndex The position
   */
  virtual const CoreSiteInterface& site(size_t siteIndex) const = 0;

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
   * @brief Get all coordinates of sites.
   *
   * @return A vector with all site coordinates.
   */
  virtual Vint getSiteCoordinates() const = 0;

  /**
   * @brief Set all coordinates of sites.
   *
   * @param vPositions A vector with all site coordinates.
   */
  virtual void setSiteCoordinates(const Vint& coordinates) = 0;
};

using AlignmentDataInterface = TemplateAlignmentDataInterface<std::string>;
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_ALIGNEDVALUESCONTAINER_H
