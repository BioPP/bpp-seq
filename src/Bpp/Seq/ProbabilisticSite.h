// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_PROBABILISTICSITE_H
#define BPP_SEQ_PROBABILISTICSITE_H


#include "CoreSite.h"
#include "ProbabilisticSymbolList.h"

namespace bpp
{
/**
 * @brief The ProbabilisticSite interface.
 *
 * Define specific attributes and methods for probabilistic sites
 * manipulation.  It is very similar to the ProbabilisticSequence
 * object (a site is a vertical probabilistic sequence!),
 * ProbabilisticSites have a 'position' attribute.
 *
 * Currently, this interface does not add any additional method to the CoreSite interface, but this may change in the future.
 */
class ProbabilisticSiteInterface :
	public virtual CoreSiteInterface,
	public virtual ProbabilisticSymbolListInterface
{
public:
virtual ~ProbabilisticSiteInterface() {
}

public:
virtual ProbabilisticSiteInterface* clone() const = 0;
};

class ProbabilisticSite :
	public virtual ProbabilisticSiteInterface,
	public AbstractCoreSite,
	public virtual ProbabilisticSymbolList // Diamond inheritance
{
public:
/**
 * @brief Build a new void ProbabilisticSite object with the specified alphabet.
 *
 * @param alpha The alphabet to use.
 */
ProbabilisticSite(std::shared_ptr<const Alphabet>& alpha) :
	ProbabilisticSymbolList(alpha),
	AbstractCoreSite()
{
}


/**
 * @brief Build a new void ProbabilisticSite object with the specified alphabet and position.
 *
 * @param alpha    The alphabet to use.
 * @param position The position attribute for this probabilistic site.
 */
ProbabilisticSite(std::shared_ptr<const Alphabet>& alpha, int position) :
	ProbabilisticSymbolList(alpha),
	AbstractCoreSite(position)
{
}


/**
 * @brief Build a new ProbabilisticSite object with the specified alphabet.
 * The content of the site is initialized from a Table<double> object.
 *
 * @param site     The content of the probabilist site.
 * @param alpha    The alphabet to use.
 * @throw Exception If the content does not match the specified alphabet or is internally inconsistent.
 */
ProbabilisticSite(const Table<double>& site, std::shared_ptr<const Alphabet>& alpha) :
	ProbabilisticSymbolList(site, alpha),
	AbstractCoreSite()
{
}


/**
 * @brief Build a new ProbabilisticSite object with the specified alphabet and position.
 * The content of the probabilistic site is initialized from a Table<double> object.
 *
 * @param site     The content of the probabilistic site.
 * @param alpha    The alphabet to use.
 * @param position The position attribute for this site.
 * @throw Exception If the content does not match the specified alphabet or is internally insconsistent.
 */
ProbabilisticSite(const Table<double>& site, std::shared_ptr<const Alphabet>& alpha, int position) :
	ProbabilisticSymbolList(site, alpha),
	AbstractCoreSite(position)
{
}


/**
 * @brief The copy constructor.
 */
ProbabilisticSite(const ProbabilisticSite& site) :
	ProbabilisticSymbolList(site),
	AbstractCoreSite(site)
{
}


ProbabilisticSite(const ProbabilisticSiteInterface& site) :
	ProbabilisticSymbolList(site),
	AbstractCoreSite(site.getCoordinate())
{
}


/**
 * @brief The assignment operator.
 */
ProbabilisticSite& operator=(const ProbabilisticSite& site)
{
	ProbabilisticSymbolList::operator=(site);
	AbstractCoreSite::operator=(site);
	return *this;
}

ProbabilisticSite& operator=(const ProbabilisticSiteInterface& site)
{
	ProbabilisticSymbolList::operator=(site);
	AbstractCoreSite::operator=(site);
	return *this;
}

/**
 * @name The Clonable interface
 *
 * @{
 */
ProbabilisticSite* clone() const {
	return new ProbabilisticSite(*this);
}

/**
 * @}
 */

// class destructor
virtual ~ProbabilisticSite() {
}

double getStateValueAt(size_t sequencePosition, int state) const
{
	if (sequencePosition  >= size()) throw IndexOutOfBoundsException("ProbabilisticSites::getStateValueAt.", sequencePosition, 0, size() - 1);
	return (*this)[sequencePosition][static_cast<size_t>(state)];
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_PROBABILISTICSITE_H
