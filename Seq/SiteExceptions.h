/***************************************************************************
                          SiteExceptions.h  -  description
                             -------------------
    begin                : dim mar 7 2004
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

#ifndef SITEEXCEPTIONS_H
#define SITEEXCEPTIONS_H


/**
 *@author Julien Dutheil
 */

class Site;

// From Utils:
#include <Utils/Exceptions.h>

/**
 * @brief The site exception base class.
 *
 * @see Exception
 */
class SiteException : public Exception {

	protected:

		/**
		 * @brief A pointer toward a site object.
		 */
		const Site * site;

	public:	// Class constructor

		/**
		 * @brief Build a new SiteException object.
		 *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param s    A const pointer toward the site that threw the exception.
		 */
		SiteException(const char *   text, const Site * s = NULL);

		/**
		 * @brief Build a new SiteException object.
		 *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param s    A const pointer toward the site that threw the exception.
		 */
		SiteException(const string & text, const Site * s = NULL);

		// Class destructor
		~SiteException() throw();

	public:

		/**
		 * @brief Get the site that threw the exception.
		 *
		 * @return A const pointer toward the site.
		 */
		virtual const Site * getSite() const;
};

class EmptySiteException : public SiteException {

	public:
		// Class constructor
		EmptySiteException(const char *   text, const Site * s = NULL);
		EmptySiteException(const string & text, const Site * s = NULL);

		// Class destructor
		~EmptySiteException() throw();
};

class SiteWithGapException : public SiteException {

	public:
		// Class constructor
		SiteWithGapException(const char *   text, const Site * s = NULL);
		SiteWithGapException(const string & text, const Site * s = NULL);

		// Class destructor
		~SiteWithGapException() throw();
};

#endif
