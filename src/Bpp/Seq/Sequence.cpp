//
// File: Sequence.cpp
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
// Created: 2003-08-21 00:00:00
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

#include "Alphabet/AlphabetTools.h"
#include "Sequence.h" // class's header file
#include "StringSequenceTools.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

/******************************************************************************/

void Sequence::setContent(const std::string& sequence)
{
  // Remove blanks in sequence
  auto alphaPtr = getAlphabet();
  content_ = StringSequenceTools::codeSequence(TextTools::removeWhiteSpaces(sequence), alphaPtr);
  // Warning, an exception may be thrown here!
}

/******************************************************************************/

void Sequence::setToSizeR(size_t newSize)
{
  // Size verification
  size_t seqSize = content_.size();
  if (newSize == seqSize)
    return;

  if (newSize < seqSize)
  {
    content_.resize(newSize);
    return;
  }

  // Add gaps up to specified size
  int gap = getAlphabet()->getGapCharacterCode();
  while (content_.size() < newSize)
    content_.push_back(gap);
}

/******************************************************************************/

void Sequence::setToSizeL(size_t newSize)
{
  // Size verification
  size_t seqSize = content_.size();
  if (newSize == seqSize)
    return;

  if (newSize < seqSize)
  {
    // We must truncate sequence from the left.
    // This is a very unefficient method!
    content_.erase(content_.begin(), content_.begin() + static_cast<ptrdiff_t>(seqSize - newSize));
    return;
  }

  // Add gaps up to specified size
  int gap = getAlphabet()->getGapCharacterCode();
  content_.insert(content_.begin(), newSize - seqSize, gap);
}

/******************************************************************************/

void Sequence::append(const SequenceInterface& seq)
{
  if (seq.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("Sequence::append", getAlphabet(), seq.getAlphabet());
  // Check list for incorrect characters
  for (auto i : seq.getContent())
  {
    content_.push_back(i);
  }
}

void Sequence::append(const std::vector<int>& content)
{
  // Check list for incorrect characters
  for (auto i : content)
  {
    if (!getAlphabet()->isIntInAlphabet(i))
      throw BadIntException(i, "Sequence::append", getAlphabet());
  }
  // Sequence is valid:
  for (auto i : content)
  {
    content_.push_back(i);
  }
}

void Sequence::append(const std::vector<std::string>& content)
{
  // Check list for incorrect characters
  for (auto i : content)
  {
    if (!getAlphabet()->isCharInAlphabet(i))
      throw BadCharException(i, "Sequence::append", getAlphabet());
  }

  // BasicSequence is valid:
  for (auto i : content)
  {
    content_.push_back(getAlphabet()->charToInt(i));
  }
}

void Sequence::append(const std::string& content)
{
  auto alphaPtr = getAlphabet();
  append(StringSequenceTools::codeSequence(content, alphaPtr));
}

/******************************************************************************/
