//
// File: BinaryAlphabet.h
// Author: L Gueguen
// Created on: vendredi 20 septembre 2013, � 23h 01
//

/*
   Copyright or � or Copr. Bio++ Development Team, (November 17, 2004)

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


#ifndef BPP_SEQ_ALPHABET_INTEGERALPHABET_H
#define BPP_SEQ_ALPHABET_INTEGERALPHABET_H


#include "AbstractAlphabet.h"

namespace bpp
{
/**
 * @brief The Integer Alphabet class, letters are from 0 to a given number, MAX.
 * @author Laurent Gueguen
 *
 */
class IntegerAlphabet :
  public AbstractAlphabet
{
private:
  unsigned int MIN_;
  unsigned int MAX_;
  

public:
  // class constructor
  IntegerAlphabet(unsigned int max, unsigned int min = 0);

  IntegerAlphabet(const IntegerAlphabet& bia) : AbstractAlphabet(bia), MIN_(bia.MIN_), MAX_(bia.MAX_) {}

  IntegerAlphabet& operator=(const IntegerAlphabet& bia)
  {
    AbstractAlphabet::operator=(bia);
    MIN_=bia.MIN_;
    MAX_=bia.MAX_;

    return *this;
  }

  IntegerAlphabet* clone() const
  {
    return new IntegerAlphabet(*this);
  }
  // class destructor
  virtual ~IntegerAlphabet() {}

public:
  unsigned int getSize() const { return MAX_ - MIN_ +1; }

  unsigned int getNumberOfTypes() const { return MAX_ - MIN_ + 1; }
  
  std::string getAlphabetType() const { return "Integer"; }
  
  int getUnknownCharacterCode() const { return static_cast<int>(MAX_+1); }
  
  bool isUnresolved(int state) const { return state == static_cast<int>(MAX_+1); }
  
  bool isUnresolved(const std::string& state) const { return state == "X"; }
  
  unsigned int getMin() const { return MIN_; }

  unsigned int getMax() const { return MAX_; }

  bool isResolvedIn(int state1, int state2) const;
    
  std::vector<int> getAlias(int state) const;

  std::vector<std::string> getAlias(const std::string& state) const;
  


};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_INTEGERALPHABET_H
