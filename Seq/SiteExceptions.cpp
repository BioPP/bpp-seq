/***************************************************************************
                          SiteExceptions.cpp  -  description
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

#include "SiteExceptions.h"
#include "Site.h"

// From utils:
#include <Utils/TextTools.h>

/***************************************************************************/

SiteException::SiteException(const char *   text, const Site * s) :
	Exception(text + (s != NULL ? "(" + TextTools::toString(s -> getPosition()) + ")" : string(""))),
	site(s) {};
    
SiteException::SiteException(const string & text, const Site * s) :
	Exception(text + (s != NULL ? "(" + TextTools::toString(s -> getPosition()) + ")" : string(""))),
	site(s) {};
    
SiteException::~SiteException() throw() {};

const Site * SiteException::getSite() const { return site; }

/***************************************************************************/

EmptySiteException::EmptySiteException(const char *   text, const Site * s) :
	SiteException(text, s) {};

EmptySiteException::EmptySiteException(const string & text, const Site * s) :
	SiteException(text, s) {};

EmptySiteException::~EmptySiteException() throw () {};

/***************************************************************************/

SiteWithGapException::SiteWithGapException(const char *   text, const Site * s) :
	SiteException(text, s) {};

SiteWithGapException::SiteWithGapException(const string & text, const Site * s) :
	SiteException(text, s) {};

SiteWithGapException::~SiteWithGapException() throw () {};

/***************************************************************************/
