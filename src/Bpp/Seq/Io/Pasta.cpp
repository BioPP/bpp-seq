//
// File: Pasta.cpp
// Authors: Murray Patterson
// Created: Tue Oct 20 2015
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#include "Pasta.h"

#include <fstream>

#include "../StringSequenceTools.h"
#include <Bpp/Text/TextTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Io/FileTools.h>

using namespace bpp;
using namespace std;

/********************************************************************************/

bool Pasta::nextSequence(istream & input, ProbabilisticSequence & seq, bool hasLabels, vector<int> & permutationMap) const
{
  if(!input)
    throw IOException("Pasta::nextSequence : can't read from istream input");

  string seqname = "";
  vector<double> tokens;
  Comments seqcmts;
  short seqcpt = 0;
  string linebuffer = "";
  char c;

  while(!input.eof()) {

    c = static_cast<char>(input.peek());
    if(input.eof())
      c = '\n';

    // detect the beginning of a sequence
    if(c == '>') {

      // stop if we find a new sequence
      if(seqcpt++)
	break;
    }

    getline(input, linebuffer);

    if(c == '>') {

      // get the sequence name line
      seqname = string(linebuffer.begin() + 1, linebuffer.end());
    }

    if(c != '>' && !TextTools::isWhiteSpaceCharacter(c)) {

      // sequence content : probabilities for each site are space-separated
      StringTokenizer st(linebuffer, " \t\n", false, false);
      while(st.hasMoreToken()) {
        double t;
        stringstream ss(st.nextToken());
        ss >> t;        
	tokens.push_back(t);
      }
    }
  }

  bool res = (!input.eof());

  // Sequence name and comments isolation (identical to that of Fasta)
  if(strictNames_ || extended_) {

    size_t pos = seqname.find_first_of(" \t\n");
    string seqcmt;

    if(pos != string::npos) {
      seqcmt = seqname.substr(pos + 1);
      seqname = seqname.substr(0, pos);
    }

    if(extended_) {
      StringTokenizer st(seqcmt, " \\", true, false);
      while(st.hasMoreToken()) {
	seqcmts.push_back(st.nextToken());
      }
    }
    else {
      seqcmts.push_back(seqcmt);
    }
    seq.setComments(seqcmts);
  }

  seq.setName(seqname);
  
  /* finally, deal with the content */

  // there is a header that specifies to which character each
  // probability is associated
  if(hasLabels) {
    DataTable content(permutationMap.size(),0);
    vector<double>::const_iterator i = tokens.begin();
    while(i != tokens.end()) {

      // junk up the tokens into groups of alphabetsize, and permute
      // according to how the header is permuted
      vector<double> row(permutationMap.size());
      for(vector<int>::const_iterator j = permutationMap.begin(); j != permutationMap.end(); ++j) {
	if(i == tokens.end())
	  throw Exception("Pasta::nextSequence : input is incomplete");
	row[*j] = *i;
      	++i;
      }

      content.addColumn(row);
    }

    // finally set the content
    seq.setContent(content.getData());
  }
  // o.w., we assume that each probability is that a (binary)
  // character is 1
  else {
    DataTable content(2,0);

    // fill in pairs of probabilities that (binary) character is 0,
    // resp. 1
    for(vector<double>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) {

      // the following will throw an exception if v[i] is not a properly
      // formatted double : a check that we want to have
      
      vector<double> pair_v{1.-*i,*i};
      content.addColumn(pair_v);
    }

    // finally, we set the content of the sequence to the above.
    // Since any number format exception was taken care of above, as
    // well as that each sequence must be of the same length by
    // construction of a DataTable object, the only thing left that
    // could go wrong is that p(0) + p(1) != 1 : a check that is done
    // in the call of the function below
    seq.setContent(content.getData());
  }

  return res;
}

/********************************************************************************/

void Pasta::appendSequencesFromStream(istream & input, VectorProbabilisticSiteContainer & container) const {

  if(!input)
    throw IOException("Pasta::appendFromStream: can't read from istream input");

  char c = '\n';
  char last_c;
  bool header = false;
  bool hasSeq = true;
  string line = "";
  Comments cmts;

  // labels for the states
  bool hasLabels = false;
  vector<string> labels;
  vector<int> permutationMap;

  while(!input.eof() && hasSeq) {

    last_c = c;
    input.get(c);

    // detect the header
    if(extended_ && c == '#') {
      header = true;
      continue;
    }

    // detect the end of the header
    if(c == '\n') {
      if(extended_ && header) {
	if(line[0] == '\\') {

	  line.erase(line.begin());
	  cmts.push_back(line);
	}
	line = "";
	header = false;
      }
      continue;
    }

    // capture the header
    if(header) {
      line.append(1,c);
    }

    // detect/get labels for the states
    if(!header && c != '>') {

      hasLabels = true;
      input.putback(c);
      c = last_c;

      getline(input, line);
      StringTokenizer st(line, " \t\n", false, false);

      while(st.hasMoreToken()) {
	string s = st.nextToken();
	labels.push_back(s);
      }

      /* check labels against alphabet of the container */
      vector<string> resolved_chars = container.getAlphabet()->getResolvedChars();
      string states = "<";
      for(vector<string>::const_iterator i = resolved_chars.begin(); i != resolved_chars.end(); ++i)
	states += " " + *i;
      states += " >";

      // check if size is the same
      if(labels.size() != resolved_chars.size())
	throw DimensionException("Pasta::appendSequencesFromStream. ", labels.size(), resolved_chars.size());

      // build permutation map on the content, error should one exist
      for(vector<string>::const_iterator i = labels.begin(); i != labels.end(); ++i) {
	bool found = false;

	for(int j = 0; j < int(resolved_chars.size()); ++j)
	  if(*i == resolved_chars[j]) {
	      permutationMap.push_back(j);
	      found = true;
	  }
	
	if(!found)
	  throw Exception("Pasta::appendSequencesFromStream. Label " + TextTools::toString(*i) + " is not found in alphabet " + TextTools::toString(states) + ".");
      }		
    }

    // detect the sequence
    if(c == '>' && last_c == '\n') {

      input.putback(c);
      c = last_c;
      shared_ptr<BasicProbabilisticSequence> tmpseq(new BasicProbabilisticSequence(container.getAlphabet())); // add probabilistic sequences instead
      hasSeq = nextSequence(input, *tmpseq, hasLabels, permutationMap);
      container.addSequence(tmpseq, checkNames_);
    }
  }
  if(extended_ && cmts.size()) {
    container.setGeneralComments(cmts);
  }
}


/********************************************************************************/

void Pasta::writeSequence(ostream & output, const ProbabilisticSequence & seq) const
{
  if(!output)
    throw IOException("Pasta::writeSequence : can't write to ostream output");

  // sequence name
  output << ">" << seq.getName();

  // sequence comments
  if(extended_) {
    for(unsigned int i = 0; i < seq.getComments().size(); ++i) {
      output << " \\" << seq.getComments()[i];
    }
  }
  output << endl;

  StlOutputStreamWrapper outs(&output);

  // sequence content
  const vector<vector<double> >& data = seq.getContent();
  
  for (auto i : data)
  {
    VectorTools::print(i,outs,"\t");
  }
}

/********************************************************************************/

void Pasta::writeSequence(ostream & output, const Sequence & seq) const
{
  if(!output)
    throw IOException("Pasta::writeSequence : can't write to ostream output");

  // sequence name
  output << ">" << seq.getName();

  // sequence comments
  if(extended_) {
    for(unsigned int i = 0; i < seq.getComments().size(); ++i) {
      output << " \\" << seq.getComments()[i];
    }
  }
  output << endl;

  // sequence content

  int nbc=(int)seq.getAlphabet()->getResolvedChars().size();
  
  for (size_t i=0; i<seq.size(); i++)
  {
    for (int s=0; s<nbc; s++)
    {
      output << seq.getStateValueAt(i,(int)s);
      output << "\t";
    }
    output << endl;
  }
}

void Pasta::writeAlignedValues(ostream& output, const AlignedValuesContainer& avc) const
{
  if (!output)
    throw IOException("Pasta::write: can't write to ostream output");

  // The alphabet

  vector<string> resolved_chars = avc.getAlphabet()->getResolvedChars();
  for (auto state : resolved_chars)
  {
    output << state << " ";
  }
  output << endl;

  
  // Main loop : for all sequences in vector container

  const AbstractProbabilisticSequenceContainer* vpsc=dynamic_cast<const AbstractProbabilisticSequenceContainer*>(&avc);

  if (vpsc)
    for (auto name : vpsc->getSequencesNames())
    {
      writeSequence(output, *vpsc->getSequence(name));
    }
  else
  {
    const SequenceContainer* sc=dynamic_cast<const SiteContainer*>(&avc);

    if (sc)
      for (auto name : sc->getSequencesNames())
      {
        writeSequence(output, sc->getSequence(name));
      }
  }
}
