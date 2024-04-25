// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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
   * @param coordinate The new position of the site.
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
