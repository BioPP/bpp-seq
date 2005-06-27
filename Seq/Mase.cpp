//
// File Mase.cpp
// Author : Guillaume Deuchst
// Last modification : Tuesday August 21 2003
//

/*
Copyright ou � ou Copr. CNRS, (17 Novembre 2004) 

Julien.Dutheil@univ-montp2.fr

Ce logiciel est un programme informatique servant � fournir des classes
pour l'analyse de s�quences.

Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilit� au code source et des droits de copie,
de modification et de redistribution accord�s par cette licence, il n'est
offert aux utilisateurs qu'une garantie limit�e.  Pour les m�mes raisons,
seule une responsabilit� restreinte p�se sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les conc�dants successifs.

A cet �gard  l'attention de l'utilisateur est attir�e sur les risques
associ�s au chargement,  � l'utilisation,  � la modification et/ou au
d�veloppement et � la reproduction du logiciel par l'utilisateur �tant 
donn� sa sp�cificit� de logiciel libre, qui peut le rendre complexe � 
manipuler et qui le r�serve donc � des d�veloppeurs et des professionnels
avertis poss�dant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invit�s � charger  et  tester  l'ad�quation  du
logiciel � leurs besoins dans des conditions permettant d'assurer la
s�curit� de leurs syst�mes et ou de leurs donn�es et, plus g�n�ralement, 
� l'utiliser et l'exploiter dans les m�mes conditions de s�curit�. 

Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accept� les
termes.
*/

/*
Copyright or � or Copr. CNRS, (November 17, 2004)

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

void Mase::appendFromStream(istream & input, VectorSequenceContainer & vsc) const throw (Exception)
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

// Method to get name of mase file format
const string Mase::getFormatName() const { return "MASE file"; }

// Method to get description of mase file format
const string Mase::getFormatDescription() const {
	return "Optional file comments (preceeded by ;;), sequence comments (preceeded by ;), sequence name, sequence";
}

