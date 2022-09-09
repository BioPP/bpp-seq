//
// File: SiteContainerIterator.h
// Authors:
//   Julien Dutheil
// Created: 2003-10-19 12:47:16
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_CONTAINER_SITECONTAINERITERATOR_H
#define BPP_SEQ_CONTAINER_SITECONTAINERITERATOR_H


#include "../SiteIterator.h"
#include "../SiteTools.h"
#include "SiteContainer.h"

namespace bpp
{
/**
 * @brief Partial implementation of the SiteIterator interface, allowing to loop over a site container.
 */
template<class SiteType, class SequenceType, class HashType>
class AbstractTemplateSiteContainerIterator :
  public virtual TemplateSiteIteratorInterface<const SiteType>
{
protected:
  const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>* sites_;
  long int currentPosition_;

public:
  AbstractTemplateSiteContainerIterator(
      const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites) :
    sites_(&sites),
    currentPosition_(0)
  {}


  AbstractTemplateSiteContainerIterator(
      const AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>& atsi) :
    sites_(atsi.sites_),
    currentPosition_(atsi.currentPosition_)
  {}

  AbstractTemplateSiteContainerIterator& operator=(
      const AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>& atsi)
  {
    sites_ = atsi.sites_;
    currentPosition_ = atsi.currentPosition_;
    return *this;
  }

  virtual ~AbstractTemplateSiteContainerIterator() {}
};





/**
 * @brief Loop over all sites in a SiteContainer.
 */
template<class SiteType, class SequenceType, class HashType>
class SimpleTemplateSiteContainerIterator : 
  public AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>
{
protected:
  using AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>::sites_;
  using AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>::currentPosition_;

public:
  SimpleTemplateSiteContainerIterator(
      const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites) :
    AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>(sites)
  {}
	  
  virtual ~SimpleTemplateSiteContainerIterator() {}

public:
  const SiteType& nextSite() override
  {
    const SiteType& s = sites_->getSite(static_cast<size_t>(currentPosition_));
    currentPosition_++;
    return s;
  }

  bool hasMoreSites() const override
  {
    return currentPosition_ < sites_->getNumberOfSites();
  }

};

using SimpleSiteContainerIterator = SimpleTemplateSiteContainerIterator<Site, Sequence, std::string>;
using SimpleProbabilisticSiteContainerIterator = SimpleTemplateSiteContainerIterator<ProbabilisticSite, ProbabilisticSequence, std::string>;

/**
 * @brief Loop over all sites without gaps in a SiteContainer.
 */
template<class SiteType, class SequenceType, class HashType>
class NoGapTemplateSiteContainerIterator : 
  public AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>
{
protected:
  using AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>::sites_;
  using AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>::currentPosition_;

public:
  NoGapTemplateSiteContainerIterator(
      const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites) : 
    AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>(sites)
  {
    currentPosition_ = nextSiteWithoutGapPosition(-1);
  }

  virtual ~NoGapTemplateSiteContainerIterator() {}

public:
  const SiteType& nextSite() override
  {
    const SiteType& s = sites_->getSite(static_cast<size_t>(currentPosition_));
    currentPosition_ = nextSiteWithoutGapPosition(currentPosition_);
    return s;
  }
  
  bool hasMoreSites() const override
  {
    return currentPosition_ < static_cast<long int>(sites_->getNumberOfSites());
  }

  long int nextSiteWithoutGapPosition(int current) const
  {
    size_t position = static_cast<size_t>(current + 1);
    while (position < sites_->getNumberOfSites() && SiteTools::hasGap(sites_->getSite(position)))
      position++;
    return static_cast<int>(position);
  }

  long int previousSiteWithoutGapPosition(int current) const
  {
    long int position = current - 1;
    while (position >= 0 && SymbolListTools::hasGap(sites_->getSite(static_cast<size_t>(position))))
      position--;
    return position;
  }

};

/**
 * @brief Loop over all complete sites in a SiteContainer
 * (i.e. sites without gap and unresolved characters).
 */
template<class SiteType, class SequenceType, class HashType>
class CompleteTemplateSiteContainerIterator : 
  public AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>
{
protected:
  using AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>::sites_;
  using AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>::currentPosition_;

public:
  CompleteTemplateSiteContainerIterator(
      const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites) : 
    AbstractTemplateSiteContainerIterator<SiteType, SequenceType, HashType>(sites)
  {
    currentPosition_ = nextCompleteSitePosition(-1);
  }

  virtual ~CompleteTemplateSiteContainerIterator() {}

public:
  const SiteType& nextSite() override
  {
    const Site& s = sites_->getSite(static_cast<size_t>(currentPosition_));
    currentPosition_ = nextCompleteSitePosition(currentPosition_);
    return s;
  }

  bool hasMoreSites() const override
  {
    return currentPosition_ < static_cast<int>(sites_->getNumberOfSites());
  }

  long int nextCompleteSitePosition(long int current) const
  {
    size_t position = static_cast<size_t>(current + 1);
    while (position < sites_->getNumberOfSites() && !SiteTools::isComplete(sites_->getSite(position)))
      position++;
    return static_cast<int>(position);
  }

  long int previousCompleteSitePosition(long int current) const
  {
    long int position = current - 1;
    while (position >= 0 && !SymbolListTools::isComplete(sites_->getSite(static_cast<size_t>(position))))
      position--;
    return position;
  }

};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SITECONTAINERITERATOR_H
