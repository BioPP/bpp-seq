//
// File: CompressedVectorSiteContainer.cpp
// Authors:
//   Julien Dutheil
// Created: 2009-12-16 12:08:00
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

#include <Bpp/Text/TextTools.h>
#include <iostream>

#include "CompressedVectorSiteContainer.h"

using namespace std;

using namespace bpp;

/** Class constructors: *******************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(
    std::vector< unique_ptr<Site> >& vs,
    shared_ptr<const Alphabet>& alphabet) :
  AbstractTemplateSequenceContainer<Sequence>(alphabet),
  siteContainer_(),
  sequenceContainer_(),
  sequenceNames_(),
  sequenceComments_(),
  index_(0)
{
  if (vs.size() == 0)
    throw Exception("CompressedVectorSiteContainer::CompressedVectorSiteContainer. Empty site set.");
  // Seq names and comments:
  size_t nbSeq = vs[0]->size();
  for (size_t i = 0; i < nbSeq; ++i) {
    sequenceNames_.push_back("Seq_" + TextTools::toString(i));
    sequenceContainer_.appendObject(nullptr, "Seq_" + TextTools::toString(i));
  }

  // Now try to add each site:
  for (auto& site : vs)
    addSite(site);
}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(
    size_t size,
    shared_ptr<const Alphabet>& alphabet) :
  AbstractTemplateSequenceContainer<Sequence>(alphabet),
  siteContainer_(),
  sequenceContainer_(),
  sequenceNames_(),
  sequenceComments_(),
  index_(0)
{
  // Seq names and comments:
  for (size_t i = 0; i < size; ++i) {
    sequenceNames_.push_back("Seq_" + TextTools::toString(i));
    sequenceContainer_.appendObject(nullptr, "Seq_" + TextTools::toString(i));
  }
}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(
    const std::vector<std::string>& sequenceKeys, 
    shared_ptr<const Alphabet>& alphabet) :
  AbstractTemplateSequenceContainer(alphabet),
  siteContainer_(),
  sequenceContainer_(),
  sequenceNames_(),
  sequenceComments_(),
  index_(0)
{
  unsigned int i = 0;
  for (auto key : sequenceKeys) {
    ++i;
    sequenceNames_.push_back("Seq_" + TextTools::toString(i));
    sequenceContainer_.appendObject(nullptr, key);
  }
}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(shared_ptr<const Alphabet>& alphabet) :
  AbstractTemplateSequenceContainer<Sequence>(alphabet),
  siteContainer_(),
  sequenceContainer_(),
  sequenceNames_(),
  sequenceComments_(),
  index_(0)
{}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(const CompressedVectorSiteContainer& vsc) :
  AbstractTemplateSequenceContainer<Sequence>(vsc),
  siteContainer_(),
  sequenceContainer_(),
  sequenceNames_(vsc.sequenceNames_),
  sequenceComments_(vsc.sequenceComments_),
  index_(vsc.index_)
{
  for (const auto& name: vsc.sequenceNames_)
    sequenceContainer_.appendObject(nullptr, name); 
  
  //Copy the compressed data:
  for (size_t i = 0; i < vsc.siteContainer_.getSize(); ++i) {
    auto sitePtr = std::shared_ptr<Site>(vsc.siteContainer_.getObject(i)->clone());
    siteContainer_.appendObject(sitePtr);
  }
}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(const SiteContainerInterface& sc) :
  AbstractTemplateSequenceContainer<Sequence>(sc),
  siteContainer_(),
  sequenceContainer_(),
  sequenceNames_(sc.getSequenceNames()),
  sequenceComments_(sc.getSequenceComments()),
  index_(0)
{
  for (const auto& name: sc.getSequenceNames())
    sequenceContainer_.appendObject(nullptr, name);
 
  // Now try to add each site:
  for (size_t i = 0; i < sc.getNumberOfSites(); ++i) {
    auto sitePtr = std::unique_ptr<Site>(sc.site(i).clone());
    addSite(sitePtr, false);
  }
}

/******************************************************************************/

CompressedVectorSiteContainer& CompressedVectorSiteContainer::operator=(const CompressedVectorSiteContainer& vsc)
{
  clear();
  AbstractTemplateSequenceContainer<Sequence>::operator=(vsc);

  for (const auto& name: vsc.getSequenceNames())
    sequenceContainer_.appendObject(nullptr, name);

  //Copy the compressed data:
  index_ = vsc.index_;
  for (size_t i = 0; i < vsc.siteContainer_.getSize(); ++i) {
    auto sitePtr = std::shared_ptr<Site>(vsc.siteContainer_.getObject(i)->clone());
    siteContainer_.appendObject(sitePtr);
  }

  sequenceNames_ = vsc.sequenceNames_;
  sequenceComments_ = vsc.sequenceComments_;
  
  return *this;
}

/******************************************************************************/

CompressedVectorSiteContainer& CompressedVectorSiteContainer::operator=(const SiteContainerInterface& sc)
{
  clear();
  AbstractTemplateSequenceContainer<Sequence>::operator=(sc);

  // Now try to add each site:
  for (size_t i = 0; i < sc.getNumberOfSites(); ++i) {
    auto sitePtr = std::unique_ptr<Site>(sc.site(i).clone());
    addSite(sitePtr, false);
  }

  sequenceNames_ = sc.getSequenceNames();
  sequenceComments_ = sc.getSequenceComments();

  return *this;
}

/******************************************************************************/

void CompressedVectorSiteContainer::setSite(size_t sitePosition, unique_ptr<Site>& site, bool checkCoordinate)
{
  if (sitePosition >= getNumberOfSites())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::setSite.", sitePosition, 0, getNumberOfSites() - 1);

  // Check size:
  if (site->size() != getNumberOfSequences())
    throw SiteException("AlignedSequenceContainer::setSite. Site does not have the appropriate length", site.get());

  // New site's alphabet and site container's alphabet matching verification
  if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("CompressedVectorSiteContainer::setSite", getAlphabet(), site->getAlphabet());

  size_t current = index_[sitePosition];
  size_t siteIndex = getSiteIndex_(*site);
  if (siteIndex == current)
  {
    // Nothing to do here, this is the same site.
  }
  else if (siteIndex < getNumberOfUniqueSites())
  {
    // The new site is already in the list, si we just update the index:
    index_[sitePosition] = siteIndex;

    // We have to check if the previous pattern was unique, and if so, remove it and update indices:
    bool test = true;
    for (size_t i = 0; test && i < index_.size(); ++i)
    {
      if (index_[i] == current)
      {
        // There is another site, so nothing to do...
        test = false;
      }
    }
    if (test)
    {
      // There was no other site pointing toward this pattern, so we remove it.
      siteContainer_.deleteObject(current);
      // Now we have to correct all indices:
      for (size_t i = 0; i < index_.size(); ++i)
      {
        if (index_[i] > current)
          index_[i]--;
      }
    }
  }
  else
  {
    // This is a new pattern, and we have to add it to the list.
    // Now we have to check if the previous pattern was unique, and if so,
    // replace it with the new one. Otherwise, add the new site at the end of the list.
    bool test = true;
    for (size_t i = 0; test && i < index_.size(); ++i)
    {
      if (i != sitePosition && index_[i] == current)
      {
        // There is another site
        test = false;
      }
    }
    if (test)
    {
      // we relace the site
      siteContainer_.addObject(std::move(site), current, false);
    }
    else
    {
      // We add the site at the end:
      siteContainer_.appendObject(std::move(site));
      index_[sitePosition] = siteIndex;
    }
  }

  // Clean Sequence Container cache
  sequenceContainer_.clear();
}

/******************************************************************************/

std::unique_ptr<Site> CompressedVectorSiteContainer::removeSite(size_t siteIndex)
{
  if (siteIndex >= getNumberOfSites())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::removeSite.", siteIndex, 0, getNumberOfSites() - 1);
  // Here we need to check whether the pattern corresponding to this site is unique:

  auto sitePtr = siteContainer_.getObject(index_[siteIndex]);
  std::get_deleter< SwitchDeleter<Site> >(sitePtr)->off();

  size_t current = index_[siteIndex];
  bool test = true;
  for (size_t j = 0; test && j < index_.size(); ++j)
  {
    if (j != siteIndex && index_[j] == current)
    {
      // There is another site, so nothing to erase
      test = false;
    }
  }
  if (test)
  {
    // There was no other site pointing toward this pattern, so we remove it.
    siteContainer_.removeObject(index_[siteIndex]);

    // Now we have to correct all indices:
    for (size_t j = 0; j < index_.size(); ++j)
    {
      if (index_[j] > current)
        index_[j]--;
    }
  }
  index_.erase(index_.begin() + static_cast<ptrdiff_t>(siteIndex));

  // Clean Sequence Container cache
  sequenceContainer_.clear();

  return std::unique_ptr<Site>(sitePtr.get());
}

/******************************************************************************/

void CompressedVectorSiteContainer::deleteSite(size_t siteIndex)
{
  if (siteIndex >= getNumberOfSites())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::deleteSite.", siteIndex, 0, getNumberOfSites() - 1);
  // Here we need to check whether the pattern corresponding to this site is unique:

  removeSite(siteIndex); //This effectively delete the object as the unique_ptr is not forwarded and will be destroyed.
}

/******************************************************************************/

void CompressedVectorSiteContainer::deleteSites(size_t siteIndex, size_t length)
{
  // This may be optimized later:
  for (size_t i = 0; i < length; ++i)
    deleteSite(siteIndex + i);

}

/***************************************************************************/

void CompressedVectorSiteContainer::addSite(std::unique_ptr<Site>& site, bool checkCoordinate)
{
  // Check size:
  if (getNumberOfSequences() != 0 && site->size() != getNumberOfSequences())
    throw SiteException("CompressedVectorSiteContainer::addSite. Site does not have the appropriate length", site.get());

  // New site's alphabet and site container's alphabet matching verification
  if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
  {
    throw AlphabetMismatchException("CompressedVectorSiteContainer::addSite", getAlphabet(), site->getAlphabet());
  }

  size_t n = site->size();
  
  size_t siteIndex = getSiteIndex_(*site);
  if (siteIndex == getNumberOfUniqueSites())
  {
    // This is a new pattern:
    std::shared_ptr<Site> sitePtr(site.release(), SwitchDeleter<Site>());
    siteContainer_.appendObject(sitePtr);
  }

  index_.push_back(siteIndex);

  // Clean Sequence Container cache
  if (getNumberOfSequences() == 0) {
    sequenceNames_.resize(n);
    sequenceComments_.resize(n);
    for (size_t i = 0; i < n; ++i) {
      sequenceNames_[i] = "Seq_" + TextTools::toString(i);
      sequenceContainer_.appendObject(nullptr, sequenceNames_[i]);
    }
  } else {
    sequenceContainer_.nullify();
  }
}

/******************************************************************************/

void CompressedVectorSiteContainer::addSite(std::unique_ptr<Site>& site, size_t siteIndex, bool checkCoordinates)
{
  if (siteIndex >= getNumberOfSites())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::addSite", siteIndex, 0, getNumberOfSites() - 1);

  // Check size:
  if (site->size() != getNumberOfSequences())
    throw SiteException("CompressedVectorSiteContainer::addSite. Site does not have the appropriate length", site.get());

  // New site's alphabet and site container's alphabet matching verification
  if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
  {
    throw AlphabetMismatchException("CompressedVectorSiteContainer::addSite", getAlphabet(), site->getAlphabet());
  }
  
  size_t n = site->size();

  size_t index = getSiteIndex_(*site);
  if (index == getNumberOfUniqueSites()) {
    // This is a new pattern:
    std::shared_ptr<Site> sitePtr(site.release(), SwitchDeleter<Site>());
    siteContainer_.appendObject(sitePtr);
  }

  index_.insert(index_.begin() + static_cast<ptrdiff_t>(siteIndex), index);

  // Clean Sequence Container cache
  if (getNumberOfSequences() == 0) {
    sequenceNames_.resize(n);
    sequenceComments_.resize(n);
    for (size_t i = 0; i < n; ++i) {
      sequenceNames_[i] = "Seq_" + TextTools::toString(i);
      sequenceContainer_.appendObject(nullptr, sequenceNames_[i]);
    }
  } else {
    sequenceContainer_.nullify();
  }
}

/******************************************************************************/

void CompressedVectorSiteContainer::reindexSites()
{
  for (size_t i = 0; i < siteContainer_.getSize(); ++i)
  {
    getSite_(i).setCoordinate(static_cast<int>(i) + 1);
  }
}

/******************************************************************************/

void CompressedVectorSiteContainer::setSiteCoordinates(const Vint& vCoordinates)
{
  if (vCoordinates.size() != getNumberOfSites())
    throw BadSizeException("CompressedVectorSiteContainer::setSitePositions bad size of positions vector", vCoordinates.size(), getNumberOfSites());

  for (size_t i = 0; i < vCoordinates.size(); ++i)
  {
    getSite_(i).setCoordinate(vCoordinates[i]);
  }
}

/******************************************************************************/

Vint CompressedVectorSiteContainer::getSiteCoordinates() const
{
  size_t n = getNumberOfSites();
  Vint coordinates(n);
  for (size_t i = 0; i < n; i++)
  {
    coordinates[i] = siteContainer_.getObject(index_[i])->getCoordinate();
  }
  return coordinates;
}

/******************************************************************************/

const Sequence& CompressedVectorSiteContainer::sequence(size_t sequencePosition) const
{
  if (sequencePosition >= getNumberOfSequences())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::getSequence.", sequencePosition, 0, getNumberOfSequences() - 1);

  // If Sequence already exsits
  auto name = sequenceContainer_.getObjectName(sequencePosition);
  if (!sequenceContainer_.isAvailableName(name))
    return *sequenceContainer_.getObject(sequencePosition);
  
  // Main loop : for all sites
  size_t n = getNumberOfSites();
  vector<int> sequence(n);
  for (size_t j = 0; j < n; ++j)
    sequence[j] = (*siteContainer_.getObject(index_[j]))[sequencePosition];

  auto alphaPtr = getAlphabet();
  auto ns = std::make_shared<Sequence>(
        sequenceNames_[sequencePosition],
        sequence,
	      sequenceComments_[sequencePosition],
        alphaPtr);

  sequenceContainer_.addObject_(ns, sequencePosition, sequenceKey(sequencePosition), false);

  return *ns;
}

/******************************************************************************/

size_t CompressedVectorSiteContainer::getSiteIndex_(const Site& site)
{
  size_t pos = getNumberOfUniqueSites();
  bool test;
  for (size_t i = 0; i < getNumberOfUniqueSites(); ++i)
  {
    test = true;
    const Site& siteI = *siteContainer_.getObject(i);

    for (size_t j = 0; test && j < site.size(); ++j) // site is supposed to have the correct size, that is the same as all the ones in the container.
    {
      if (site[j] != siteI[j])
        test = false;
    }

    if (test)
    {
      pos = i;
      break;
    }
  }
  return pos;
}

/******************************************************************************/

