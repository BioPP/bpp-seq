// 
// File:    Fastq.cpp
// Author:  Sylvain Gaillard
// Created: 22/11/2011 09:56:29
// 

/*
Copyright or © or Copr. Bio++ Development Team, (November 22, 2011)

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

#include "Fastq.h"
#include "../SequenceWithQuality.h"

#include <typeinfo>

using namespace bpp;

bool Fastq::nextSequence(std::istream& input, Sequence& seq) const throw (Exception) {
  if (input && ! input.eof()) {
    //SequenceWithQuality& sq;
    std::string buffer;
    while (TextTools::isEmpty(buffer) && !input.eof()) {
      getline(input, buffer);
    }
    if (input.eof()) { // We hit the end of the file
      return false;
    }
    // first line: seq name
    if (buffer[0] == '@') {
      seq.setName(std::string(buffer.begin() + 1, buffer.end()));
    }
    // second line: seq
    getline(input, buffer);
    seq.setContent(buffer);
    // third line: seq name (again)
    getline(input, buffer);
    if (repeatName()) {
      std::string secName = std::string(buffer.begin() + 1, buffer.end());
      if (secName != seq.getName()) {
        throw Exception("Names are not equivalent for sequence(@ line) and quality (+ line)");
      }
    }
    // fourth line: quality
    getline(input, buffer);
    try {
      SequenceWithQuality& sq = dynamic_cast<SequenceWithQuality&>(seq);
      for (size_t i = 0 ; i < buffer.size() ; i++) {
        sq.setQuality(i, static_cast<int>(buffer[i]));
      }
    } catch (...) {
    }
    return true;
  }
  return false;
}

/******************************************************************************/

void Fastq::writeSequence(std::ostream& output, const bpp::Sequence& seq) const throw (Exception) {
  std::string qual(seq.size(), static_cast<char>(33));
  try {
    const SequenceWithQuality& sq = dynamic_cast<const SequenceWithQuality&>(seq);
    for (size_t i = 0 ; i < sq.size() ; i++) {
      int q = sq.getQuality(i);
      if (q < 33 || q > 126) {
        throw BadIntegerException("Quality must lie between 33 and 126", q);
      }
      qual[i] = q;
    }
  } catch (const std::bad_cast& e) {
    throw Exception("seq must be a SequenceWithQuality object");
  }
  output << "@" << seq.getName() << std::endl;
  output << seq.toString() << std::endl;
  output << "+";
  if (repeatName()) {
    output << seq.getName();
  }
  output << std::endl;
  output << qual << std::endl;
}
