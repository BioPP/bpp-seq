// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_DNATORNA_H
#define BPP_SEQ_DNATORNA_H


#include "Alphabet/AlphabetTools.h"
#include "Alphabet/DNA.h"
#include "Alphabet/RNA.h"
#include "Transliterator.h"

namespace bpp
{
/**
 * @brief Reverse iterator to convert RNA to DNA.
 *
 * This only changes U to T characters.
 *
 * @see NucleicAcidsReplication
 */
class DNAToRNA :
  public AbstractReverseTransliterator
{
private:
  std::shared_ptr<const Alphabet> dna_, rna_;

public:
  DNAToRNA() :
    AbstractReverseTransliterator(),
    dna_(AlphabetTools::DNA_ALPHABET),
    rna_(AlphabetTools::RNA_ALPHABET)
  {}

  DNAToRNA(const DNAToRNA& d2r) : AbstractReverseTransliterator(d2r), dna_(d2r.dna_), rna_(d2r.rna_) {}

  DNAToRNA& operator=(const DNAToRNA& d2r)
  {
    AbstractReverseTransliterator::operator=(d2r);
    dna_ = d2r.dna_;
    rna_ = d2r.rna_;
    return *this;
  }

  virtual ~DNAToRNA() {}

public:

  std::shared_ptr<const Alphabet> getSourceAlphabet() const override { return dna_; }
  
  const Alphabet& sourceAlphabet() const override { return *dna_; }

  std::shared_ptr<const Alphabet> getTargetAlphabet() const override { return rna_; }
  
  const Alphabet& targetAlphabet() const override { return *rna_; }

  int translate(int state) const override;

  std::string translate(const std::string& state) const override;

  std::unique_ptr<Sequence> translate(const SequenceInterface& sequence) const override
  {
    return AbstractReverseTransliterator::translate(sequence);
  }

  int reverse(int state) const override;

  std::string reverse(const std::string& state) const override;

  std::unique_ptr<Sequence> reverse(const SequenceInterface& sequence) const override
  {
    return AbstractReverseTransliterator::reverse(sequence);
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_DNATORNA_H
