//
// File: NucleicAcidsReplication.h
// Authors:
//   Julien Dutheil
// Created: 2005-05-20 14:20:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
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
  public ReverseTransliterator
{
private:
  std::shared_ptr<const NucleicAlphabet> nuc1_, nuc2_;
  mutable std::map<int, int> trans_;

public:
  NucleicAcidsReplication(
      std::shared_ptr<const NucleicAlphabet> nuc1,
      std::shared_ptr<const NucleicAlphabet> nuc2);

  NucleicAcidsReplication(const NucleicAcidsReplication& nar) :
    ReverseTransliterator(nar),
    nuc1_(nar.nuc1_), nuc2_(nar.nuc2_), trans_(nar.trans_)
  {}

  NucleicAcidsReplication& operator=(const NucleicAcidsReplication& nar)
  {
    ReverseTransliterator::operator=(nar);
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

  std::unique_ptr<Sequence> translate(const Sequence& sequence) const override;

  int reverse(int state) const override;

  std::string reverse(const std::string& state) const override;

  std::unique_ptr<Sequence> reverse(const Sequence& sequence) const override;
};
} // end of namespace bpp.
#endif // BPP_SEQ_NUCLEICACIDSREPLICATION_H
