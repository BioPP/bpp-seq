// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "SiteContainerExceptions.h"

using namespace bpp;
using namespace std;

/*******************************************************************************
 *                       Site containers exceptions                            *
 *******************************************************************************/

SiteNotFoundException::SiteNotFoundException(const char*   text, const char* sId) :
  Exception("SequenceNotFoundException: " + string(text) + "(" + sId + ")"),
  id(sId)
{}

SiteNotFoundException::SiteNotFoundException(const string& text, const string& sId) :
  Exception("SequenceNotFoundException: " + text + "(" + sId + ")"),
  id(sId)
{}

SiteNotFoundException::~SiteNotFoundException()
{}

const string SiteNotFoundException::getSiteId() const
{
  return id;
}
