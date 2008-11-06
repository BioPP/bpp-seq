//
// File: PhredPhd.cpp
// Created by: Sylvain Gaillard
// Created on: Wed Nov 5 2008
//

/*
Copyright or © or Copr. CNRS, (November 5, 2008)

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

#include "PhredPhd.h"

// From Utils:
#include <Utils/TextTools.h>
#include <Utils/StringTokenizer.h>

using namespace bpp;

/******************************************************************************/

PhredPhd::PhredPhd(double quality, unsigned int lframe, unsigned int rframe) : _quality(quality), _lframe(lframe), _rframe(rframe) {}

/******************************************************************************/

void PhredPhd::appendFromStream(istream & input, VectorSequenceContainer & vsc) const throw (Exception) {
  if (!input) { throw IOException ("PhredPhd::read: fail to open stream"); }

  string temp, name, sequence = "";  // Initialization
  vector<double> q;

  const Alphabet * alpha = vsc.getAlphabet();
  // Read sequence info
  // Main loop : for all lines
  while (!input.eof()) {
    getline(input, temp, '\n');  // Copy current line in temporary string
    StringTokenizer st(temp, " ");
    if (st.hasMoreToken()) {
      if (st.getToken(0) == "BEGIN_SEQUENCE") {
        name = st.getToken(1);
      }
      string flag = st.getToken(0);
      while (flag != "END_SEQUENCE" && !input.eof()) {
        getline(input, temp, '\n');
        StringTokenizer st2(temp, " ");
        if (st2.hasMoreToken()) {
          flag = st2.getToken(0);
        }
        if (flag == "BEGIN_DNA") {
          while (flag != "END_DNA" && !input.eof()) {
            getline(input, temp, '\n');
            StringTokenizer st3(temp, " ");
            if (st3.hasMoreToken()) {
              flag = st3.getToken(0);
              if (alpha->isCharInAlphabet(flag) && st3.numberOfRemainingTokens() == 3) {
                sequence += flag;
                q.push_back(TextTools::toInt(st3.getToken(1)));
              }
            }
          }
        }
      }
    }
  }
  // Sequence creation
  if(name == "") throw Exception("PhredPhd::read: sequence without name!");
  Sequence * seq = new Sequence(name, sequence, alpha);
  // Filter sequence
  for (unsigned int i = 0 ; i < sequence.size() ; ++i) {
    double lq = 0.;
    unsigned int lb, hb;
    if (i < _lframe ) {
      lb = 0;
    } else {
      lb = i - _lframe;
    }
    if (i + _rframe >= sequence.size()) {
      hb = sequence.size() - 1;
    } else {
      hb = i + _rframe;
    }
    for (unsigned int j = lb ; j <= hb ; ++j) {
      lq += q[j];
    }
    lq = lq / (hb - lb + 1);
    if (lq < _quality) {
      seq->setElement(i, "N");
    }
  }
  // Adding sequence to container
  vsc.addSequence(* seq);
}

/******************************************************************************/
