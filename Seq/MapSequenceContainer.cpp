/*
 * File MapSequenceContainer.cpp
 * Authors : Guillaume Deuchst <GDeuchst@ifrance.com>
 *           Sylvain Gaillard <yragael2001@yahoo.fr>
 * Last modification : Monday July 19 2004
*/

#include "MapSequenceContainer.h"

// From Utils:
#include <Utils/TextTools.h>

// Class constructors
MapSequenceContainer::MapSequenceContainer(const map<string, Sequence *> & ms, const Alphabet * alpha) : AbstractSequenceContainer(alpha) 
{
	for (map<string, Sequence *>::const_iterator i = ms.begin() ; i != ms.end() ; i++) {		
		addSequence((*i).first, *((*i).second));
	}
}

MapSequenceContainer::MapSequenceContainer(const Alphabet * alpha) : AbstractSequenceContainer(alpha) {}

// Copy constructors
MapSequenceContainer::MapSequenceContainer(const MapSequenceContainer & msc) :
	AbstractSequenceContainer(msc.getAlphabet()) 
{ 
	unsigned int max = msc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < max ; i++) addSequence(msc.getKey(i), * msc.getSequence(i), false);
	_comments = msc.getGeneralComments(); 
}

MapSequenceContainer & MapSequenceContainer::operator = (const MapSequenceContainer & msc) 
{
	_alphabet = msc.getAlphabet();
	_comments = msc.getGeneralComments();
	
	// Sequences insertion
	vector<string> keys = msc.getKeys();
	for (unsigned int i = 0 ; i < _sequences.size(); i++) {
		addSequence(keys[i], * msc.getSequence(i), false);
	}

	return * this;
}

// Class destructor
MapSequenceContainer::~MapSequenceContainer() 
{
	clear();
}

// The Clonble interface
Clonable * MapSequenceContainer::clone() const {
	return new MapSequenceContainer(* this);
}

// Methods to get an sequence object from sequence container
const Sequence * MapSequenceContainer::getSequence(unsigned int i) const throw (IndexOutOfBoundsException) {
	// Specified sequence existence verification
	if (i < _sequences.size()) {
		map<string, Sequence *>::const_iterator it = _sequences.begin();
		for (unsigned int j = 0 ; j < i ; j++) { it++; }
		return it -> second;
	}
	throw IndexOutOfBoundsException("MapSequenceContainer::getSequence", i, 0, _sequences.size() - 1);
	return NULL;
}

const Sequence * MapSequenceContainer::getSequence(const string & name) const throw (SequenceNotFoundException) {
	// Specified sequence name research into all sequences
	for (map<string, Sequence *>::const_iterator i = _sequences.begin() ; i != _sequences.end() ; i++) {
		if (i -> second -> getName() == name) return i -> second;
	}
	throw SequenceNotFoundException("MapSequenceContainer::getSequence", name);
	return NULL;
}

Sequence * MapSequenceContainer::getSequence(unsigned int i) throw (IndexOutOfBoundsException) {
	// Specified sequence existence verification
	if (i < _sequences.size()) {
		map<string, Sequence *>::const_iterator it = _sequences.begin();
		for (unsigned int j = 0 ; j < i ; j++) { it++; }
		return it -> second;
	}
	throw IndexOutOfBoundsException("MapSequenceContainer::getSequence", i, 0, _sequences.size() - 1);
	return NULL;
}

Sequence * MapSequenceContainer::getSequence(const string & name) throw (SequenceNotFoundException) {
	// Specified sequence name research into all sequences
	for (map<string, Sequence *>::const_iterator i = _sequences.begin() ; i != _sequences.end() ; i++) {
		if (i -> second -> getName() == name) return i -> second;
	}
	throw SequenceNotFoundException("MapSequenceContainer::getSequence", name);
	return NULL;
}

const Sequence * MapSequenceContainer::getSequenceByKey(const string & key) const
    throw (SequenceNotFoundException)
{
	map<string, Sequence *>::const_iterator i = _sequences.find(key);
	if (i != _sequences.end()) return i -> second;

	throw SequenceNotFoundException("MapSequenceContainer::getSequenceByKey", key);
}

unsigned int MapSequenceContainer::getSequencePosition(const string & name)
    const throw (SequenceNotFoundException)
{
	// Specified sequence name research into all sequences
	int pos = 0;
	for (map<string, Sequence *>::const_iterator i = _sequences.begin() ; i != _sequences.end() ; i++) {
		if (i -> second -> getName() == name) return pos;
		pos++;
	}

	throw SequenceNotFoundException("MapSequenceContainer::getSequencePosition", name);
}

void MapSequenceContainer::setSequence(unsigned int i, const Sequence & sequence, bool checkNames)
    throw (IndexOutOfBoundsException)
{
	// Sequence's name existence checking
	if (checkNames) {
		unsigned int j = 0;
		// For all names in map : throw exception if name already exists
		for (map<string, Sequence *>::const_iterator it = _sequences.begin() ; it != _sequences.end() ; it++) {
			if (it -> second -> getName() == sequence.getName()) 
				if (j != i) throw Exception("MapSequenceContainer::setSequence : Sequence's name already exists in container");
			j++;
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType()) {
		// Delete old sequence
		delete _sequences[getKey(i)];
		// New sequence insertion in sequence container
		_sequences[getKey(i)] = dynamic_cast<Sequence *>(sequence.clone());
	} else throw AlphabetMismatchException("MapSequenceContainer::setSequence", _alphabet, sequence.getAlphabet());
}

void MapSequenceContainer::setSequence(const string & name, const Sequence & sequence, bool checkNames) throw (SequenceNotFoundException)
{
	// Sequence's name existence checking
	if (checkNames) {
		// For all names in map : throw exception if name already exists
		for (map<string, Sequence *>::const_iterator it = _sequences.begin() ; it != _sequences.end() ; it++) {
			if (it -> second -> getName() == name) 
				if (it -> second -> getName() != name) 
					throw Exception("MapSequenceContainer::setSequence : Sequence's name already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType()) {
		// Delete old sequence
		delete _sequences[name];
		// New sequence insertion in sequence container
		_sequences[name] = dynamic_cast<Sequence *>(sequence.clone());
	} else throw AlphabetMismatchException("MapSequenceContainer::setSequence", _alphabet, sequence.getAlphabet());
}

void MapSequenceContainer::setSequenceByKey(const string & key, const Sequence & sequence, bool checkNames) throw (SequenceNotFoundException)
{
	// Sequence's name existence checking
	if (checkNames) {
		// For all names in map : throw exception if name already exists
		for (map<string, Sequence *>::const_iterator it = _sequences.begin() ; it != _sequences.end() ; it++) {
			if (it -> second -> getName() == sequence.getName()) 
				if (it -> first != key) 
					throw Exception("MapSequenceContainer::setSequenceByKey : Sequence's name already exists in container");
		}
	}

	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType()) {
		// Delete old sequence
		delete _sequences[key];
		// New sequence insertion in sequence container
		_sequences[key] = dynamic_cast<Sequence *>(sequence.clone());
	} else throw AlphabetMismatchException("MapSequenceContainer::setSequenceByKey", _alphabet, sequence.getAlphabet());
}
	
// Methods to extract (and remove) a sequence from sequence container
Sequence * MapSequenceContainer::removeSequence(unsigned int i) throw (IndexOutOfBoundsException)
{
	//Copy sequence:
	Sequence * deleted = const_cast <Sequence *> (getSequence(i));
	//Remove pointer toward old sequence:
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (; i > 0 ; i--) { it++; }
	_sequences.erase(it);
	//Send copy:
	return deleted;
}

Sequence * MapSequenceContainer::removeSequence(const string & name) throw (SequenceNotFoundException)
{
	//Copy sequence
	Sequence * deleted = const_cast <Sequence *> (getSequence(name));
	//Remove pointer toward old sequence
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (int i = getSequencePosition(name) ; i > 0 ; i--) { it++; }
	_sequences.erase(it);
	//Send copy
	return deleted;
}

Sequence * MapSequenceContainer::removeSequenceByKey(const string & key) {
	// Copy sequence
	Sequence * deleted = const_cast <Sequence *> (getSequenceByKey(key));
	// Remove pointer toward old sequence
	_sequences.erase(key);
	// Send copy
	return deleted;
}

// Methods to delete a sequence from sequence container
void MapSequenceContainer::deleteSequence(unsigned int i) throw (IndexOutOfBoundsException) {
	// Delete sequence
	const Sequence * deleted = getSequence(i);	
	delete deleted;
	// Remove pointer toward old sequence:
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (; i > 0 ; i--) { it++; }
	_sequences.erase(it);
}

void MapSequenceContainer::deleteSequence(const string & name) throw (SequenceNotFoundException) {
	// Delete sequence
	const Sequence * deleted = getSequence(name);
	delete deleted;
	// Remove pointer toward old sequence
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (int i = getSequencePosition(name) ; i > 0 ; i--) { it++; }
	_sequences.erase(it);
}

void MapSequenceContainer::deleteSequenceByKey(const string & key) {
	// Delete sequence
	const Sequence * deleted = getSequenceByKey(key);
	delete deleted;
	// Remove pointer toward old sequence
	_sequences.erase(key);
}

// Method to add sequence in sequence container
void MapSequenceContainer::addSequence(const string & key, const Sequence & sequence, bool checkNames) throw (Exception)
{
	// Sequence's name existence checking
	if (checkNames) {
		// For all names in map : throw exception if name already exists
		for (map<string, Sequence *>::const_iterator it = _sequences.begin() ; it != _sequences.end() ; it++) {
			if (it -> second -> getName() == sequence.getName()) 
				throw Exception("MapSequenceContainer::addSequence : Sequence's name already exists in container");
		}
	}
	
	// Check if the key is not used
	for (map<string, Sequence *>::const_iterator i = _sequences.begin() ; i != _sequences.end() ; i++)
		if (key == i->first)
			throw Exception("MapSequenceContainer::addSequence: key already in use. (" + key + ")");
	
	// New sequence's alphabet and sequence container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() == _alphabet -> getAlphabetType())
		_sequences.insert(make_pair(key, dynamic_cast<Sequence *>(sequence.clone())));
	else throw AlphabetMismatchException("MapSequenceContainer::addSequence", _alphabet, sequence.getAlphabet());
}

// Method to get number of sequences contained in sequence container
unsigned int MapSequenceContainer::getNumberOfSequences() const { return _sequences.size(); }

// Method to get all sequence keys in sequence container
vector<string> MapSequenceContainer::getKeys() const {
	vector<string> keys;
	for (map<string, Sequence *>::const_iterator i = _sequences.begin() ; i != _sequences.end() ; i++) {
		keys.push_back(i -> first);
	}
	return keys;
}

string MapSequenceContainer::getKey(unsigned int pos) const throw (IndexOutOfBoundsException)
{
	if (pos >= getNumberOfSequences())
		throw IndexOutOfBoundsException("MapSequenceContainer::getKey", pos, 0, _sequences.size() - 1);
	map<string, Sequence *>::const_iterator it = _sequences.begin();
	for (unsigned int i = 0 ; i < pos ; i++) it++;
	return it->first;
}

string MapSequenceContainer::getKey(const string & name) const throw (SequenceNotFoundException) {
	try {
		return getKey(getSequencePosition(name));
	}
	catch (SequenceNotFoundException & snfe) {
		throw SequenceNotFoundException("MapSequenceContainer::getKey", snfe.getSequenceId());
	}
}
	
// Method to set the comments (OrderedSequenceContainer interface)
void MapSequenceContainer::setComments(unsigned int pos, const Comments & comments) throw (IndexOutOfBoundsException) {
	if (pos >= getNumberOfSequences())
		throw IndexOutOfBoundsException("MapSequenceContainer::setComments", pos, 0, _sequences.size() - 1);
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (unsigned int i = 0 ; i < pos ; i++) it++;
	it->second->setComments(comments);
}

// Methods to deal with all sequences' names (OrderedSequenceContainer interface)
vector<string> MapSequenceContainer::getSequencesNames() const {
	vector<string> names;
	for (map<string, Sequence *>::const_iterator it = _sequences.begin() ; it != _sequences.end() ; it++)
		names.push_back(it->second->getName());
	return names;
}

void MapSequenceContainer::setSequencesNames(const vector<string> & names, bool checkNames) throw (Exception) {
	if(names.size() != getNumberOfSequences())
		throw BadIntegerException("MapSequenceContainer::setSequenceNames : bad number of names", names.size());
	if(checkNames) {
		// check if there is no repeat names in teh vector
		for (unsigned int i = 0 ; i < names.size() ; i++)
			for (unsigned int j = 0 ; j < i ; j++)
				if (names[j] == names[i])
					throw Exception("MapSequenceContainer::setSequencesNames: Sequence's name already exists in container");
	}
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (unsigned int i = 0 ; i < names.size() ; i++) {
		it->second->setName(names[i]);
		it++;
	}
}
	
void MapSequenceContainer::clear()
{
	// Delete sequences
	for (map<string, Sequence *>::iterator i = _sequences.begin() ; i != _sequences.end() ; i++) { delete i -> second; }
	// Delete all sequence pointers
	_sequences.clear();
}

/******************************************************************************/

SequenceContainer * MapSequenceContainer::getEmptyContainer() const
{ 
	MapSequenceContainer * msc = new MapSequenceContainer(_alphabet);
	msc -> setGeneralComments(_comments);
	return(msc);
}

/******************************************************************************/
