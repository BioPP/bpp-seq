//
// File: StringSequenceTools.h
// Created by: Julien Dutheil
// Created on: Sun Nov 30 11:29:07 2003
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

#include "StringSequenceTools.h"

#include "AlphabetTools.h"
#include "DNA.h"
#include "RNA.h"
#include "ProteicAlphabet.h"

// From Utils:
#include <Utils/TextTools.h>

// From NumCalc:
#include <NumCalc/RandomTools.h>

// From the STL:
#include <map>
#include <ctype.h>
#include <algorithm>

/****************************************************************************************/

string StringSequenceTools::subseq(const string & sequence, int begin, int end) throw (Exception)
{
	// Checking interval
	if (end < begin) throw Exception ("StringSequenceTools::subseq: Invalid interval");

	// Copy sequence
	string temp(sequence);

	// Truncate sequence
	temp.erase(temp.begin() + end + 1, temp.end());
	temp.erase(temp.begin(), temp.begin() + begin);

	// Send result
	return temp;
}
	
/****************************************************************************************/

string StringSequenceTools::setToSizeR(const string & sequence, int size)
{
	return TextTools::resizeRight(sequence, size, '-');
}
	
string StringSequenceTools::setToSizeL(const string & sequence, int size)
{
	return TextTools::resizeLeft(sequence, size, '-');
}
	
/****************************************************************************************/

string StringSequenceTools::deleteChar(const string & sequence, char chars)
{
	// Copy sequence
	string result(sequence);

	// Search and delete specified char
	for (unsigned int i = 0 ; i < result.size() ; i++) { if (result[i] == chars) result.erase(result.begin()+i); }

	return result;
}

/****************************************************************************************/

string StringSequenceTools::deleteChar(const string & sequence, string chars)
{
	// Copy sequence
	string result(sequence);

	// For all characters to delete
	for (unsigned int i = 0 ; i < chars.size() ; i++) {
		// Search and delete char
		for (unsigned int j = 0 ; j < result.size() ; j++) { if (result[j] == chars[i]) result.erase(result.begin()+j); }
	}

	return result;
}

/****************************************************************************************/

bool StringSequenceTools::isPalindrome(const string & sequence)
{
	if (sequence == "") return false;

	// Initializing
	int size = sequence.size();

	// For all sequence characters or until two characters is unequals at opposite positions
	for (int i = 0 ; i < size ; i++) {
		if (sequence[i] != sequence[size-i-1]) return false;
	}

	return true;
}

/****************************************************************************************/

string * StringSequenceTools::reverse(const string & sequence)
{
	// Initializing
	string * result = new string;

	// Main loop : reverse all characters of sequence
	unsigned int size = sequence.size();
	for (unsigned int i = 0 ; i < size ; i++) { * result += sequence[size - i - 1]; }

	// Send result
	return result;
}

/****************************************************************************************/

string * StringSequenceTools::complement(const string & sequence)
{
	// Initializing
	string * result = new string;

	// Main loop : completement all characters
	unsigned int size = sequence.size();
	for (unsigned int i = 0 ; i < size ; i++) {
		switch (sequence[i]) {
			case 'A' : * result += 'T';
					   break;
			case 'C' : * result += 'G';
					   break;
			case 'G' : * result += 'C';
					   break;
			case 'T' : * result += 'A';
					   break;
			case 'M' : * result += 'K';
					   break;
			case 'R' : * result += 'Y';
					   break;
			case 'Y' : * result += 'R';
					   break;
			case 'K' : * result += 'M';
					   break;
			case 'V' : * result += 'B';
					   break;
			case 'H' : * result += 'D';
					   break;
			case 'D' : * result += 'H';
					   break;
			case 'B' : * result += 'V';
					   break;
			default  : * result += sequence[i];
					   break;
		}
	}

	// Send new sequence
	return result;
}

/****************************************************************************************/

double StringSequenceTools::getGCcontent(const string & sequence, unsigned int pos, unsigned int window) throw (BadIntegerException, Exception)
{
	// Frequency counts for nucleotids A, C, G, T
	map<char, double> counts;

	// Window size checking
	if (window <= 0) throw BadIntegerException("StringSequenceTools::getGCContent : window must be positive", window);
	if ((int)sequence.size() - (int)window < 0) throw BadIntegerException("StringSequenceTools::getGCContent : specified window too high", window);

	// For last nucleotides
	if (pos + window > sequence.size()) { pos = sequence.size() - window; }

	// Main loop
	for (unsigned int i = pos ; i < pos + window ; i++)
	{
		switch (toupper(sequence[i])) {
			case 'A' : counts['A'] += 1;
					   break;
			case 'C' : counts['C'] += 1;
					   break;
			case 'G' : counts['G'] += 1;
					   break;
			case 'T' : counts['T'] += 1;
					   break;
			case 'M' : counts['A'] += 0.5;
					   counts['C'] += 0.5;
					   break;
			case 'R' : counts['A'] += 0.5;
					   counts['G'] += 0.5;
					   break;
			case 'W' : counts['A'] += 0.5;
					   counts['T'] += 0.5;
					   break;
			case 'S' : counts['C'] += 0.5;
					   counts['G'] += 0.5;
					   break;
			case 'Y' : counts['C'] += 0.5;
					   counts['T'] += 0.5;
					   break;
			case 'K' : counts['G'] += 0.5;
					   counts['T'] += 0.5;
					   break;
			case 'V' : counts['A'] += 0.34;
					   counts['C'] += 0.34;
					   counts['G'] += 0.34;
					   break;
			case 'H' : counts['A'] += 0.34;
					   counts['C'] += 0.34;
					   counts['T'] += 0.34;
					   break;
			case 'D' : counts['A'] += 0.34;
					   counts['G'] += 0.34;
					   counts['T'] += 0.34;
					   break;
			case 'B' : counts['C'] += 0.34;
					   counts['G'] += 0.34;
					   counts['T'] += 0.34;
					   break;
			case '-' : throw Exception("StringSequenceTools::getGCContent : Gap found in sequence");
					   break;
			// Unresolved bases
			default  : counts['A'] += 0.25;
					   counts['C'] += 0.25;
					   counts['G'] += 0.25;
					   counts['T'] += 0.25;
		}
	}

	// Calculate and send GC rate
	return ((counts['G'] + counts['C']) / window);
}

/****************************************************************************************/

string StringSequenceTools::randomShuffle(const string & sequence, unsigned int window, bool throwIn)
{
	// Initializing
	unsigned int size = sequence.size();

	// Window size checking
	if (size < window) window = size;

	// Calculate number of Windows
	unsigned int nb = size / window;
	if (nb == 0) nb++;

	// Initializing
	vector<unsigned int> pos;
	unsigned int i;
	for (i = 0 ; i < nb ; i++) { pos.push_back(i); }
	if (size - (nb * window) > 0) pos.push_back(i);

	// Windows numbers random shuffle
	if (throwIn) {
		vector<unsigned int> swap;
		for (i = 0 ; i < pos.size() ; i++) 
			swap.push_back(RandomTools::giveIntRandomNumberBetweenZeroAndEntry(pos[pos.size()-1]));
		pos = swap;
	} else random_shuffle(pos.begin(), pos.end());

	// Windows content shuffle
	string temp, result = "";
	for (i = 0 ; i < pos.size() ; i++) {
		if ((window * pos[i]) + window > size) temp = string(sequence.begin() + (window * pos[i]), sequence.end());
		else temp = string(sequence.begin() + (window * pos[i]), sequence.begin() + (window * pos[i]) + window);
		result += temp;
	}

	// Send result
	return result;
}

/****************************************************************************************/

vector<int> StringSequenceTools::codeSequence(const string & sequence, const Alphabet * alphabet)
    throw (BadCharException)
{
	unsigned int size = AlphabetTools::getAlphabetCodingSize(alphabet); //Warning, an exception may be casted here!
	vector<int> code((int)floor((double)sequence.size() / (double)size));
	unsigned int pos = 0;
	unsigned int count = 0;
	while(pos + size <= sequence.size()) {
		code[count] = alphabet -> charToInt(sequence.substr(pos, size));
		count++;
		pos += size;
	}
	return code;
}

/****************************************************************************************/

string StringSequenceTools::decodeSequence(const vector<int> & sequence, const Alphabet * alphabet) throw (BadIntException)
{
	string result = "";
	for(unsigned int i = 0; i < sequence.size(); i++) {
		result += alphabet -> intToChar(sequence[i]);
	}
	return result;	
}

/****************************************************************************************/

Alphabet* StringSequenceTools::getAlphabetFromSequence(const string sequence)
throw (EmptySequenceException, SequenceException, AlphabetException)
{
	// empty sequence test
	if (sequence.size() == 0) {
		throw EmptySequenceException ("Sequence::getAlphabetFromSequence : Empty sequence string");
		return NULL;
	}

	// initialisation
	int nucleic = 0;	  // number of nucleic characters
	int proteic = 0;	  // number of proteic characters
	int unresolved = 0;   // number of unresolved characters (can be nucleic or proteic)
	bool tLetter = false; // boolean indicates that a T letter is found
	bool uLetter = false; // boolean indicates that the U nucleotid is found
	bool unknow = false;  // boolean indicates that a unknow character is found
	
	// Main loop : for all character in sequence
    for (unsigned int i = 0 ; i < sequence.size() ; i++) {
		// Character analyse
        switch (AlphabetTools::getType(sequence[i])) {
			// if character can't be nucleic or proteic
			case -1 : unknow = true;
					  break;
			// if character can't identify identify alphabet type (letters A, C, D, G, H, K, M, N, R, S, V, W, X, Y, -)
			case 0 : unresolved++;
					 break;
			// if character identify nucleic alphabet (letters B, O, ?, 0)
			case 1 : nucleic++;
					 break;
			// if character may identify DNA alphabet (letter T)
			case 2 : tLetter = true;
					 unresolved++;
					 break;
			// if character identify RNA alphabet (letter U)
			case 3 : uLetter = true;
					 nucleic++;
					 break;
			// if character identify proteic alphabet (letters E, F, I, L, P, Q)
			case 4 : proteic++;
					 break;
		}
	}

	if (unknow) {
		throw AlphabetException ("Sequence::getAlphabetFromSequence : Unknow character detected in specified sequence");
		return NULL;
	}

	if ((nucleic > 0) && (proteic == 0)) {
		if (tLetter && !uLetter) return new DNA();
		if (!tLetter && uLetter) return new RNA();
		if (tLetter && uLetter) {
			throw SequenceException ("Sequence::getAlphabetFromSequence : Confused sequence types found");
			return NULL;
		}
		
		// if (!tLetter && !uLetter) : heuristic method :
		// if more than 95% of sequence is nucleic or unresolved, sequence is probably DNA
		if (((nucleic + unresolved) / sequence.size()) > 0.95) return (new DNA());
	}		

	if ((nucleic == 0) && (proteic > 0)) {
		if (uLetter) {
			throw SequenceException ("getAlphabetFromSequence : Confused sequence types found");
			return NULL;
		}
		if (tLetter) return new ProteicAlphabet();
		
		// if (!tLetter && !uLetter) : heuristic method :
		// if more than 95% of sequence if proteic or unresolved, sequence if probably proteic
		if (((proteic + unresolved) / sequence.size()) > 0.95) return new ProteicAlphabet();
	}

	if ((nucleic == 0) && (proteic == 0) && tLetter) return new DNA();

	throw SequenceException ("Sequence::getAlphabetFromSequence : Unresolved sequence type");
	return NULL;
}

/****************************************************************************************/
