//
// File: Phylip.cpp
// Created by: Julien Dutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Oct 27 12:22:56 2003
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

#include "Phylip.h"
#include "SequenceContainerTools.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/StringTokenizer.h>
#include <Utils/FileTools.h>

// From the STL:
#include <sstream>

using namespace std;

/******************************************************************************/

Phylip::Phylip(bool extended, bool sequential, unsigned int charsByLine): _charsByLine(charsByLine), _extended(extended), _sequential(sequential)
{
	//cout << (this -> extended ? "extended" : "classical") << endl;
	//cout << (this -> sequential ? "sequential" : "interleaved") << endl;	
}


Phylip::~Phylip() {}

//Replaced by the corresponding FileTools method.
//string Phylip::getNextNonEmptyLine(istream & in) {
//	string temp = "";
//	while(!in.eof() && TextTools::isEmpty(temp)) getline(in, temp, '\n');
//	return temp;
//} 

/******************************************************************************/

const vector<string> Phylip::splitNameAndSequence(const string & s) const
{
	vector<string> v(2);
	if(_extended) {
		int index = s.find("  ");
		v[0] = TextTools::removeSurroundingWhiteSpaces(s.substr(0, index));
		v[1] = TextTools::removeFirstWhiteSpaces      (s.substr(index + 2)); //There may be more than 6 white spaces.
	} else {
		v[0] = TextTools::removeSurroundingWhiteSpaces(s.substr(0, 10));
		v[1] = s.substr(10);
	}
	return v;
}	

/******************************************************************************/

void Phylip::readSequential(istream & in, AlignedSequenceContainer & asc) const throw (Exception)
{
	string temp;
	
	//Ignore first line:
	getline(in, temp, '\n');  // Copy current line in temporary string
	temp = FileTools::getNextLine(in);
	
	while(!in.eof()) {
		// Read each sequence:
		vector<string> v = splitNameAndSequence(temp);
		string name = v[0];
		string seq  = v[1];
		while(!TextTools::isEmpty(temp)) {
			//Sequences are separated by at least one blank line:
			getline(in, temp, '\n');  // read next line in file.
			seq += TextTools::removeWhiteSpaces(temp);			
		}
		//end of this sequence:
		asc.addSequence(Sequence(name, seq, asc.getAlphabet()), true);
		temp = FileTools::getNextLine(in);
	}
}

/******************************************************************************/

void Phylip::readInterleaved(istream & in, AlignedSequenceContainer & asc) const throw (Exception)
 {
	string temp;
	
	//Read first line:
	getline(in, temp, '\n'); // Copy current line in temporary string
	StringTokenizer st(temp);
	int nbSequences = TextTools::toInt(st.nextToken());
	//int nbSites     = TextTools::toInt(st.nextToken());
	temp = FileTools::getNextLine(in);
	
	vector<string> names, seqs;
	// Read first block:
	for(int i = 0; i < nbSequences && !in.eof() && !TextTools::isEmpty(temp); i++) {
		vector<string> v = splitNameAndSequence(temp);
		names.push_back(v[0]);
		seqs.push_back(v[1]);
		getline(in, temp, '\n');  // read next line in file.
	}
	
	//Then read all other blocks:
	temp = FileTools::getNextLine(in);
	while(!in.eof()) {
		for(unsigned int i = 0; i < names.size(); i++) {
			if(TextTools::isEmpty(temp))
				throw IOException("Phylip::readInterleaved. Bad file,there are not the same number of sequence in each block.");
			seqs[i] += TextTools::removeWhiteSpaces(temp);			
			getline(in, temp, '\n');  // read next line in file.
		}
		temp = FileTools::getNextLine(in);
	}
	for(unsigned int i = 0; i < names.size(); i++) {
		asc.addSequence(Sequence(names[i], seqs[i], asc.getAlphabet()));
	}
}
	
/******************************************************************************/

void Phylip::appendFromStream(istream & input, AlignedSequenceContainer & vsc) const throw (Exception)
{
	// Checking the existence of specified file
	if (!input) { throw IOException ("Phylip::read: fail to open file"); }
	
	if(_sequential) readSequential (input, vsc);
	else            readInterleaved(input, vsc);
}

/******************************************************************************/

int Phylip::getNumberOfSequences(const string & path) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Phylip::getNumberOfSequences: failed to open file"); }
	string firstLine = FileTools::getNextLine(file);
	StringTokenizer st(firstLine, " \t");
	istringstream iss(st.nextToken());
	int nb;
	iss >> nb;
	return nb;
}
 
/******************************************************************************/

vector<string> Phylip::getSizedNames(const vector<string> & names) const {
	vector<string> sizedNames(names.size());
	if(_extended) {
		//Add 6 white spaces to the larger name and align other names.
		//First, determine the size of the wider name:
		unsigned int sizeMax = 0;
		for(unsigned int i = 0; i < names.size(); i++)
			if(names[i].size() > sizeMax) sizeMax = names[i].size();
		//Quite easy ;-) Now update all lengths:
		for(unsigned int i = 0; i < names.size(); i++) sizedNames[i] = TextTools::resizeRight(names[i], sizeMax + 2);
			
	} else {
		//We trunc all names to ten characters:
		for(unsigned int i = 0; i < names.size(); i++) sizedNames[i] = TextTools::resizeRight(names[i], 10);
		cout << "Warning: names have been truncated to 10 characters. They may be ambiguous sequence names then." << endl;
	}
	return sizedNames;
}

/******************************************************************************/

void Phylip::writeSequential(ostream & out, const SequenceContainer & sc, int charsByLine) const {
	//cout << "Write sequential" << endl;
	int numberOfSites = sc.getSequence(sc.getSequencesNames()[0]) -> size();
	out << sc.getNumberOfSequences() << " " << numberOfSites << " S" << endl;
	
	vector<string> seqNames = sc.getSequencesNames();
	vector<string> names = getSizedNames(seqNames);
	for(unsigned int i = 0; i < seqNames.size(); i++) {
		vector<string> seq = TextTools::split(sc.toString(seqNames[i]), charsByLine);
		out << names[i] << seq[0] << endl;
		for(unsigned int j = 1; j < seq.size(); j++) {
			out << string(names[i].size(), ' ') << seq[j] << endl;
		}
		out << endl;
	}
}

void Phylip::writeInterleaved(ostream & out, const SequenceContainer & sc, int charsByLine) const {
	//cout << "Write interleaved;" << endl;
	int numberOfSites = sc.getSequence(sc.getSequencesNames()[0]) -> size();
	out << sc.getNumberOfSequences() << " " << numberOfSites << " I" << endl;
	
	vector<string> seqNames = sc.getSequencesNames();
	vector<string> names = getSizedNames(seqNames);
	//Split sequences:
	vector< vector<string> > seqs(sc.getNumberOfSequences());
	for(unsigned int i = 0; i < seqNames.size(); i++) {
		seqs[i] = TextTools::split(sc.toString(seqNames[i]), charsByLine);
	}
	//Write first block:
	for(unsigned int i = 0; i < names.size(); i++) {
		out << names[i] << seqs[i][0] << endl;
	}
	out << endl;
	//Write other blocks:
	for(unsigned int j = 1; j < seqs[0].size(); j++) {
		for(unsigned int i = 0; i < sc.getNumberOfSequences(); i++) {
			out << seqs[i][j] << endl;
		}
		out << endl;
	}
}

/******************************************************************************/

void Phylip::write(ostream & output, const SequenceContainer & sc) const throw (Exception)
{
	//First must check if all sequences are aligned:
	if(sc.getNumberOfSequences() == 0)
		throw Exception("Phylip::write. SequenceContainer appear to contain no sequence.");
	
	if(!SequenceContainerTools::sequencesHaveTheSameLength(sc))
		throw SequenceNotAlignedException("Phylip::write. Sequences have to e of same length.", NULL);
	
	// Checking the existence of specified file, and possibility to open it in write mode
	if (!output) { throw IOException ("Phylip::write : failed to open file"); }

	if(_sequential) writeSequential (output, sc, _charsByLine);
	else            writeInterleaved(output, sc, _charsByLine);
}

/******************************************************************************/

const string Phylip::getFormatName() const { return "Phylip file, " + string(_extended ? "extended," : "") + string(_sequential ? "sequential" : "interleaved"); }

/******************************************************************************/

const string Phylip::getFormatDescription() const {
	return "Phylip file format, sequential and interleaved. PAML extension also supported.";
}

/******************************************************************************/

