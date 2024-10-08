// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SITETOOLS_H
#define BPP_SEQ_SITETOOLS_H

#include <Bpp/Exceptions.h>

#include "Site.h"
#include "SymbolListTools.h"

// From the STL:
#include <map>

namespace bpp
{
/**
 * @brief Utilitary methods dealing with sites.
 *
 * This class is an alias for SymbolListTools.
 */
class SiteTools :
  public SymbolListTools
{
public:
  SiteTools()
  {}
  virtual ~SiteTools()
  {}
};
}
#endif // BPP_SEQ_SITETOOLS_H
