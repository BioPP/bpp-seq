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
  Commentable(),
  alphabet_(alpha)
{}

/********************************************************************************/

void VectorProbabilisticSiteContainer::addSite(std::shared_ptr<ProbabilisticSite> site, bool checkPosition) 
{
  size_t nbSi=getNumberOfSites();
  
  // check size :
  if(site->size() != getNumberOfSequences())
    throw Exception("VectorProbabilisticSiteContainer::addSite. Site does not have the appropriate length: " + TextTools::toString(site->size()) + ", should be " + TextTools::toString(getNumberOfSequences()) + ".");

  // new site's alphabet and site container's alphabet must match :
  if(site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::addSite.", getAlphabet(), site->getAlphabet());

  size_t position = (size_t)site->getPosition();

  // check position :
  if (checkPosition && position<=nbSi)
  {
    if (getSite(position-1)!=nullptr)
      throw Exception("VectorProbabilisticSiteContainer::addSite. Site position: " + TextTools::toString(position) + ", already used in container.");
  }

  
  if (position<=nbSi)
    VectorPositionedContainer<ProbabilisticSite>::addObject(site, position-1);
  else
  {
    VectorPositionedContainer<ProbabilisticSite>::appendObject(site);
    site->setPosition((int)nbSi+1);
  }
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::addSequence(std::shared_ptr<BasicProbabilisticSequence> sequence, bool checkName)
{
  // if the container has no sequence, we set the size to the size of this sequence :
  if(getNumberOfSequences() == 0)
    VectorPositionedContainer<ProbabilisticSite>::setSize((size_t)sequence->size());

  // new sequence's alphabet and site container's alphabet must match :
  if(sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorProbabilisticSiteContainer::addSequence->", getAlphabet(), sequence->getAlphabet());

  if(sequence->size() != getNumberOfSites())
    throw Exception("VectorProbabilisticSiteContainer::addSequence. Sequence does not have the appropriate length: " + TextTools::toString(sequence->size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".");

  // check name :
  if (checkName)
    if (hasObject(sequence->getName()))
      throw Exception("VectorProbabilisticSiteContainer::addSequence. Name: " + sequence->getName() + ", already exists in the container.");

  // new sites :
  for(size_t i = 0; i < getNumberOfSites(); ++i)
  {
    if (!getSite(i))
      addSite(std::shared_ptr<ProbabilisticSite>(new ProbabilisticSite(sequence->getAlphabet(), (int)(i+1))));

    ProbabilisticSite bpl(sequence->getAlphabet(), (int)(i+1));
  }

  // append :
  VectorMappedContainer<BasicProbabilisticSequence>::appendObject(sequence, sequence->getName());
  
  // append elements at each site :
    
  for(size_t i = 0; i < getNumberOfSites(); ++i)
    getSite(i)->addElement(sequence->getContent()[i]);
  
  // // append comments :
  setComments(sequence->getComments());
}

/********************************************************************************/

void VectorProbabilisticSiteContainer::clear()
{
  VectorPositionedContainer<ProbabilisticSite>::clear(); // call VectorSiteContainer clear
  VectorMappedContainer<BasicProbabilisticSequence>::clear();
}  

/********************************************************************************/

void VectorProbabilisticSiteContainer::reindexSites()
{
  int pos = 1; // start at position 1
  VectorPositionedContainer<ProbabilisticSite>::Iterator it(*this), end(*this,getNumberOfSites());
  for(; it!=end; it++)
    (*it)->setPosition(++pos);
}

