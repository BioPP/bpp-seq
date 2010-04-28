//
// File: MafAlignmentParser.cpp
// Authors: Julien Dutheil
// Created: Tue Apr 27 2010
//

/*
Copyright or © or Copr. Bio++ Development Team, (2010)

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

#include "MafAlignmentParser.h"
#include <Utils/KeyvalTools.h>
#include <Utils/TextTools.h>

using namespace std;
using namespace bpp;

MafBlock* MafAlignmentParser::nextBlock() throw (Exception)
{
  MafBlock* block = 0;

  string line;
  bool test = true;
  while (test)
  {
    getline(*stream_, line, '\n');
    if (TextTools::isEmpty(line))
    {
      //end of paragraph
      test = false;
    }
    else if (line[0] == 'a')
    {
      //New block.
      block = new MafBlock();

      map<string, string> args;
      if (line.size() > 2)
      {
        KeyvalTools::multipleKeyvals(line.substr(3), args, " ");

        if (args.find("score") != args.end())
          block->setScore(TextTools::toDouble(args["score"]));
    
        if (args.find("pass") != args.end())
          block->setPass(TextTools::toDouble(args["pass"]));
      }
    }
    else if (line[0] == 's')
    {
      StringTokenizer st(line);
      st.nextToken(); //The 's' tag
      string name = st.nextToken();
      unsigned int start = TextTools::to<unsigned int>(st.nextToken());
      unsigned int size = TextTools::to<unsigned int>(st.nextToken());
      string tmp = st.nextToken();
      if (tmp.size() != 1) throw Exception("MafAlignmentParser::nextBlock. Strand specification is incorrect, should be only one character long, found " + TextTools::toString(tmp.size()) + ".");
      char strand = tmp[0];

      unsigned int srcSize = TextTools::to<unsigned int>(st.nextToken());
      MafSequence sequence(name, st.nextToken(), start, strand, srcSize);
      if (sequence.getGenomicSize() != size)
        throw Exception("MafAlignmentParser::nextBlock. sequence found does not match specified size: " + TextTools::toString(sequence.getGenomicSize()) + ", should be " + TextTools::toString(size) + ".");
      block->addSequence(sequence);
    }
  }
  return block;
}

