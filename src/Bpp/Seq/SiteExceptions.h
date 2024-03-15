// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SITEEXCEPTIONS_H
#define BPP_SEQ_SITEEXCEPTIONS_H

#include <Bpp/Exceptions.h>
#include "CoreSite.h"


namespace bpp
{

/**
 * @brief The site exception base class.
 *
 * @see Exception
 */
class SiteException :
  public Exception
{
private:
  /**
   * @brief A reference toward a site object.
   */
  const CoreSiteInterface* site_;

public:
  // Class constructor

  /**
   * @brief Build a new SiteException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param s    A const reference toward the site that threw the exception.
   */
  SiteException(const std::string& text, const CoreSiteInterface* s) :
    Exception(text + (s != 0 ? "(" + TextTools::toString(s->getCoordinate()) + ")" : std::string(""))),
    site_(s)
  {}

  SiteException(const SiteException& se):
    Exception(se),
    site_(se.site_)
  {}

  SiteException& operator=(const SiteException& se)
  {
    Exception::operator=(se);
    site_ = se.site_;
    return *this;
  }

  // Class destructor
  virtual ~SiteException() {}

public:
  /**
   * @brief Get the site that threw the exception.
   *
   * @return A const reference toward the site.
   */
  virtual const CoreSiteInterface* getSite() const { return site_; }
};

/**
 * @brief Exception sent when a empty site is found.
 */
class EmptySiteException :
  public SiteException
{
public:
  EmptySiteException(const std::string& text, const CoreSiteInterface* s) :
	 SiteException(text, s) {}

  virtual ~EmptySiteException() {}
};

/**
 * @brief Exception sent when a site containing gap is found.
 */
class SiteWithGapException :
  public SiteException
{
public:
  SiteWithGapException(const std::string& text, const CoreSiteInterface* s) :
	 SiteException(text, s) {}

  virtual ~SiteWithGapException() {}
};
} // end of namespace bpp.
#endif // BPP_SEQ_SITEEXCEPTIONS_H
