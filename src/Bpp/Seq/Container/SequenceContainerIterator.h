//
// File: SequenceContainerIterator.h
// Authors:
//   Julien Dutheil
// Created: 2013-02-26 14:34:00
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

#ifndef BPP_SEQ_CONTAINER_SEQUENCECONTAINERITERATOR_H
#define BPP_SEQ_CONTAINER_SEQUENCECONTAINERITERATOR_H


#include "../Sequence.h"
#include "../SequenceIterator.h"
#include "SequenceContainer.h"

namespace bpp
{
/**
 * @brief Partial implementation of the SequenceIterator interface, allowing to loop over a sequence container.
 */
template<class SequenceType, class HashType = std::string>
class AbstractTemplateSequenceContainerIterator :
  public virtual TemplateSequenceIteratorInterface<const SequenceType>
{
protected:
  const TemplateSequenceContainerInterface<SequenceType, HashType>* sequences_;
  size_t currentPosition_;

public:
  AbstractTemplateSequenceContainerIterator(const TemplateSequenceContainerInterface<SequenceType, HashType>& sequences) :
    sequences_(&sequences),
    currentPosition_(0)
  {}


  AbstractTemplateSequenceContainerIterator(const AbstractTemplateSequenceContainerIterator& atsi) :
    sequences_(atsi.sequences_),
    currentPosition_(atsi.currentPosition_)
  {}

  AbstractSequenceContainerIterator& operator=(const AbstractTemplateSequenceContainerIterator& atsi)
  {
    sequences_ = atsi.sequences_;
    currentPosition_ = atsi.currentPosition_;
    return *this;
  }

  virtual ~AbstractTemplateSequenceContainerIterator() {}
};

/**
 * @brief Loop over all sequences in a SequenceContainer.
 */
template<class SequenceType, class HashType = std::string>
class SimpleTemplateSequenceContainerIterator :
  public AbstractTemplateSequenceContainerIterator<SequenceType, HashType>
{
public:
  SimpleSequenceContainerIterator(const TemplateSequenceContainerInterface<SequenceType, HashType>& sequences) :
    AbstractTemplateSequenceContainerIterator<SequenceType, HashType>(&sequences) {}

  virtual ~SimpleTemplateSequenceContainerIterator() {}

public:
  const SequenceType& nextSequence() override
  {	  {
    const Sequence& seq = sequences_->sequence(currentPosition_);
    currentPosition_++;
    return s;
  }
  
  bool hasMoreSequences() const override
  {
    return currentPosition_ < sequences_->getNumberOfSequences();
  }

};

} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCECONTAINERITERATOR_H
