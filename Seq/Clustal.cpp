
#include "Clustal.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/FileTools.h>
#include <Utils/StringTokenizer.h>

// From the STL:
#include <iostream>
#include <iomanip>
using namespace std;

void Clustal::read(istream & input, VectorSequenceContainer & sc) const throw (Exception)
{
	//!!! ClustalX add 6 spaces at longest name.

	// Checking the existence of specified file
	if (!input) { throw IOException ("Clustal::read : fail to open file"); }

  const Alphabet * alpha = sc.getAlphabet();
	vector<Sequence> sequences;

	string lineRead("");

	Comments comments(1);
	comments[0] = FileTools::getNextLine(input); // First line gives file generator.

  lineRead = FileTools::getNextLine(input); // This is the first sequence of the first block.
		
	unsigned int beginSeq = lineRead.find_last_of("      ") + 1;
	if(beginSeq == string::npos) throw IOException("Clustal::read. Bad intput file.");

	unsigned int countSequences = 0;

  //Read first sequences block:
	bool test = true;
	do {
		sequences.push_back(Sequence(TextTools::removeSurroundingWhiteSpaces(lineRead.substr(0, beginSeq - 6)), lineRead.substr(beginSeq), alpha));
		getline(input, lineRead, '\n');
		countSequences++;
		test = !TextTools::isEmpty(lineRead) && !TextTools::isEmpty(lineRead.substr(0, beginSeq - 6));
	} while(input && test);

	// Read other blocks
	lineRead = FileTools::getNextLine(input); // Read first sequence of next block.
	while(!TextTools::isEmpty(lineRead)) {
		// Read next block:
		for(unsigned int i = 0; i < countSequences; i++) {// Complete sequences
			if(TextTools::isEmpty(lineRead)) throw IOException("Clustal::read. Bad intput file.");
		 	sequences[i].append(lineRead.substr(beginSeq));
			getline(input, lineRead, '\n');
  	}
		//At this point, lineRead is the first line after the current block.
		lineRead = FileTools::getNextLine(input);
	}

	for(unsigned int i = 0; i < countSequences; i++) sc.addSequence(sequences[i]);
	sc.setGeneralComments(comments);
}

const string Clustal::getFormatName() const { return "Clustal"; }
const string Clustal::getFormatDescription() const { return "The Clustal alignment tool output format."; }
