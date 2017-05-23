//
// File: VectorIntSiteContainer.cpp
// Created by: Laurent Gueguen
// Created on: vendredi 31 mars 2017, à 15h 52
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

#include "VectorIntSiteContainer.h"

#include <Bpp/Text/TextTools.h>

using namespace bpp;
using namespace std;

/********************************************************************************/

VectorIntSiteContainer::VectorIntSiteContainer(const Alphabet * alpha) :
  AbstractSequenceContainer(alpha),
  VectorPositionedContainer<Site>(),
  VectorMappedContainer<Sequence>()
{}

/********************************************************************************/

void VectorIntSiteContainer::addSite(std::shared_ptr<Site> site, bool checkPosition) 
{
  size_t nbSi=getNumberOfSites();
  
  // check size :
  if(site->size() != getNumberOfSequences())
    throw Exception("VectorIntSiteContainer::addSite. Site does not have the appropriate length: " + TextTools::toString(site->size()) + ", should be " + TextTools::toString(getNumberOfSequences()) + ".");

  // new site's alphabet and site container's alphabet must match :
  if(site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorIntSiteContainer::addSite.", getAlphabet(), site->getAlphabet());

  size_t position = (size_t)site->getPosition();

  // check position :
  if (checkPosition && position<=nbSi)
  {
    if (getSite(position-1)!=nullptr)
      throw Exception("VectorIntSiteContainer::addSite. Site position: " + TextTools::toString(position) + ", already used in container.");
  }

  
  if (position<=nbSi)
    VectorPositionedContainer<Site>::addObject(site, position-1);
  else
  {
    VectorPositionedContainer<Site>::appendObject(site);
    site->setPosition((int)nbSi+1);
  }
}

/********************************************************************************/
void VectorIntSiteContainer::addSequence(const Sequence& sequence, bool checkName)
{
  addSequence(std::shared_ptr<Sequence>(sequence.clone()),checkName);
}

void VectorIntSiteContainer::addSequence(std::shared_ptr<Sequence> sequence, bool checkName)
{
  // if the container has no sequence, we set the size to the size of this sequence :
  if(getNumberOfSequences() == 0)
    VectorPositionedContainer<Site>::setSize((size_t)sequence->size());

  // new sequence's alphabet and site container's alphabet must match :
  if(sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("VectorIntSiteContainer::addSequence->", getAlphabet(), sequence->getAlphabet());

  if(sequence->size() != getNumberOfSites())
    throw Exception("VectorIntSiteContainer::addSequence. Sequence does not have the appropriate length: " + TextTools::toString(sequence->size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".");

  // check name :
  if (checkName)
    if (hasObject(sequence->getName()))
      throw Exception("VectorIntSiteContainer::addSequence. Name: " + sequence->getName() + ", already exists in the container.");

  // new sites :
  for(size_t i = 0; i < getNumberOfSites(); ++i)
  {
    if (!getSite(i))
      addSite(std::shared_ptr<Site>(new Site(sequence->getAlphabet(), (int)(i+1))));

    Site bpl(sequence->getAlphabet(), (int)(i+1));
  }

  // append :
  VectorMappedContainer<Sequence>::appendObject(sequence, sequence->getName());
  
  // append elements at each site :
    
  for(size_t i = 0; i < getNumberOfSites(); ++i)
    getSite(i)->addElement(sequence->getContent()[i]);
  
  // // append comments :
  AbstractSequenceContainer::setComments(sequence->getComments());
}

/********************************************************************************/

void VectorIntSiteContainer::clear()
{
  VectorPositionedContainer<Site>::clear(); // call VectorSiteContainer clear
  VectorMappedContainer<Sequence>::clear();
}  

/********************************************************************************/

void VectorIntSiteContainer::reindexSites()
{
  for (size_t i=0; i<getNumberOfSites(); i++)
    getSite(i)->setPosition((int)i+1);
}

