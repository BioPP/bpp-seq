// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SITEITERATOR_H
#define BPP_SEQ_SITEITERATOR_H


namespace bpp
{
/**
 * @brief Generic site iterator interface, allowing to loop over sites.
 */
template<class SiteType>
class TemplateSiteIteratorInterface
{
public:
  TemplateSiteIteratorInterface() {}
  virtual ~TemplateSiteIteratorInterface() {}

public:
  virtual SiteType& nextSite() = 0;
  virtual bool hasMoreSites() const = 0;
};
} // end of namespace bpp.
#endif // BPP_SEQ_SITEITERATOR_H
