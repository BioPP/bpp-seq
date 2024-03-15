// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "Alphabet/AlphabetTools.h"
#include "SequenceWithAnnotation.h" // class's header file
#include "StringSequenceTools.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

/******************************************************************************/

void SequenceWithAnnotation::setContent(const std::string& sequence)
{
  IntSymbolListEditionEvent event(this);
  fireBeforeSequenceChanged(event);
  // Remove blanks in sequence
  auto alphaPtr =  getAlphabet();
  setContent(StringSequenceTools::codeSequence(TextTools::removeWhiteSpaces(sequence), alphaPtr));
  // Warning, an exception may be thrown here!
  fireAfterSequenceChanged(event);
}

/******************************************************************************/

void SequenceWithAnnotation::setToSizeR(size_t newSize)
{
  // Size verification
  size_t seqSize = content_.size();
  if (newSize == seqSize)
    return;

  if (newSize < seqSize)
  {
    IntSymbolListDeletionEvent event(this, newSize, seqSize - newSize);
    fireBeforeSequenceDeleted(event);
    content_.resize(newSize);
    fireAfterSequenceDeleted(event);
    return;
  }

  // Add gaps up to specified size
  IntSymbolListInsertionEvent event(this, seqSize, newSize - seqSize);
  fireBeforeSequenceInserted(event);
  int gap = getAlphabet()->getGapCharacterCode();
  while (content_.size() < newSize)
    content_.push_back(gap);
  fireAfterSequenceInserted(event);
}

/******************************************************************************/

void SequenceWithAnnotation::setToSizeL(size_t newSize)
{
  // Size verification
  size_t seqSize = content_.size();
  if (newSize == seqSize)
    return;

  if (newSize < seqSize)
  {
    // We must truncate sequence from the left.
    IntSymbolListDeletionEvent event(this, 0, seqSize - newSize);
    fireBeforeSequenceDeleted(event);
    content_.erase(content_.begin(), content_.begin() + static_cast<ptrdiff_t>(seqSize - newSize));
    fireAfterSequenceDeleted(event);
    return;
  }

  // Add gaps up to specified size
  IntSymbolListInsertionEvent event(this, 0, newSize - seqSize);
  fireBeforeSequenceInserted(event);
  int gap = getAlphabet()->getGapCharacterCode();
  content_.insert(content_.begin(), newSize - seqSize, gap);
  fireAfterSequenceInserted(event);
}

/******************************************************************************/

void SequenceWithAnnotation::append(const SequenceInterface& seq)
{
  if (seq.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SequenceWithAnnotation::append", getAlphabet(), seq.getAlphabet());
  IntSymbolListInsertionEvent event(this, content_.size(), seq.size());
  fireBeforeSequenceInserted(event);
  for (size_t i = 0; i < seq.size(); ++i)
  {
    content_.push_back(seq[i]);
  }

  fireAfterSequenceInserted(event);
}

void SequenceWithAnnotation::append(const std::vector<int>& content)
{
  IntSymbolListInsertionEvent event(this, content_.size(), content.size());
  fireBeforeSequenceInserted(event);
  // Check list for incorrect characters
  for (auto i : content)
  {
    if (!getAlphabet()->isIntInAlphabet(i))
      throw BadIntException(i, "SequenceWithAnnotation::append", getAlphabet());
  }
  // SequenceWithAnnotation is valid:
  for (auto i : content)
  {
    content_.push_back(i);
  }

  fireAfterSequenceInserted(event);
}

void SequenceWithAnnotation::append(const std::vector<std::string>& content)
{
  IntSymbolListInsertionEvent event(this, content_.size(), content.size());
  fireBeforeSequenceInserted(event);
  // Check list for incorrect characters
  for (auto i : content)
  {
    if (!getAlphabet()->isCharInAlphabet(i))
      throw BadCharException(i, "SequenceWithAnnotation::append", getAlphabet());
  }

  // SequenceWithAnnotation is valid:
  for (auto i : content)
  {
    content_.push_back(getAlphabet()->charToInt(i));
  }

  fireAfterSequenceInserted(event);
}

void SequenceWithAnnotation::append(const std::string& content)
{
  auto alphaPtr = getAlphabet();
  append(StringSequenceTools::codeSequence(content, alphaPtr));
}

/******************************************************************************/

vector<string> SequenceWithAnnotation::getAnnotationTypes() const
{
  vector<string> types;
  for (unsigned int i = 0; i < getNumberOfListeners(); ++i)
  {
    try {
      const SequenceAnnotation& anno = dynamic_cast<const SequenceAnnotation&>(listener(i));
      types.push_back(anno.getType());
    } catch (bad_cast&) {}
  }
  return types;
}

/******************************************************************************/

void SequenceWithAnnotation::merge(const SequenceWithAnnotation& swa)
{
  // Sequence's alphabets matching verification
  if ((swa.getAlphabet()->getAlphabetType()) != (getAlphabet()->getAlphabetType()))
    throw AlphabetMismatchException("SequenceWithAnnotation::merge: Sequence's alphabets don't match ", swa.getAlphabet(), getAlphabet());

  // Sequence's names matching verification
  if (swa.getName() != getName())
    throw Exception ("SequenceWithAnnotation::merge: Sequence's names don't match");

  // Concatenate sequences and send result
  propagateEvents(false);
  append(swa.getContent());
  propagateEvents(true);

  // Try to merge annotations.
  // First start with annotations in this sequence:
  vector<string> types = getAnnotationTypes();
  vector<string> newTypes = swa.getAnnotationTypes();
  for (unsigned int i = 0; i < types.size(); ++i)
  {
    vector<string>::iterator it = find(newTypes.begin(), newTypes.end(), types[i]);
    if (it != newTypes.end())
    {
      // Merge annotations:
      annotation(types[i]).merge(swa.annotation(types[i]));
      // Remove annotation from the list:
      newTypes.erase(it);
    }
    else
    {
      // Extend annotation to the right:
      unique_ptr<SequenceAnnotation> anno(annotation(types[i]).clone());
      anno->init(swa);
      annotation(types[i]).merge(*anno);
    }
  }
  // Now look for annotations in the input sequence:
  for (unsigned int i = 0; i < newTypes.size(); ++i)
  {
    // Extend annotation from the left:
    shared_ptr<SequenceAnnotation> anno(swa.annotation(newTypes[i]).clone());
    anno->init(*this);
    anno->merge(swa.annotation(newTypes[i]));
    addAnnotation(anno);
  }
}


/******************************************************************************/
