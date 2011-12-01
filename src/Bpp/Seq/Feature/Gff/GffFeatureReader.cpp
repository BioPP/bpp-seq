//
// File: GffFeatureReader.cpp
// Created by: Julien Dutheil
// Created on: Mon Nov 21 2011
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

#include "GffFeatureReader.h"

//From bpp-core:
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>
#include <Bpp/Text/KeyvalTools.h>

//From the STL:
#include <string>
#include <iostream>

using namespace bpp;
using namespace std;

const std::string GffFeatureReader::GFF_PHASE;
const std::string GffFeatureReader::GFF_NAME;
const std::string GffFeatureReader::GFF_ALIAS;
const std::string GffFeatureReader::GFF_PARENT;
const std::string GffFeatureReader::GFF_TARGET;
const std::string GffFeatureReader::GFF_GAP;
const std::string GffFeatureReader::GFF_DERIVES_FROM;
const std::string GffFeatureReader::GFF_NOTE;
const std::string GffFeatureReader::GFF_DBXREF;
const std::string GffFeatureReader::GFF_ONTOLOGY_TERM;
const std::string GffFeatureReader::GFF_IS_CIRCULAR;


void GffFeatureReader::getNextLine_() {
  nextLine_ = "";
  while (TextTools::isEmpty(nextLine_) || nextLine_.size() < 2 || nextLine_[0] == '#') {
    if (input_->eof()) {
      nextLine_ = "";
      return;
    }
    getline(*input_, nextLine_);
  }
}

const BasicSequenceFeature GffFeatureReader::nextFeature() throw (Exception)
{
  if (!hasMoreFeature())
    throw Exception("GffFeatureReader::nextFeature(). No more feature in file.");
  
  //Parse current line:
  StringTokenizer st(nextLine_, "\t");
  if (st.numberOfRemainingTokens() != 9)
    throw Exception("GffFeatureReader::nextFeature(). Wrong GFF3 file format: should have 9 tab delimited columns.");
  
  //if ok, we can parse each column:
  string seqId       = st.nextToken();
  string source      = st.nextToken();
  string type        = st.nextToken();
  unsigned int start = TextTools::to<unsigned int>(st.nextToken()) - 1;
  unsigned int end   = TextTools::to<unsigned int>(st.nextToken());
  double score       = TextTools::to<double>(st.nextToken());
  string strand      = st.nextToken();
  string phase       = st.nextToken();
  string attrDesc    = st.nextToken();
  map<string, string> attributes;
  KeyvalTools::multipleKeyvals(attrDesc, attributes, ";", false);
  string id = attributes["ID"];
  BasicSequenceFeature feature(id, seqId, source, type, start, end, (strand == "-"), score);
  
  //Set phase attributes:
  if (phase != ".") feature.setAttribute("GFF_PHASE", phase);

  //now check additional attributes:
  for (map<string, string>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
    if (it->first != "ID")
      feature.setAttribute(it->first, it->second); //We accept all attributes, even if they are not standard.
  }
  
  //Read the next line:
  getNextLine_();

  return feature;
}

