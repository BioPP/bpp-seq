/***************************************************************************
                          SiteContainerExceptions.h  -  description
                             -------------------
    begin                : mer mar 31 2004
    copyright            : (C) 2004 by Julien Dutheil
    email                : Julien.Dutheil@univ-montp2.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SITECONTAINEREXCEPTIONS_H
#define SITECONTAINEREXCEPTIONS_H

// From Utils:
#include <Utils/Exceptions.h>

/**
 * @brief The site not found exception base class.
 *
 * @see Exception
 */
class SiteNotFoundException : public Exception {

	protected:

		/**
		 * @brief The id of the site that was to be found.
		 */
		const string id;

	public:	// Class constructor
	
		/**
		 * @brief Build a new SiteNotFoundException object.
		 *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param sId  A the id of the site that was to be found.
		 */
		SiteNotFoundException(const char *   text, const char * sId = "");

		/**
		 * @brief Build a new SiteNotFoundException object.
		 *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param sId  A the id of the site that was to be found.
		 */
		SiteNotFoundException(const string & text, const string & sId = "");

		// Class destructor
		~SiteNotFoundException() throw();

	public:

		/**
		 * @brief Get the id of the site that was to be found.
		 *
		 * @return The id of the site that was to be found.
		 */
		virtual const string getSiteId() const;
};

#endif
