//
// File: Fasta.cpp
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Tue Aug 21 2003
//

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

#include "Fasta.h"

#include "StringSequenceTools.h"

using namespace bpp;

/****************************************************************************************/

void Fasta::appendFromStream(istream & input, VectorSequenceContainer & vsc) const throw (Exception)
{
	if (!input) { throw IOException ("Fasta::read: fail to open file"); }

	string temp, name, sequence = "";  // Initialization

	// Main loop : for all file lines
	while(!input.eof())
  {
		getline(input, temp, '\n');  // Copy current line in temporary string

		// If first character is >
		if(temp[0] == '>')
    {
			// If a name and a sequence were foundez
			if((name != "") && (sequence != ""))
      {
				// New sequence creation, and addition in existing VectorSequenceContainer
				vsc.addSequence(Sequence(name, sequence, vsc.getAlphabet()), _checkNames);
				sequence = "";
			}
			// Sequence name isolation
			name = temp;
			name.erase(name.begin());  // Character > deletion
		}
    else sequence += temp;  // Sequence isolation
	}
	
	// Addition of the last sequence in file
	if((name != "") && (sequence != ""))
  {
		vsc.addSequence(Sequence(name, sequence, vsc.getAlphabet()), _checkNames);
	}
}

/****************************************************************************************/

void Fasta::write(ostream & output, const SequenceContainer & sc) const throw (Exception)
{
	// Checking the existence of specified file, and possibility to open it in write mode
	if (! output) { throw IOException ("Fasta::write: failed to open file"); }

	string seq, temp = "";  // Initialization

	// Main loop : for all sequences in vector container
	vector<string> names = sc.getSequencesNames();
	for (unsigned int i = 0; i < names.size(); i ++)
  {
    // Sequence's commentaries writing
		output << ">" << names[i] << endl;
		
		// Sequence cutting to specified characters number per line
		seq = sc.toString(names[i]);
		while (seq != "")
    {
			if (seq.size() > _charsByLine)
      {
				temp = seq;
				temp.erase(temp.begin() + _charsByLine , temp.end());
				output << temp  << endl;
				seq.erase(seq.begin(), seq.begin() + _charsByLine);
			}
      else
      {
				output << seq << endl;
				seq = "";
			}
		}
	}
}

/****************************************************************************************/

