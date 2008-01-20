//
// File MapSequenceContainer.cpp
// Authors : Guillaume Deuchst
//           Julien Dutheil
//           Sylvain Gaillard
// Last modification : Monday July 19 2004
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

#include "MapSequenceContainer.h"

// From Utils:
#include <Utils/TextTools.h>

using namespace bpp;

using namespace std;

/******************************************************************************/

MapSequenceContainer::MapSequenceContainer(const map<string, Sequence *> & ms, const Alphabet * alpha) :
    AbstractSequenceContainer(alpha) 
{
	for (map<string, Sequence *>::const_iterator i = ms.begin() ; i != ms.end() ; i++) {		
		addSequence((*i).first, *((*i).second));
	}
}

/******************************************************************************/

MapSequenceContainer::MapSequenceContainer(const Alphabet * alpha) : AbstractSequenceContainer(alpha) {}

/******************************************************************************/

MapSequenceContainer::MapSequenceContainer(const MapSequenceContainer & msc) :
	AbstractSequenceContainer(msc.getAlphabet()) 
{ 
	unsigned int max = msc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < max ; i++) addSequence(msc.getKey(i), * msc.getSequence(i), false);
	_comments = msc.getGeneralComments(); 
}

/******************************************************************************/

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

/******************************************************************************/

MapSequenceContainer::~MapSequenceContainer() 
{
	clear();
}

/******************************************************************************/

Clonable * MapSequenceContainer::clone() const
{
	return new MapSequenceContainer(* this);
}

/******************************************************************************/

const Sequence * MapSequenceContainer::getSequence(unsigned int i) const throw (IndexOutOfBoundsException)
{
	// Specified sequence existence verification
	if (i < _sequences.size()) {
		map<string, Sequence *>::const_iterator it = _sequences.begin();
		for (unsigned int j = 0 ; j < i ; j++) { it++; }
		return it -> second;
	}
	throw IndexOutOfBoundsException("MapSequenceContainer::getSequence", i, 0, _sequences.size() - 1);
	return NULL;
}

/******************************************************************************/

const Sequence * MapSequenceContainer::getSequence(const string & name) const throw (SequenceNotFoundException)
{
	// Specified sequence name research into all sequences
	for (map<string, Sequence *>::const_iterator i = _sequences.begin() ; i != _sequences.end() ; i++) {
		if (i -> second -> getName() == name) return i -> second;
	}
	throw SequenceNotFoundException("MapSequenceContainer::getSequence", name);
	return NULL;
}

/******************************************************************************/

Sequence * MapSequenceContainer::getSequenceInner(unsigned int i) throw (IndexOutOfBoundsException)
{
	// Specified sequence existence verification
	if (i < _sequences.size()) {
		map<string, Sequence *>::const_iterator it = _sequences.begin();
		for (unsigned int j = 0 ; j < i ; j++) { it++; }
		return it -> second;
	}
	throw IndexOutOfBoundsException("MapSequenceContainer::getSequence", i, 0, _sequences.size() - 1);
	return NULL;
}

/******************************************************************************/

Sequence * MapSequenceContainer::getSequenceInner(const string & name) throw (SequenceNotFoundException)
{
	// Specified sequence name research into all sequences
	for (map<string, Sequence *>::const_iterator i = _sequences.begin() ; i != _sequences.end() ; i++) {
		if (i -> second -> getName() == name) return i -> second;
	}
	throw SequenceNotFoundException("MapSequenceContainer::getSequence", name);
	return NULL;
}

/******************************************************************************/

const Sequence * MapSequenceContainer::getSequenceByKey(const string & key) const
    throw (SequenceNotFoundException)
{
	map<string, Sequence *>::const_iterator i = _sequences.find(key);
	if (i != _sequences.end()) return i -> second;

	throw SequenceNotFoundException("MapSequenceContainer::getSequenceByKey", key);
}

/******************************************************************************/

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

/******************************************************************************/

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

/******************************************************************************/

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

/******************************************************************************/

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

/******************************************************************************/

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

/******************************************************************************/

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

/******************************************************************************/

Sequence * MapSequenceContainer::removeSequenceByKey(const string & key)throw (SequenceNotFoundException)
{
	// Copy sequence
	Sequence * deleted = const_cast <Sequence *> (getSequenceByKey(key));
	// Remove pointer toward old sequence
	_sequences.erase(key);
	// Send copy
	return deleted;
}

/******************************************************************************/

void MapSequenceContainer::deleteSequence(unsigned int i) throw (IndexOutOfBoundsException)
{
	// Delete sequence
	const Sequence * deleted = getSequence(i);	
	delete deleted;
	// Remove pointer toward old sequence:
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (; i > 0 ; i--) { it++; }
	_sequences.erase(it);
}

/******************************************************************************/

void MapSequenceContainer::deleteSequence(const string & name) throw (SequenceNotFoundException)
{
	// Delete sequence
	const Sequence * deleted = getSequence(name);
	delete deleted;
	// Remove pointer toward old sequence
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (int i = getSequencePosition(name) ; i > 0 ; i--) { it++; }
	_sequences.erase(it);
}

/******************************************************************************/

void MapSequenceContainer::deleteSequenceByKey(const string & key) throw (SequenceNotFoundException)
{
	// Delete sequence
	const Sequence * deleted = getSequenceByKey(key);
	delete deleted;
	// Remove pointer toward old sequence
	_sequences.erase(key);
}

/******************************************************************************/

void MapSequenceContainer::addSequence(const string & key, const Sequence & sequence, bool checkNames) throw (Exception)
{
	// Sequence's name existence checking
	if (checkNames)
  {
		// For all names in map : throw exception if name already exists
		for (map<string, Sequence *>::const_iterator it = _sequences.begin() ; it != _sequences.end() ; it++)
    {
			if (it->second->getName() == sequence.getName()) 
				throw Exception("MapSequenceContainer::addSequence: Sequence '" + sequence.getName() + ", already exists in container");
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

/******************************************************************************/

unsigned int MapSequenceContainer::getNumberOfSequences() const { return _sequences.size(); }

/******************************************************************************/

vector<string> MapSequenceContainer::getKeys() const
{
	vector<string> keys;
	for (map<string, Sequence *>::const_iterator i = _sequences.begin() ; i != _sequences.end() ; i++) {
		keys.push_back(i -> first);
	}
	return keys;
}

/******************************************************************************/

string MapSequenceContainer::getKey(unsigned int pos) const throw (IndexOutOfBoundsException)
{
	if (pos >= getNumberOfSequences())
		throw IndexOutOfBoundsException("MapSequenceContainer::getKey", pos, 0, _sequences.size() - 1);
	map<string, Sequence *>::const_iterator it = _sequences.begin();
	for (unsigned int i = 0 ; i < pos ; i++) it++;
	return it->first;
}

/******************************************************************************/

string MapSequenceContainer::getKey(const string & name) const throw (SequenceNotFoundException)
{
	try {
		return getKey(getSequencePosition(name));
	}
	catch (SequenceNotFoundException & snfe) {
		throw SequenceNotFoundException("MapSequenceContainer::getKey", snfe.getSequenceId());
	}
}
	
/******************************************************************************/

void MapSequenceContainer::setComments(unsigned int pos, const Comments & comments) throw (IndexOutOfBoundsException)
{
	if (pos >= getNumberOfSequences())
		throw IndexOutOfBoundsException("MapSequenceContainer::setComments", pos, 0, _sequences.size() - 1);
	map<string, Sequence *>::iterator it = _sequences.begin();
	for (unsigned int i = 0 ; i < pos ; i++) it++;
	it->second->setComments(comments);
}

/******************************************************************************/

vector<string> MapSequenceContainer::getSequencesNames() const
{
	vector<string> names;
	for (map<string, Sequence *>::const_iterator it = _sequences.begin() ; it != _sequences.end() ; it++)
		names.push_back(it->second->getName());
	return names;
}

/******************************************************************************/

void MapSequenceContainer::setSequencesNames(const vector<string> & names, bool checkNames) throw (Exception)
{
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

/******************************************************************************/

void MapSequenceContainer::clear()
{
	// Delete sequences
	for (map<string, Sequence *>::iterator i = _sequences.begin() ; i != _sequences.end() ; i++) { delete i -> second; }
	// Delete all sequence pointers
	_sequences.clear();
}

/******************************************************************************/

SequenceContainer * MapSequenceContainer::createEmptyContainer() const
{ 
	MapSequenceContainer * msc = new MapSequenceContainer(_alphabet);
	msc -> setGeneralComments(_comments);
	return(msc);
}

/******************************************************************************/

