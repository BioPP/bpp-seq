//
// File: VectorProbabilisticSiteContainer.cpp
// Authors:
//   Murray Patterson
// Created: 2015-10-19 00:00:00
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

#include "VectorProbabilisticSiteContainer.h"

using namespace bpp;
using namespace std;

/********************************************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(const Alphabet* alpha) :
  AbstractValuesContainer(alpha),
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<ProbabilisticSequence>(),
  AbstractProbabilisticSequenceContainer(alpha)
{}

/** Class constructors: *******************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(
  const std::vector<const CruxSymbolListSite*>& vs,
  const Alphabet* alpha,
  bool checkPositions) :
  AbstractValuesContainer(alpha),
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<ProbabilisticSequence>(),
  AbstractProbabilisticSequenceContainer(alpha)
{
  if (vs.size() == 0)
    throw Exception("VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(vector). Empty site set.");

  size_t nbSeq = vs[0]->size();
  // Seq names and comments:

  for (size_t i = 0; i < nbSeq; i++)
    VectorMappedContainer<ProbabilisticSequence>::appendObject(nullptr, "Seq_" + TextTools::toString(i));

  // Now try to add each site:
  for (size_t i = 0; i < vs.size(); i++)
  {
    if (!dynamic_cast<const ProbabilisticSite*>(vs[i]))
      throw Exception("VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer : Not a ProbabilisticSite in position " + TextTools::toString(i));

    addSite(shared_ptr<ProbabilisticSite>(dynamic_cast<ProbabilisticSite*>(vs[i]->clone())), checkPositions); // This may throw an exception if position argument already exists or is size is not valid.
  }
}


VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(
  const std::vector<std::string>& names, const Alphabet* alpha) :
  AbstractValuesContainer(alpha),
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<ProbabilisticSequence>(),
  AbstractProbabilisticSequenceContainer(alpha)
{
  for (auto i : names)
    VectorMappedContainer<ProbabilisticSequence>::appendObject(nullptr, i);
}

/******************************************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(
  const AlignedValuesContainer& avc) :
  AbstractValuesContainer(avc.getAlphabet()),
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<ProbabilisticSequence>(),
  AbstractProbabilisticSequenceContainer(avc.getAlphabet())
{
  if (avc.getNumberOfSites() == 0)
    throw Exception("VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(AlignedValuesContainer). Empty site set.");

  // Seq names and comments:
  for (const auto& name : avc.getSequenceNames())
    VectorMappedContainer<ProbabilisticSequence>::appendObject(nullptr, name);

  // Now add each site:
  for (size_t i = 0; i < avc.getNumberOfSites(); i++)
  {
    shared_ptr<ProbabilisticSite> ps(new ProbabilisticSite(avc.getSymbolListSite(i)));
    addSite(ps);
  }
}


/******************************************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(const VectorProbabilisticSiteContainer& vpsc) :
  AbstractValuesContainer(vpsc),
  VectorPositionedContainer<ProbabilisticSite>(vpsc),
  VectorMappedContainer<ProbabilisticSequence>(vpsc),
  AbstractProbabilisticSequenceContainer(vpsc)
{}

/******************************************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(const OrderedSequenceContainer& osc) :
  AbstractValuesContainer(osc.getGeneralComments(), osc.getAlphabet()),
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<ProbabilisticSequence>(),
  AbstractProbabilisticSequenceContainer(osc.getAlphabet())
{
  // Now try to add each site:
  for (size_t i = 0; i < osc.getNumberOfSequences(); i++)
    addSequence(osc.getSequence(i), false); // We assume that positions are correct.
  
  setSequenceNames(osc.getSequenceNames(), false);
}

/******************************************************************************/

VectorProbabilisticSiteContainer& VectorProbabilisticSiteContainer::operator=(const VectorProbabilisticSiteContainer& vpsc)
{
  clear();
  AbstractProbabilisticSequenceContainer::operator=(vpsc);
  
  // Now try to add each site:
  for (size_t i = 0; i < vpsc.getNumberOfSites(); i++)
    addSite(shared_ptr<ProbabilisticSite>(vpsc.getSite(i)->clone()), false); // We assume that positions are correct.

  const auto& vnames=vpsc.getSequenceNames();
  for (const auto& name:vnames)
    VectorMappedContainer<ProbabilisticSequence>::appendObject(nullptr, name);

  return *this;
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::addSite(std::shared_ptr<ProbabilisticSite> site, bool checkPosition)
{
  // check size :
  if (site->size() != getNumberOfSequences())
    throw Exception("VectorProbabilisticSiteContainer::addSite. Site does not have the appropriate length: " + TextTools::toString(site->size()) + ", should be " + TextTools::toString(getNumberOfSequences()) + ".");

  // new site's alphabet and site container's alphabet must match :
  if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::addSite.", getAlphabet(), site->getAlphabet());

  int position = site->getPosition();

  // check position :
  if (checkPosition)
  {
    // For all positions in vector : throw exception if position already exists
    for (size_t i = 0; i < getNumberOfSites(); i++)
    {
      if (getSite(i) != nullptr && getSite(i)->getPosition() == position)
        throw Exception("VectorProbabilisticSiteContainer::addSite. Site position: " + TextTools::toString(position) + ", already used in container.");
    }
  }
  VectorPositionedContainer<ProbabilisticSite>::appendObject(site);
  if (getNumberOfSequences() == 0)
    for (size_t i = 0; i < site->size(); i++)
    {
      VectorMappedContainer<ProbabilisticSequence>::appendObject(nullptr, "Seq_" + TextTools::toString(i));
    }
  else
  {// Clean Sequence Container cache
    VectorMappedContainer<ProbabilisticSequence>::nullify();
  }
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::addSequence(const ProbabilisticSequence& sequence, bool checkName)
{
  // new sequence's alphabet and site container's alphabet must match :

  if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::addSequence->", getAlphabet(), sequence.getAlphabet());

  // check name :
  if (checkName)
    if (hasObject(sequence.getName()))
      throw Exception("VectorProbabilisticSiteContainer::addSequence. Name: " + sequence.getName() + ", already exists in the container.");

  // if the container has no sequence, we set the size to the size of
  // this sequence :
  if (getNumberOfSequences() == 0)
  {
    VectorPositionedContainer<ProbabilisticSite>::setSize(0);
    for (size_t i = 0; i < (size_t)sequence.size(); i++)
    {
      addSite(shared_ptr<ProbabilisticSite>(new ProbabilisticSite(getAlphabet(), (int)(i) + 1)));
    }
  }
  else if (sequence.size() != getNumberOfSites())
    throw Exception("VectorProbabilisticSiteContainer::addSequence. Sequence does not have the appropriate length: " + TextTools::toString(sequence.size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".");

  // append null sequence

  VectorMappedContainer<ProbabilisticSequence>::appendObject(nullptr, sequence.getName());

  // append elements at each site :

  for (size_t i = 0; i < getNumberOfSites(); ++i)
     getSite(i)->addElement(sequence.getContent()[i]);
 

  // // append comments :
  setGeneralComments(sequence.getComments());
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::addSequence(const Sequence& sequence, bool checkName)
{
  // new sequence's alphabet and site container's alphabet must match :
  if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::addSequence", getAlphabet(), sequence.getAlphabet());

  // check name :
  if (checkName)
    if (hasObject(sequence.getName()))
      throw Exception("VectorProbabilisticSiteContainer::addSequence. Name: " + sequence.getName() + ", already exists in the container.");

  // if the container has no sequence, we set the size to the size of this sequence :
  if (getNumberOfSequences() == 0)
  {
    VectorPositionedContainer<ProbabilisticSite>::setSize(0);

    for (size_t i = 0; i < (size_t)sequence.size(); i++)
       addSite(shared_ptr<ProbabilisticSite>(new ProbabilisticSite(getAlphabet(), (int)(i) + 1)));
  }
  else if (sequence.size() != getNumberOfSites())
    throw Exception("VectorProbabilisticSiteContainer::addSequence. Sequence does not have the appropriate length: " + TextTools::toString(sequence.size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".");

  // append null sequence

  VectorMappedContainer<ProbabilisticSequence>::appendObject(nullptr, sequence.getName());

  // conversion

  size_t size = getAlphabet()->getSize();

  vector<double> vd(size, 0.);

  for (size_t i = 0; i < sequence.size(); i++)
  {
    for (size_t s = 0; s < size; s++)
    {
      vd[s] = sequence.getStateValueAt(i, (int)s);
    }
    getSite(i)->addElement(vd);
  }

  // // append comments :
  setGeneralComments(sequence.getComments());
}


/********************************************************************************/

void VectorProbabilisticSiteContainer::clear()
{
  VectorPositionedContainer<ProbabilisticSite>::clear();
  VectorMappedContainer<ProbabilisticSequence>::clear();
}

void VectorProbabilisticSiteContainer::realloc(size_t n)
{
  clear();
  auto s = std::make_shared<ProbabilisticSite>(getAlphabet());

  for (size_t i = 0; i < n; i++)
    addSite(s, false);

  reindexSites();
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::reindexSites()
{
  for (size_t i = 0; i < getNumberOfSites(); i++)
  {
    getSite(i)->setPosition((int)i + 1);
  }
}

/******************************************************************************/

Vint VectorProbabilisticSiteContainer::getSitePositions() const
{
  Vint positions(getNumberOfSites());
  for (size_t i = 0; i < getNumberOfSites(); i++)
  {
    positions[i] = getSite(i)->getPosition();
  }

  return positions;
}

/******************************************************************************/

void VectorProbabilisticSiteContainer::setSitePositions(Vint vPositions)
{
  if (vPositions.size() != getNumberOfSites())
    throw BadSizeException("VectorSiteContainer::setSitePositions bad size of positions vector", vPositions.size(), getNumberOfSites());

  for (size_t i = 0; i < getNumberOfSites(); i++)
  {
    getSite(i)->setPosition(vPositions[i]);
  }
}

/******************************************************************************/

VectorProbabilisticSiteContainer* VectorProbabilisticSiteContainer::createEmptyContainer() const
{
  VectorProbabilisticSiteContainer* vsc = new VectorProbabilisticSiteContainer(getAlphabet());
  vsc->setGeneralComments(getGeneralComments());
  return vsc;
}

/******************************************************************************/

const ProbabilisticSequence& VectorProbabilisticSiteContainer::getSequence(std::size_t i) const
{
  if (i >= getNumberOfSequences())
    throw IndexOutOfBoundsException("VectorProbabilisticSiteContainer::getSequence.", i, 0, getNumberOfSequences() - 1);

  // If Sequence already exsits
  auto name = VectorMappedContainer<ProbabilisticSequence>::getObjectName(i);
  if (!isAvailableName(name))
    return *VectorMappedContainer<ProbabilisticSequence>::getObject(i);

  // Main loop : for all sites
  size_t n = getNumberOfSites();
  Table<double> sequence(getAlphabet()->getSize(), 0);
  for (size_t j = 0; j < n; j++)
  {
    sequence.addColumn((*getSite(j))[i]);
  }

  shared_ptr<ProbabilisticSequence> ns(shared_ptr<ProbabilisticSequence>(new BasicProbabilisticSequence(name, sequence, VectorMappedContainer<ProbabilisticSequence>::getObject(i)?VectorMappedContainer<ProbabilisticSequence>::getObject(i)->getComments():std::vector<string>(1,""), getAlphabet())));

  VectorMappedContainer<ProbabilisticSequence>::addObject_(ns, i, ns->getName(), false);

  return *ns;
}

/******************************************************************************/

const ProbabilisticSequence& VectorProbabilisticSiteContainer::getSequence(const string& name) const
{
  // Look for sequence name:
  size_t pos = getSequencePosition(name);
  return getSequence(pos);
}


/******************************************************************************/

void VectorProbabilisticSiteContainer::setSequence(const string& name, const ProbabilisticSequence& sequence, bool checkNames)
{
  // Look for sequence name:
  size_t pos = getSequencePosition(name);
  setSequence(pos, sequence, checkNames);
}

/******************************************************************************/

void VectorProbabilisticSiteContainer::setSequence(size_t pos, const ProbabilisticSequence& sequence, bool checkNames)
{
  if (pos >= getNumberOfSequences())
    throw IndexOutOfBoundsException("VectorProbabilisticSiteContainer::setSequence", pos, 0, getNumberOfSequences() - 1);

  // New sequence's alphabet and site container's alphabet matching verification
  if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::setSequence", getAlphabet(), sequence.getAlphabet());

  const string& sname = sequence.getName();

  // If the container has only one sequence, we set the size to the size of this sequence:
  if (getNumberOfSequences() == 1)
  {
    realloc(sequence.size());
  }
  else
  {
    if (sequence.size() != getNumberOfSites())
      throw BadSizeException("VectorProbabilisticSiteContainer::setSequence. Sequence has not the appropriate length.", sequence.size(), getNumberOfSites());
    
    if (checkNames)
    {
      if (VectorMappedContainer<ProbabilisticSequence>::hasObject(sname) && VectorMappedContainer<ProbabilisticSequence>::getObjectPosition(sname) != pos)
        throw Exception("VectorProbabilisticSiteContainer::setSequence. Name already exists in another sequence in container " + sname);
    }
  }

  // Update elements at each site:
  for (size_t i = 0; i < getNumberOfSites(); i++)
    getSite(i)->setElement(pos, sequence.getValue(i));
  
  // update sequence name
  VectorMappedContainer<ProbabilisticSequence>::changeName(getObjectName(pos), sname);

  // nullifies related ProbabilisticSequence in Container
  VectorMappedContainer<ProbabilisticSequence>::getObject(sname)=nullptr;
}
