//
// File: YeastbrateMitochondrialCodonAlphabet.cpp
// Created by: Benoit Nabholz
// Created on: Sun Oct 10 14:33 CET 2010
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

#include "YeastMitochondrialCodonAlphabet.h"

using namespace bpp;
using namespace std;

YeastMitochondrialCodonAlphabet::YeastMitochondrialCodonAlphabet(const NucleicAlphabet * alpha) : CodonAlphabet(alpha)
{
  string A = alpha->intToChar(0);
  string G = alpha->intToChar(2);
  string T = alpha->intToChar(3);
                            
  vector<string> vstop;
  
  vstop.push_back(T + A + A);
  vstop.push_back(T + A + G);

  int istop;
  unsigned int j;
  for (unsigned int i = 0 ; i < vstop.size() ; i++) {
    istop = charToInt(vstop[i]);
    stopCodons_.push_back(istop);
          
    j = 0;
    while (j < getNumberOfChars()) {
      if (getStateAt(j).getNum() == istop) {
        getStateAt(j).setName(STOP);
        break;
      }
      j++;
    }
  }
}

