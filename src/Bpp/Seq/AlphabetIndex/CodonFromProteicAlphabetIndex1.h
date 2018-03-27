//
// File: CodonFromProteicAlphabetIndex1.h
// Created by: Laurent Guéguen
// Created on: jeudi 22 mars 2018, à 09h 04
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

#ifndef _CODON_FROM_PROTEIC_ALPHABET_INDEX1_H_
#define _CODON_FROM_PROTEIC_ALPHABET_INDEX1_H_

#include "AlphabetIndex1.h"
#include "../Alphabet/CodonAlphabet.h"
#include "../GeneticCode/GeneticCode.h"

namespace bpp
{
  /*
   * @brief AlphabetIndex1 for codon based on ProteicAlphabetIndex1.
   *
   */
  
  class CodonFromProteicAlphabetIndex1 :
    public AlphabetIndex1
  {
  private:
    const CodonAlphabet* alpha_;
    const GeneticCode* gencode_;

    std::vector<double> vIndex_;
    
  public:
    CodonFromProteicAlphabetIndex1(const GeneticCode* gencode, const AlphabetIndex1* protalphindex) :
      AlphabetIndex1(),
      alpha_(&AlphabetTools::CODON_ALPHABET),
      gencode_(gencode),
      vIndex_(64)
    {
      if (!AlphabetTools::isProteicAlphabet(protalphindex->getAlphabet()))
        throw Exception("CodonFromProteicAlphabetIndex1: Not a Proteic Alphabet for CodonAlphabetIndex1.");
      fillIndex_(protalphindex);
    }

    CodonFromProteicAlphabetIndex1(const CodonFromProteicAlphabetIndex1& cfp) :
      alpha_(cfp.alpha_),
      gencode_(cfp.gencode_),
      vIndex_(cfp.vIndex_)
    {
    }

    CodonFromProteicAlphabetIndex1& operator=(const CodonFromProteicAlphabetIndex1& cfp)
    {
      alpha_ = cfp.alpha_;
      gencode_ = cfp.gencode_;
      vIndex_ = cfp.vIndex_;

      return *this;
    }

    virtual ~CodonFromProteicAlphabetIndex1() {}

    CodonFromProteicAlphabetIndex1* clone() const { return new CodonFromProteicAlphabetIndex1(*this); }

  public:
    double getIndex(int state) const
    {
      return vIndex_[getAlphabet()->getStateIndex(state)-1];
    }

    double getIndex(const std::string& state) const
    {
      return vIndex_[getAlphabet()->getStateIndex(state)-1];
    }

    const Alphabet* getAlphabet() const
    {
      return alpha_;
    }
    
    std::vector<double>* getIndexVector() const {
      return new std::vector<double>(vIndex_);
    }

  private:
    void fillIndex_(const AlphabetIndex1* protAlphIndex_)
    {
      for (int i=0; i<64; i++)
        vIndex_[i]=gencode_->isStop(i)?0:protAlphIndex_->getIndex(gencode_->translate(i));
    }
    
  };
} // end of namespace bpp.

#endif // _CODON_FROM_PROTEIC_ALPHABET_INDEX1_H_

