/*
 * File VectorSequenceContainer.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Wednesday July 30 2003
*/

#include "VectorSequenceContainer.h"

// From Utils:
#include <Utils/TextTools.h>

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
	AbstractSequenceContainer(vsc.getAlphabet())
{
	unsigned int max = vsc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < max ; i++) addSequence(* vsc.getSequence(i), false);
	_comments = vsc.getGeneralComments();
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

/** Assignation operator: *****************************************************/

VectorSequenceContainer & VectorSequenceContainer::operator = (
	const VectorSequenceContainer & vsc)
{
	_alphabet = vsc.getAlphabet();
	_comments = vsc.getGeneralComments();

	// Sequences insertion
	int max = vsc.getNumberOfSequences();
	for (int i = 0 ; i < max ; i++)	addSequence(* vsc.getSequence(i), false);

	return * this;
}

VectorSequenceContainer & VectorSequenceContainer::operator = (
	const OrderedSequenceContainer & osc)
{
 	_alphabet = osc.getAlphabet();
	_comments = osc.getGeneralComments();

	// Sequences insertion
	int max = osc.getNumberOfSequences();
	for (int i = 0 ; i < max ; i++)	addSequence(*osc.getSequence(i), false);
    
	return * this;
}

/** Class destructor: *********************************************************/

VectorSequenceContainer::~VectorSequenceContainer() { clear(); }

/******************************************************************************/

const Sequence * VectorSequenceContainer::getSequence(unsigned int i) const throw (IndexOutOfBoundsException)
{
	// Specified sequence existence verification
	if (i < _sequences.size()) return _sequences[i];
	throw IndexOutOfBoundsException("VectorSequenceContainer::getSequence.", i, 0, _sequences.size() - 1);
}

const Sequence * VectorSequenceContainer::getSequence(const string & name) const throw (SequenceNotFoundException)
{
	// Specified sequence name research into all sequences
	for(unsigned int i = 0; i < _sequences.size(); i++) {
		if(_sequences[i] -> getName() == name) return _sequences[i];
	}
	throw SequenceNotFoundException("VectorSequenceContainer::getSequence : Specified sequence doesn't exist", name);
}

Sequence * VectorSequenceContainer::getSequence(unsigned int i) throw (IndexOutOfBoundsException)
{
	// Specified sequence existence verification
	if (i < _sequences.size()) return _sequences[i];
	throw IndexOutOfBoundsException("VectorSequenceContainer::getSequence.", i, 0, _sequences.size() - 1);
}

Sequence * VectorSequenceContainer::getSequence(const string & name) throw (SequenceNotFoundException)
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

void VectorSequenceContainer::setSequence(unsigned int i, const Sequence & sequence, bool checkNames) throw (Exception)
{
	// Sequence's name existence checking
	if (checkNames) {
		// For all names in vector : throw exception if name already exists
		for(unsigned int j = 0; j < _sequences.size(); j++) {
			if (_sequences[j] -> getName() == sequence.getName())
				if (j != i) throw Exception("VectorSequenceContainer::setSequence : Sequence's name already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType()) {
		// Delete old sequence
		delete _sequences[i];
		// New sequence insertion in sequence container
		_sequences[i] = dynamic_cast<Sequence *>(sequence.clone());
	} else throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", _alphabet, sequence.getAlphabet());
}

/******************************************************************************/



void VectorSequenceContainer::setSequence(const string & name, const Sequence & sequence, bool checkNames) throw (Exception)
{
	// Sequence's name existence checking
	unsigned int i = getSequencePosition(name);
	if (checkNames) {
		// For all names in vector : throw exception if name already exists
		for(unsigned int j = 0; j < _sequences.size(); j++) {
			if (_sequences[j] -> getName() == sequence.getName())
				if (j != i) throw Exception("VectorSequenceContainer::setSequence : Sequence's name already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType()) {
		// Delete old sequence
		delete _sequences[i];
		// New sequence insertion in sequence container
		_sequences[i] = dynamic_cast<Sequence *>(sequence.clone());
	} else throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", _alphabet, sequence.getAlphabet());
}

/******************************************************************************/

Sequence * VectorSequenceContainer::removeSequence(unsigned int i) throw (IndexOutOfBoundsException)
{
	// Copy sequence:
	Sequence * deleted = const_cast <Sequence *> (getSequence(i));
	// Remove pointer toward old sequence:
	_sequences.erase(_sequences.begin() + i);
	// Send copy:
	return deleted;
}

Sequence * VectorSequenceContainer::removeSequence(const string & name) throw (SequenceNotFoundException)
{
	// Copy sequence
	Sequence * deleted = const_cast <Sequence *> (getSequence(name));
	// Remove pointer toward old sequence
	_sequences.erase(_sequences.begin() + getSequencePosition(name));
	// Send copy
	return deleted;
}

/******************************************************************************/

void VectorSequenceContainer::deleteSequence(unsigned int i) throw (IndexOutOfBoundsException)
{
	// Delete sequence
	const Sequence * deleted = getSequence(i);
	delete deleted;
	// Remove pointer toward old sequence:
	_sequences.erase(_sequences.begin() + i);
}

void VectorSequenceContainer::deleteSequence(const string & name) throw (SequenceNotFoundException) {
	//Delete sequence
	const Sequence * deleted = getSequence(name);
	delete deleted;
	//Remove pointer toward old sequence
	_sequences.erase(_sequences.begin() + getSequencePosition(name));
}

/******************************************************************************/

void VectorSequenceContainer::addSequence(const Sequence & sequence, bool checkNames) throw (Exception)
{
	// Sequence's name existence checking
	if (checkNames) {
		// For all names in vector : throw exception if name already exists
		for(unsigned int i = 0; i < _sequences.size(); i++) {
			if (_sequences[i] -> getName() == sequence.getName())
				throw Exception("VectorSequenceContainer::addSequence : Sequence's name already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType()) {
		//push_back(new Sequence(sequence.getName(), sequence.getContent(), alphabet));
		_sequences.push_back(dynamic_cast<Sequence *>(sequence.clone()));
	} else throw AlphabetMismatchException("VectorSequenceContainer::addSequence : Alphabets don't match", _alphabet, sequence.getAlphabet());
}

void VectorSequenceContainer::addSequence(const Sequence & sequence, int pos, bool checkNames) throw (Exception)
{
	// Sequence's name existence checking
	if (checkNames) {
		// For all names in vector : throw exception if name already exists
		for(unsigned int i = 0; i < _sequences.size(); i++) {
			if (_sequences[i] -> getName() == sequence.getName())
				throw Exception("VectorSequenceContainer::addSequence : Sequence's name already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet()->getAlphabetType() == _alphabet->getAlphabetType()) {
		//insert(begin() + pos, new Sequence(sequence.getName(), sequence.getContent(), alphabet));
		_sequences.insert(_sequences.begin() + pos, dynamic_cast<Sequence *>(sequence.clone()));
	} else throw AlphabetMismatchException("VectorSequenceContainer::addSequence : Alphabets don't match", _alphabet, sequence.getAlphabet());
}

/******************************************************************************/

unsigned int VectorSequenceContainer::getNumberOfSequences() const { return _sequences.size(); }

/******************************************************************************/

vector<string> VectorSequenceContainer::getSequencesNames() const {
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

void VectorSequenceContainer::setComments(unsigned int i, const Comments & comments) throw (IndexOutOfBoundsException)
{
	_sequences[i] -> setComments(comments);
}

/******************************************************************************/
