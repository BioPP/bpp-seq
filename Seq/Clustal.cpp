
#include "Clustal.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/FileTools.h>
#include <Utils/StringTokenizer.h>

// From the STL:
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

void Clustal::read(const string & path, VectorSequenceContainer & sc) const throw (Exception)
{
	//!!! ClustalX add 6 spaces at longest name.

	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (!file) { throw IOException ("Clustal::read : fail to open file"); }

  const Alphabet * alpha = sc.getAlphabet();
	vector<Sequence> sequences;

	string lineRead("");

	Comments comments(1);
	comments[0] = FileTools::getNextLine(file); // First line gives file generator.

  lineRead = FileTools::getNextLine(file); // This is the first sequence of the first block.
		
	unsigned int beginSeq = lineRead.find_last_of("      ") + 1;
	if(beginSeq == string::npos) throw IOException("Clustal::read. Bad intput file.");

	unsigned int countSequences = 0;

  //Read first sequences block:
	bool test = true;
	do {
		sequences.push_back(Sequence(lineRead.substr(0, beginSeq - 6), lineRead.substr(beginSeq), alpha));
		getline(file, lineRead, '\n');
		countSequences++;
		test = !TextTools::isEmpty(lineRead) && !TextTools::isEmpty(lineRead.substr(0, beginSeq - 6));
	} while(file && test);

	// Read other blocks
	lineRead = FileTools::getNextLine(file); // Read first sequence of next block.
	while(lineRead != "") {
		// Read next block:
		for(unsigned int i = 0; i < countSequences; i++) {// Complete sequences
			sequences[i].append(lineRead.substr(beginSeq));
			getline(file, lineRead, '\n');
			if(TextTools::isEmpty(lineRead)) throw IOException("Clustal::read. Bad intput file.");
		}
		//lineRead = FileTools::getNextLine(file);
		// Drop consensus line if it exists:
		if(TextTools::isEmpty(lineRead.substr(0, beginSeq - 6)))
			lineRead = FileTools::getNextLine(file);
	}

	for(unsigned int i = 0; i < countSequences; i++) sc.addSequence(sequences[i]);
	sc.setGeneralComments(comments);
}

const string Clustal::getFormatName() const { return "Clustal"; }
const string Clustal::getFormatDescription() const { return "The Clustal alignment tool output format."; }
