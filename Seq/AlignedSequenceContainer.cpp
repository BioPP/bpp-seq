/*
 * File AlignedSequenceContainer.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Friday August 22 2003
*/

#include "AlignedSequenceContainer.h"

// From Utils:
#include <Utils/TextTools.h>

// From the STL:
#include <iostream>
using namespace std;

/** Class constructors: *******************************************************/

AlignedSequenceContainer::AlignedSequenceContainer(const Alphabet * alpha):
	VectorSequenceContainer(alpha)
{
	length = 0;
	reindexSites();
	Site * s = NULL; sites = vector<Site *>(length, s);
}

/** Copy constructors: ********************************************************/

AlignedSequenceContainer::AlignedSequenceContainer(const SiteContainer & sc) : VectorSequenceContainer(sc)
{
	length = sc.getNumberOfSites();
	positions = sc.getSitePositions();
	Site * s = NULL; sites = vector<Site *>(length, s);

	// General comments:
	setGeneralComments(sc.getGeneralComments());
}

AlignedSequenceContainer::AlignedSequenceContainer(const OrderedSequenceContainer & osc):
	VectorSequenceContainer(osc.getAlphabet())
{
	// Initializing
	length = 0;
	int max = osc.getNumberOfSequences();

	// Sequences insertion after size checking
	for (int i = 0 ; i < max ; i++) addSequence(* osc.getSequence(i), false);
	reindexSites();
	Site * s = NULL; sites = vector<Site *>(length, s);

	// General comments:
	setGeneralComments(osc.getGeneralComments());
}

AlignedSequenceContainer & AlignedSequenceContainer::operator = (const SiteContainer & sc) 
{
	// Initializing
	length = sc.getNumberOfSites();
	int max = sc.getNumberOfSequences();

	_alphabet = sc.getAlphabet();

	// Sequences insertion after size checking
	for (int i = 0 ; i < max ; i++)	addSequence(*sc.getSequence(i), false);

	positions = sc.getSitePositions();
	Site * s = NULL; sites = vector<Site *>(length, s);

	// General comments:
	setGeneralComments(sc.getGeneralComments());

	return * this;
}

AlignedSequenceContainer & AlignedSequenceContainer::operator = (const OrderedSequenceContainer & osc) throw (SequenceNotAlignedException)
{
	// Initializing
	length = 0;
	int max = osc.getNumberOfSequences();

	_alphabet = osc.getAlphabet();

	// Sequences insertion after size checking
	for (int i = 0 ; i < max ; i++) addSequence(*osc.getSequence(i), false); //may throw a SequenceNotAlignedException.
	
	reindexSites();
	Site * s = NULL; sites = vector<Site *>(length, s);

	// General comments:
	setGeneralComments(osc.getGeneralComments());



	return * this;
}

/** Class destructor: *********************************************************/

AlignedSequenceContainer::~AlignedSequenceContainer() {
	//delete all sites:
	for(unsigned int i = 0; i < sites.size(); i++) if(sites[i] != NULL) delete sites[i];	
}

// Method to get an site object from site container
const Site * AlignedSequenceContainer::getSite(unsigned int i) const throw (IndexOutOfBoundsException)
{
	if (i >= length) throw IndexOutOfBoundsException("AlignedSequenceContainer::getSite", i, 0, getNumberOfSites() - 1);

	// Main loop : for all sequences
	unsigned int n = _sequences.size();
	vector<int> site(n);
	for (unsigned int j = 0 ; j < n ; j++) {
        site[j] =  (_sequences[j] -> getContent())[i];
    }

	if(sites[i] != NULL) { delete sites[i]; }
	sites[i] = new Site(site, _alphabet, positions[i]);
	return sites[i];
}

// Method to replace a site in site container
void AlignedSequenceContainer::setSite(unsigned int pos, const Site & site, bool checkPositions) throw (Exception)
{
	// New site's alphabet and site container's alphabet matching verification
	if(pos >= getNumberOfSites()) throw IndexOutOfBoundsException("AlignedSequenceContainer::setSite", pos, 0, getNumberOfSites() - 1);
	if (site.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType())
		throw AlphabetMismatchException("AlignedSequenceContainer::setSite", _alphabet, site.getAlphabet());

		
	vector<int> s = site.getContent();

	// Check size:
	if(s.size() != getNumberOfSequences()) throw SiteException("AlignedSequenceContainer::setSite, site does not have the appropriate length", &site);

	// Check position:
	int position = site.getPosition();
	if(checkPositions) {
		// For all positions in vector : throw exception if position already exists
		for(unsigned int i = 0; i < positions.size(); i++) {
			if (positions[i] == position) throw SiteException("AlignedSequenceContainer::setSite: Site position already exists in container", &site);
		}
	}
	
	// For all sequences
	for (unsigned int j = 0 ; j < _sequences.size(); j++) {
		VectorSequenceContainer::getSequence(j) -> setElement(pos, s[j]);
	}
	positions[pos] = site.getPosition();
}

// Method to extract (and remove) a site from site container
Site * AlignedSequenceContainer::removeSite(unsigned int pos) throw (IndexOutOfBoundsException)
{
	if(pos >= getNumberOfSites()) throw IndexOutOfBoundsException("AlignedSequenceContainer::removeSite", pos, 0, getNumberOfSites() - 1);

	// Get old site
	Site * old = const_cast<Site *>(getSite(pos));

	// For all sequences
	for (unsigned int j = 0 ; j < _sequences.size() ; j++) {
		VectorSequenceContainer::getSequence(j) -> deleteElement(pos);
	}

	// Delete site's position
	positions.erase(positions.begin() + pos);
	length--;
	
	//Actualizes the 'sites' vector:
	delete sites[pos];
	sites.erase(sites.begin() + pos);
	
	// Send result
	return old;
}

// Method to delete a site from site container
void AlignedSequenceContainer::deleteSite(unsigned int pos) throw (IndexOutOfBoundsException) {
	if(pos >= getNumberOfSites()) throw IndexOutOfBoundsException("AlignedSequenceContainer::deleteSite", pos, 0, getNumberOfSites() - 1);

	// Initializing
	string s;

	// For all sequences
	for (unsigned int j = 0 ; j < _sequences.size() ; j++) {
		VectorSequenceContainer::getSequence(j) -> deleteElement(pos);
	}

	// Delete site's position
	positions.erase(positions.begin() + pos);
	length--;

	//Actualizes the 'sites' vector:
	delete sites[pos];
	sites.erase(sites.begin() + pos);
}
		
// Method to add site in site container
void AlignedSequenceContainer::addSite(const Site & site, bool checkPositions) throw (Exception)
{
	// New site's alphabet and site container's alphabet matching verification
	if (site.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType()) {
		throw AlphabetMismatchException("AlignedSequenceContainer::addSite");
	}

	// Initializing
	vector<int> s = site.getContent();
	
	// Check size:
	if(s.size() != getNumberOfSequences()) throw SiteException("AlignedSequenceContainer::addSite, site does not have the appropriate length", &site);

	// Check position:

	int position = site.getPosition();
	if(checkPositions) {
		// For all positions in vector : throw exception if position already exists
		for(unsigned int i = 0; i < positions.size(); i++) {
			if (positions[i] == position) throw SiteException("AlignedSequenceContainer::addSite: Site position already exists in container", &site);
		}
	}
	
	// For all sequences
	for (unsigned int j = 0 ; j < _sequences.size() ; j++) {
		VectorSequenceContainer::getSequence(j) -> addElement(s[j]);
	}

	length++;
	positions.push_back(position);

	//Actualizes the 'sites' vector:
	sites.push_back(NULL);
}

void AlignedSequenceContainer::addSite(const Site & site, unsigned int pos, bool checkPositions) throw (Exception)
{
	if(pos >= getNumberOfSites()) throw IndexOutOfBoundsException("AlignedSequenceContainer::addSite", pos, 0, getNumberOfSites() - 1);

	// New site's alphabet and site container's alphabet matching verification
	if (site.getAlphabet() -> getAlphabetType() != _alphabet -> getAlphabetType()) {
		throw AlphabetMismatchException("AlignedSequenceContainer::addSite", _alphabet, site.getAlphabet());
	}

	vector<int> s = site.getContent();
	
	// Check size:
	if(s.size() != getNumberOfSequences()) throw SiteException("AlignedSequenceContainer::addSite, site does not have the appropriate length", &site);

	// Check position:
	int position = site.getPosition();
	if(checkPositions) {
		// For all positions in vector : throw exception if position already exists
		for(unsigned int i = 0; i < positions.size(); i++) {
			if (positions[i] == position) throw SiteException("AlignedSequenceContainer::addSite: Site position already exists in container", &site);
		}
	}
	
	// For all sequences
	for (unsigned int j = 0 ; j < _sequences.size() ; j++) {
		VectorSequenceContainer::getSequence(j) -> addElement(pos, site[j]);
	}

	length++;
	positions.insert(positions.begin() + pos, position);

	//Actualizes the 'sites' vector:
	sites.insert(sites.begin() + pos, NULL);	
}

// Method to get number of sites contained in site container
unsigned int AlignedSequenceContainer::getNumberOfSites() const { return length; }

// Method to get sites's positions
Vint AlignedSequenceContainer::getSitePositions() const { return positions; }

// Method to reindex sites's positions
void AlignedSequenceContainer::reindexSites() 
{	
	positions.resize(length);
	for (unsigned int i = 0 ; i < length ; i++) { positions[i] = i; }

}

// Overloaded methods to replace a sequence in sequence container
// CheckName : boolean for enable or disable sequence's name existence checking
void AlignedSequenceContainer::setSequence(unsigned int i, const Sequence & sequence, bool checkNames) throw (Exception)
{
	if(i >= getNumberOfSequences()) throw IndexOutOfBoundsException("AlignedSequenceContainer::setaddSequence", i, 0, getNumberOfSequences() - 1);
	// if container has only one sequence
	if (getNumberOfSequences() == 1) length = sequence.size();
	if (checkSize(sequence)) VectorSequenceContainer::setSequence(i, sequence, checkNames);
	else throw SequenceNotAlignedException("AlignedSequenceContainer::setSequence", &sequence);
}

void AlignedSequenceContainer::setSequence(const string & name, const Sequence & sequence, bool checkNames) throw (Exception)
{
	// if container has only one sequence
	if (getNumberOfSequences() == 1) length = sequence.size();  
	if (checkSize(sequence)) VectorSequenceContainer::setSequence(name, sequence, checkNames);
	else throw SequenceNotAlignedException("AlignedSequenceContainer::setSequence", &sequence);
}

// Overloaded methods to add sequence in sequences container
// CheckName : boolean for enable or disable sequence's name existence checking
void AlignedSequenceContainer::addSequence(const Sequence & sequence, bool checkNames) throw (Exception)
{
	// if container has only one sequence
	if (length == 0) length = sequence.size();
	if (checkSize(sequence)) VectorSequenceContainer::addSequence(sequence, checkNames);
	else throw SequenceNotAlignedException("AlignedSequenceContainer::addSequence", &sequence);
}

void AlignedSequenceContainer::addSequence(const Sequence & sequence, unsigned int i, bool checkNames) throw (Exception)
{
	if(i >= getNumberOfSequences()) throw IndexOutOfBoundsException("AlignedSequenceContainer::addSequence", i, 0, getNumberOfSequences() - 1);
	// if container has only one sequence
	if (length == 0) length = sequence.size();
	if (checkSize(sequence)) VectorSequenceContainer::addSequence(sequence, i, checkNames);
	else throw SequenceNotAlignedException("AlignedSequenceContainer::addSequence", &sequence);
}

// Private method to verify sequence's size before insertion in sequence container
bool AlignedSequenceContainer::checkSize(const Sequence & sequence) { return (sequence.size() == length); }

// Overloaded method to purge the sequences container
void AlignedSequenceContainer::clear()
{
	length = 0;
	VectorSequenceContainer::clear();
}
