//
// File: CodonFromProteicAlphabetIndex2.h
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

#ifndef _CODON_FROM_PROTEIC_ALPHABET_INDEX2_H_
#define _CODON_FROM_PROTEIC_ALPHABET_INDEX2_H_

#include "AlphabetIndex2.h"
#include "../Alphabet/CodonAlphabet.h"
#include "../GeneticCode/GeneticCode.h"
#include <Bpp/Numeric/Matrix/Matrix.h>

namespace bpp
{
  /*
   * @brief AlphabetIndex2 for codon based on ProteicAlphabetIndex2.
   *
   */
  
  class CodonFromProteicAlphabetIndex2 :
    public AlphabetIndex2
  {
  private:
    const CodonAlphabet* alpha_;
    const GeneticCode* gencode_;

    LinearMatrix<double> vIndex_;
    
    bool isSymmetric_;
    
  public:
    CodonFromProteicAlphabetIndex2(const GeneticCode* gencode, const AlphabetIndex2* protalphindex) :
      AlphabetIndex2(),
      alpha_(&AlphabetTools::CODON_ALPHABET),
      gencode_(gencode),
      vIndex_(64,64),
      isSymmetric_(protalphindex->isSymmetric())
    {
      if (!AlphabetTools::isProteicAlphabet(protalphindex->getAlphabet()))
        throw Exception("CodonFromProteicAlphabetIndex2: Not a Proteic Alphabet for CodonAlphabetIndex2.");
      fillIndex_(protalphindex);
    }

    CodonFromProteicAlphabetIndex2(const CodonFromProteicAlphabetIndex2& cfp) :
      alpha_(cfp.alpha_),
      gencode_(cfp.gencode_),
      vIndex_(cfp.vIndex_),
      isSymmetric_(cfp.isSymmetric_)
    {
    }

    CodonFromProteicAlphabetIndex2& operator=(const CodonFromProteicAlphabetIndex2& cfp) 
    {
      alpha_ = cfp.alpha_;
      gencode_ = cfp.gencode_;
      vIndex_ = cfp.vIndex_;
      isSymmetric_ = cfp.isSymmetric_;

      return *this;
    }

    virtual ~CodonFromProteicAlphabetIndex2() {}

    CodonFromProteicAlphabetIndex2* clone() const { return new CodonFromProteicAlphabetIndex2(*this); }

  public:
    double getIndex(int state1, int state2) const
    {
      return vIndex_(getAlphabet()->getStateIndex(state1)-1,getAlphabet()->getStateIndex(state2)-1);
    }

    double getIndex(const std::string& state1, const std::string& state2) const
    {
      return vIndex_(getAlphabet()->getStateIndex(state1)-1,getAlphabet()->getStateIndex(state2)-1);
    }

    const Alphabet* getAlphabet() const
    {
      return alpha_;
    }
    
    Matrix<double>* getIndexMatrix() const {
      return new LinearMatrix<double>(vIndex_);
    }

    bool isSymmetric() const
    {
      return isSymmetric_;
    }
    
  private:
    void fillIndex_(const AlphabetIndex2* protAlphIndex_)
    {
      for (int i=0; i<64; i++)
        for (int j=0; j<64; j++)
          vIndex_(i,j)=gencode_->isStop(i)?0:
            gencode_->isStop(j)?0:
            protAlphIndex_->getIndex(gencode_->translate(i),gencode_->translate(j));
    } 
  };
  
} // end of namespace bpp.

#endif // _CODON_FROM_PROTEIC_ALPHABET_INDEX2_H_

