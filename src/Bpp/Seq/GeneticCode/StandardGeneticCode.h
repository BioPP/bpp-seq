// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_GENETICCODE_STANDARDGENETICCODE_H
#define BPP_SEQ_GENETICCODE_STANDARDGENETICCODE_H


#include "../Alphabet/NucleicAlphabet.h"
#include "GeneticCode.h"

namespace bpp
{
/**
 * @brief This class implements the standard genetic code as describe on the NCBI
 *        web site: http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi?mode=t#SG1
 */
class StandardGeneticCode :
  public virtual GeneticCode
{
public:
  StandardGeneticCode(std::shared_ptr<const NucleicAlphabet> alphabet) :
    GeneticCode(alphabet)
  {
    init_();
  }

  virtual ~StandardGeneticCode() {}

  virtual StandardGeneticCode* clone() const
  {
    return new StandardGeneticCode(*this);
  }

public:
  size_t getNumberOfStopCodons() const { return 3; }

  std::vector<int> getStopCodonsAsInt() const
  {
    return std::vector<int>({48, 50, 56});
  }

  std::vector<std::string> getStopCodonsAsChar() const
  {
    return std::vector<std::string>({"TAA", "TAG", "TGA"});
  }

  bool isStop(int state) const
  {
    // Test:
    codonAlphabet_->intToChar(state); // throw exception if invalid state!
    return state == 48 || state == 50 || state == 56;
  }

  bool isStop(const std::string& state) const
  {
    int i = codonAlphabet_->charToInt(state);
    return i == 48 || i == 50 || i == 56;
  }

  bool isAltStart(int state) const
  {
    // Test:
    codonAlphabet_->intToChar(state); // throw exception if invalid state!
    return state == 62 || state == 30;
  }

  bool isAltStart(const std::string& state) const
  {
    int i = codonAlphabet_->charToInt(state);
    return i == 62 || i == 30;
  }

private:
  void init_();
};
} // end of namespace bpp.
#endif // BPP_SEQ_GENETICCODE_STANDARDGENETICCODE_H
