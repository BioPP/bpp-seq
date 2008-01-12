//
// File Site.cpp
// Author: Julien Dutheil
//         Guillaume Deuchst
// Created on: Tuesday August 7 2003
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

#include "Site.h"

#include "StringSequenceTools.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

/****************************************************************************************/

Site::Site(const Alphabet * alpha) : SymbolList(alpha), _position(0) {}

Site::Site(const Alphabet * alpha, int position) : SymbolList(alpha), _position(position) {}

Site::Site(const vector<string> & site, const Alphabet * alpha) throw (BadCharException) : SymbolList(site, alpha), _position(0) {}

Site::Site(const vector<string> & site, const Alphabet * alpha, int position) throw (BadCharException) : SymbolList(site, alpha), _position(position) {}

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

