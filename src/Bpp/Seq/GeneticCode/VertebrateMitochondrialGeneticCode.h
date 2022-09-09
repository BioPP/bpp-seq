//
// File: VertebrateMitochondrialGeneticCode.h
// Authors:
//   Eric Bazin
// Created: wen mar  2 16:01:59 CET 2005
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
    std::vector<int> v(4);
    v[0] = 8;
    v[1] = 10;
    v[2] = 48;
    v[3] = 50;
    return v;
  }

  std::vector<std::string> getStopCodonsAsChar() const
  {
    std::vector<std::string> v(4);
    v[0] = "AGA";
    v[1] = "AGG";
    v[2] = "TAA";
    v[4] = "TAG";
    return v;
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
