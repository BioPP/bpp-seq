//
// File: GtfFeatureReader.cpp
// Created by: Sylvain Gaillard
// Created on: Fry Jan 27 2012
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

#include "GtfFeatureReader.h"

//From bpp-core:
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>
#include <Bpp/Text/KeyvalTools.h>

//From the STL:
#include <string>
#include <iostream>

using namespace bpp;
using namespace std;

const std::string GtfFeatureReader::GTF_PHASE = "GTF_PHASE";
const std::string GtfFeatureReader::GTF_GENE_ID = "gene_id";
const std::string GtfFeatureReader::GTF_TRANSCRIPT_ID = "transcript_id";

void GtfFeatureReader::getNextLine_() {
  nextLine_ = "";
  // Remove comments
  /*
    todo: remove characters between # and end-of-line taking care of double quotes
  */
  while (TextTools::isEmpty(nextLine_) || nextLine_.size() < 2 || nextLine_[0] == '#') {
    if (input_->eof()) {
      nextLine_ = "";
      return;
    }
    getline(*input_, nextLine_);
  }
}

const BasicSequenceFeature GtfFeatureReader::nextFeature() throw (Exception)
{
  if (!hasMoreFeature())
    throw Exception("GtfFeatureReader::nextFeature(). No more feature in file.");
  
  //Parse current line:
  StringTokenizer st(nextLine_, "\t");
  if (st.numberOfRemainingTokens() != 9)
    throw Exception("GtfFeatureReader::nextFeature(). Wrong GTF file format: should have 9 tab delimited columns.");
  
  //if ok, we can parse each column:
  std::string seqId       = st.nextToken();
  std::string source      = st.nextToken();
  std::string type        = st.nextToken();
  unsigned int start      = TextTools::to<unsigned int>(st.nextToken()) - 1;
  unsigned int end        = TextTools::to<unsigned int>(st.nextToken());
  double score            = TextTools::to<double>(st.nextToken());
  std::string strand      = st.nextToken();
  std::string phase       = st.nextToken();
  std::string attrDesc    = st.nextToken();
  std::map<string, string> attributes;
  bpp::StringTokenizer st1(attrDesc, ";");
  while (st1.hasMoreToken()) {
    std::string item(bpp::TextTools::removeSurroundingWhiteSpaces(st1.nextToken()));
    std::string::size_type idx = item.find_first_of(' ');
    std::string key(item.substr(0, idx));
    std::string value(item.substr(idx));
    // remove first "
    while (
        value.size() > 0 
        && (
          value[0] == '"' 
          || bpp::TextTools::isWhiteSpaceCharacter(value[0])
          )
        ) {
      value.erase(value.begin());
    }
    // remove last "
    while (
        value.size() > 0 
        && (
          value[value.size() - 1] == '"' 
          || bpp::TextTools::isWhiteSpaceCharacter(value[value.size() - 1])
          )
        ) {
      value.erase(value.end() - 1);
    }
    attributes[key] = value;
    //std::cout << "[" << key << "] = [" << value << "]" << std::endl;
  }
  //KeyvalTools::multipleKeyvals(attrDesc, attributes, ";", false);
  //std::string id = attributes["ID"];
  std::string id = "";
  BasicSequenceFeature feature(id, seqId, source, type, start, end, strand[0], score);
  
  //Set phase attributes:
  phase = bpp::TextTools::removeSurroundingWhiteSpaces(phase);
  if (phase != ".") feature.setAttribute(GTF_PHASE, phase);

  //now check additional attributes:
  for (std::map<std::string, std::string>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
    feature.setAttribute(it->first, it->second); //We accept all attributes, even if they are not standard.
    //std::cout << "[" << it->first << "] = [" << it->second << "]" << std::endl;
    //std::cout << "phase: " << phase << std::endl;
  }
  
  //Read the next line:
  getNextLine_();

  return feature;
}

