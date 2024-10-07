// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_CODONFROMPROTEICALPHABETINDEX1_H
#define BPP_SEQ_ALPHABETINDEX_CODONFROMPROTEICALPHABETINDEX1_H


#include "../Alphabet/CodonAlphabet.h"
#include "../GeneticCode/GeneticCode.h"
#include "AlphabetIndex1.h"

namespace bpp
{
/**
 * @brief AlphabetIndex1 for codon based on ProteicAlphabetIndex1.
 */
class CodonFromProteicAlphabetIndex1 :
  public virtual AlphabetIndex1
{
private:
  std::shared_ptr<const CodonAlphabet> alpha_;
  std::shared_ptr<const GeneticCode> gencode_;
  std::vector<double> vIndex_;

public:
  CodonFromProteicAlphabetIndex1(
      std::shared_ptr<const GeneticCode> gencode,
      std::shared_ptr<const AlphabetIndex1> protalphindex) :
    AlphabetIndex1(),
    alpha_(AlphabetTools::DNA_CODON_ALPHABET),
    gencode_(gencode),
    vIndex_(64)
  {
    if (!AlphabetTools::isProteicAlphabet(protalphindex->alphabet()))
      throw Exception("CodonFromProteicAlphabetIndex1: Not a Proteic Alphabet for CodonAlphabetIndex1.");
    fillIndex_(protalphindex);
  }

  CodonFromProteicAlphabetIndex1(const CodonFromProteicAlphabetIndex1& cfp) :
    alpha_(cfp.alpha_),
    gencode_(cfp.gencode_),
    vIndex_(cfp.vIndex_)
  {}

  CodonFromProteicAlphabetIndex1& operator=(const CodonFromProteicAlphabetIndex1& cfp)
  {
    alpha_ = cfp.alpha_;
    gencode_ = cfp.gencode_;
    vIndex_ = cfp.vIndex_;

    return *this;
  }

  virtual ~CodonFromProteicAlphabetIndex1()
  {}

  CodonFromProteicAlphabetIndex1* clone() const override
  {
    return new CodonFromProteicAlphabetIndex1(*this);
  }

public:
  double getIndex(int state) const override
  {
    return vIndex_[getAlphabet()->getStateIndex(state) - 1];
  }

  double getIndex(const std::string& state) const override
  {
    return vIndex_[getAlphabet()->getStateIndex(state) - 1];
  }

  std::shared_ptr<const Alphabet> getAlphabet() const override
  {
    return alpha_;
  }

  const Alphabet& alphabet() const override
  {
    return *alpha_;
  }

  const std::vector<double>& indexVector() const override
  {
    return vIndex_;
  }

private:
  void fillIndex_(std::shared_ptr<const AlphabetIndex1> protAlphIndex_)
  {
    for (int i = 0; i < 64; i++)
    {
      vIndex_[getAlphabet()->getStateIndex(i) - 1] = gencode_->isStop(i) ? 0 : protAlphIndex_->getIndex(gencode_->translate(i));
    }
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_CODONFROMPROTEICALPHABETINDEX1_H
