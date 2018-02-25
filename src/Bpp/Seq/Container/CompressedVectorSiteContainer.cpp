//
// File: CompressedCompressedVectorSiteContainer.cpp
// Created by: Julien Dutheil
// Created on: Wed Dec  16 12:08 2009
//

/*
   Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

   This software is a computer program whose purpose is to provide classes
   for sequences analysis.

   This software is governed by the CeCILL  license under French law and
   abiding by the rules of distribution of free software.  You can  use,
   modify and/ or redistribute the software under the terms of the CeCILL
   license as circulated by CEA, CNRS and INRIA at the following URL
   "http://www.cecill.info".

   As a counterpart to the access to the source code and  rights to copy,
   modify and redistribute granted by the license, users are provided only
   with a limited warranty  and the software's author,  the holder of the
   economic rights,  and the successive licensors  have only  limited
   liability.

   In this respect, the user's attention is drawn to the risks associated
   with loading,  using,  modifying and/or developing or reproducing the
   software by the user in light of its specific status of free software,
   that may mean  that it is complicated to manipulate,  and  that  also
   therefore means  that it is reserved for developers  and  experienced
   professionals having in-depth computer knowledge. Users are therefore
   encouraged to load and test the software's suitability as regards their
   requirements in conditions enabling the security of their systems and/or
   data to be ensured and,  more generally, to use and operate it in the
   same conditions as regards security.

   The fact that you are presently reading this means that you have had
   knowledge of the CeCILL license and that you accept its terms.
 */

#include "CompressedVectorSiteContainer.h"
#include <Bpp/Text/TextTools.h>

#include <iostream>

using namespace std;

using namespace bpp;

/** Class constructors: *******************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(
  const std::vector<const Site*>& vs,
  const Alphabet* alpha) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha),
  index_(0)
{
  if (vs.size() == 0) throw Exception("CompressedVectorSiteContainer::CompressedVectorSiteContainer. Empty site set.");
  // Seq names and comments:
  size_t nbSeq = vs[0]->size();
  for (size_t i = 0; i < nbSeq; i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", alpha)), "Seq_" + TextTools::toString(i));

  // Now try to add each site:
  for (size_t i = 0; i < vs.size(); i++)
  {
    addSite(*vs[i]); 
  }
}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(size_t size, const Alphabet* alpha) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha),
  index_(0)
{
  // Seq names and comments:
  for (size_t i = 0; i < size; i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", alpha)), "Seq_" + TextTools::toString(i));
}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(const std::vector<std::string>& names, const Alphabet* alpha) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha),
  index_(0)
{
  // Seq names and comments:
  for (size_t i = 0; i < names.size(); i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", alpha)), "Seq_" + TextTools::toString(i));
}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(const Alphabet* alpha) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha),
  index_(0)
{}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(const CompressedVectorSiteContainer& vsc) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(vsc),
  index_(vsc.index_)
{
  vector<string> vNames=vsc.getSequencesNames();
  
  for (size_t i = 0; i < vsc.getNumberOfSequences(); i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence(vNames[i], "", getAlphabet())), vNames[i]);
  
  // Now try to add each site:
  for (size_t i = 0; i < vsc.getNumberOfSites(); i++)
  {
    addSite(vsc.getSite(i), false);
  }
}

/******************************************************************************/

CompressedVectorSiteContainer::CompressedVectorSiteContainer(const SiteContainer& sc) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(sc),
  index_(0)
{
  vector<string> vNames=sc.getSequencesNames();
  
  for (size_t i = 0; i < sc.getNumberOfSequences(); i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence(vNames[i], "", getAlphabet())), vNames[i]);
  // Now try to add each site:
  for (size_t i = 0; i < sc.getNumberOfSites(); i++)
  {
    addSite(sc.getSite(i), false); 
  }
}

/******************************************************************************/

CompressedVectorSiteContainer& CompressedVectorSiteContainer::operator=(const CompressedVectorSiteContainer& vsc)
{
  clear();
  AbstractSequenceContainer::operator=(vsc);
  
  vector<string> vNames=vsc.getSequencesNames();  
  for (size_t i = 0; i < vsc.getNumberOfSequences(); i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence(vNames[i], "", getAlphabet())), vNames[i]);

  for (size_t i = 0; i < vsc.getNumberOfSites(); i++)
    addSite(vsc.getSite(i), false);

  index_ = vsc.index_;

  return *this;
}

/******************************************************************************/

CompressedVectorSiteContainer& CompressedVectorSiteContainer::operator=(const SiteContainer& sc)
{
  clear();
  AbstractSequenceContainer::operator=(sc);
  // Now try to add each site:
  vector<string> vNames=sc.getSequencesNames();  
  for (size_t i = 0; i < sc.getNumberOfSequences(); i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence(vNames[i], "", getAlphabet())), vNames[i]);

  for (size_t i = 0; i < sc.getNumberOfSites(); i++)
    addSite(sc.getSite(i), false); 

  return *this;
}

/******************************************************************************/

void CompressedVectorSiteContainer::setSite(size_t pos, const Site& site, bool checkPositions)
{
  if (pos >= getNumberOfSites())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::setSite.", pos, 0, getNumberOfSites() - 1);

  // Check size:
  if (site.size() != getNumberOfSequences())
    throw SiteException("AlignedSequenceContainer::setSite. Site does not have the appropriate length", &site);

  // New site's alphabet and site container's alphabet matching verification
  if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("CompressedVectorSiteContainer::setSite", getAlphabet(), site.getAlphabet());
  
  size_t current = index_[pos];
  size_t siteIndex = getSiteIndex_(site);
  if (siteIndex == current)
  {
    //Nothing to do here, this is the same site.
  }
  else if (siteIndex < getNumberOfUniqueSites())
  {
    //The new site is already in the list, si we just update the index:
    index_[pos] = siteIndex;

    //We have to check if the previous pattern was unique, and if so, remove it and update indices:
    bool test = true;
    for (size_t i = 0; test && i < index_.size(); ++i)
    {
      if (index_[i] == current)
      {
        //There is another site, so nothing to do...
        test = false;
      }
    }
    if (test)
    {
      //There was no other site pointing toward this pattern, so we remove it.
      VectorPositionedContainer<Site>::deleteObject(current);
      //Now we have to correct all indices:
      for (size_t i = 0; i < index_.size(); ++i)
      {
        if (index_[i] > current) index_[i]--;
      }
    }
  }
  else
  {
    //This is a new pattern, and we have to add it to the list...
    shared_ptr<Site> copy=shared_ptr<Site>(site.clone());

    //Now we have to check if the previous pattern was unique, and if so,
    //replace it with the new one. Otherwise, add the new site at the end of the list.
    bool test = true;
    for (size_t i = 0; test && i < index_.size(); ++i)
    {
      if (i != pos && index_[i] == current)
      {
        //There is another site
        test = false;
      }
    }
    if (test)
    {
      // we relace the site
      VectorPositionedContainer<Site>::addObject(copy, current, false);
    }
    else
    {
      //We add the site at the end:
      VectorPositionedContainer<Site>::appendObject(copy);
      index_[pos] = siteIndex;
    }
  }
}

/******************************************************************************/

std::shared_ptr<Site> CompressedVectorSiteContainer::deleteSite(size_t siteIndex)
{
  if (siteIndex >= getNumberOfSites())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::deleteSite.", siteIndex, 0, getNumberOfSites() - 1);
  //Here we need to check whether the pattern corresponding to this site is unique:

  std::shared_ptr<Site> ss= VectorPositionedContainer<Site>::getObject(index_[siteIndex]);

  size_t current = index_[siteIndex];
  bool test = true;
  for (size_t j = 0; test && j < index_.size(); ++j)
  {
    if (j != siteIndex && index_[j] == current)
    {
      //There is a nother site, so nothing to...
      test = false;
    }
  }
  if (test)
  {
    //There was no other site pointing toward this pattern, so we remove it.
    VectorPositionedContainer<Site>::deleteObject(index_[siteIndex]).get();

    //Now we have to correct all indices:
    for (size_t j = 0; j < index_.size(); ++j)
    {
      if (index_[j] > current) index_[j]--;
    }
  }
  index_.erase(index_.begin() + static_cast<ptrdiff_t>(siteIndex));

  return ss;
}

/******************************************************************************/

void CompressedVectorSiteContainer::deleteSites(size_t siteIndex, size_t length)
{
  //This may be optimized later:
  for (size_t i = 0; i < length; ++i) {
    deleteSite(siteIndex + i);
  }
}

/***************************************************************************/

void CompressedVectorSiteContainer::addSite(const Site& site, bool checkPositions)
{
  // Check size:
  if (getNumberOfSequences()!=0 && site.size() != getNumberOfSequences())
    throw SiteException("CompressedVectorSiteContainer::addSite. Site does not have the appropriate length", &site);

  // New site's alphabet and site container's alphabet matching verification
  if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
  {
    throw AlphabetMismatchException("CompressedVectorSiteContainer::addSite", getAlphabet(), site.getAlphabet());
  }
  
  size_t siteIndex = getSiteIndex_(site);
  if (siteIndex == getNumberOfUniqueSites())
  {
    //This is a new pattern:
    VectorPositionedContainer<Site>::appendObject(shared_ptr<Site>(site.clone()));
  }
  
  index_.push_back(siteIndex);

  if (getNumberOfSequences()==0)
    for (size_t i = 0; i < site.size(); i++)
      VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", getAlphabet())), "Seq_" + TextTools::toString(i));

}

/******************************************************************************/

void CompressedVectorSiteContainer::addSite(const Site& site, size_t siteIndex, bool checkPositions)
{
  if (siteIndex >= getNumberOfSites())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::addSite", siteIndex, 0, getNumberOfSites() - 1);

  // Check size:
  if (site.size() != getNumberOfSequences())
    throw SiteException("CompressedVectorSiteContainer::addSite. Site does not have the appropriate length", &site);

  // New site's alphabet and site container's alphabet matching verification
  if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
  {
    throw AlphabetMismatchException("CompressedVectorSiteContainer::addSite", getAlphabet(), site.getAlphabet());
  }

  size_t index = getSiteIndex_(site);
  if (index == getNumberOfUniqueSites())
    VectorPositionedContainer<Site>::appendObject(shared_ptr<Site>(site.clone()));

  index_.insert(index_.begin() + static_cast<ptrdiff_t>(siteIndex), index);
}

/******************************************************************************/

void CompressedVectorSiteContainer::reindexSites()
{
  for (size_t i=0; i<VectorPositionedContainer<Site>::getSize(); i++)
    getSite(i).setPosition((int)i+1);
}

/******************************************************************************/

void CompressedVectorSiteContainer::setSitePositions(Vint vPositions)
{
  if (vPositions.size() != getNumberOfSites())
    throw BadSizeException("CompressedVectorSiteContainer::setSitePositions bad size of positions vector", vPositions.size(), getNumberOfSites());

  for (size_t i=0; i< vPositions.size(); i++)
    getSite(i).setPosition(vPositions[i]);
}

/******************************************************************************/

Vint CompressedVectorSiteContainer::getSitePositions() const
{
  size_t n = getNumberOfSites();
  Vint positions(n);
  for (size_t i = 0; i < n; i++)
  {
    positions[i] = VectorPositionedContainer<Site>::getObject(index_[i])->getPosition();
  }
  return positions;
}

/******************************************************************************/

const Sequence& CompressedVectorSiteContainer::getSequence(size_t i) const
{
  if (i >= getNumberOfSequences())
    throw IndexOutOfBoundsException("CompressedVectorSiteContainer::getSequence.", i, 0, getNumberOfSequences() - 1);

  // Main loop : for all sites
  size_t n = getNumberOfSites();
  vector<int> sequence(n);
  for (size_t j = 0; j < n; j++)
  {
    sequence[j] = (*VectorPositionedContainer<Site>::getObject(index_[j]))[i];
  }

  shared_ptr<Sequence> ns(shared_ptr<Sequence>(new BasicSequence(VectorMappedContainer<Sequence>::getObject(i)->getName(), sequence, VectorMappedContainer<Sequence>::getObject(i)->getComments(), getAlphabet())));

  VectorMappedContainer<Sequence>::addObject_(ns,i,ns->getName());

  return *ns;
}

/******************************************************************************/

const Sequence& CompressedVectorSiteContainer::getSequence(const std::string& name) const
{
  // Look for sequence name:
  size_t pos = getSequencePosition(name);
  return getSequence(pos);
}

/******************************************************************************/

void CompressedVectorSiteContainer::clear()
{
  VectorPositionedContainer<Site>::clear();
  VectorMappedContainer<Sequence>::clear();
  index_.clear();
}

/******************************************************************************/

void CompressedVectorSiteContainer::setComments(size_t sequenceIndex, const Comments& comments)
{
  VectorMappedContainer<Sequence>::getObject(sequenceIndex)->setComments(comments);
}

/******************************************************************************/

CompressedVectorSiteContainer* CompressedVectorSiteContainer::createEmptyContainer() const
{
   CompressedVectorSiteContainer* vsc = new CompressedVectorSiteContainer(getAlphabet());
   vsc->setGeneralComments(getGeneralComments());
  return vsc;
}

/******************************************************************************/

size_t CompressedVectorSiteContainer::getSiteIndex_(const Site& site)
{
  size_t pos = getNumberOfUniqueSites();
  bool test;
  for (size_t i = 0; i < getNumberOfUniqueSites(); ++i)
  {
    test = true;
    const Site& siteI=*VectorPositionedContainer<Site>::getObject(i);
    
    for (size_t j = 0; test && j < site.size(); ++j) //site is supposed to have the correct size, that is the same as all the ones in the container.
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

