
#include "DefaultAlphabet.h"

// From Utils:
#include <Utils/TextTools.h>

const string DefaultAlphabet::chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890?";

// class constructor
DefaultAlphabet::DefaultAlphabet()
{
	// Alphabet size definition
	alphabet.resize(chars.size() + 1);

	// Alphabet content definition
	alphabet[0].num = -1;
	alphabet[0].letter = "-";
	alphabet[0].abbr = "GAP";
	alphabet[0].name = "Gap";

	for(unsigned int i = 0; i < chars.size(); i++) {
		alphabet[i+1].num = i;
		alphabet[i+1].letter = TextTools::toString(chars[i]);
		alphabet[i+1].abbr = "";
		alphabet[i+1].name = "";
	}
}

unsigned int DefaultAlphabet::getSize() const { return 26; }

unsigned int DefaultAlphabet::getNumberOfTypes() const { return 27; }

string DefaultAlphabet::getAlphabetType() const { return "Default alphabet"; }
