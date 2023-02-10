//
// File: CoreSite.h
// Authors:
//   Murray Patterson
//   Julien Dutheil
// Created: 2015-10-12 00:00:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_CORESITE_H
#define BPP_SEQ_CORESITE_H

#include <Bpp/Clonable.h>
#include "CoreSymbolList.h"

namespace bpp
{
/**
 * @brief The core site interface.
 *
 * The core interface for sites manipulation. It is very similar to
 * the CoreSequence interface (a site is a vertical sequence!). Sites
 * have a 'coordinate' attribute.
 * This attribute stands for an index in an alignment, and may be
 * used as a unique identifier, in the same manner that names identify
 * sequence objects.
 */
class CoreSiteInterface :
  public virtual CruxSymbolListInterface
{
public:
  /**
   * @name The Clonable interface
   *
   * @{
   */
  CoreSiteInterface* clone() const override = 0;

  /**
   * @}
   */

  // class destructor
  virtual ~CoreSiteInterface() {}

  /**
   * @name Setting/getting the position of the site.
   *
   * @{
   */

  /**
   * @brief Get the coordinate associated to this site.
   *
   * @return The coordinate of this site.
   */
  virtual int getCoordinate() const = 0;

  /**
   * @brief Set the position of this site.
   *
   * @param position The new position of the site.
   */
  virtual void setCoordinate(int coordinate) = 0;

  /**
   * @}
   */

  /**
   * @brief get value of a state at a position
   *
   * @param sequencePosition index of the looked value in the site
   * @param state            state in the alphabet
   * @return                 The state value at the given position.
   */
  virtual double getStateValueAt(size_t sequencePosition, int state) const override = 0;

};

/**
 * @brief A partial implementation of the CoreSite interface.
 */
class AbstractCoreSite :
  public virtual CoreSiteInterface
{
private:
  /**
   * @brief The position associated with this site.
   */
  int coordinate_;

public:
  /**
   * @brief Constructor of the AbstractCoreSite object.
   *
   * Construct an 'empty' object, with a coordinate set to 0.
   */
  AbstractCoreSite() :
    coordinate_(0) {}

  /**
   * @brief Constructor of the AbstractCoreSite object.
   *
   * @param coordinate The coordinate of the site.
   */
  AbstractCoreSite(int coordinate) :
    coordinate_(coordinate) {}

  /**
   * @name The copy constructors.
   *
   * @{
   */
  AbstractCoreSite(const CoreSiteInterface& site) :
    coordinate_(site.getCoordinate()) {}

  AbstractCoreSite(const AbstractCoreSite& site) :
    coordinate_(site.coordinate_) {}

  /**
   * @}
   */

  /**
   * @name The assignment operators.
   *
   * @{
   */
  AbstractCoreSite& operator=(const CoreSiteInterface& site)
  {
    coordinate_ = site.getCoordinate();
    return *this;
  }

  AbstractCoreSite& operator=(const AbstractCoreSite& site)
  {
    coordinate_ = site.coordinate_;
    return *this;
  }

  /**
   * @}
   */

  // class destructor
  virtual ~AbstractCoreSite() {}

public:
  int getCoordinate() const override { return coordinate_; }

  void setCoordinate(int coordinate) override { coordinate_ = coordinate; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_CORESITE_H
