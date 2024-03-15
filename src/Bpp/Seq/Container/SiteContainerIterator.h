// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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
    const SiteType& s = sites_->site(static_cast<size_t>(currentPosition_));
    currentPosition_++;
    return s;
  }

  bool hasMoreSites() const override
  {
    return static_cast<unsigned int>(currentPosition_) < sites_->getNumberOfSites();
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
    const SiteType& s = sites_->site(static_cast<size_t>(currentPosition_));
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
    while (position < sites_->getNumberOfSites() && SiteTools::hasGap(sites_->site(position)))
      position++;
    return static_cast<int>(position);
  }

  long int previousSiteWithoutGapPosition(int current) const
  {
    long int position = current - 1;
    while (position >= 0 && SymbolListTools::hasGap(sites_->site(static_cast<size_t>(position))))
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
    const Site& s = sites_->site(static_cast<size_t>(currentPosition_));
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
    while (position < sites_->getNumberOfSites() && !SiteTools::isComplete(sites_->site(position)))
      position++;
    return static_cast<int>(position);
  }

  long int previousCompleteSitePosition(long int current) const
  {
    long int position = current - 1;
    while (position >= 0 && !SymbolListTools::isComplete(sites_->site(static_cast<size_t>(position))))
      position--;
    return position;
  }

};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SITECONTAINERITERATOR_H
