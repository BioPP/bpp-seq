//
// File: Phylip.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Oct 27 12:22:56 2003
//

#include "Phylip.h"
#include "SequenceContainerTools.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/StringTokenizer.h>

// From the STL:
#include <sstream>

using namespace std;

Phylip::Phylip(bool extended, bool sequential, unsigned int charsByLine): _charsByLine(charsByLine), _extended(extended), _sequential(sequential)
{
	//cout << (this -> extended ? "extended" : "classical") << endl;
	//cout << (this -> sequential ? "sequential" : "interleaved") << endl;	
}


Phylip::~Phylip() {}

string Phylip::getNextNonEmptyLine(istream & in) {
	string temp = "";
	while(!in.eof() && TextTools::isEmpty(temp)) getline(in, temp, '\n');
	return temp;
} 

const vector<string> Phylip::splitNameAndSequence(const string & s) const {
	vector<string> v(2);
	if(_extended) {
		int index = s.find("  ");
		v[0] = TextTools::removeSurroundingWhiteSpaces(s.substr(0, index));
		v[1] = TextTools::removeFirstWhiteSpaces      (s.substr(index + 2)); //There may be more than 6 white spaces.
	} else {
		v[0] = TextTools::removeSurroundingWhiteSpaces(s.substr(0, 10));
		v[1] = s.substr(11);
	}
	return v;
}	

void Phylip::readSequential(istream & in, VectorSequenceContainer & vsc) const throw (Exception) {
	string temp;
	
	//Ignore first line:
	getline(in, temp, '\n');  // Copy current line in temporary string
	temp = getNextNonEmptyLine(in);
	
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
		vsc.addSequence(Sequence(name, seq, vsc.getAlphabet()), true);
		temp = getNextNonEmptyLine(in);
	}
}

void Phylip::readInterleaved(istream & in, VectorSequenceContainer & vsc) const throw (Exception)
 {
	string temp;
	
	//Read first line:
	getline(in, temp, '\n'); // Copy current line in temporary string
	StringTokenizer st(temp);
	int nbSequences = TextTools::toInt(st.nextToken());
	//int nbSites     = TextTools::toInt(st.nextToken());
	temp = getNextNonEmptyLine(in);
	
	vector<string> names, seqs;
	// Read first block:
	for(int i = 0; i < nbSequences && !in.eof() && !TextTools::isEmpty(temp); i++) {
		vector<string> v = splitNameAndSequence(temp);
		names.push_back(v[0]);
		seqs .push_back(v[1]);
	}
	
	//Then read all other blocks:
	temp = getNextNonEmptyLine(in);
	while(!in.eof()) {
		for(unsigned int i = 0; i < names.size(); i++) {
			if(TextTools::isEmpty(temp))
				throw IOException("Phylip::readInterleaved. Bad file,there are not the same number of sequence in each block.");
			getline(in, temp, '\n');  // read next line in file.
			seqs[i] += TextTools::removeWhiteSpaces(temp);			
		}
		temp = getNextNonEmptyLine(in);
	}
	for(unsigned int i = 0; i < names.size(); i++) {
		vsc.addSequence(Sequence(names[i], seqs[i], vsc.getAlphabet()));
	}
}
	
// Method to read a mase file, with filling of an existing vector type sequences container
void Phylip::read(const string & path, VectorSequenceContainer & vsc) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Phylip::read: fail to open file"); }
	
	if(_sequential) readSequential (file, vsc);
	else            readInterleaved(file, vsc);
		
	file.close();
}

/*
// Method to read mase file containing many sequences containers
// (many groups of file commentaries (;;)
const vector<VectorSequenceContainer *> * Phylip::multiContainersRead(const string & path, const Alphabet * alpha) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Mase::multiContainersRead : fail to open file"); }

	// Initialization
	vector<VectorSequenceContainer *> * vect;
	VectorSequenceContainer * vsc;
	vector<const Sequence *> vs;

	Comments seqComments, fileComments;
	string temp, name, sequence = "";
	bool fileComm, seqComm = false;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is ;
		if (temp[0] == ';') {
			// If second character is also ;
			if (temp[1] == ';') {
				// File commentaries isolation
				temp.erase(0,2);  // Characters ;; deletion
				if (temp != "") fileComments.push_back(temp);
				fileComm = true;
			} else {
				// If file commentaries was just isolated
				if (fileComm) {
					// Creation and addition in vector of a new VectorSequenceContainer
					vsc = new VectorSequenceContainer(vs, alpha);
					vsc -> setGeneralComments(fileComments);
					vect -> push_back(vsc);
					vs.clear();
					fileComm = false;
				}

				// If a name and a sequence were founded
				if ((name != "") && (sequence != "")) {
					// Addition in current temporary vector of sequence
					vs.push_back(new Sequence(name, sequence, seqComments, alpha));
					name = "";
					sequence = "";
					seqComments.clear();
				}
				
				// Sequence commentaries isolation
				temp.erase(temp.begin());  // Character ; deletion
				if (temp != "") seqComments.push_back(temp);
				seqComm = true;
			}
		} else {
			// If sequence commentaries were just isolated
			if (seqComm) {
				// Sequence name isolation
				name = temp;
				seqComm = false;
			} else sequence += temp;  // Sequence isolation
		}
	}

	// Addition of the last sequence in file
	if ((name != "") && (sequence != "")) vs.push_back(new Sequence(name, sequence, seqComments, alpha));

	// Addition in vector of the last VectorSequenceContainer filled
	if (vs.size() > 0) {
		vsc = new VectorSequenceContainer(vs, alpha);
		vsc -> setGeneralComments(fileComments);
		vect -> push_back(vsc);
	}

	file.close();

	return vect;
}*/

// Method to get number of sequences contained in specified file
int Phylip::getNumberOfSequences(const string & path) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Phylip::getNumberOfSequences: failed to open file"); }
	string firstLine = getNextNonEmptyLine(file);
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
// Methods to write a sequence container in phylip file
// Specified file will be created if not exists, and else the new sequences will be added at end of file
void Phylip::write(const string & path, const SequenceContainer & sc, bool overwrite) const throw (Exception) {
	//First must check if all sequences are aligned:
	if(sc.getNumberOfSequences() == 0)
		throw Exception("Phylip::write. SequenceContainer appear to contain no sequence.");
	
	if(!SequenceContainerTools::sequencesHaveTheSameLength(sc))
		throw SequenceNotAlignedException("Phylip::write. Sequences have to e of same length.", NULL);
	
	// Open file in specified mode
	ofstream file(path.c_str(), overwrite ? (ios::out) : (ios::out|ios::app));

	// Checking the existence of specified file, and possibility to open it in write mode
	if (! file) { throw IOException ("Phylip::write : failed to open file"); }

	if(_sequential) writeSequential (file, sc, _charsByLine);
	else            writeInterleaved(file, sc, _charsByLine);
		
	file.close();
}

// Method to get name of mase file format
const string Phylip::getFormatName() const { return "Phylip file, " + string(_extended ? "extended," : "") + string(_sequential ? "sequential" : "interleaved"); }

// Method to get description of mase file format
const string Phylip::getFormatDescription() const {
	return "Phylip file format, sequential and interleaved. PAML extension also supported.";
}
