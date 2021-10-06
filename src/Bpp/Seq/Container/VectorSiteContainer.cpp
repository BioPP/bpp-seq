//
// File: VectorSiteContainer.cpp
// Created by: Julien Dutheil
// Created on: Mon Oct  6 11:50:40 2003
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

#include "VectorSiteContainer.h"

#include <iostream>

using namespace std;

#include <Bpp/Text/TextTools.h>

using namespace bpp;

/** Class constructors: *******************************************************/

VectorSiteContainer::VectorSiteContainer(
  const std::vector<const CruxSymbolListSite*>& vs,
  const Alphabet* alpha,
  bool checkPositions) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha)
{
  if (vs.size() == 0) throw Exception("VectorSiteContainer::VectorSiteContainer. Empty site set.");

  size_t nbSeq=vs[0]->size();
  
  // Seq names and comments:
  
  for (size_t i = 0; i < nbSeq; i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", alpha)), "Seq_" + TextTools::toString(i));

  // Now try to add each site:
  for (size_t i = 0; i < vs.size(); i++)
  {
    if (!dynamic_cast<const Site*>(vs[i]))
      throw Exception("VectorSiteContainer::VectorSiteContainer : Not a Site in position " + TextTools::toString(i));
    
    addSite(dynamic_cast<const Site&>(*vs[i]), checkPositions); // This may throw an exception if position argument already exists or its size is not valid.
  }
}

VectorSiteContainer::VectorSiteContainer(
  const std::vector<const Site*>& vs,
  const Alphabet* alpha,
  bool checkPositions) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha)
{
  if (vs.size() == 0) throw Exception("VectorSiteContainer::VectorSiteContainer. Empty site set.");

  size_t nbSeq=vs[0]->size();
  
  // Seq names and comments:
  
  for (size_t i = 0; i < nbSeq; i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", alpha)), "Seq_" + TextTools::toString(i));

  // Now try to add each site:
  for (size_t i = 0; i < vs.size(); i++)
  {
    addSite(*vs[i], checkPositions); // This may throw an exception if position argument already exists or is size is not valid.
  }
}

/******************************************************************************/

VectorSiteContainer::VectorSiteContainer(size_t size, const Alphabet* alpha) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha)
{
  // Seq names and comments:
  for (size_t i = 0; i < size; i++)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", alpha)), "Seq_" + TextTools::toString(i));
}

/******************************************************************************/

VectorSiteContainer::VectorSiteContainer(const std::vector<std::string>& names, const Alphabet* alpha) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha)
{
  // Seq names and comments:
  for (auto i : names)
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence(i, "", alpha)), i);
}

/******************************************************************************/

VectorSiteContainer::VectorSiteContainer(const Alphabet* alpha) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(alpha)
{}

/******************************************************************************/

VectorSiteContainer::VectorSiteContainer(const VectorSiteContainer& vsc) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(vsc)
{
  // Now try to add each site:
  for (size_t i = 0; i < vsc.getNumberOfSites(); i++)
  {
    addSite(vsc.getSite(i), false); // We assume that positions are correct.
  }
  setSequencesNames(vsc.getSequencesNames(), false);
}

/******************************************************************************/

VectorSiteContainer::VectorSiteContainer(const SiteContainer& sc) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(sc)
{
  // Now try to add each site:
  for (size_t i = 0; i < sc.getNumberOfSites(); i++)
  {
    addSite(sc.getSite(i), false); // We assume that positions are correct.
  }
  setSequencesNames(sc.getSequencesNames(), false);
}

/******************************************************************************/

VectorSiteContainer::VectorSiteContainer(const OrderedSequenceContainer& osc) :
  VectorPositionedContainer<Site>(), 
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(osc)
{
  for (size_t i = 0; i < osc.getNumberOfSequences(); i++)
  {
    addSequence(osc.getSequence(i), false);
  }
  reindexSites();
}

/******************************************************************************/

VectorSiteContainer::VectorSiteContainer(const SequenceContainer& sc) :
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(sc)
{
  vector<string> names = sc.getSequencesNames();
  for (size_t i = 0; i < names.size(); i++)
  {
    addSequence(sc.getSequence(names[i]), false);
  }
  reindexSites();
}

/******************************************************************************/

VectorSiteContainer::VectorSiteContainer(const AlignedValuesContainer& avc) :
  VectorPositionedContainer<Site>(), 
  VectorMappedContainer<Sequence>(),
  AbstractSequenceContainer(avc.getAlphabet())
{
  const Alphabet* alpha=avc.getAlphabet();

  if (avc.getNumberOfSites() == 0)
    throw Exception("VectorSiteContainer::VectorSiteContainer(AlignedValuesContainer): Empty site set.");

  size_t nbSeq=avc.getNumberOfSequences();
  
  // Seq names and comments:
  
  for (size_t i = 0; i < nbSeq; i++)
  {
    std::shared_ptr<Sequence> ps(new BasicSequence(avc.getName(i), "", alpha));
    
    VectorMappedContainer<Sequence>::appendObject(ps, avc.getName(i));
  }
  
  // Now try to add each site:
  for (size_t i = 0; i < avc.getNumberOfSites(); i++)
  {
    if (!dynamic_cast<const Site*>(&avc.getSymbolListSite(i)))
      throw Exception("VectorSiteContainer::VectorSiteContainer(AlignedValuesContainer) : Not a Site in position " + TextTools::toString(i));
    
    addSite(dynamic_cast<const Site&>(avc.getSymbolListSite(i))); 
  }
}

/******************************************************************************/

VectorSiteContainer& VectorSiteContainer::operator=(const VectorSiteContainer& vsc)
{
  clear();
  AbstractSequenceContainer::operator=(vsc);
  // Now try to add each site:
  for (size_t i = 0; i < vsc.getNumberOfSites(); i++)
  {
    addSite(vsc.getSite(i), false); // We assume that positions are correct.
  }
  setSequencesNames(vsc.getSequencesNames(), false);
  return *this;
}

/******************************************************************************/

VectorSiteContainer& VectorSiteContainer::operator=(const SiteContainer& sc)
{
  clear();
  AbstractSequenceContainer::operator=(sc);
  // Now try to add each site:
  for (size_t i = 0; i < sc.getNumberOfSites(); i++)
  {
    addSite(sc.getSite(i), false); // We assume that positions are correct.
  }
  setSequencesNames(sc.getSequencesNames(), false);

  return *this;
}

/******************************************************************************/

VectorSiteContainer& VectorSiteContainer::operator=(const OrderedSequenceContainer& osc)
{
  clear();
  AbstractSequenceContainer::operator=(osc);

  size_t nbSeq = osc.getNumberOfSequences();
  for (size_t i = 0; i < nbSeq; i++)
  {
    addSequence(osc.getSequence(i), false);
  }
  reindexSites();

  return *this;
}

/******************************************************************************/

VectorSiteContainer& VectorSiteContainer::operator=(const SequenceContainer& sc)
{
  clear();
  AbstractSequenceContainer::operator=(sc);

  vector<string> names = sc.getSequencesNames();
  for (size_t i = 0; i < names.size(); i++)
  {
    addSequence(sc.getSequence(names[i]), false);
  }
  reindexSites();

  return *this;
}

/******************************************************************************/

void VectorSiteContainer::setSite(size_t pos, const Site& site, bool checkPositions)
{
  if (pos >= getNumberOfSites())
    throw IndexOutOfBoundsException("VectorSiteContainer::setSite.", pos, 0, getNumberOfSites() - 1);

  // Check size:
  if (site.size() != getNumberOfSequences())
    throw SiteException("AlignedSequenceContainer::setSite. Site does not have the appropriate length", &site);

  // New site's alphabet and site container's alphabet matching verification
  if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorSiteContainer::setSite", getAlphabet(), site.getAlphabet());

  // Check position:
  if (checkPositions)
  {
    int position = site.getPosition();
    // For all positions in vector : throw exception if position already exists
    for (size_t i = 0; i < getNumberOfSites(); i++)
    {
      if (getSite(i).getPosition() == position)
        throw SiteException("VectorSiteContainer::setSite: Site position already exists in container", &site);
    }
  }
  VectorPositionedContainer<Site>::addObject(shared_ptr<Site>(site.clone()), pos, false);
}

/******************************************************************************/

void VectorSiteContainer::addSite(const Site& site, bool checkPositions)
{
  // Check size:
  if (getNumberOfSequences()!=0 && (site.size() != getNumberOfSequences()))
    throw SiteException("VectorSiteContainer::addSite. Site does not have the appropriate length", &site);

  // New site's alphabet and site container's alphabet matching verification
  if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorSiteContainer::addSite", getAlphabet(), site.getAlphabet());

  // Check position:
  if (checkPositions)
  {
    int position = site.getPosition();
    // For all positions in vector : throw exception if position already exists
    for (size_t i = 0; i < getNumberOfSites(); i++)
    {
      if (getSite(i).getPosition() == position)
        throw SiteException("VectorSiteContainer::addSite. Site position already exists in container", &site);
    }
  }
  
  VectorPositionedContainer<Site>::appendObject(shared_ptr<Site>(site.clone()));
  
  if (getNumberOfSequences()==0)
    for (size_t i = 0; i < site.size(); i++)
      VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", getAlphabet())), "Seq_" + TextTools::toString(i));
}

/******************************************************************************/

void VectorSiteContainer::addSite(const Site& site, int position, bool checkPositions)
{
  // Check size:
  if (getNumberOfSequences()!=0 && (site.size() != getNumberOfSequences()))
    throw SiteException("VectorSiteContainer::addSite. Site does not have the appropriate length", &site);

  // New site's alphabet and site container's alphabet matching verification
  if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorSiteContainer::addSite", getAlphabet(), site.getAlphabet());

  // Check position:
  if (checkPositions)
  {
    // For all positions in vector : throw exception if position already exists
    for (size_t i = 0; i < getNumberOfSites(); i++)
    {
      if (getSite(i).getPosition() == position)
        throw SiteException("VectorSiteContainer::addSite. Site position already exists in container", &site);
    }
  }
  
  shared_ptr<Site> nsite(shared_ptr<Site>(site.clone()));
  nsite->setPosition(position);
  VectorPositionedContainer<Site>::appendObject(nsite);
  
  if (getNumberOfSequences()==0)
    for (size_t i = 0; i < site.size(); i++)
      VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence("Seq_" + TextTools::toString(i), "", getAlphabet())), "Seq_" + TextTools::toString(i));
}

/******************************************************************************/

void VectorSiteContainer::addSite(const Site& site, size_t siteIndex, bool checkPositions)
{
  if (siteIndex >= getNumberOfSites())
    throw IndexOutOfBoundsException("VectorSiteContainer::addSite", siteIndex, 0, getNumberOfSites() - 1);

  // Check size:
  if (site.size() != getNumberOfSequences())
    throw SiteException("VectorSiteContainer::addSite. Site does not have the appropriate length", &site);

  // New site's alphabet and site container's alphabet matching verification
  if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
  {
    throw AlphabetMismatchException("VectorSiteContainer::addSite", getAlphabet(), site.getAlphabet());
  }

  // Check position:
  if (checkPositions)
  {
    int position = site.getPosition();
    // For all positions in vector : throw exception if position already exists
    for (size_t i = 0; i < getNumberOfSites(); i++)
    {
      if (getSite(i).getPosition() == position)
        throw SiteException("VectorSiteContainer::addSite. Site position already exists in container", &site);
    }
  }
  
  VectorPositionedContainer<Site>::insertObject(shared_ptr<Site>(site.clone()), siteIndex);
}

/******************************************************************************/

void VectorSiteContainer::addSite(const Site& site, size_t siteIndex, int position, bool checkPositions)
{
  if (siteIndex >= getNumberOfSites())
    throw IndexOutOfBoundsException("VectorSiteContainer::addSite", siteIndex, 0, getNumberOfSites() - 1);

  // Check size:
  if (site.size() != getNumberOfSequences())
    throw SiteException("VectorSiteContainer::addSite. Site does not have the appropriate length", &site);

  // New site's alphabet and site container's alphabet matching verification
  if (site.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
  {
    throw AlphabetMismatchException("VectorSiteContainer::addSite", getAlphabet(), site.getAlphabet());
  }

  // Check position:
  if (checkPositions)
  {
    // For all positions in vector : throw exception if position already exists
    for (size_t i = 0; i < getNumberOfSites(); i++)
    {
      if (getSite(i).getPosition() == position)
        throw SiteException("VectorSiteContainer::addSite. Site position already exists in container", &site);
    }
  }

  shared_ptr<Site> nsite(shared_ptr<Site>(site.clone()));
  nsite->setPosition(position);
  VectorPositionedContainer<Site>::insertObject(nsite, siteIndex);
}


/******************************************************************************/

void VectorSiteContainer::reindexSites()
{
  for (size_t i=0; i<getNumberOfSites(); i++)
    getSite(i).setPosition((int)i+1);
}

/******************************************************************************/

Vint VectorSiteContainer::getSitePositions() const
{
  Vint positions(getNumberOfSites());
  for (size_t i = 0; i < getNumberOfSites(); i++)
    positions[i] = getSite(i).getPosition();

  return positions;
}


void VectorSiteContainer::setSitePositions(Vint vPositions)
{
  if (vPositions.size() != getNumberOfSites())
    throw BadSizeException("VectorSiteContainer::setSitePositions bad size of positions vector", vPositions.size(), getNumberOfSites());
  
  for (size_t i = 0; i < getNumberOfSites(); i++)
    getSite(i).setPosition(vPositions[i]);
}

/******************************************************************************/

const Sequence& VectorSiteContainer::getSequence(size_t i) const
{
  if (i >= getNumberOfSequences())
    throw IndexOutOfBoundsException("VectorSiteContainer::getSequence.", i, 0, getNumberOfSequences() - 1);

  // Main loop : for all sites
  size_t n = getNumberOfSites();
  vector<int> sequence(n);
  for (size_t j = 0; j < n; j++)
  {
    sequence[j] = getSite(j)[i];
  }

  shared_ptr<Sequence> ns(shared_ptr<Sequence>(new BasicSequence(VectorMappedContainer<Sequence>::getObjectName(i), sequence, VectorMappedContainer<Sequence>::getObject(i)->getComments(), getAlphabet())));

  VectorMappedContainer<Sequence>::addObject_(ns,i,ns->getName(), false);

  return *ns;
}

/******************************************************************************/

const Sequence& VectorSiteContainer::getSequence(const string& name) const
{
  // Look for sequence name:
  size_t pos = getSequencePosition(name);
  return getSequence(pos);
}

/******************************************************************************/

void VectorSiteContainer::setSequence(const string& name, const Sequence& sequence, bool checkNames)
{
  // Look for sequence name:
  size_t pos = getSequencePosition(name);
  setSequence(pos, sequence, checkNames);
}

/******************************************************************************/

void VectorSiteContainer::setSequence(size_t pos, const Sequence& sequence, bool checkNames)
{
  if (pos >= getNumberOfSequences())
    throw IndexOutOfBoundsException("VectorSiteContainer::setSequence", pos, 0, getNumberOfSequences() - 1);

  // New sequence's alphabet and site container's alphabet matching verification
  if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorSiteContainer::setSequence", getAlphabet(), sequence.getAlphabet());

  const string& sname=sequence.getName();

  // If the container has only one sequence, we set the size to the size of this sequence:
  if (getNumberOfSequences() == 1)
  {
    realloc(sequence.size());
    VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence(sname, "", getAlphabet())), sname);
  }
  else
  {
    if (sequence.size() != getNumberOfSites())
      throw SequenceException("VectorSiteContainer::setSequence. Sequence has not the appropriate length.", &sequence);
  
    if (checkNames)
    {
      if (VectorMappedContainer<Sequence>::hasObject(sname) && VectorMappedContainer<Sequence>::getObjectPosition(sname)!=pos)
        throw SequenceException("VectorSiteContainer::setSequence. Name already exists in another sequence in container.", &sequence);
    }
  
    // Update name:
    
    VectorMappedContainer<Sequence>::setObjectName(pos, sname);
    VectorMappedContainer<Sequence>::getObject(pos)->setName(sname);
  }
  
  // Update elements at each site:
  for (size_t i = 0; i < getNumberOfSites(); i++)
  {
    getSite(i).setElement(pos, sequence.getValue(i));
  }

  // comments
  VectorMappedContainer<Sequence>::getObject_(pos)->setComments(sequence.getComments());
}

/******************************************************************************/

Sequence* VectorSiteContainer::removeSequence(size_t i)
{
  getSequence(i);
  
  return VectorMappedContainer<Sequence>::getObject(i).get();
}

/******************************************************************************/

Sequence* VectorSiteContainer::removeSequence(const string& name)
{
  // Look for sequence name:
  size_t pos = getSequencePosition(name);
  return removeSequence(pos);
}

/******************************************************************************/

void VectorSiteContainer::addSequence(const Sequence& sequence, bool checkNames)
{
  // If the container has no sequence, we set the size to the size of this sequence:
  if (getNumberOfSequences() == 0)
    realloc(sequence.size());

  // New sequence's alphabet and site container's alphabet matching verification
  if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorSiteContainer::addSequence", getAlphabet(), sequence.getAlphabet());

  if (sequence.size() != getNumberOfSites())
    throw SequenceException("VectorSiteContainer::addSequence. Sequence has not the appropriate length: " + TextTools::toString(sequence.size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".", &sequence);

  const string& sname=sequence.getName();
  
  if (checkNames && VectorMappedContainer<Sequence>::hasObject(sname))
    throw SequenceException("VectorSiteContainer::addSequence. Name already exists in container.", &sequence);

  VectorMappedContainer<Sequence>::appendObject(std::shared_ptr<Sequence>(new BasicSequence(sname, "", sequence.getComments(), getAlphabet())), sname);

  // Update elements at each site:
  for (size_t i = 0; i < getNumberOfSites(); i++)
    getSite(i).addElement(sequence.getValue(i));

}
    
/******************************************************************************/

void VectorSiteContainer::addSequence(const Sequence& sequence, size_t pos, bool checkNames)
{
  if (pos >= getNumberOfSequences())
    throw IndexOutOfBoundsException("VectorSiteContainer::addSequence.", pos, 0, getNumberOfSequences() - 1);
  
  if (sequence.size() != getNumberOfSites())
    throw SequenceNotAlignedException("VectorSiteContainer::addSequence", &sequence);

  // New sequence's alphabet and site container's alphabet matching verification
  if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorSiteContainer::addSequence", getAlphabet(), sequence.getAlphabet());

  const string& sname=sequence.getName();
  
  if (checkNames && VectorMappedContainer<Sequence>::hasObject(sname))
    throw SequenceException("VectorSiteContainer::addSequence. Name already exists in container.", &sequence);

  VectorMappedContainer<Sequence>::insertObject(std::shared_ptr<Sequence>(new BasicSequence(sname, "", sequence.getComments(), getAlphabet())), pos, sname);
  
  // Update elements at each site:
  for (size_t i = 0; i < getNumberOfSites(); i++)
    getSite(i).addElement(pos, sequence.getValue(i));
}

/******************************************************************************/

void VectorSiteContainer::clear()
{
  VectorPositionedContainer<Site>::clear();
  VectorMappedContainer<Sequence>::clear();
}

/******************************************************************************/

void VectorSiteContainer::realloc(size_t n)
{
  clear();
  Site s(getAlphabet());
    
  for (size_t i=0;i<n;i++)
    addSite(s,false);
    
  reindexSites();
}

/******************************************************************************/

void VectorSiteContainer::setComments(size_t sequenceIndex, const Comments& comments)
{
  VectorMappedContainer<Sequence>::getObject(sequenceIndex)->setComments(comments);
}

/******************************************************************************/

VectorSiteContainer* VectorSiteContainer::createEmptyContainer() const
{
  VectorSiteContainer* vsc = new VectorSiteContainer(getAlphabet());
  vsc->setGeneralComments(getGeneralComments());
  return vsc;
}

/******************************************************************************/

