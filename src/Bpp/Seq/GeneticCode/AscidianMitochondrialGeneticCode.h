// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_GENETICCODE_ASCIDIANMITOCHONDRIALGENETICCODE_H
#define BPP_SEQ_GENETICCODE_ASCIDIANMITOCHONDRIALGENETICCODE_H


#include "../Alphabet/NucleicAlphabet.h"
#include "GeneticCode.h"

namespace bpp
{
/**
 * @brief This class implements the ascidian mitochondrial genetic code as describe on the NCBI
 *        web site: http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi?mode=t#SG13
 */
class AscidianMitochondrialGeneticCode :
  public virtual GeneticCode
{
public:
  AscidianMitochondrialGeneticCode(std::shared_ptr<const NucleicAlphabet> alphabet) :
    GeneticCode(alphabet)
  {
    init_();
  }

  virtual ~AscidianMitochondrialGeneticCode() {}

  virtual AscidianMitochondrialGeneticCode* clone() const
  {
    return new AscidianMitochondrialGeneticCode(*this);
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
    return state == 8 || state == 10 || state == 48 || state == 50;
  }

  bool isStop(const std::string& state) const
  {
    return isStop(codonAlphabet_->charToInt(state));
  }

  bool isAltStart(int state) const
  {
    // Test:
    codonAlphabet_->intToChar(state); // throw exception if invalid state!
    return state == 12 || state == 46 || state == 62;
  }

  bool isAltStart(const std::string& state) const
  {
    int i = codonAlphabet_->charToInt(state);
    return i == 12 || i == 46 || i == 62;
  }

private:
  void init_();
};
} // end of namespace bpp.
#endif // BPP_SEQ_GENETICCODE_ASCIDIANMITOCHONDRIALGENETICCODE_H
