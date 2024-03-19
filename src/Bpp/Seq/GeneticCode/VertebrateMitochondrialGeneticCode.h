// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_GENETICCODE_VERTEBRATEMITOCHONDRIALGENETICCODE_H
#define BPP_SEQ_GENETICCODE_VERTEBRATEMITOCHONDRIALGENETICCODE_H


#include "../Alphabet/NucleicAlphabet.h"
#include "GeneticCode.h"

namespace bpp
{
/**
 * @brief This class implements the vertebrate mitochondrial genetic code as describe on the NCBI
 *        web site: http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi?mode=t#SG2
 */
class VertebrateMitochondrialGeneticCode :
  public virtual GeneticCode
{
public:
  VertebrateMitochondrialGeneticCode(std::shared_ptr<const NucleicAlphabet> alphabet) :
    GeneticCode(alphabet)
  {
    init_();
  }

  virtual ~VertebrateMitochondrialGeneticCode() {}

  virtual VertebrateMitochondrialGeneticCode* clone() const
  {
    return new VertebrateMitochondrialGeneticCode(*this);
  }

public:
  size_t getNumberOfStopCodons() const { return 4; }

  std::vector<int> getStopCodonsAsInt() const
  {
    return std::vector<int>({8, 10, 48, 50});
  }

  std::vector<std::string> getStopCodonsAsChar() const
  {
    return std::vector<std::string>({"AGA", "AGG", "TAA", "TAG"});
  }

  bool isStop(int state) const
  {
    // Test:
    codonAlphabet_->intToChar(state); // throw exception if invalid state!
    return state == 48 || state == 50 || state == 8 || state == 10;
  }

  bool isStop(const std::string& state) const
  {
    int i = codonAlphabet_->charToInt(state);
    return i == 48 || i == 50 || i == 8 || i == 10;
  }

  bool isAltStart(int state) const
  {
    // Test:
    codonAlphabet_->intToChar(state); // throw exception if invalid state!
    return state == 12 || state == 13 || state == 15 || state == 46;
  }

  bool isAltStart(const std::string& state) const
  {
    int i = codonAlphabet_->charToInt(state);
    return i == 12 || i == 13 || i == 15 || i == 46;
  }

private:
  void init_();
};
} // end of namespace bpp.
#endif // BPP_SEQ_GENETICCODE_VERTEBRATEMITOCHONDRIALGENETICCODE_H
