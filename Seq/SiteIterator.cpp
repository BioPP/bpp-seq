//
// File: SiteIterator.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 19 12:47:16 2003
//

#include "SiteIterator.h"
#include "SiteTools.h"

// From the STL:
#include <iostream>
using namespace std;

SiteIterator::~SiteIterator() {}

/******************************************************************************/
	
AbstractSiteIterator::AbstractSiteIterator(const SiteContainer & sites) {
	this -> sites = &sites;
	currentPosition = 0;	
}

AbstractSiteIterator::~AbstractSiteIterator() {}

/******************************************************************************/
	
SimpleSiteIterator::SimpleSiteIterator(const SiteContainer & sites): AbstractSiteIterator(sites) {}

SimpleSiteIterator::~SimpleSiteIterator() {}

const Site * SimpleSiteIterator::nextSite() {
	const Site * s = sites -> getSite(currentPosition);
	currentPosition++;
	return s;
}

bool SimpleSiteIterator::hasMoreSites() const {
	return currentPosition < sites -> getNumberOfSites();
}

/******************************************************************************/
	
NoGapSiteIterator::NoGapSiteIterator(const SiteContainer & sites): AbstractSiteIterator(sites) {
	currentPosition = nextSiteWithoutGapPosition(-1);
}

NoGapSiteIterator::~NoGapSiteIterator() {}

const Site * NoGapSiteIterator::nextSite() {
	const Site * s = sites -> getSite(currentPosition);
	currentPosition = nextSiteWithoutGapPosition(currentPosition);
	return s;
}

bool NoGapSiteIterator::hasMoreSites() const {
	return currentPosition < sites -> getNumberOfSites();
}

int NoGapSiteIterator::nextSiteWithoutGapPosition(int current) const {
	unsigned int position = current + 1;
	while(position < sites -> getNumberOfSites() && SiteTools::hasGap(* sites -> getSite(position))) position ++;
	return position;
}

int NoGapSiteIterator::previousSiteWithoutGapPosition(int current) const {
	int position = current - 1;
	while(position >= 0 && SiteTools::hasGap(* sites -> getSite(position))) position --;
	return position;
}

/******************************************************************************/
	
CompleteSiteIterator::CompleteSiteIterator(const SiteContainer & sites): AbstractSiteIterator(sites) {
	currentPosition = nextCompleteSitePosition(-1);
}

CompleteSiteIterator::~CompleteSiteIterator() {}

const Site * CompleteSiteIterator::nextSite() {
	const Site * s = sites -> getSite(currentPosition);
	currentPosition = nextCompleteSitePosition(currentPosition);
	return s;
}

bool CompleteSiteIterator::hasMoreSites() const {
	return currentPosition < sites -> getNumberOfSites();
}

int CompleteSiteIterator::nextCompleteSitePosition(int current) const {
    unsigned int position = current + 1;
	while(position < sites -> getNumberOfSites() && !SiteTools::isComplete(* sites -> getSite(position))) position ++;
	return position;
}

int CompleteSiteIterator::previousCompleteSitePosition(int current) const {
    int position = current - 1;
	while(position >= 0 && !SiteTools::isComplete(* sites -> getSite(position))) position --;
	return position;
}
