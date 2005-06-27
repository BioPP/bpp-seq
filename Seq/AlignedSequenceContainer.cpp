//
// File: AlignedSequenceContainer.cpp
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Friday August 22 2003
//

/*
Copyright ou © ou Copr. CNRS, (17 Novembre 2004) 

Julien.Dutheil@univ-montp2.fr

Ce logiciel est un programme informatique servant à fournir des classes
pour l'analyse de séquences.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

Julien.Dutheil@univ-montp2.fr

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

#include "AlignedSequenceContainer.h"

// From Utils:
#include <Utils/TextTools.h>

// From the STL:
#include <iostream>
using namespace std;

/** Class constructors: *******************************************************/

AlignedSequenceContainer::AlignedSequenceContainer(const Alphabet * alpha) :
	VectorSequenceContainer(alpha),
	AbstractSequenceContainer(alpha)
{
	length = 0;
	reindexSites();
	//Site * s = NULL; sites = vector<Site *>(length, s);
}

/*                                        ***                                 */

AlignedSequenceContainer::AlignedSequenceContainer(const SiteContainer & sc) : 
	VectorSequenceContainer(sc),
	AbstractSequenceContainer(sc.getAlphabet())
{
	length = sc.getNumberOfSites();
	positions = sc.getSitePositions();
	//Site * s = NULL; sites = vector<Site *>(length, s);
	sites.resize(length);

	// General comments:
	setGeneralComments(sc.getGeneralComments());
}

/*                                        ***                                 */

AlignedSequenceContainer::AlignedSequenceContainer(const AlignedSequenceContainer & asc) : 
	VectorSequenceContainer(asc),
	AbstractSequenceContainer(asc.getAlphabet())
{
	length = asc.getNumberOfSites();
	positions = asc.getSitePositions();
	//Site * s = NULL; sites = vector<Site *>(length, s);
	sites.resize(length);

	// General comments:
	setGeneralComments(asc.getGeneralComments());
}

/*                                        ***                                 */

AlignedSequenceContainer::AlignedSequenceContainer(const OrderedSequenceContainer & osc):
	VectorSequenceContainer(osc.getAlphabet()),
	AbstractSequenceContainer(osc.getAlphabet())
{
	// Initializing
	length = 0;
	int max = osc.getNumberOfSequences();

	// Sequences insertion after size checking
	for (int i = 0 ; i < max ; i++) addSequence(* osc.getSequence(i), false);
	reindexSites();
	//Site * s = NULL; sites = vector<Site *>(length, s);

	// General comments:
	setGeneralComments(osc.getGeneralComments());
}

/*                                        ***                                 */

AlignedSequenceContainer & AlignedSequenceContainer::operator = (const AlignedSequenceContainer & asc) 
{
	// Initializing
	length = asc.getNumberOfSites();
	int max = asc.getNumberOfSequences();

	_alphabet = asc.getAlphabet();

	// Sequences insertion after size checking
	for (int i = 0 ; i < max ; i++)	addSequence(*asc.getSequence(i), false);

	positions = asc.getSitePositions();
	Site * s = NULL; sites = vector<Site *>(length, s);

	// General comments:
	setGeneralComments(asc.getGeneralComments());

	return * this;
}

/*                                        ***                                 */

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

/*                                        ***                                 */

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

/***************************************************************************/

Clonable * AlignedSequenceContainer::clone() const {
	return dynamic_cast<VectorSequenceContainer *>(new AlignedSequenceContainer(*this));
}

/***************************************************************************/

// Method to get an site object from site container
const Site * AlignedSequenceContainer::getSite(unsigned int i) const throw (IndexOutOfBoundsException)
{
	if (i >= length) throw IndexOutOfBoundsException("AlignedSequenceContainer::getSite", i, 0, getNumberOfSites() - 1);

	// Main loop : for all sequences
	unsigned int n = _sequences.size();
	vector<int> site(n);
	for (unsigned int j = 0 ; j < n ; j++) {
    site[j] = (_sequences[j] -> getContent())[i];
  }

	if(sites[i] != NULL) { delete sites[i]; }
	cout << "ok1" << endl;
	sites[i] = new Site(site, _alphabet, positions[i]);
	cout << "ok2" << endl;
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
		VectorSequenceContainer::getSequenceInner(j) -> setElement(pos, s[j]);
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
		VectorSequenceContainer::getSequenceInner(j) -> deleteElement(pos);
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
		VectorSequenceContainer::getSequenceInner(j) -> deleteElement(pos);
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
		VectorSequenceContainer::getSequenceInner(j) -> addElement(s[j]);
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
		VectorSequenceContainer::getSequenceInner(j) -> addElement(pos, site[j]);
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
	if (length == 0) {
		length = sequence.size();
		sites.resize(length);
		reindexSites();
	}
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

/******************************************************************************/

SequenceContainer * AlignedSequenceContainer::getEmptyContainer() const
{ 
	AlignedSequenceContainer * asc = new AlignedSequenceContainer(_alphabet);
	asc -> setGeneralComments(_comments);
	return(dynamic_cast<AbstractSequenceContainer *>(asc));
}

/******************************************************************************/


