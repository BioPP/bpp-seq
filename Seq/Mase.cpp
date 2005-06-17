//
// File Mase.cpp
// Author : Guillaume Deuchst <GDeuchst@ifrance.com>
// Last modification : Tuesday August 21 2003
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

#include "Mase.h"
#include "StringSequenceTools.h"

Mase::Mase(unsigned int charsByLine): _charsByLine(charsByLine) {}

// Method to read a mase file, with creation of a vector type sequences container
/*
VectorSequenceContainer * Mase::read(const string & path, const Alphabet * alpha) const throw (Exception)
{
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Mase::read : fail to open file"); }
	
	// Initialization
	Comments seqComments, fileComments;
	vector<const Sequence *> vs;
	string temp, name, sequence = "";
	bool comments = false;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file, temp, '\n');  // Copy current line in temporary string
		//cout << temp << endl;
		
		// If first character is ;
		if (temp[0] == ';') {
			// If second character is also ;
			if (temp[1] == ';') {
				// File commentaries isolation
				temp.erase(0,2);  // Characters ;; deletion
				if (temp != "") fileComments.push_back(temp);
			} else {
				// If a name and a sequence were founded
				if ((name != "") && (sequence != "")) {
					// New sequence addition in temporary vector of sequences
					vs.push_back(new Sequence(name, sequence, seqComments, alpha));
					name = "";
					sequence = "";
					seqComments.clear();
				}
				// Sequence commentaries isolation
				temp.erase(temp.begin());  // Character ; deletion
				if (temp != "") seqComments.push_back(temp);
				comments = true;
			}
		} else {
			// If sequence commentaries were just isolated
			if (comments) {
				// Sequence name isolation
				name = temp;
				comments = false;
			} else sequence += temp;  // Sequence isolation
		}
	}

	// Addition of the last sequence in file
	if ((name != "") && (sequence != "")) vs.push_back(new Sequence(name, sequence, seqComments, alpha));

	file.close();


	// Send founded sequences (in new VectorSequenceContainer)
	VectorSequenceContainer * vsc = new VectorSequenceContainer(vs, alpha);
	vsc -> setGeneralComments(fileComments);
    for(unsigned int i = 0; i < vs.size(); i++) delete vs[i];
	return vsc;
} */

void Mase::read(istream & input, VectorSequenceContainer & vsc) const throw (Exception)
{
	if (!input) { throw IOException ("Mase::read : fail to open file"); }
	
	// Initialization
	Comments seqComments, fileComments;
	string temp, name, sequence = "";
	bool comments = false;

	// Get current general comments is VectorSequenceContainer
	fileComments = vsc.getGeneralComments();

	// Main loop : for all file lines
	while (!input.eof()) {
		getline(input, temp, '\n');  // Copy current line in temporary string
		
		// If first character is ;
		if (temp[0] == ';') {
			// If second character is also ;
			if (temp[1] == ';') {
				// File commentaries isolation
				temp.erase(0,2);  // Characters ;; deletion
				if (temp != "") fileComments.push_back(temp);
			} else {
				// If a name and a sequence were founded
				if ((name != "") && (sequence != "")) {
					// New sequence creation, and addition in existing VectorSequenceContainer
					Sequence * seq = new Sequence(name, sequence, seqComments, vsc.getAlphabet());
					vsc.addSequence(* seq);
					name = "";
					sequence = "";
					seqComments.clear();
				}
				
				// Sequence commentaries isolation
				temp.erase(temp.begin());  // Character ; deletion
				if (temp != "") seqComments.push_back(temp);
				comments = true;
			}
		} else {
			// If sequence commentaries were just isolated
			if (comments) {
				// Sequence name isolation
				name = temp;
				comments = false;
			} else sequence += temp;  // Sequence isolation
		}
	}
	
	// Addition of the last sequence in file
	if ((name != "") && (sequence != "")) {
		Sequence * seq = new Sequence(name, sequence, seqComments, vsc.getAlphabet());
		vsc.addSequence(* seq);
	}

	// Set new general comments in VectorSequenceContainer (old + new comments)
	vsc.setGeneralComments(fileComments);
}

// Method to read mase file containing many sequences containers
// (many groups of file commentaries (;;)
/*
vector<VectorSequenceContainer *> * Mase::multiContainersRead(const string & path, const Alphabet * alpha) const throw (Exception)
{
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
}
*/
/*
// Method to read one sequence from a fasta file by his number
Sequence * Mase::readSequence(const string & path, unsigned int number, const Alphabet * alpha) const throw (Exception)
{
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Mase::readSequence : fail to open file"); }
	
	// Initialization
	unsigned int position = 0;

	Comments seqComments;
	string temp, name, sequence = "";
	bool comments = false;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string
		
		// If first character is ;
		if (temp[0] == ';') {
			// If second character isn't also ;
			if (temp[1] != ';') {
				// If a name and a sequence were founded
				if ((name != "") && (sequence != "")) {
					// Sending searched sequence when found
					position++;
					if (position == number) return new Sequence(name, sequence, seqComments, alpha);
					name = "";
					sequence = "";
					seqComments.clear();
				}
				
				// Sequence commentaries isolation
				temp.erase(temp.begin());  // Character ; deletion
				if (temp != "") seqComments.push_back(temp);
				comments = true;
			}
		} else {
			// If sequence commentaries were just isolated
			if (comments) {
				// Sequence name isolation
				name = temp;
				comments = false;
			} else sequence += temp;  // Sequence isolation
		}
	}
	
	file.close();

	// Sending last sequence in file if searched
	if ((name != "") && (sequence != "")) {
		position++;
		if (position == number) return new Sequence(name, sequence, seqComments, alpha);
		else throw IOException ("Mase::readSequence : Specified sequence number doesn't exists");
	}

	// If any reading error was met
	throw IOException ("Mase::readSequence : File format error");
	return NULL;
}

// Method to read one sequence from a fasta file by his number and with an unknown alphabet
Sequence * Mase::readSequence(const string & path, unsigned int number) const throw (Exception)
{
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Mase::readSequence : fail to open file"); }
	
	// Initialization
	unsigned int position = 0;

	Comments seqComments;

	string temp, name, sequence = "";
	bool comments = false;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string
		
		// If first character is ;
		if (temp[0] == ';') {
			// If second character isn't also ;
			if (temp[1] != ';') {
				// If a name and a sequence were founded
				if ((name != "") && (sequence != "")) {
					// Sending searched sequence when found
					position++;
					if (position == number) return new Sequence(name, sequence, seqComments, StringSequenceTools::getAlphabetFromSequence(sequence));
					name = "";
					sequence = "";
					seqComments.clear();
				}
				
				// Sequence commentaries isolation
				temp.erase(temp.begin());  // Character ; deletion
				if (temp != "") seqComments.push_back(temp);
				comments = true;
			}
		} else {
			// If sequence commentaries were just isolated
			if (comments) {
				// Sequence name isolation
				name = temp;
				comments = false;
			} else sequence += temp;  // Sequence isolation
		}
	}
	

	file.close();	
	
	// Sending last sequence in file if searched
	if ((name != "") && (sequence != "")) {
		position++;
		if (position == number) return new Sequence(name, sequence, seqComments, StringSequenceTools::getAlphabetFromSequence(sequence));
		else throw IOException ("Mase::readSequence : Specified sequence number doesn't exists");
	}

	// If any reading error was met
	throw IOException ("Mase::readSequence : File format error");
	return NULL;
}
	
// Method to read one sequence from a fasta file by his name
Sequence * Mase::readSequence(const string & path, const string & name, const Alphabet * alpha) const throw (Exception)
{
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Mase::readSequence : fail to open file"); }
	
	// Initialization
	Comments seqComments;
	string temp, currentName, sequence = "";
	bool comments = false;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string
		
		// If first character is ;
		if (temp[0] == ';') {
			// If second character isn't also ;
			if (temp[1] != ';') {
				// If a name and a sequence were founded
				if ((currentName != "") && (sequence != "")) {
					// Sending searched sequence when found
					if (currentName == name) return new Sequence(name, sequence, seqComments, alpha);
					currentName = "";
					sequence = "";
					seqComments.clear();
				}
				
				// Sequence commentaries isolation
				temp.erase(temp.begin());  // Character ; deletion
				if (temp != "") seqComments.push_back(temp);
				comments = true;
			}
		} else {
			// If sequence commentaries were just isolated
			if (comments) {
				// Sequence name isolation
				currentName = temp;
				comments = false;
			} else sequence += temp;  // Sequence isolation
		}
	}
	
	// Sending last sequence in file if searched
	if ((currentName != "") && (sequence != "")) {
		if (currentName == name) return new Sequence(name, sequence, seqComments, alpha);
		else throw IOException ("Mase::readSequence : Specified sequence name doesn't exists");
	}

	file.close();

	// If any reading error was met
	throw IOException	("Mase::readSequence : File format error");
	return NULL;
}

// Method to read one sequence from a fasta file by his name and with an unknown alphabet
Sequence * Mase::readSequence(const string & path, const string & name) const throw (Exception)
{
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Mase::readSequence : fail to open file"); }
	
	// Initialization
	Comments seqComments;
	string temp, currentName, sequence = "";
	bool comments = false;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string
		
		// If first character is ;
		if (temp[0] == ';') {
			// If second character isn't also ;
			if (temp[1] != ';') {
				// If a name and a sequence were founded
				if ((currentName != "") && (sequence != "")) {
					// Sending searched sequence when found
					if (currentName == name) return new Sequence(name, sequence, seqComments, StringSequenceTools::getAlphabetFromSequence(sequence));
					currentName = "";
					sequence = "";
					seqComments.clear();
				}
				
				// Sequence commentaries isolation
				temp.erase(temp.begin());  // Character ; deletion
				if (temp != "") seqComments.push_back(temp);
				comments = true;
			}
		} else {
			// If sequence commentaries were just isolated
			if (comments) {
				// Sequence name isolation
				currentName = temp;
				comments = false;
			} else sequence += temp;  // Sequence isolation
		}
	}

	// Sending last sequence in file if searched
	if ((name != "") && (sequence != "")) {
		if (currentName == name) return new Sequence(name, sequence, seqComments, StringSequenceTools::getAlphabetFromSequence(sequence));
		else throw IOException ("Mase::readSequence : Specified sequence name doesn't exists");
	}

	file.close();

	// If any reading error was met
	throw IOException ("Mase::readSequence : File format error");
	return NULL;
}

// Method to get number of sequence characters per line
// (return the size of first sequence line found in file)
int Mase::getCharsByLine(const string & path) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Mase::getCharsByLine : failed to open file"); }

	// Initialization
	string temp, name, sequence = "";
	bool comments = false;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is ;
		if (temp[0] == ';') {
			// If second character isn't also ;
			if (temp[1] != ';') {
				// If a name and a sequence were founded
				if ((name != "") && (sequence != "")) {
					file.close();
					return sequence.size();
				}
				
				comments = true;
			}
		} else {
			// If sequence commentaries were just isolated
			if (comments) {
				// Sequence name isolation
				name = temp;
				comments = false;
			} else if (sequence == "") sequence = temp;
		}
	}

	file.close();

	// If any reading error was met
	throw IOException ("Mase::getCharsByLine : File format error");
	return -1;
}

// Method to get number of sequences contained in specified file
int Mase::getNumberOfSequences(const string & path) const throw (Exception) {
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (! file) { throw IOException ("Mase::getNumberOfSequences : failed to open file"); }

	// Initialization
	int nb = 0;
	string temp;
	bool comments = false;

	// Main loop : for all file lines
	while (! file.eof()) {
		getline(file,temp,'\n');  // Copy current line in temporary string

		// If first character is ;
		if (temp[0] == ';') {
			// If second character isn't also ;
			if (temp[1] != ';') comments = true;
		} else {
			// If sequence commentaries were juste detected
			if (comments) {
				nb++;
				comments = false;
			}
		}
	}

	file.close();

	return nb;
}

*/

void Mase::write(ostream & output, const SequenceContainer & sc) const throw (Exception)
{
	// Checking the existence of specified file, and possibility to open it in write mode
	if (!output) { throw IOException ("Mase::write : failed to open file"); }

	Comments comments = sc.getGeneralComments();

	// Writing all general commentaries in file
	for (unsigned int i = 0 ; i < comments.size() ; i++) {
		output << ";;" << comments[i] << endl;
	}

	string seq, temp = "";  // Initialization

	// Main loop : for all sequences
	vector<string> names = sc.getSequencesNames();
	for (unsigned int i = 0 ; i < names.size() ; i ++) {
		comments = sc.getComments(names[i]);

		// Writing all sequence commentaries in file
		// If no comments are associated with current sequence, an empy commentary line will be writed
		if (comments.size() == 0) {
			output << ";" << endl;
		} else {
			for (unsigned int j = 0 ; j < comments.size() ; j++) {
				output << ";" << comments[j] << endl;
			}
		}

		// Sequence name writing
		output << names[i] << endl;

		// Sequence cutting to specified characters number per line
		seq = sc.toString(names[i]);
		while (seq != "") {
			if (seq.size() > _charsByLine) {
				temp = seq;
				temp.erase(temp.begin() + _charsByLine , temp.end());
				output << temp  << endl;
				seq.erase(seq.begin(), seq.begin() + _charsByLine);
			}
			else {
				output << seq << endl;
				seq = "";
			}
		}
	}
}

// Methods to write a sequence in mase file
// Specified file will be created if not exists, and else the new sequence will be added at end of file
/*
void Mase::write(const string & path, const Sequence * sequence, bool overwrite) const throw (Exception)
{

	// Open file in specified mode
	ofstream file(path.c_str(), overwrite ? (ios::out) : (ios::out|ios::app));

	// Checking the existence of specified file, and possibility to open it in write mode
	if (! file) { throw IOException ("Mase::write : failed to open file"); }
	
	// Writing all sequence commentaries in file
	// If no comments are associated with current sequence, an empy commentary line will be writed
	Comments comments = sequence -> getComments();
	if (comments.size() == 0) {
			file << ";" << endl;
	} else {
		for (unsigned int i = 0 ; i < comments.size() ; i++) {
			file << ";" << comments[i] << endl;
		}
	}

	// Sequence name writing
	file << sequence -> getName() << endl;

	string seq = sequence -> toString();
	string temp;

	// Sequence cutting to specified characters number per line
	while (seq != "") {
		if (seq.size() > charsByLine) {
			temp = seq;
			temp.erase(temp.begin() + charsByLine , temp.end());
			file << temp  << endl;
			seq.erase(seq.begin(), seq.begin() + charsByLine);
		}
		else {
			file << seq << endl;
			seq = "";
		}
	}

	file.close();
} */

// Method to get name of mase file format
const string Mase::getFormatName() const { return "MASE file"; }

// Method to get description of mase file format
const string Mase::getFormatDescription() const {
	return "Optional file comments (preceeded by ;;), sequence comments (preceeded by ;), sequence name, sequence";
}
