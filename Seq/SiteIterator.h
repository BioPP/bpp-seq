//
// File: SiteIterator.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 19 12:47:16 2003
//

#ifndef _SITEITERATOR_H_
#define _SITEITERATOR_H_

#include "Site.h"
#include "SiteContainer.h"

class SiteIterator
{
	public:
		virtual ~SiteIterator();
	
	public:
		virtual const Site * nextSite()       = 0;
		virtual     bool hasMoreSites() const = 0;
};

class AbstractSiteIterator: public SiteIterator
{
	protected:
		const SiteContainer * sites;
		unsigned int currentPosition;
	
	public:
		AbstractSiteIterator(const SiteContainer & sites);
		virtual ~AbstractSiteIterator();
	
	public:
		virtual const Site * nextSite()       = 0;
		virtual     bool hasMoreSites() const = 0;
};

class SimpleSiteIterator: public AbstractSiteIterator
{
	public:
		SimpleSiteIterator(const SiteContainer & sites);
		virtual ~SimpleSiteIterator();
	
	public:
		const Site * nextSite();
		    bool hasMoreSites() const;
};

class NoGapSiteIterator: public AbstractSiteIterator
{
	public:
		NoGapSiteIterator(const SiteContainer & sites);
		virtual ~NoGapSiteIterator();
	
	public:
		const Site * nextSite();
		    bool hasMoreSites() const;
		int nextSiteWithoutGapPosition(int current) const;
		int previousSiteWithoutGapPosition(int current) const;
};

class CompleteSiteIterator: public AbstractSiteIterator
{
	public:
		CompleteSiteIterator(const SiteContainer & sites);
		virtual ~CompleteSiteIterator();
	
	public:
		const Site * nextSite();
		    bool hasMoreSites() const;
		int nextCompleteSitePosition(int current) const;
		int previousCompleteSitePosition(int current) const;
};

#endif	//_SITEITERATOR_H_
