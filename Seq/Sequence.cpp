/*
 * File Sequence.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 21 2003
*/

#include "Sequence.h" // class's header file

#include "AlphabetTools.h"
#include "StringSequenceTools.h"

// From utils:
#include <Utils/TextTools.h>

// From the STL:
#include <iostream>
using namespace std;

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
	while(_content.size() < seqSize) _content.push_back(-1);	
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
