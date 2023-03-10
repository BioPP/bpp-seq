//
// File: Transliterator.cpp
// Authors:
//   Julien Dutheil
// Created: 2003-10-12 14:25:25
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

#include <memory>

using namespace std;

#include "Transliterator.h"

using namespace bpp;

unique_ptr<Sequence> AbstractTransliterator::translate(const SequenceInterface& sequence) const
{
  if (sequence.alphabet().getAlphabetType() != sourceAlphabet().getAlphabetType())
    throw AlphabetMismatchException("AbstractTransliterator::translate", getSourceAlphabet(), getTargetAlphabet());
  auto alphaPtr = getTargetAlphabet();
  auto tSeq = make_unique<Sequence>(sequence.getName(), "", sequence.getComments(), alphaPtr);
  int gap = sequence.getAlphabet()->getGapCharacterCode();
  for (size_t i = 0; i < sequence.size(); ++i)
  {
    int state = sequence.getValue(i);
    if (state == gap)
      tSeq->addElement(gap);
    else
      tSeq->addElement(translate(state));
  }
  return tSeq;
}

unique_ptr<Sequence> AbstractReverseTransliterator::reverse(const SequenceInterface& sequence) const
{
  if (sequence.alphabet().getAlphabetType() != targetAlphabet().getAlphabetType())
    throw AlphabetMismatchException("AbstractReverseTransliterator::reverse", getSourceAlphabet(), getTargetAlphabet());
  auto alphaPtr = getSourceAlphabet();
  auto rSeq = make_unique<Sequence>(sequence.getName(), "", sequence.getComments(), alphaPtr);
  for (size_t i = 0; i < sequence.size(); ++i)
  {
    rSeq->addElement(reverse(sequence.getValue(i)));
  }
  return rSeq;
}

