/*
 * File Site.cpp
 * Author : Julien Dutheil <julien.dutheil@ens-lyon.fr>
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 7 2003
*/
 
#include "Site.h"

#include "StringSequenceTools.h"

// From the STL:
#include <iostream>

using namespace std;

/****************************************************************************************/

Site::Site(const Alphabet * alpha) : SymbolList(alpha), _position(0) {}

Site::Site(const Alphabet * alpha, int position) : SymbolList(alpha), _position(position) {}

Site::Site(const vector<string> & site, const Alphabet * alpha) throw (BadCharException) : SymbolList(alpha), _position(0) {}

Site::Site(const vector<string> & site, const Alphabet * alpha, int position) throw (BadCharException) : SymbolList(alpha), _position(position) {}

Site::Site(const vector<int> & site, const Alphabet * alpha) throw (BadIntException) : SymbolList(site, alpha), _position(0) {}

Site::Site(const vector<int> & site, const Alphabet * alpha, int position) throw (BadIntException) : SymbolList(site, alpha), _position(position) {}

/****************************************************************************************/

Site::Site(const Site & site): SymbolList(site), _position(site.getPosition()) {}

Site Site::operator = (const Site & s)
{
	_content  = s.getContent();
	_position = s.getPosition();
	_alphabet = s.getAlphabet();

	return * this;
}

/****************************************************************************************/

Site::~Site() {}

/****************************************************************************************/

Clonable * Site::clone() const { return new Site(*  this); }

/****************************************************************************************/

int Site::getPosition() const { return _position; };

void Site::setPosition(int position) { _position = position; }

/****************************************************************************************/

bool operator==(const Site & site1, const Site & site2)
{
	// Verify that site's size, position and content are equals
	if(site1.size() != site2.size()) return false;
	if(site1.getPosition() != site2.getPosition()) return false;
	else {
		for(unsigned int i = 0; i < site1.size(); i++) {
			if(site1[i] != site2[i]) return false;
		}
		return true;
	}
}

/****************************************************************************************/

bool operator<(const Site & site1, const Site & site2)
{
	return site1.getPosition() < site2.getPosition();
}

/****************************************************************************************/
