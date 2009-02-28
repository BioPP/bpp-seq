//
// File VectorSequenceContainer.cpp
// Author : Guillaume Deuchst
//          Julien Dutheil
// Last modification : Wednesday July 30 2003
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

#include "VectorSequenceContainer.h"

// From Utils:
#include <Utils/TextTools.h>

using namespace bpp;

using namespace std;

/** Class constructors: *******************************************************/

VectorSequenceContainer::VectorSequenceContainer(
	const vector<const Sequence *> & vs,
	const Alphabet * alpha)
	throw (AlphabetMismatchException):
	AbstractSequenceContainer(alpha)
{
	for (vector<const Sequence *>::const_iterator i = vs.begin() ; i < vs.end() ; i++) {
		addSequence(**i);
	}
}

VectorSequenceContainer::VectorSequenceContainer(const Alphabet * alpha):
AbstractSequenceContainer(alpha) {}

/** Copy constructors: ********************************************************/
	
VectorSequenceContainer::VectorSequenceContainer(
	const VectorSequenceContainer & vsc):
	AbstractSequenceContainer(vsc)
{
	unsigned int max = vsc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < max ; i++) addSequence(* vsc.getSequence(i), false);
}

VectorSequenceContainer::VectorSequenceContainer(
	const OrderedSequenceContainer & osc):
	AbstractSequenceContainer(osc.getAlphabet())
{
	// Sequences insertion
	unsigned int max = osc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < max ; i++) addSequence(* osc.getSequence(i), false);

	_comments = osc.getGeneralComments();
}

VectorSequenceContainer::VectorSequenceContainer(
	const SequenceContainer & sc):
	AbstractSequenceContainer(sc.getAlphabet())
{
	// Sequences insertion
  vector<string> names = sc.getSequencesNames();  
  for(unsigned int i = 0; i < names.size(); i++)
  {
    addSequence(* sc.getSequence(names[i]), false);
  }

	_comments = sc.getGeneralComments();
}

/** Assignation operator: *****************************************************/

VectorSequenceContainer & VectorSequenceContainer::operator = (
	const VectorSequenceContainer & vsc)
{
	_alphabet = vsc.getAlphabet();
	_comments = vsc.getGeneralComments();

	// Sequences insertion
	unsigned int max = vsc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < max ; i++)	addSequence(* vsc.getSequence(i), false);

	return * this;
}

VectorSequenceContainer & VectorSequenceContainer::operator = (
	const OrderedSequenceContainer & osc)
{
 	_alphabet = osc.getAlphabet();
	_comments = osc.getGeneralComments();

	// Sequences insertion
	unsigned int max = osc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < max ; i++)	addSequence(*osc.getSequence(i), false);
    
	return * this;
}

/******************************************************************************/

VectorSequenceContainer& VectorSequenceContainer::operator = (
  const SequenceContainer & sc)
{
  //Setting up alphabet:
  _alphabet = sc.getAlphabet();
  
  //Seq names:
  vector<string> names = sc.getSequencesNames();
  
  for(unsigned int i = 0; i < names.size(); i++)
  {
    addSequence(*sc.getSequence(names[i]), false);
  }

  // General comments:
  _comments = sc.getGeneralComments();

  return * this;
}

/******************************************************************************/

const Sequence * VectorSequenceContainer::getSequence(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException)
{
	// Specified sequence existence verification
	if (sequenceIndex < _sequences.size()) return _sequences[sequenceIndex];
	throw IndexOutOfBoundsException("VectorSequenceContainer::getSequence.", sequenceIndex, 0, _sequences.size() - 1);
}

/******************************************************************************/

const Sequence * VectorSequenceContainer::getSequence(const string & name) const throw (SequenceNotFoundException)
{
	// Specified sequence name research into all sequences
	for(unsigned int i = 0; i < _sequences.size(); i++) {
		if(_sequences[i] -> getName() == name) return _sequences[i];
	}
	throw SequenceNotFoundException("VectorSequenceContainer::getSequence : Specified sequence doesn't exist", name);
}

/******************************************************************************/

Sequence * VectorSequenceContainer::getSequenceInner(unsigned int sequenceIndex) throw (IndexOutOfBoundsException)
{
	// Specified sequence existence verification
	if (sequenceIndex < _sequences.size()) return _sequences[sequenceIndex];
	throw IndexOutOfBoundsException("VectorSequenceContainer::getSequence.", sequenceIndex, 0, _sequences.size() - 1);
}

/******************************************************************************/

Sequence * VectorSequenceContainer::getSequenceInner(const string & name) throw (SequenceNotFoundException)
{
	// Specified sequence name research into all sequences
	for(unsigned int i = 0; i < _sequences.size(); i++) {
		if(_sequences[i] -> getName() == name) return _sequences[i];
	}
	throw SequenceNotFoundException("VectorSequenceContainer::getSequence : Specified sequence doesn't exist", name);
}

/******************************************************************************/

unsigned int VectorSequenceContainer::getSequencePosition(const string & name) const throw (SequenceNotFoundException)
{
	// Specified sequence name research into all sequences
	for(unsigned int i = 0; i < _sequences.size(); i++) {
		if(_sequences[i] -> getName() == name) return i;
	}
	throw SequenceNotFoundException("VectorSequenceContainer::getSequencePosition : Specified sequence doesn't exist", name);
}

/******************************************************************************/

void VectorSequenceContainer::setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName) throw (Exception)
{
	// Sequence's name existence checking
	if (checkName) {
		// For all names in vector : throw exception if name already exists
		for(unsigned int j = 0; j < _sequences.size(); j++) {
			if (_sequences[j] -> getName() == sequence.getName())
				if (j != sequenceIndex) throw Exception("VectorSequenceContainer::setSequence : Sequence's name already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType()) {
		// Delete old sequence
		delete _sequences[sequenceIndex];
		// New sequence insertion in sequence container
		_sequences[sequenceIndex] = dynamic_cast<Sequence *>(sequence.clone());
	} else throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", _alphabet, sequence.getAlphabet());
}

/******************************************************************************/

Sequence * VectorSequenceContainer::removeSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException)
{
	// Copy sequence:
	Sequence * deleted = const_cast <Sequence *> (getSequence(sequenceIndex));
	// Remove pointer toward old sequence:
	_sequences.erase(_sequences.begin() + sequenceIndex);
	// Send copy:
	return deleted;
}

/******************************************************************************/

void VectorSequenceContainer::deleteSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException)
{
	// Delete sequence
	const Sequence * deleted = getSequence(sequenceIndex);
	delete deleted;
	// Remove pointer toward old sequence:
	_sequences.erase(_sequences.begin() + sequenceIndex);
}

/******************************************************************************/

void VectorSequenceContainer::addSequence(const Sequence & sequence, bool checkName) throw (Exception)
{
	// Sequence's name existence checking
	if (checkName)
  {
		// For all names in vector : throw exception if name already exists
		for(unsigned int i = 0; i < _sequences.size(); i++)
    {
			if (_sequences[i] -> getName() == sequence.getName())
				throw Exception("VectorSequenceContainer::addSequence : Sequence '" + sequence.getName() + "' already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType())
  {
		//push_back(new Sequence(sequence.getName(), sequence.getContent(), alphabet));
		_sequences.push_back(dynamic_cast<Sequence *>(sequence.clone()));
	}
  else throw AlphabetMismatchException("VectorSequenceContainer::addSequence : Alphabets don't match", _alphabet, sequence.getAlphabet());
}

void VectorSequenceContainer::addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName) throw (Exception)
{
	// Sequence's name existence checking
	if (checkName)
  {
		// For all names in vector : throw exception if name already exists
		for(unsigned int i = 0; i < _sequences.size(); i++)
    {
			if (_sequences[i] -> getName() == sequence.getName())
				throw Exception("VectorSequenceContainer::addSequence : Sequence '" + sequence.getName() + "' already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType())
  {
		//insert(begin() + pos, new Sequence(sequence.getName(), sequence.getContent(), alphabet));
		_sequences.insert(_sequences.begin() + sequenceIndex, dynamic_cast<Sequence *>(sequence.clone()));
	}
  else throw AlphabetMismatchException("VectorSequenceContainer::addSequence : Alphabets don't match", _alphabet, sequence.getAlphabet());
}

/******************************************************************************/

unsigned int VectorSequenceContainer::getNumberOfSequences() const { return _sequences.size(); }

/******************************************************************************/

vector<string> VectorSequenceContainer::getSequencesNames() const
{
	vector<string> names;
	for(unsigned int i = 0; i < _sequences.size(); i++) names.push_back(_sequences[i] -> getName());
	return names;
}

/******************************************************************************/

void VectorSequenceContainer::setSequencesNames(
	const vector<string> & names,
	bool checkNames)
	throw(Exception)
{
	if(names.size() != getNumberOfSequences())
		throw BadIntegerException("VectorSequenceContainer::setSequenceNames : bad number of names", names.size());
	if(checkNames) {
		for(unsigned int i = 0; i < names.size(); i++) {
			// For all names in vector : throw exception if name already exists
			for(unsigned int j = 0; j < i; j++) {
				if (names[j] == names[i])
					throw Exception("VectorSiteContainer::setSequencesNames : Sequence's name already exists in container");
			}
		}
	}
	for(unsigned int i = 0; i < names.size(); i++) {
		_sequences[i] -> setName(names[i]);
	}
}

/******************************************************************************/

void VectorSequenceContainer::clear()
{
	// Delete sequences
	for(unsigned int i = 0; i < _sequences.size(); i++) delete _sequences[i];
	// Delete all sequence pointers
	_sequences.clear();
}

/******************************************************************************/

void VectorSequenceContainer::setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException)
{
	_sequences[sequenceIndex] -> setComments(comments);
}

/******************************************************************************/

SequenceContainer * VectorSequenceContainer::createEmptyContainer() const
{ 
	VectorSequenceContainer * vsc = new VectorSequenceContainer(_alphabet);
	vsc -> setGeneralComments(_comments);
	return(vsc);
}

/******************************************************************************/

