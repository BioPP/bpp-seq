// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_NUCLEICACIDSREPLICATION_H
#define BPP_SEQ_NUCLEICACIDSREPLICATION_H


#include "Alphabet/NucleicAlphabet.h"
#include "Transliterator.h"

// From the STL:
#include <map>
#include <memory>

namespace bpp
{
/**
 * @brief Replication between to nucleic acids.
 *
 * Example of use:
 * - DNA -> DNA: DNA Replication
 * - RNA -> RNA: RNA Replication
 * - DNA -> RNA: Transcription
 * - RNA -> DNA: Reverse transcription
 *
 * Since this is an instance of the ReverseIterator interface, transcription and
 * reverse transcription may be achieved from the same instance of the object by
 * using the translate and reverse methods.
 */
class NucleicAcidsReplication :
  public ReverseTransliteratorInterface
{
private:
  std::shared_ptr<const NucleicAlphabet> nuc1_, nuc2_;
  mutable std::map<int, int> trans_;

public:
  NucleicAcidsReplication(
      std::shared_ptr<const NucleicAlphabet> nuc1,
      std::shared_ptr<const NucleicAlphabet> nuc2);

  NucleicAcidsReplication(const NucleicAcidsReplication& nar) :
    ReverseTransliteratorInterface(nar),
    nuc1_(nar.nuc1_), nuc2_(nar.nuc2_), trans_(nar.trans_)
  {}

  NucleicAcidsReplication& operator=(const NucleicAcidsReplication& nar)
  {
    ReverseTransliteratorInterface::operator=(nar);
    nuc1_ = nar.nuc1_;
    nuc2_ = nar.nuc2_;
    trans_ = nar.trans_;
    return *this;
  }

  virtual ~NucleicAcidsReplication() {}

public:
  std::shared_ptr<const Alphabet> getSourceAlphabet() const override { return nuc1_; }

  const Alphabet& sourceAlphabet() const override { return *nuc1_; }

  std::shared_ptr<const Alphabet> getTargetAlphabet() const override { return nuc2_; }

  const Alphabet& targetAlphabet() const override { return *nuc2_; }

  int translate(int state) const override;

  std::string translate(const std::string& state) const override;

  std::unique_ptr<Sequence> translate(const SequenceInterface& sequence) const override;

  int reverse(int state) const override;

  std::string reverse(const std::string& state) const override;

  std::unique_ptr<Sequence> reverse(const SequenceInterface& sequence) const override;
};
} // end of namespace bpp.
#endif // BPP_SEQ_NUCLEICACIDSREPLICATION_H
