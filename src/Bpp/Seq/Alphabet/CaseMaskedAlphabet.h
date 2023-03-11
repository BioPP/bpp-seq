//
// File: CaseMaskedAlphabet.h
// Authors:
//   Julien Dutheil
// Created: 2010-09-05 00:00:00
//

/*
  Copyright or Â© or Copr. CNRS, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_ALPHABET_CASEMASKEDALPHABET_H
#define BPP_SEQ_ALPHABET_CASEMASKEDALPHABET_H


#include "LetterAlphabet.h"

// From the STL:
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
  std::shared_ptr<const LetterAlphabet> nocaseAlphabet_;

public:
  CaseMaskedAlphabet(std::shared_ptr<const LetterAlphabet> nocaseAlphabet);

  CaseMaskedAlphabet(const CaseMaskedAlphabet& cma) :
    LetterAlphabet(cma), nocaseAlphabet_(cma.nocaseAlphabet_) {}

  CaseMaskedAlphabet& operator=(const CaseMaskedAlphabet& cma)
  {
    LetterAlphabet::operator=(cma);
    nocaseAlphabet_ = cma.nocaseAlphabet_;
    return *this;
  }

  CaseMaskedAlphabet* clone() const override
  {
    return new CaseMaskedAlphabet(*this);
  }

public:
  unsigned int getSize() const override { return nocaseAlphabet_->getSize(); }
  unsigned int getNumberOfTypes() const override { return nocaseAlphabet_->getNumberOfTypes(); }
  std::string getAlphabetType() const override { return "CaseMasked"; }
  int getUnknownCharacterCode() const override { return nocaseAlphabet_->getUnknownCharacterCode(); }
  bool isUnresolved(int state) const override { return nocaseAlphabet_->isUnresolved(state); }
  bool isUnresolved(const std::string& state) const override { return nocaseAlphabet_->isUnresolved(state); }

  std::shared_ptr<const Alphabet> getUnmaskedAlphabet() const { return nocaseAlphabet_; }

  bool isMasked(int state) const { return state >= 100; }
  bool isMasked(const std::string& state) const
  {
    char c = state.c_str()[0];
    return isMasked(c);
  }
  bool isMasked(char state) const
  {
    return isalpha(state) && !isupper(state);
  }

  /**
   * @brief Get the masked state equivalent to the input one.
   *
   * If the input state is masked, returns it "as is".
   * @param state The input state.
   * @throw BadIntException if the input state is not supported, or if there is no quivallent masked state.
   */
  int getMaskedEquivalentState(int state) const;
  /**
   * @brief Get the masked state equivalent to the input one.
   *
   * If the input state is masked, returns it "as is".
   * @param state The input state.
   * @throw BadCharException if the input state is not supported.
   * @throw BadIntException if there is no equivalent masked state.
   */
  const std::string getMaskedEquivalentState(const std::string& state) const;
};
} // end of namespace bpp
#endif // BPP_SEQ_ALPHABET_CASEMASKEDALPHABET_H
