//
// File: VectorSiteContainer.h
// Created by: jdutheil <jdutheil@Deedlit>
// Created on: Mon Oct  6 11:50:40 2003
//

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 

#include "VectorSiteContainer.h"

#include <iostream>

using namespace std;

//From Utils:
#include <Utils/TextTools.h>

/** Class constructors: *******************************************************/

VectorSiteContainer::VectorSiteContainer(
	const vector<const Site *> & vs,
	const Alphabet * alpha)
	throw (Exception):
	AbstractSequenceContainer(alpha)
{
	if(vs.size() == 0) throw Exception("VectorSiteContainer::VectorSiteContainer. Empty site set.");
	//Seq names and comments:
	unsigned int nbSeq = vs[0] -> size();
	_names.resize(nbSeq);
	_comments.resize(nbSeq);
	for(unsigned int i = 0; i < nbSeq; i++) {
		_names[i]    = new string("Seq_" + TextTools::toString(i));
		_comments[i] = new Comments(); 
	}
	//Now try to add each site:
	for(unsigned int i = 0; i < vs.size(); i++) {
		addSite(*vs[i], true); //This may throw an exception if position argument already exists or is size is not valid.
	}
	
	Sequence * s = NULL; _sequences = vector<Sequence *>(nbSeq, s);
}

/*                                     ***                                    */
  
VectorSiteContainer::VectorSiteContainer(unsigned int size, const Alphabet * alpha):
	AbstractSequenceContainer(alpha)
{
	//Seq names and comments:
	_names.resize(size);
	_comments.resize(size);
	for(unsigned int i = 0; i < size; i++) {
		_names[i]    = new string("Seq_" + i);
		_comments[i] = new Comments();
	}
	Sequence * s = NULL; _sequences = vector<Sequence *>(size, s);
}

/*                                     ***                                    */

VectorSiteContainer::VectorSiteContainer(const Alphabet * alpha): AbstractSequenceContainer(alpha) {}

/*                                     ***                                    */

	VectorSiteContainer::VectorSiteContainer(const VectorSiteContainer & vsc):
	AbstractSequenceContainer(vsc.getAlphabet())
{
	// Seq names:
	int nbSeq = vsc.getNumberOfSequences();
	_names.resize(nbSeq);
	setSequencesNames(vsc.getSequencesNames(), true);
	//Now try to add each site:
	for(unsigned int i = 0; i < vsc.getNumberOfSites(); i++) {
		addSite(* vsc.getSite(i), true); //This may throw an exception if position argument already exists or is size is not valid.
	}
	// Seq comments:
	_comments.resize(nbSeq);
	for(int i = 0; i < nbSeq; i++) {
		_comments[i] = new Comments(vsc.getComments(i));
	}
	Sequence * s = NULL; _sequences = vector<Sequence *>(nbSeq, s);

	// General comments:
	setGeneralComments(vsc.getGeneralComments());
}

/*                                     ***                                    */

VectorSiteContainer::VectorSiteContainer(const SiteContainer & sc):
	AbstractSequenceContainer(sc.getAlphabet())
{
	// Seq names:
	int nbSeq = sc.getNumberOfSequences();
	_names.resize(nbSeq);
	setSequencesNames(sc.getSequencesNames(), true);
	//Now try to add each site:
	for(unsigned int i = 0; i < sc.getNumberOfSites(); i++) {
		addSite(* sc.getSite(i), true); //This may throw an exception if position argument already exists or is size is not valid.
	}
	// Seq comments:
	_comments.resize(nbSeq);
	for(int i = 0; i < nbSeq; i++) {
		_comments[i] = new Comments(sc.getComments(i));
	}
	Sequence * s = NULL; _sequences = vector<Sequence *>(nbSeq, s);

	// General comments:
	setGeneralComments(sc.getGeneralComments());
}

/*                                     ***                                    */

VectorSiteContainer::VectorSiteContainer(const OrderedSequenceContainer & osc):
	AbstractSequenceContainer(osc.getAlphabet())
{
	unsigned int nbSeq = osc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < nbSeq ; i++) addSequence(* osc.getSequence(i), true);
	reindexSites();

	// General comments:
	setGeneralComments(osc.getGeneralComments());
}

/******************************************************************************/

VectorSiteContainer& VectorSiteContainer::operator = (const VectorSiteContainer & vsc) {
	//Setting up alphabet:
	_alphabet = vsc.getAlphabet();
	//Seq names:
	_names.resize(vsc.getNumberOfSequences());
	setSequencesNames(vsc.getSequencesNames(), true);
	//Now try to add each site:
	for(unsigned int i = 0; i < vsc.getNumberOfSites(); i++) {
		addSite(* vsc.getSite(i), true); //This may throw an exception if position argument already exists or is size is not valid.
	}
	//Seq comments:
	unsigned int nbSeq = vsc.getNumberOfSequences();
	_comments.resize(nbSeq);
	for(unsigned int i = 0; i < nbSeq; i++) {
		_comments[i] = new Comments(vsc.getComments(i));
	}
	Sequence * s = NULL; _sequences = vector<Sequence *>(nbSeq, s);

	// General comments:
	setGeneralComments(vsc.getGeneralComments());

	return * this;
}

/*                                     ***                                    */

VectorSiteContainer& VectorSiteContainer::operator = (const SiteContainer & sc) {
	//Setting up alphabet:
	_alphabet = sc.getAlphabet();
	//Seq names:
	_names.resize(sc.getNumberOfSequences());
	setSequencesNames(sc.getSequencesNames(), true);
	//Now try to add each site:
	for(unsigned int i = 0; i < sc.getNumberOfSites(); i++) {
		addSite(* sc.getSite(i), true); //This may throw an exception if position argument already exists or is size is not valid.
	}
	//Seq comments:
	unsigned int nbSeq = sc.getNumberOfSequences();
	_comments.resize(nbSeq);
	for(unsigned int i = 0; i < nbSeq; i++) {
		_comments[i] = new Comments(sc.getComments(i));
	}
	Sequence * s = NULL; _sequences = vector<Sequence *>(nbSeq, s);

	// General comments:
	setGeneralComments(sc.getGeneralComments());

	return * this;
}

/*                                     ***                                    */

VectorSiteContainer& VectorSiteContainer::operator = (const OrderedSequenceContainer & osc)
{
	//Setting up alphabet:
	_alphabet = osc.getAlphabet();

	unsigned int nbSeq = osc.getNumberOfSequences();
	for (unsigned int i = 0 ; i < nbSeq ; i++) addSequence(* osc.getSequence(i), true);
	reindexSites();

	// General comments:
	setGeneralComments(osc.getGeneralComments());

	return * this;
}

/**  Class destructor ************************************************************/

VectorSiteContainer::~VectorSiteContainer() { clear(); }

/******************************************************************************/

Clonable * VectorSiteContainer::clone() const {
	return dynamic_cast<SiteContainer *>(new VectorSiteContainer(*this));
}

/***************************************************************************/

const Site * VectorSiteContainer::getSite(unsigned int i) const throw (IndexOutOfBoundsException)
{
	if(i >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::getSite.", i, 0, getNumberOfSites() - 1);
	return _sites[i];
}

/******************************************************************************/

void VectorSiteContainer::setSite(unsigned int pos, const Site & site, bool checkPositions) throw (Exception)
{
	if(pos >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::setSite.", pos, 0, getNumberOfSites() - 1);

    // Check size:
	if(site.size() != getNumberOfSequences()) throw SiteException("AlignedSequenceContainer::addSite. Site does not have the appropriate length", &site);

	// New site's alphabet and site container's alphabet matching verification
	if (site.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType()) {
		throw AlphabetMismatchException("VectorSiteContainer::addSite", _alphabet, site.getAlphabet());
	}

	// Check position:
	if(checkPositions) {
		int position = site.getPosition();
		// For all positions in vector : throw exception if position already exists
		for(unsigned int i = 0; i < _sites.size(); i++) {
			if (_sites[i] -> getPosition() == position) throw SiteException("VectorSiteContainer::setSite: Site position already exists in container", &site);
		}
	}
    delete _sites[pos];
    _sites[pos] = dynamic_cast<Site *>(site.clone());
}

/******************************************************************************/

Site * VectorSiteContainer::removeSite(unsigned int i) throw (IndexOutOfBoundsException)
{
	if(i >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::removeSite.", i, 0, getNumberOfSites() - 1);
	Site * site = _sites[i];
	_sites.erase(_sites.begin() + i);
	return site;
}

/******************************************************************************/

void VectorSiteContainer::deleteSite(unsigned int i) throw (IndexOutOfBoundsException)
{
	if(i >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::deleteSite.", i, 0, getNumberOfSites() - 1);
	delete _sites[i];
	_sites.erase(_sites.begin() + i);	
}

/******************************************************************************/

void VectorSiteContainer::addSite(const Site & site, bool checkPositions) throw (Exception)
{
	// Check size:
	if(site.size() != getNumberOfSequences()) throw SiteException("VectorSiteContainer::addSite. Site does not have the appropriate length", &site);

	// New site's alphabet and site container's alphabet matching verification
	if (site.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType()) {
		throw AlphabetMismatchException("VectorSiteContainer::addSite", _alphabet, site.getAlphabet());
	}

    // Check position:
	if(checkPositions) {
		int position = site.getPosition();
		// For all positions in vector : throw exception if position already exists
		for(unsigned int i = 0; i < _sites.size(); i++) {
			if (_sites[i] -> getPosition() == position) throw SiteException("VectorSiteContainer::addSite. Site position already exists in container", &site);
		}
	}
	
	_sites.push_back(dynamic_cast<Site *>(site.clone()));	
}

/******************************************************************************/

void VectorSiteContainer::addSite(const Site & site, unsigned int pos, bool checkPositions) throw (Exception)
{
	if(pos >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::addSite", pos, 0, getNumberOfSites() - 1);

	// Check size:
	if(site.size() != getNumberOfSequences()) throw SiteException("VectorSiteContainer::addSite. Site does not have the appropriate length", &site);

	// New site's alphabet and site container's alphabet matching verification
	if (site.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType()) {
		throw AlphabetMismatchException("VectorSiteContainer::addSite", _alphabet, site.getAlphabet());
	}

	// Check position:
	if(checkPositions) {
		int position = site.getPosition();
		// For all positions in vector : throw exception if position already exists
		for(unsigned int i = 0; i < _sites.size(); i++) {
			if (_sites[i] -> getPosition() == position) throw SiteException("VectorSiteContainer::addSite. Site position already exists in container", &site);
		}
	}

	//insert(begin() + pos, new Site(site));
	_sites.insert(_sites.begin() + pos, dynamic_cast<Site *>(site.clone()));
}

/******************************************************************************/

unsigned int VectorSiteContainer::getNumberOfSites() const {
	return _sites.size();
}

// Method to get sites's positions
const vector<int> VectorSiteContainer::getPositions() const {
	vector<int> positions(getNumberOfSites());
	for(unsigned int i = 0; i < positions.size(); i++) {
		positions[i] = getSite(i) -> getPosition();
	}
	return positions;
}

/******************************************************************************/

void VectorSiteContainer::reindexSites() {
	int pos = 0; //first position is 0.
	for(vector<Site *>::iterator i = _sites.begin(); i < _sites.end(); i++) {
		(*i) -> setPosition(pos++);
	}	
}

/******************************************************************************/

Vint VectorSiteContainer::getSitePositions() const
{
	Vint positions(_sites.size());
	for(unsigned int i = 0; i < _sites.size(); i++) positions[i] = _sites[i] -> getPosition();
	return positions;
}

/******************************************************************************/

const Sequence * VectorSiteContainer::getSequence(unsigned int i) const throw (IndexOutOfBoundsException)
{
	if(i >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::getSequence.", i, 0, getNumberOfSequences() - 1);

	string sequence = "";
	for(unsigned int j = 0; j < getNumberOfSites(); j++) {
		sequence += getSite(j) -> getChar(i);
	}
	if(_sequences[i] != NULL) delete _sequences[i];
	_sequences[i] = new Sequence(*_names[i], sequence, *_comments[i], _alphabet);
	return _sequences[i];
}

/******************************************************************************/

const Sequence * VectorSiteContainer::getSequence(const string & name) const throw (SequenceNotFoundException)
{
	//Look for sequence name:
	int pos = getSequencePosition(name);
	return getSequence(pos);
}

/******************************************************************************/

unsigned int VectorSiteContainer::getSequencePosition(const string & name) const throw (SequenceNotFoundException)
{
	//Look for sequence name:
	for(unsigned int pos = 0; pos < _names.size(); pos++) {
		if(*_names[pos] == name) return pos;
	}
	throw SequenceNotFoundException("VectorSiteContainer::getSequencePosition().", name);
}

/******************************************************************************/

void VectorSiteContainer::setSequence(const string & name, const Sequence & sequence, bool checkNames) throw (Exception)
{
	//Look for sequence name:
	unsigned int pos = getSequencePosition(name);
	setSequence(pos, sequence, checkNames);
}

/******************************************************************************/

void VectorSiteContainer::setSequence(unsigned int pos, const Sequence & sequence, bool checkNames)
    throw (Exception)
{
	if(pos >= getNumberOfSequences()) throw BadIntegerException("VectorSiteContainer::saddSequence", pos);

	// New sequence's alphabet and site container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType()) {
		throw AlphabetMismatchException("VectorSiteContainer::addSite", _alphabet, sequence.getAlphabet());
	}

	//If the container has only one sequence, we set the size to the size of this sequence:
	if(getNumberOfSequences() == 1) realloc(sequence.size());

	if(sequence.size() != _sites.size()) throw SequenceException("VectorSiteContainer::addSequence. Sequence has not the appropriate length.", &sequence);

	if(checkNames) {
		for(unsigned int i = 0; i < _names.size(); i++) {
			if(i != pos && sequence.getName() == *_names[i]) throw SequenceException("VectorSiteContainer::addSequence. Name already exists in container.", &sequence);
		}
	}
	//Update name:
	delete _names[pos]; _names[pos] = new string(sequence.getName());
	//Update elements at each site:
	for(unsigned int i = 0; i < _sites.size(); i++) {
		_sites[i] -> setElement(pos, sequence.getValue(i));
	}
	//Update comments:
	if(_comments[pos] != NULL) delete _comments[pos];
	_comments[pos] = new Comments(sequence.getComments());
	//Update sequences:
	if(_sequences[pos] != NULL) delete _sequences[pos];
	_sequences[pos] = NULL;
}

/******************************************************************************/

Sequence * VectorSiteContainer::removeSequence(unsigned int i) throw (IndexOutOfBoundsException)
{
	if(i >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::removeSequence.", i, 0, getNumberOfSequences() - 1);

	Sequence * sequence = const_cast<Sequence *>(getSequence(i));
	for(unsigned int j = 0; j < _sites.size(); j++) {
		//For each site:
		_sites[j] -> deleteElement(i);
	}
	
	//Now actualize names and comments:
	delete _names[i]; _names.erase(_names.begin() + i);
	if(_comments[i] != NULL) delete _comments[i];
	_comments.erase(_comments.begin() + i);
	// We remove the sequence, so the destruction of the sequence is up to the user:
	//if(_sequences[i] != NULL) delete _sequences[i];
	_sequences.erase(_sequences.begin() + i);
	return sequence;
}

/******************************************************************************/

Sequence * VectorSiteContainer::removeSequence(const string & name) throw (SequenceNotFoundException)
{
	//Look for sequence name:
	int pos = getSequencePosition(name);
	return removeSequence(pos);
}

/******************************************************************************/

void VectorSiteContainer::deleteSequence(unsigned int i) throw (IndexOutOfBoundsException)
{
	if(i >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::demeteSequence.", i, 0, getNumberOfSequences() - 1);
	for(unsigned int j = 0; j < _sites.size(); j++) _sites[j] -> deleteElement(i);

	//Now actualize names and comments:
	delete _names[i]; _names.erase(_names.begin() + i);
	if(_comments[i] != NULL) delete _comments[i];
	_comments.erase(_comments.begin() + i);
	if(_sequences[i] != NULL) delete _sequences[i];
	_sequences.erase(_sequences.begin() + i);
}

/******************************************************************************/

void VectorSiteContainer::deleteSequence(const string & name) throw (SequenceNotFoundException)
{
	//Look for sequence name:
	int pos = getSequencePosition(name);
	deleteSequence(pos);	
}

/******************************************************************************/

void VectorSiteContainer::addSequence(const Sequence & sequence, bool checkNames) throw (Exception)
{
	//If the container has no sequence, we set the size to the size of this sequence:
	if(getNumberOfSequences() == 0) realloc(sequence.size());
		
	// New sequence's alphabet and site container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType()) {
		throw AlphabetMismatchException("VectorSiteContainer::addSite", _alphabet, sequence.getAlphabet());
	}

	if(sequence.size() != _sites.size()) throw SequenceException("VectorSiteContainer::addSequence. Sequence has not the appropriate length.", &sequence);

	if(checkNames) {
		for(unsigned int i = 0; i < _names.size(); i++) {
			if(sequence.getName() == *_names[i]) throw SequenceException("VectorSiteContainer::addSequence. Name already exists in container.", &sequence);
		}
	}
	
	//Append name:
	_names.push_back(new string(sequence.getName()));
    
	//Append elements at each site:
	for(unsigned int i = 0; i < _sites.size(); i++) {
		_sites[i] -> addElement(sequence.getValue(i));
	}
    
	//Append comments:
	_comments.push_back(new Comments(sequence.getComments()));
    
	//Sequences pointers:
	_sequences.push_back(NULL);
}

/******************************************************************************/

void VectorSiteContainer::addSequence(
	const Sequence & sequence,
	unsigned int pos,
	bool checkNames)
	throw (Exception)
{
	if(pos >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::addSequence.", pos, 0, getNumberOfSequences() - 1);
	if(sequence.size() != _sites.size()) throw SequenceNotAlignedException("VectorSiteContainer::setSequence", &sequence);

	// New sequence's alphabet and site container's alphabet matching verification
	if (sequence.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType()) {
		throw AlphabetMismatchException("VectorSiteContainer::addSite", _alphabet, sequence.getAlphabet());
	}

	if(checkNames) {
		for(unsigned int i = 0; i < _names.size(); i++) {
			if(sequence.getName() == *_names[i]) throw SequenceException("VectorSiteContainer::addSequence. Name already exists in container.", &sequence);
		}
	}

  for(unsigned int i = 0; i < _sites.size(); i++) {
		//For each site:
		_sites[i] -> addElement(pos, sequence.getValue(i));
	}
	//Actualize names and comments:
	_names.insert(_names.begin() + pos, new string(sequence.getName()));	
	_comments.insert(_comments.begin() + pos, new Comments(sequence.getComments()));
	_sequences.insert(_sequences.begin() + pos, NULL);
}

/******************************************************************************/

unsigned int VectorSiteContainer::getNumberOfSequences() const {
	return _names.size();
}

/******************************************************************************/

void VectorSiteContainer::clear()
{
	//Must delete all sites in the container:
	for(unsigned int i = 0; i < _sites.size(); i++) delete _sites[i];

	//must delete all sequence names too:
	for(unsigned int i = 0; i < _names.size(); i++) delete _names[i];

	//must delete all comments too:
	for(unsigned int i = 0; i < _comments.size(); i++) if(_comments[i] != NULL) delete _comments[i];

	//Delete all sequences retrieved:
	for(unsigned int i = 0; i < _sequences.size(); i++) if(_sequences[i] != NULL) delete(_sequences[i]);

    // Delete all sites pointers
	_sites.clear();
    _names.clear();
    _comments.clear();
    _sequences.clear();
    
}

/******************************************************************************/

void VectorSiteContainer::realloc(unsigned int n) {
	clear();
	_sites.resize(n);
	for(unsigned int i = 0; i < n; i++) {
		_sites[i] = new Site(_alphabet);
	}
	reindexSites();
}

/******************************************************************************/

vector<string> VectorSiteContainer::getSequencesNames() const
 {
	vector<string> seqnames(_names.size());
	for(unsigned int i = 0; i < _names.size(); i++) seqnames[i] = *_names[i];
	return seqnames;
}

/******************************************************************************/

void VectorSiteContainer::setSequencesNames(
	const vector<string> & names,
	bool checkNames)
	throw (Exception)
{
	if(names.size() != getNumberOfSequences())
		throw BadIntegerException("VectorSiteContainer::setSequenceNames: bad number of names.", names.size());
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
		delete _names[i];
		_names[i] = new string(names[i]);
	}
}

/******************************************************************************/

void VectorSiteContainer::setComments(unsigned int i, const Comments & comments) throw (IndexOutOfBoundsException)
{
	_comments[i] = new Comments(comments);
}

/******************************************************************************/
