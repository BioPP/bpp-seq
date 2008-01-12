//
// File: AAIndex2Entry.cpp
// Created by: Julien Dutheil
// Created on: Fri Jan 19 17:07 2007
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

#include "AAIndex2Entry.h"

using namespace bpp;

//From Utils:
#include <Utils/FileTools.h>
#include <Utils/TextTools.h>
#include <Utils/StringTokenizer.h>

AAIndex2Entry::AAIndex2Entry(istream & input, bool sym) throw (IOException)
{
  _alpha = new ProteicAlphabet();
  _property.resize(20, 20);

  //Parse entry:
  string line;
  bool ok = false;
  bool diag = false;
  do
  {
    line = FileTools::getNextLine(input);
    if(line[0] == 'M')
    {
      for(unsigned int i = 0; i < 20; i++)
      {
        line = FileTools::getNextLine(input);
        StringTokenizer st1(line, " ");
        if(i == 0 && st1.numberOfRemainingTokens() == 1)
        {
          //Lower triangle only:
          diag = true;
        }
        //Amino acids are in the same order in the AAIndex1 database than in the ProteicAlphabet class:
        if(diag)
        {
          if(st1.numberOfRemainingTokens() != (int)(i+1)) break;
          for(unsigned int j = 0; j <= i; j++)
            _property(i,j) = TextTools::toDouble(st1.nextToken());
        }
        else
        {
          if(st1.numberOfRemainingTokens() != 20) break;
          for(unsigned int j = 0; j < 20; j++)
            _property(i,j) = TextTools::toDouble(st1.nextToken());
        }
      }
      //Jump to next entry...
      FileTools::getNextLine(input);
      ok = true;
    }
  }
  while(!ok);
  if(!ok) throw IOException("AAIndex2Entry: invalid AAIndex2 entry.");
}

