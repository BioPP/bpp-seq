/*
 * File AbstractAlphabet.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
*/

#include "AbstractAlphabet.h" // class's header file

// From Utils:
#include <Utils/TextTools.h>

// From the STL:
#include <ctype.h>

/****************************************************************************************/

unsigned int AbstractAlphabet::getNumberOfChars() const { return alphabet.size(); }

/****************************************************************************************/

string AbstractAlphabet::getName(const string & letter) const throw (BadCharException)
{
	string LETTER = TextTools::toUpper(letter);
    
	for (vector<sletter>::const_iterator i = alphabet.begin() ; i < alphabet.end() ; i++)
        if (i -> letter == LETTER) return i -> name;
    
    throw BadCharException(letter, "AbstractAlphabet::getName : Specified base unknown", this);
	return "";
}

/****************************************************************************************/

string AbstractAlphabet::getName(int letter) const throw (BadIntException)
{
    for (unsigned int i = 0 ; i < alphabet.size() ; i++)
        if (alphabet[i].num == letter) return alphabet[i].name;
        
    throw BadIntException(letter, "AbstractAlphabet::getName : Specified base unknown", this);
	return "";
}

/****************************************************************************************/

int AbstractAlphabet::charToInt(const string & letter) const throw (BadCharException)
{
    string LETTER = TextTools::toUpper(letter);
    
    for (unsigned int i = 0; i < alphabet.size(); i++)
        if (alphabet[i].letter == LETTER) return alphabet[i].num;
    
    throw BadCharException(letter, "AbstractAlphabet::charToInt : Specified base unknown", this);
	return -2;
}

/****************************************************************************************/

string AbstractAlphabet::intToChar(int letter) const throw (BadIntException)
{
    for (unsigned int i = 0; i < alphabet.size(); i++)
        if (alphabet[i].num == letter) return alphabet[i].letter;
        
    throw BadIntException(letter, "AbstractAlphabet::intToChar : Specified base unknown", this);
	return "";
}

/****************************************************************************************/

bool  AbstractAlphabet::isIntInAlphabet(int letter) const
{
    for (unsigned int i = 0; i < alphabet.size(); i++)
        if (alphabet[i].num == letter) return true;
	return false;
}

/****************************************************************************************/

bool AbstractAlphabet::isCharInAlphabet(const string & letter) const
{
    string C = TextTools::toUpper(letter);
    
    for (unsigned int i = 0; i < alphabet.size(); i++)
        if (alphabet[i].letter == C) return true;
	return false;
}	

/****************************************************************************************/

bool operator == (AbstractAlphabet::sletter & l1, AbstractAlphabet::sletter & l2)
{
    return (l1.name == l2.name);
}

/****************************************************************************************/
