//
// File: Fasta.cpp
// Authors: Guillaume Deuchst
//          Julien Dutheil
//          Sylvain Gaillard
// Created: Tue Aug 21 2003
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
#include <Utils/TextTools.h>
#include <Utils/FileTools.h>
#include <Utils/StringTokenizer.h>

using namespace bpp;

/******************************************************************************/

void Fasta::nextSequence(istream& input, Sequence& seq) const throw (Exception) {
  if (!input) {
    throw IOException("Fasta::nextSequence: can't read from istream input");
  }
  string seqname ="";
  Comments seqcmts;
  char c = '\n';
  char last_c;
  short seqcpt = 0;
  bool inseq = false;
  bool inseqname = false;
  seq.setContent("");
  while (!input.eof()) {
    last_c = c;
    input.get(c);
    // Sequence begining detection
    if (c == '>' && last_c == '\n') {
      seqcpt++;
      inseqname = true;
      inseq = true;
      // Stop if find a new sequence
      if (seqcpt > 1) {
        input.putback(c);
        break;
      }
      continue;
    }
    // Sequence name end line detection and new line suppression
    if (c == '\n') {
      inseqname = false;
      continue;
    }
    // Get the sequence name line
    if (inseqname) {
      seqname.append(1, c);
      continue;
    }
    // Suppress spaces in sequence
    if (c == ' ') {
      continue;
    }
    // Sequence content
    if (inseq && ! inseqname) {
      seq.append(TextTools::toString(c));
    }
  }
  // Sequence name and comments isolation
  if (extended_) {
    StringTokenizer * st = new StringTokenizer(seqname, " \\", true, false);
    seqname = st->nextToken();
    while (st->hasMoreToken()) {
      seqcmts.push_back(st->nextToken());
    }
    delete st;
    seq.setComments(seqcmts);
  }
  seq.setName(seqname);
}

/******************************************************************************/

void Fasta::writeSequence(ostream& output, const Sequence& seq) const throw (Exception) {
  if (!output) {
    throw IOException("Fasta::writeSequence: can't write to ostream output");
  }
  output << ">" << seq.getName();
  if (extended_) {
    for (unsigned int i = 0 ; i < seq.getComments().size() ; i++) {
      output << " \\" << seq.getComments()[i];
    }
  }
  output << endl;
  for (unsigned int i = 0 ; i < seq.size() ; i++) {
    output << seq.getChar(i);
    if ((i && ((i + 1) % charsByLine_) == 0) || i + 1 == seq.size()) {
      output << endl;
    }
  }
}


/******************************************************************************/

void Fasta::appendFromStream(istream & input, VectorSequenceContainer & vsc) const throw (Exception)
{
  if (!input) {
    throw IOException("Fasta::appendFromStream: can't read from istream input");
  }
  char c = '\n';
  char last_c;
  bool header = false;
  string line = "";
  Comments cmts;
  while (!input.eof()) {
    last_c = c;
    input.get(c);
    // Header detection
    if (extended_ && c == '#') {
      header = true;
      continue;
    }
    // Header end detection
    if (c == '\n') {
      if (extended_ && header) {
        if (line[0] == '\\') {
          line.erase(line.begin());
          cmts.push_back(line);
        }
        line = "";
        header = false;
      }
      continue;
    }
    // Header capture
    if (header) {
      line.append(1, c);
    }
    // Sequence detection
    if (c == '>' && last_c == '\n') {
      input.putback(c);
      c = last_c;
      Sequence tmpseq("", "", vsc.getAlphabet());
      nextSequence(input, tmpseq);
      vsc.addSequence(tmpseq, checkNames_);
    }
  }
  if (extended_ && cmts.size()) {
    vsc.setGeneralComments(cmts);
  }
}

/******************************************************************************/

void Fasta::write(ostream & output, const SequenceContainer & sc) const throw (Exception) {
	if (!output) {
    throw IOException("Fasta::write: can't write to ostream output");
  }

  if (extended_) {
    // Loop for all general comments
    for (unsigned int i = 0 ; i < sc.getGeneralComments().size() ; i++) {
      output << "#\\" << sc.getGeneralComments()[i] << endl;
    }
    output << endl;
  }

	// Main loop : for all sequences in vector container
	vector<string> names = sc.getSequencesNames();
	for (unsigned int i = 0; i < names.size(); i ++) {
    writeSequence(output, sc.getSequence(names[i]));
	}
}

/******************************************************************************/

