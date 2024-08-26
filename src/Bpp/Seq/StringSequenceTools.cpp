// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Random/RandomTools.h>
#include <Bpp/Text/TextTools.h>

#include "Alphabet/AlphabetTools.h"
#include "Alphabet/DNA.h"
#include "Alphabet/ProteicAlphabet.h"
#include "Alphabet/RNA.h"
#include "StringSequenceTools.h"

using namespace bpp;

// From the STL:
#include <map>
#include <ctype.h>
#include <algorithm>
#include <iostream>

using namespace std;

/****************************************************************************************/

string StringSequenceTools::subseq(const string& sequence, size_t begin, size_t end)
{
	// Checking interval
	if (end < begin)
		throw Exception ("StringSequenceTools::subseq: Invalid interval");

	// Copy sequence
	string temp(sequence);

	// Truncate sequence
	temp.erase(temp.begin() + static_cast<ptrdiff_t>(end + 1), temp.end());
	temp.erase(temp.begin(), temp.begin() + static_cast<ptrdiff_t>(begin));

	// Send result
	return temp;
}

/****************************************************************************************/

string StringSequenceTools::setToSizeR(const string& sequence, size_t size)
{
	return TextTools::resizeRight(sequence, size, '-');
}

string StringSequenceTools::setToSizeL(const string& sequence, size_t size)
{
	return TextTools::resizeLeft(sequence, size, '-');
}

/****************************************************************************************/

string StringSequenceTools::deleteChar(const string& sequence, char chars)
{
	// Copy sequence
	string result(sequence);

	// Search and delete specified char
	for (unsigned int i = 0; i < result.size(); i++)
	{
		if (result[i] == chars)
			result.erase(result.begin() + i);
	}

	return result;
}

/****************************************************************************************/

string StringSequenceTools::deleteChar(const string& sequence, string chars)
{
	// Copy sequence
	string result(sequence);

	// For all characters to delete
	for (unsigned int i = 0; i < chars.size(); i++)
	{
		// Search and delete char
		for (unsigned int j = 0; j < result.size(); j++)
		{
			if (result[j] == chars[i])
				result.erase(result.begin() + j);
		}
	}

	return result;
}

/****************************************************************************************/

string* StringSequenceTools::reverse(const string& sequence)
{
	// Initializing
	string* result = new string;

	// Main loop : reverse all characters of sequence
	size_t size = sequence.size();
	for (size_t i = 0; i < size; i++)
	{
		*result += sequence[size - i - 1];
	}

	// Send result
	return result;
}

/****************************************************************************************/

string* StringSequenceTools::complement(const string& sequence)
{
	// Initializing
	string* result = new string;

	// Main loop : completement all characters
	size_t size = sequence.size();
	for (unsigned int i = 0; i < size; i++)
	{
		switch (sequence[i])
		{
		case 'A': *result += 'T';
			break;
		case 'C': *result += 'G';
			break;
		case 'G': *result += 'C';
			break;
		case 'T': *result += 'A';
			break;
		case 'M': *result += 'K';
			break;
		case 'R': *result += 'Y';
			break;
		case 'Y': *result += 'R';
			break;
		case 'K': *result += 'M';
			break;
		case 'V': *result += 'B';
			break;
		case 'H': *result += 'D';
			break;
		case 'D': *result += 'H';
			break;
		case 'B': *result += 'V';
			break;
		default: *result += sequence[i];
			break;
		}
	}

	// Send new sequence
	return result;
}

/****************************************************************************************/

double StringSequenceTools::getGCcontent(const string& sequence, size_t pos, size_t window)
{
	// Frequency counts for nucleotids A, C, G, T
	map<char, double> counts;

	// Window size checking
	if (window < sequence.size())
		throw BadIntegerException("StringSequenceTools::getGCContent : specified window too high", static_cast<int>(window));

	// For last nucleotides
	if (pos + window > sequence.size())
	{
		pos = sequence.size() - window;
	}

	// Main loop
	for (size_t i = pos; i < pos + window; i++)
	{
		switch (toupper(sequence[i]))
		{
		case 'A': counts['A'] += 1;
			break;
		case 'C': counts['C'] += 1;
			break;
		case 'G': counts['G'] += 1;
			break;
		case 'T': counts['T'] += 1;
			break;
		case 'M': counts['A'] += 0.5;
			counts['C'] += 0.5;
			break;
		case 'R': counts['A'] += 0.5;
			counts['G'] += 0.5;
			break;
		case 'W': counts['A'] += 0.5;
			counts['T'] += 0.5;
			break;
		case 'S': counts['C'] += 0.5;
			counts['G'] += 0.5;
			break;
		case 'Y': counts['C'] += 0.5;
			counts['T'] += 0.5;
			break;
		case 'K': counts['G'] += 0.5;
			counts['T'] += 0.5;
			break;
		case 'V': counts['A'] += 0.34;
			counts['C'] += 0.34;
			counts['G'] += 0.34;
			break;
		case 'H': counts['A'] += 0.34;
			counts['C'] += 0.34;
			counts['T'] += 0.34;
			break;
		case 'D': counts['A'] += 0.34;
			counts['G'] += 0.34;
			counts['T'] += 0.34;
			break;
		case 'B': counts['C'] += 0.34;
			counts['G'] += 0.34;
			counts['T'] += 0.34;
			break;
		case '-': throw Exception("StringSequenceTools::getGCContent : Gap found in sequence");
			break;
		// Unresolved bases
		default: counts['A'] += 0.25;
			counts['C'] += 0.25;
			counts['G'] += 0.25;
			counts['T'] += 0.25;
		}
	}

	// Calculate and send GC rate
	return (counts['G'] + counts['C']) / static_cast<double>(window);
}

/****************************************************************************************/

vector<int> StringSequenceTools::codeSequence(const string& sequence, std::shared_ptr<const Alphabet>& alphabet)
{
	unsigned int size = AlphabetTools::getAlphabetCodingSize(*alphabet); // Warning,
	                                                                     // an
	                                                                     // exception
	                                                                     // may
	                                                                     // be
	                                                                     // casted
	                                                                     // here!
	vector<int> code(static_cast<size_t>(floor(static_cast<double>(sequence.size()) / static_cast<double>(size))));
	size_t pos = 0;
	size_t count = 0;
	while (pos + size <= sequence.size())
	{
		code[count] = alphabet->charToInt(sequence.substr(pos, size));
		count++;
		pos += size;
	}
	return code;
}

/****************************************************************************************/

string StringSequenceTools::decodeSequence(const vector<int>& sequence, std::shared_ptr<const Alphabet>& alphabet)
{
	string result = "";
	for (auto i : sequence)
	{
		result += alphabet->intToChar(i);
	}
	return result;
}

/****************************************************************************************/

std::shared_ptr<const Alphabet> StringSequenceTools::getAlphabetFromSequence(const std::string& sequence)
{
	// empty sequence test
	if (sequence.size() == 0)
	{
		throw Exception("Sequence::getAlphabetFromSequence : Empty sequence string");
	}

	// initialisation
	bool p = false; // indicates that a protein specific character is found
	bool r = false; // indicates that a RNA specific character is found
	bool u = false; // indicates that an unknown character is found
	bool pd = false; // Protein or DNA (T)

	// Main loop : for all character in sequence
	for (auto i : sequence)
	{
		// Character analyse
		switch (AlphabetTools::getType(i))
		{
		case 0: u = true; break;
		case 3: p = true; break;
		case 2: r = true; break;
		case 5: pd = true; break;
		}
	}

	if (u)
		throw Exception("Sequence::getAlphabetFromSequence : Unknown character detected in specified sequence");
	if (r && pd)
		throw Exception("Sequence::getAlphabetFromSequence : Both 'T' and 'U' in the same sequence!");
	if (r && p)
		throw Exception("Sequence::getAlphabetFromSequence : Protein character and 'U' in the same sequence!");
	if (p)
		return AlphabetTools::PROTEIN_ALPHABET;
	if (r)
		return AlphabetTools::RNA_ALPHABET;
	return AlphabetTools::DNA_ALPHABET;
}

/****************************************************************************************/
