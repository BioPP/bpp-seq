// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_SITECONTAINEREXCEPTIONS_H
#define BPP_SEQ_CONTAINER_SITECONTAINEREXCEPTIONS_H


// From STL
#include <string>
#include <Bpp/Exceptions.h>

namespace bpp
{
/**
 * @brief The site not found exception base class.
 *
 * @see Exception
 */
class SiteNotFoundException :
	public Exception
{
protected:
/**
 * @brief The id of the site that was to be found.
 */
const std::string id;

public:
// Class constructor

/**
 * @brief Build a new SiteNotFoundException object.
 *
 * @param text A message to be passed to the exception hierarchy.
 * @param sId  A the id of the site that was to be found.
 */
SiteNotFoundException(const char*   text, const char* sId = "");

/**
 * @brief Build a new SiteNotFoundException object.
 *
 * @param text A message to be passed to the exception hierarchy.
 * @param sId  A the id of the site that was to be found.
 */
SiteNotFoundException(const std::string& text, const std::string& sId = "");

// Class destructor
~SiteNotFoundException();

public:
/**
 * @brief Get the id of the site that was to be found.
 *
 * @return The id of the site that was to be found.
 */
virtual const std::string getSiteId() const;
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SITECONTAINEREXCEPTIONS_H
