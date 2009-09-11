// 
// File:    LetterAlphabet.h
// Author:  Sylvain Gaillard
// Created: 11/09/2009 14:31:05
// 

#ifndef _LETTERALPHABET_
#define _LETTERALPHABET_

#include "AbstractAlphabet.h"

namespace bpp {
  class LetterAlphabet: public AbstractAlphabet {
    public:
      LetterAlphabet() {
        for (unsigned int i = 0 ; i < 256 ; i++) {
          letters_[i] = LETTER_UNDEF_VALUE;
        }
      }
      virtual ~LetterAlphabet() {}

    public:
      bool isCharInAlphabet(char state) const {
        return letters_[static_cast<unsigned int>(state)] != LETTER_UNDEF_VALUE;
      }
      bool isCharInAlphabet(const std::string& state) const {
        return isCharInAlphabet(state[0]);
      }
      int charToInt (const std::string &state) const throw (BadCharException) {
        if (!isCharInAlphabet(state))
          throw BadCharException("Not in alphabet");
        return letters_[static_cast<unsigned int>(state[0])];
      }

    protected:
      void registerState(const AlphabetState& st) {
        AbstractAlphabet::registerState(st);
        letters_[static_cast<unsigned int>(st.getLetter()[0])] = st.getNum();
      }

      void setState(unsigned int pos, const AlphabetState& st) throw (IndexOutOfBoundsException) {
        AbstractAlphabet::setState(pos, st);
        letters_[static_cast<unsigned int>(st.getLetter()[0])] = st.getNum();
      }

    private:
      int letters_[256];
      static const int LETTER_UNDEF_VALUE = -99;
  };
}

#endif // _LETTERALPHABET_
