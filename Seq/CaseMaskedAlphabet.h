//
// File: CaseMaskedAlphabet.h
// Created by: Julien Dutheil
// Created on: Sun Sep 05 2010
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

#ifndef _CASEMASKEDALPHABET_H_
#define _CASEMASKEDALPHABET_H_

#include "LetterAlphabet.h"

//From the STL:
#include <string>

namespace bpp
{

/**
 * @brief Case-sensitive letter alphabet.
 *
 * This alphabet is used for parsing comodity.
 * It takes as input another LetterAlphabet and will create duplicate any aphanumerical and upper case state,
 * by creating a lower case version of the state, also named "masked" state.
 * Helper functions are provided to determine whether a given state is masked or unmasked.
 */
class CaseMaskedAlphabet :
  public LetterAlphabet
{
  public:
    const LetterAlphabet* nocaseAlphabet_;

  public:
    CaseMaskedAlphabet(const LetterAlphabet* nocaseAlphabet);
    CaseMaskedAlphabet(const CaseMaskedAlphabet& cma) :
      LetterAlphabet(cma), nocaseAlphabet_(cma.nocaseAlphabet_) {}
    CaseMaskedAlphabet& operator=(const CaseMaskedAlphabet& cma) {
      LetterAlphabet::operator=(cma);
      nocaseAlphabet_ = cma.nocaseAlphabet_;
      return *this;
    }

  public:
    bool isMasked(int state) const { return state >= 100; }
    bool isMasked(const std::string& state) const {
      char c = state.c_str()[0];
      return isalpha(c) && !isupper(c);
    }

};

} // end of namespace bpp

#endif //_CASEMASKEDALPHABET_H_

