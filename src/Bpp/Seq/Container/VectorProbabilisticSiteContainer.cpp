//
// File: VectorProbabilisticSiteContainer.cpp
// Created by: Murray Patterson
// Created on: Mon Oct 19 2015
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

#include "VectorProbabilisticSiteContainer.h"

#include <Bpp/Text/TextTools.h>

using namespace bpp;
using namespace std;

/********************************************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(const Alphabet * alpha) :
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<BasicProbabilisticSequence>(),
  AbstractValuesContainer(alpha),
  AbstractProbabilisticSequenceContainer(alpha)
{}

/** Class constructors: *******************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(
  const std::vector<const CruxSymbolListSite*>& vs,
  const Alphabet* alpha,
  bool checkPositions) :
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<BasicProbabilisticSequence>(),
  AbstractValuesContainer(alpha),
  AbstractProbabilisticSequenceContainer(alpha)
{
  size_t nRow=alpha->getSize();
  if (vs.size() == 0)
    throw Exception("VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(vector). Empty site set.");
  
  size_t nbSeq=vs[0]->size();
  // Seq names and comments:
  
  for (size_t i = 0; i < nbSeq; i++)
    VectorMappedContainer<BasicProbabilisticSequence>::appendObject(std::shared_ptr<BasicProbabilisticSequence>(new BasicProbabilisticSequence("Seq_" + TextTools::toString(i), Table<double>(nRow,0), alpha)), "Seq_" + TextTools::toString(i));

  // Now try to add each site:
  for (size_t i = 0; i < vs.size(); i++)
  {
    if (!dynamic_cast<const ProbabilisticSite*>(vs[i]))
      throw Exception("VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer : Not a ProbabilisticSite in position " + TextTools::toString(i));
    
    appendSite(shared_ptr<ProbabilisticSite>(dynamic_cast<ProbabilisticSite*>(vs[i]->clone())), checkPositions); // This may throw an exception if position argument already exists or is size is not valid.
  }
}


VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(
  const std::vector<std::string>& names, const Alphabet* alpha) :
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<BasicProbabilisticSequence>(),
  AbstractValuesContainer(alpha),
  AbstractProbabilisticSequenceContainer(alpha)
{
  size_t nRow=alpha->getSize();
  
  for (auto i : names)
    VectorMappedContainer<BasicProbabilisticSequence>::appendObject(std::shared_ptr<BasicProbabilisticSequence>(new BasicProbabilisticSequence(i, Table<double>(nRow,0), alpha)), i);
}


/** Class constructors: *******************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(
  const AlignedValuesContainer& avc) :
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<BasicProbabilisticSequence>(),
  AbstractValuesContainer(avc.getAlphabet()),
  AbstractProbabilisticSequenceContainer(avc.getAlphabet())
{
  const Alphabet* alpha=avc.getAlphabet();
  
  size_t nRow=alpha->getSize();
  if (avc.getNumberOfSites() == 0) throw Exception("VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(AlignedValuesContainer). Empty site set.");

  // Seq names and comments:

  vector<string> vn=avc.getSequencesNames();
  for (auto& n : vn)
    VectorMappedContainer<BasicProbabilisticSequence>::appendObject(std::shared_ptr<BasicProbabilisticSequence>(new BasicProbabilisticSequence(n, Table<double>(nRow,0), alpha)), n);

  // Now add each site:
  for (size_t i = 0; i < avc.getNumberOfSites(); i++)
  {
    shared_ptr<ProbabilisticSite> ps(new ProbabilisticSite(avc.getSymbolListSite(i)));
    appendSite(ps);
  }
}


/******************************************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(const VectorProbabilisticSiteContainer& vpsc) :
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<BasicProbabilisticSequence>(),
  AbstractValuesContainer(vpsc),
  AbstractProbabilisticSequenceContainer(vpsc)
{
  // Now try to add each site:
  for (size_t i = 0; i < vpsc.getNumberOfSites(); i++)
  {
    appendSite(shared_ptr<ProbabilisticSite>(vpsc.getSite(i)->clone()), false); // We assume that positions are correct.
  }
  setSequencesNames(vpsc.getSequencesNames(), false);
}

/******************************************************************************/

VectorProbabilisticSiteContainer::VectorProbabilisticSiteContainer(const OrderedSequenceContainer& osc) :
  VectorPositionedContainer<ProbabilisticSite>(),
  VectorMappedContainer<BasicProbabilisticSequence>(),
  AbstractValuesContainer(osc.getGeneralComments(), osc.getAlphabet()),
  AbstractProbabilisticSequenceContainer(osc.getAlphabet())
{
  // Now try to add each site:
  for (size_t i = 0; i < osc.getNumberOfSequences(); i++)
  {
    addSequence(osc.getSequence(i), false); // We assume that positions are correct.
  }
  setSequencesNames(osc.getSequencesNames(), false);
}

/******************************************************************************/

VectorProbabilisticSiteContainer& VectorProbabilisticSiteContainer::operator=(const VectorProbabilisticSiteContainer& vpsc)
{
  clear();
  AbstractProbabilisticSequenceContainer::operator=(vpsc);
  
  // Now try to add each site:
  for (size_t i = 0; i < vpsc.getNumberOfSites(); i++)
  {
    appendSite(shared_ptr<ProbabilisticSite>(vpsc.getSite(i)->clone()), false); // We assume that positions are correct.
  }
  setSequencesNames(vpsc.getSequencesNames(), false);
  return *this;
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::appendSite(std::shared_ptr<ProbabilisticSite> site, bool checkPosition) 
{
  // check size :
  if(site->size() != getNumberOfSequences())
    throw Exception("VectorProbabilisticSiteContainer::appendSite. Site does not have the appropriate length: " + TextTools::toString(site->size()) + ", should be " + TextTools::toString(getNumberOfSequences()) + ".");

  // new site's alphabet and site container's alphabet must match :
  if(site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::appendSite.", getAlphabet(), site->getAlphabet());

  int position = site->getPosition();

  // check position :
  if (checkPosition)
  {
    // For all positions in vector : throw exception if position already exists
    for (size_t i = 0; i < getNumberOfSites(); i++)
    {
      if (getSite(i)!=nullptr && getSite(i)->getPosition() == position)
        throw Exception("VectorProbabilisticSiteContainer::appendSite. Site position: " + TextTools::toString(position) + ", already used in container.");
    }
  }

  VectorPositionedContainer<ProbabilisticSite>::appendObject(site);
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::addSequence(const std::shared_ptr<BasicProbabilisticSequence> sequence, bool checkName)
{
  // new sequence's alphabet and site container's alphabet must match :
  if(sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::addSequence->", getAlphabet(), sequence->getAlphabet());

  // check name :
  if (checkName)
    if (hasObject(sequence->getName()))
      throw Exception("VectorProbabilisticSiteContainer::addSequence. Name: " + sequence->getName() + ", already exists in the container.");

  // if the container has no sequence, we set the size to the size of
  // this sequence :
  if (getNumberOfSequences() == 0)
  {
    VectorPositionedContainer<ProbabilisticSite>::setSize(0);
    for (size_t i=0;i<(size_t)sequence->size();i++)
      appendSite(shared_ptr<ProbabilisticSite>(new ProbabilisticSite(getAlphabet(),(int)(i)+1)));
  }
  else 
    if(sequence->size() != getNumberOfSites())
      throw Exception("VectorProbabilisticSiteContainer::addSequence. Sequence does not have the appropriate length: " + TextTools::toString(sequence->size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".");

  // append empty sequence

  VectorMappedContainer<BasicProbabilisticSequence>::appendObject(std::shared_ptr<BasicProbabilisticSequence>(new BasicProbabilisticSequence(sequence->getName(), Table<double>(getAlphabet()->getSize(),0), getAlphabet())), sequence->getName());

  // append elements at each site :
  
  for(size_t i = 0; i < getNumberOfSites(); ++i)
  {
    getSite(i)->addElement(sequence->getContent()[i]);
  }
  
  // // append comments :
  setGeneralComments(sequence->Commentable::getComments());
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::addSequence(const Sequence& sequence, bool checkName)
 {
  // new sequence's alphabet and site container's alphabet must match :
  if(sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::addSequence", getAlphabet(), sequence.getAlphabet());

  // check name :
  if (checkName)
    if (hasObject(sequence.getName()))
      throw Exception("VectorProbabilisticSiteContainer::addSequence. Name: " + sequence.getName() + ", already exists in the container.");
  
  // if the container has no sequence, we set the size to the size of this sequence :
  if (getNumberOfSequences() == 0)
  {
    VectorPositionedContainer<ProbabilisticSite>::setSize(0);
    
    for (size_t i=0;i<(size_t)sequence.size();i++)
      appendSite(shared_ptr<ProbabilisticSite>(new ProbabilisticSite(getAlphabet(),(int)(i)+1)));
  }
  else if(sequence.size() != getNumberOfSites())
    throw Exception("VectorProbabilisticSiteContainer::addSequence. Sequence does not have the appropriate length: " + TextTools::toString(sequence.size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".");

  // append the empty sequence

  VectorMappedContainer<BasicProbabilisticSequence>::appendObject(shared_ptr<BasicProbabilisticSequence>(new BasicProbabilisticSequence(sequence.getName(), Table<double>(getAlphabet()->getSize(), 0),  getAlphabet())), sequence.getName());

  // conversion

  size_t size=getAlphabet()->getSize();
  
  vector<double> vd(size,0.);
  
  for (size_t i=0; i<sequence.size(); i++)
  {
    for (size_t s=0; s<size; s++)
      vd[s]=sequence.getStateValueAt(i,(int)s);
    getSite(i)->addElement(vd);
  }
      
  // // append comments :
  setGeneralComments(sequence.getComments());
}


/********************************************************************************/

void VectorProbabilisticSiteContainer::clear()
{
  VectorPositionedContainer<ProbabilisticSite>::clear(); 
  VectorMappedContainer<BasicProbabilisticSequence>::clear();
}  

/********************************************************************************/

void VectorProbabilisticSiteContainer::reindexSites()
{
  for(size_t i=0; i<getNumberOfSites(); i++)
    getSite(i)->setPosition((int)i+1);
}

/******************************************************************************/

Vint VectorProbabilisticSiteContainer::getSitePositions() const
{
  Vint positions(getNumberOfSites());
  for (size_t i = 0; i < getNumberOfSites(); i++)
    positions[i] = getSite(i)->getPosition();

  return positions;
}

/******************************************************************************/

void VectorProbabilisticSiteContainer::setSitePositions(Vint vPositions)
{
  if (vPositions.size() != getNumberOfSites())
    throw BadSizeException("VectorSiteContainer::setSitePositions bad size of positions vector", vPositions.size(), getNumberOfSites());
  
  for (size_t i = 0; i < getNumberOfSites(); i++)
    getSite(i)->setPosition(vPositions[i]);
}

/******************************************************************************/

VectorProbabilisticSiteContainer* VectorProbabilisticSiteContainer::createEmptyContainer() const
{
  VectorProbabilisticSiteContainer* vsc = new VectorProbabilisticSiteContainer(getAlphabet());
  vsc->setGeneralComments(getGeneralComments());
  return vsc;
}

/******************************************************************************/

const std::shared_ptr<BasicProbabilisticSequence> VectorProbabilisticSiteContainer::getSequence(size_t i) const
{
  if (i >= getNumberOfSequences())
    throw IndexOutOfBoundsException("VectorProbabilisticSiteContainer::getSequence.", i, 0, getNumberOfSequences() - 1);

  // Main loop : for all sites
  size_t n = getNumberOfSites();
  Table<double> sequence(getAlphabet()->getSize(),0);
  for (size_t j = 0; j < n; j++)
  {
    sequence.addColumn((*getSite(j))[i]);
  }

  shared_ptr<BasicProbabilisticSequence> ns(shared_ptr<BasicProbabilisticSequence>(new BasicProbabilisticSequence(VectorMappedContainer<BasicProbabilisticSequence>::getObjectName(i), sequence, VectorMappedContainer<BasicProbabilisticSequence>::getObject(i)->getComments(), getAlphabet())));

  VectorMappedContainer<BasicProbabilisticSequence>::addObject_(ns,i,ns->getName(), false);

  return ns;
}

/******************************************************************************/

const std::shared_ptr<BasicProbabilisticSequence> VectorProbabilisticSiteContainer::getSequence(const string& name) const
{
  // Look for sequence name:
  size_t pos = getSequencePosition(name);
  return getSequence(pos);
}


