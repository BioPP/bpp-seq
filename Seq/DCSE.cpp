/***************************************************************************
                          DCSE.cpp  -  description
                             -------------------
    begin                : mer mar 3 2004
    copyright            : (C) 2004 by Julien Dutheil
    email                : Julien.Dutheil@univ-montp2.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "DCSE.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/FileTools.h>
#include <Utils/StringTokenizer.h>

void DCSE::read(const string & path, VectorSequenceContainer & sc) const throw (Exception)
{
	// Checking the existence of specified file
	ifstream file (path.c_str(), ios::in);
	if (!file) { throw IOException ("DCSE::read : fail to open file"); }

	// Initialization
  const Alphabet * alpha = sc.getAlphabet();
	string line, name, sequence = "";

	line = FileTools::getNextLine(file); // Copy current line in temporary string
	StringTokenizer st(line);
	st.nextToken();
	//First line ignored for now!
	//int n1 = TextTools::toInt(st.nextToken());
	//int n2 = TextTools::toInt(st.nextToken());
	//int nbSites = n2 - n1
	//cout << nbSpecies << " species and " << nbSites << " sites." << endl;

	// Main loop : for all file lines
	while(!file.eof()) {
		line = FileTools::getNextLine(file); // Copy current line in temporary string
		if(line == "") break;
		unsigned int endOfSeq = line.find("     ");
		if(endOfSeq == line.npos) break;
		sequence = string(line.begin(), line.begin() + endOfSeq);
		sequence = TextTools::removeWhiteSpaces(sequence);
		sequence = TextTools::removeChar(sequence, '{');
		sequence = TextTools::removeChar(sequence, '}');
		sequence = TextTools::removeChar(sequence, '[');
		sequence = TextTools::removeChar(sequence, ']');
		sequence = TextTools::removeChar(sequence, '(');
		sequence = TextTools::removeChar(sequence, ')');
		sequence = TextTools::removeChar(sequence, '^');
		name     = string(line.begin() + endOfSeq + 1, line.end()),
		name     = TextTools::removeFirstWhiteSpaces(name);
		if(name.find("Helix numbering") == name.npos
		&& name.find("mask") == name.npos)
			sc.addSequence(Sequence(name, sequence, alpha));
	}
	file.close();
}

const string DCSE::getFormatName() const { return "DCSE"; }
const string DCSE::getFormatDescription() const { return "RNA structure format"; }
