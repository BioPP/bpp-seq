/***************************************************************************
                          SiteContainerExceptions.cpp  -  description
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

#include "SiteContainerExceptions.h"

/*******************************************************************************
 *                       Site containers exceptions                            *
 *******************************************************************************/

SiteNotFoundException::SiteNotFoundException(const char *   text, const char * sId) :
	Exception("SequenceNotFoundException: " + string(text) + "(" + sId + ")"),
	id(sId) {};
SiteNotFoundException::SiteNotFoundException(const string & text, const string & sId) :
	Exception("SequenceNotFoundException: " + text + "(" + sId + ")"),
	id(sId) {};
SiteNotFoundException::~SiteNotFoundException() throw() {};
const string SiteNotFoundException::getSiteId() const { return id; }
