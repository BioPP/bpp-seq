// 
// File:    SequenceWithQuality.cpp
// Author:  Sylvain Gaillard
// Created: 19/01/2010 16:09:02
// 

/*
Copyright or © or Copr. CNRS, (January 19, 2010)

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

#include "SequenceWithQuality.h"

#include <NumCalc/VectorTools.h>

using namespace bpp;

#include <iostream>

const int SequenceWithQuality::DEFAULT_QUALITY_VALUE = 20;

/******************************************************************************/

void SequenceWithQuality::setToSizeR(unsigned int newSize) {
  Sequence::setToSizeR(newSize);
  unsigned int seqSize = qualScores_.size();
  if (newSize == seqSize) return;
  if (newSize < seqSize) {
    qualScores_.resize(newSize);
    return;
  }
  if (newSize > seqSize) {
    extendQualityScores_();
  }
}

/******************************************************************************/

void SequenceWithQuality::setToSizeL(unsigned int newSize) {
  Sequence::setToSizeL(newSize);
  unsigned int seqSize = qualScores_.size();
  if (newSize == seqSize) return;
  if (newSize < seqSize) {
    //We must truncate vector from the left
    //This is a very unefficient method!
    qualScores_.erase(qualScores_.begin(), qualScores_.begin() + (seqSize - newSize));
    return;
  }
  if (newSize > seqSize) {
    qualScores_.insert(qualScores_.begin(), newSize - seqSize, DEFAULT_QUALITY_VALUE);
  }
}

/******************************************************************************/

void SequenceWithQuality::append(const std::vector<int>& content) throw (BadIntException) {
  Sequence::append(content);
  extendQualityScores_();
}

void SequenceWithQuality::append(const std::vector<std::string>& content) throw (BadCharException) {
  Sequence::append(content);
  extendQualityScores_();
}

void SequenceWithQuality::append(const std::string& content) throw (BadCharException) {
  extendQualityScores_();
}

/******************************************************************************/

void SequenceWithQuality::addElement(const std::string& c) throw (BadCharException) {
  Sequence::addElement(c);
  qualScores_.push_back(DEFAULT_QUALITY_VALUE);
}

void SequenceWithQuality::addElement(unsigned int pos, const std::string& c) throw (BadCharException, IndexOutOfBoundsException) {
  Sequence::addElement(pos, c);
  qualScores_.insert(qualScores_.begin() + pos, DEFAULT_QUALITY_VALUE);
}

void SequenceWithQuality::addElement(int v) throw (BadIntException) {
  Sequence::addElement(v);
  qualScores_.push_back(DEFAULT_QUALITY_VALUE);
}

void SequenceWithQuality::addElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException) {
  Sequence::addElement(pos, v);
  qualScores_.insert(qualScores_.begin() + pos, DEFAULT_QUALITY_VALUE);
}

/******************************************************************************/

void SequenceWithQuality::setContent(const std::string& sequence) throw (BadCharException) {
  Sequence::setContent(sequence);
  extendQualityScores_();
}

void SequenceWithQuality::setContent(const std::vector<int>& list) throw (BadIntException) {
  Sequence::setContent(list);
  extendQualityScores_();
}

void SequenceWithQuality::setContent(const std::vector<std::string>& list) throw (BadCharException) {
  Sequence::setContent(list);
  extendQualityScores_();
}

/******************************************************************************/

void SequenceWithQuality::deleteElement(unsigned int pos) throw (IndexOutOfBoundsException) {
  Sequence::deleteElement(pos);
  qualScores_.erase(qualScores_.begin() + pos);
}
