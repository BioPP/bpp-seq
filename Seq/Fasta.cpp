/*
 * File Fasta.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 21 2003
*/

#include "Fasta.h"

#include "StringSequenceTools.h"

/** Class constructor *******************************************************************/

Fasta::Fasta(unsigned int charsByLine): _charsByLine(charsByLine) {}

/** Class destructor ********************************************************************/

Fasta::~Fasta() {}

/****************************************************************************************/

// Method to read a fasta file, with creation of a vector type sequences container
/*
const VectorSequenceContainer * Fasta::read(const string & path, const Alphabet * alpha) const throw(Exception)
{	
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Fasta::read : fail to open file"); }

	// Initialization
	vector<const Sequence *> vs;
	string temp, name, sequence = "";
	
	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n'); // Copy current line in temporary string
		
		// If first character is >
		if (temp[0] == '>') {
			// If a name and a sequence were founded
			if ((name != "") && (sequence != "")) {
				// New sequence addition in temporary vector of sequences
				vs.push_back(new Sequence(name, sequence, alpha));
				name = "";
				sequence = "";
			}
			// Sequence name isolation
			name = temp;
			name.erase(name.begin());  // Character > deletion
		} else sequence += temp;  // Sequence isolation
	}

	// Addition of the last sequence in file
	if ((name != "") && (sequence != "")) vs.push_back(new Sequence(name, sequence, alpha));

	file.close(); 

	// Send founded sequences (in new VectorSequenceContainer)
	VectorSequenceContainer * vsc = new VectorSequenceContainer(vs, alpha);
    for(unsigned int i = 0; i < vs.size(); i++) delete vs[i];
	return vsc;
}*/

/****************************************************************************************/

void Fasta::read(const string & path, VectorSequenceContainer & vsc) const throw (Exception)
{
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Fasta::read : fail to open file"); }

	string temp, name, sequence = "";  // Initialization

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is >
		if (temp[0] == '>') {
			// If a name and a sequence were founded
			if ((name != "") && (sequence != "")) {
				// New sequence creation, and addition in existing VectorSequenceContainer
				Sequence * seq = new Sequence(name, sequence, vsc.getAlphabet());
				vsc.addSequence(* seq);
				//name = ""; no need for that, no?
				sequence = "";
			}
			// Sequence name isolation
			name = temp;
			name.erase(name.begin());  // Character > deletion
		} else sequence += temp;  // Sequence isolation
	}
	
	// Addition of the last sequence in file
	if ((name != "") && (sequence != "")) {
		Sequence * seq = new Sequence(name, sequence, vsc.getAlphabet());
		vsc.addSequence(* seq);
	}

	file.close();
}

/****************************************************************************************/
/*
// Method to read one sequence from a fasta file by his number
Sequence * Fasta::readSequence(const string & path, unsigned int number, const Alphabet * alpha) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) {
		throw IOException ("Fasta::readSequence : fail to open file");
	}

	// Initialization
	unsigned int position = 0;
	string temp, name, sequence = "";

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is >
		if (temp[0] == '>') {
			// If a name and a sequence were founded
			if ((name != "") && (sequence != "")) {
				position++;
				// Sending searched sequence when found
				if (position == number) return new Sequence(name, sequence, alpha);
				name = "";
				sequence = "";
			}
			// Sequence name isolation
			name = temp;
			name.erase(name.begin());  // Character > deletion
		} else sequence += temp;  // Sequence isolation
	}
	
	// Sending last sequence in file if searched
	if ((name != "") && (sequence != "")) {
		position++;
		if (position == number) return new Sequence(name, sequence, alpha);
	}

	file.close();

	// If specified sequence number is upper then number of sequences in file, throw exception
	if (number > position) throw IOException ("readSequence : Specified sequence number doesn't exists");

	// If any reading error was met
	throw IOException ("Fasta::readSequence : File format error");
	return NULL;
}

// Method to read one sequence from a fasta file by his number and with an unknown alphabet
Sequence * Fasta::readSequence(const string & path, unsigned int number) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) {
		throw IOException ("Fasta::readSequence : fail to open file");
	}

	// Initialization
	unsigned int position = 0;
	string temp, name, sequence = "";

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is >
		if (temp[0] == '>') {
			// If a name and a sequence were founded
			if ((name != "") && (sequence != "")) {
				position++;
				// Sending searched sequence when found
				if (position == number) return new Sequence(name, sequence, StringSequenceTools::getAlphabetFromSequence(sequence));
				name = "";
				sequence = "";
			}
			// Sequence name isolation
			name = temp;
			name.erase(name.begin());  // Character > deletion
		} else sequence += temp;  // Sequence isolation
	}
	
	// Sending last sequence in file if searched
	if ((name != "") && (sequence != "")) {
		position++;
		if (position == number) return new Sequence(name, sequence, StringSequenceTools::getAlphabetFromSequence(sequence));
	}

	file.close();

	// If specified sequence number is upper then number of sequences in file, throw exception
	if (number > position) throw IOException ("readSequence : Specified sequence number doesn't exists");

	// If any reading error was met
	throw IOException ("Fasta::readSequence : File format error");
	return NULL;
}

// Method to read one sequence from a fasta file by his name
Sequence * Fasta::readSequence(const string & path, const string & name, const Alphabet * alpha) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) {
		throw IOException ("Fasta::readSequence : fail to open file");
	}

	string temp, currentname, sequence = "";  // Initialization

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is >
		if (temp[0] == '>') {
			// If a name and a sequence were founded
			if ((currentname != "") && (sequence != "")) {
				// Sending searched sequence when found
				if (currentname == name) return new Sequence(name, sequence, alpha);
				currentname = "";
				sequence = "";
			}
			// Sequence name isolation
			currentname = temp;
			currentname.erase(currentname.begin());  // Character > deletion
		} else sequence += temp;  // Sequence isolation
	}
	
	// Sending last sequence in file if searched
	if ((currentname != "") && (sequence != "")) {
		if (currentname == name) return new Sequence(name, sequence, alpha);
		else throw IOException ("readSequence : Specified name doesn't exists");
	}

	file.close();

	// If any reading error was met
	throw IOException ("Fasta::readSequence : File format error");
	return NULL;
}

// Method to read one sequence from a fasta file by his name and with an unknown alphabet
Sequence * Fasta::readSequence(const string & path, const string & name) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) {
		throw IOException ("Fasta::readSequence : fail to open file");
	}

	string temp, currentname, sequence = "";  // Initialization

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is >
		if (temp[0] == '>') {
			// If a name and a sequence were founded
			if ((currentname != "") && (sequence != "")) {
				// Sending searched sequence when found
				if (currentname == name) return new Sequence(name, sequence, StringSequenceTools::getAlphabetFromSequence(sequence));
				currentname = "";
				sequence = "";
			}
			// Sequence name isolation
			currentname = temp;
			currentname.erase(currentname.begin());  // Character > deletion
		} else sequence += temp;  // Sequence isolation
	}
	
	// Sending last sequence in file if searched
	if ((currentname != "") && (sequence != "")) {
		if (currentname == name) return new Sequence(name, sequence, StringSequenceTools::getAlphabetFromSequence(sequence));
		else throw IOException ("readSequence : Specified name doesn't exists");
	}

	file.close();

	// If any reading error was met
	throw IOException ("Fasta::readSequence : File format error");
	return NULL;
}

// Method to get number of sequence characters per line
// (return the size of first sequence line found in file)
int Fasta::getCharsByLine(const string & path) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Fasta::getCharsByLine : failed to open file"); }

	string temp, name, sequence = "";  // Initialization

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is >
		if (temp[0] == '>') {
			// If a name and a sequence were founded
			if ((name != "") && (sequence != "")) {
				file.close();
				return sequence.size();  // Send size
			}
			// Sequence name isolation
			name = temp;
		} else if (sequence == "") sequence = temp;  // Sequence first line isolation
	}

	file.close();

	// If any reading error was met
	throw IOException ("Fasta::getCharsByLine : File format error");
	return -1;
}

// Method to get number of sequences contained in specified file
int Fasta::getNumberOfSequences(const string & path) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Fasta::getNumberOfSequences : failed to open file"); }

	// Initialization
	int nb = 0;
	string temp;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is >, new sequence is detected
		if (temp[0] == '>') nb++;
	}

	file.close();

	// Send result
	return nb;
}
*/

/****************************************************************************************/

void Fasta::write(const string & path, const SequenceContainer & sc, bool overwrite) const throw (Exception)
{
	// Open file in specified mode
	ofstream file(path.c_str(), overwrite ? (ios::out) : (ios::out|ios::app));


	// Checking the existence of specified file, and possibility to open it in write mode
	if (! file) { throw IOException ("Fasta::write : failed to open file"); }

	string seq, temp = "";  // Initialization

	// Main loop : for all sequences in vector container
	vector<string> names = sc.getSequencesNames();
	for (unsigned int i = 0 ; i < names.size() ; i ++) {
		// Sequence's commentaries writing
		file << ">" << names[i] << endl;
		
		// Sequence cutting to specified characters number per line
		seq = sc.toString(names[i]);
		while (seq != "") {
			if (seq.size() > _charsByLine) {
				temp = seq;
				temp.erase(temp.begin() + _charsByLine , temp.end());
				file << temp  << endl;
				seq.erase(seq.begin(), seq.begin() + _charsByLine);
			} else {
				file << seq << endl;
				seq = "";
			}
		}
	}

	file.close();
}

/****************************************************************************************/

// Methods to write a sequence in fasta file
// Specified file will be created if not exists, and else the new sequence will be added at end of file
/*
void Fasta::write(const string & path, const Sequence * sequence, bool overwrite) const throw (Exception)
{

	// Open file in specified mode
	ofstream file(path.c_str(), overwrite ? (ios::out) : (ios::out|ios::app));

	// Checking the existence of specified file, and possibility to open it in write mode
	if (! file) { throw IOException ("Fasta::write : failed to open file"); }
	
	// Sequence's commentaries writing
	file << ">" << sequence -> getName() << endl;

	// Initialization
	string seq, temp = "";

	// Sequence cutting to specified characters number per line
	seq = sequence -> toString();
	while (seq != "") {
		if (seq.size() > _charsByLine) {
			temp = seq;
			temp.erase(temp.begin() + _charsByLine , temp.end());
			file << temp  << endl;
			seq.erase(seq.begin(), seq.begin() + _charsByLine);
		}
		else {
			file << seq << endl;
			seq = "";
		}
	}
	file.close();
}
*/

/****************************************************************************************/

const string Fasta::getFormatName() const { return "FASTA file"; }

/****************************************************************************************/

const string Fasta::getFormatDescription() const {
	return "Sequence name (preceded by >) in one line, sequence content, no comments";
}

/****************************************************************************************/
