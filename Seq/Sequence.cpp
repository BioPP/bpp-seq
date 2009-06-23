//
// File: Sequence.cpp
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Tue Aug 21 2003
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

#include "Sequence.h" // class's header file

#include "AlphabetTools.h"
#include "StringSequenceTools.h"

// From utils:
#include <Utils/TextTools.h>

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

const bool Sequence::SENSE = true;
const bool Sequence::ANTISENSE = false;

/** Constructors: ***********************************************************************/

Sequence::Sequence(const string& name, const string& sequence, const Alphabet* alpha)
throw (BadCharException) :
	SymbolList(alpha),
	name_(name),
	sense_(true)
{
	setContent(sequence);
}

Sequence::Sequence(const string& name, const string& sequence, const Comments& comments, const Alphabet* alpha)
  throw (BadCharException) :
	SymbolList(alpha),
	name_(name),
	comments_(comments),
	sense_(true)
{
	setContent(sequence);
}

Sequence::Sequence(const string& name, const vector<string>& sequence, const Alphabet* alpha)
throw (BadCharException) :
	SymbolList(sequence, alpha),
	name_(name),
	sense_(true)
{}

Sequence::Sequence(const string& name, const vector<string>& sequence, const Comments& comments, const Alphabet* alpha)
  throw (BadCharException) :
	SymbolList(sequence, alpha),
	name_(name),
	comments_(comments),
	sense_(true)
{}

Sequence::Sequence(const string& name, const vector<int>& sequence, const Alphabet* alpha)
  throw (BadIntException) :
	SymbolList(sequence, alpha),
	name_(name),
	sense_(true)
{}

Sequence::Sequence(const string& name, const vector<int>& sequence, const Comments& comments, const Alphabet* alpha)
  throw (BadIntException) :
	SymbolList(sequence, alpha),
	name_(name),
	comments_(comments),
	sense_(true)
{}

/** Copy constructors: ******************************************************************/

Sequence::Sequence(const Sequence& s) :
	SymbolList(s),
	name_(s.getName()),
	comments_(s.getComments()),
	sense_(s.getSense()) { }

/** Assignation operator: ***************************************************************/

Sequence & Sequence::operator=(const Sequence& s)
{
  SymbolList::operator=(s);
	name_     = s.getName();
	comments_ = s.getComments();
	sense_    = s.getSense();
	return *this;
}

/****************************************************************************************/

void Sequence::setContent(const string & sequence) throw (BadCharException)
{
	// Remove blanks in sequence
	content_ = StringSequenceTools::codeSequence(TextTools::removeWhiteSpaces(sequence), getAlphabet());
  //Warning, an exception may be thrown here!
}

/****************************************************************************************/

void Sequence::setToSizeR(unsigned int size)
{
	unsigned int seqSize = content_.size();
	// Size verification
	if (size < seqSize)
  {
		content_.resize(size);
		return;
	}
	if (size == seqSize) return;

	// Add gaps up to specified size
	while(content_.size() < size) content_.push_back(-1);
}

/****************************************************************************************/

void Sequence::setToSizeL(unsigned int size)
{
	// Size verification
	unsigned int seqSize = content_.size();
	if (size < seqSize)
  {
		//We must truncate sequence from the left.
		//This is a very unefficient method!
		content_.erase(content_.begin(), content_.begin() + (seqSize - size));
		return;
	}
	if (size == seqSize) return;

	// Add gaps up to specified size
	content_.insert(content_.begin(), size - seqSize, -1);
}

/****************************************************************************************/

void Sequence::append(const vector<int>& content) throw (BadIntException)
{
	// Check list for incorrect characters
	for (unsigned int i = 0; i < content.size(); i++)
		if(!getAlphabet()->isIntInAlphabet(content[i]))
      throw BadIntException(content[i], "Sequence::append", getAlphabet());
	//Sequence is valid:
	for (unsigned int i = 0; i < content.size(); i++)
		content_.push_back(content[i]);
}

void Sequence::append(const vector<string>& content) throw (BadCharException)
{
	// Check list for incorrect characters
	for (unsigned int i = 0; i < content.size(); i++)
		if(!getAlphabet()->isCharInAlphabet(content[i]))
      throw BadCharException(content[i], "Sequence::append", getAlphabet());
	
	//Sequence is valid:
	for (unsigned int i = 0; i < content.size(); i++)
		content_.push_back(getAlphabet()->charToInt(content[i]));
}

void Sequence::append(const string& content) throw (BadCharException)
{
	append(StringSequenceTools::codeSequence(content, getAlphabet()));
}

/****************************************************************************************/

