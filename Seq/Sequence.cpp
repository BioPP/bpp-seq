//
// File: SymbolList.h
// Created by: Guillaume Deuchst
// Created on: Tue Aug 21 2003
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

#include "Sequence.h" // class's header file

#include "AlphabetTools.h"
#include "StringSequenceTools.h"

// From utils:
#include <Utils/TextTools.h>

// From the STL:
#include <iostream>
using namespace std;

const bool Sequence::SENSE = true;
const bool Sequence::ANTISENSE = false;

/** Constructors: ***********************************************************************/

Sequence::Sequence(const string & name, const string & sequence, const Alphabet * alpha)
throw (BadCharException) :
	SymbolList(alpha),
	_name(name),
	_sense(true)
{
	setContent(sequence);
}

Sequence::Sequence(const string & name, const string & sequence, const Comments comments, const Alphabet * alpha)
throw (BadCharException) :
	SymbolList(alpha),
	_name(name),
	_comments(comments),
	_sense(true)
{
	setContent(sequence);
}

Sequence::Sequence(const string & name, const vector<string> & sequence, const Alphabet * alpha)
throw (BadCharException) :
	SymbolList(sequence, alpha),
	_name(name),
	_sense(true)
{}

Sequence::Sequence(const string & name, const vector<string> & sequence, const Comments comments, const Alphabet * alpha)
throw (BadCharException) :
	SymbolList(sequence, alpha),
	_name(name),
	_comments(comments),
	_sense(true)
{}

Sequence::Sequence(const string & name, const vector<int> & sequence, const Alphabet * alpha)
throw (BadIntException) :
	SymbolList(sequence, alpha),
	_name(name),
	_sense(true)
{}

Sequence::Sequence(const string & name, const vector<int> & sequence, const Comments comments, const Alphabet * alpha)
throw (BadIntException) :
	SymbolList(sequence, alpha),
	_name(name),
	_comments(comments),
	_sense(true)
{}

/** Copy constructors: ******************************************************************/

Sequence::Sequence(const Sequence & s) :
	SymbolList(s),
	_name(s.getName()),
	_comments(s.getComments()),
	_sense(s.getSense()) { }

/** Assignation operator: ***************************************************************/

Sequence & Sequence::operator = (const Sequence & s) {
	_alphabet = s.getAlphabet();
	_content  = s.getContent();
	_comments = s.getComments();
	_sense    = s.getSense();
	_name     = s.getName();

	return * this;
}

/** Destructor: *************************************************************************/

Sequence::~Sequence() {}

/****************************************************************************************/

Clonable * Sequence::clone() const { return new Sequence(*  this); }

/****************************************************************************************/

const string Sequence::getName() const { return _name; }

void Sequence::setName(const string & name) { _name = name; }

/****************************************************************************************/

const Comments Sequence::getComments() const { return _comments; }

void Sequence::setComments(const Comments & comments) { _comments = comments; }

/****************************************************************************************/

bool Sequence::getSense() const { return _sense; }

void Sequence::setSense(bool sense) { _sense = sense; }

/****************************************************************************************/

void Sequence::setContent(const string & sequence) throw (BadCharException) {
	// Remove blanks in sequence
	_content = StringSequenceTools::codeSequence(TextTools::removeWhiteSpaces(sequence), _alphabet);//Warning, an exception may be casted here!
}

/****************************************************************************************/

void Sequence::setToSizeR(unsigned int size)
{
	unsigned int seqSize = _content.size();
	// Size verification
	if (size < seqSize) {
		_content.resize(size);
		return;
	}
	if (size == seqSize) return;

	// Add gaps up to specified size
	while(_content.size() < size) _content.push_back(-1);
}

/****************************************************************************************/

void Sequence::setToSizeL(unsigned int size)
{
	// Size verification
	unsigned int seqSize = _content.size();
	if (size < seqSize) {
		//We must truncate sequence from the left.
		//This is a very unefficient method!
		_content.erase(_content.begin(), _content.begin() + (seqSize - size));
		return;
	}
	if (size == seqSize) return;

	// Add gaps up to specified size
	_content.insert(_content.begin(), size - seqSize, -1);
}

/****************************************************************************************/

void Sequence::append(const vector<int> & content) throw (BadIntException)
{
	// Check list for incorrect characters
	for (unsigned int i = 0; i < content.size(); i++) {
		if(!_alphabet -> isIntInAlphabet(content[i])) throw BadIntException(content[i], "Sequence::append", _alphabet);
	}
	//Sequence is valid:
	for (unsigned int i = 0; i < content.size(); i++) {
		_content.push_back(content[i]);
	}
}

void Sequence::append(const vector<string> & content) throw (BadCharException)
{
	// Check list for incorrect characters
	for (unsigned int i = 0; i < content.size(); i++) {
		if(!_alphabet -> isCharInAlphabet(content[i])) throw BadCharException(content[i], "Sequence::append", _alphabet);
	}
	//Sequence is valid:
	for (unsigned int i = 0; i < content.size(); i++) {
		_content.push_back(_alphabet -> charToInt(content[i]));
	}
}

void Sequence::append(const string & content) throw (BadCharException)
{
	append(StringSequenceTools::codeSequence(content, _alphabet));
}

/****************************************************************************************/
