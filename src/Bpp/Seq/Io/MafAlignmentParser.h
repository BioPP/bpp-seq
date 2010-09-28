//
// File: MafAlignmentParser.h
// Authors: Julien Dutheil
// Created: Tue Apr 27 2010
//

/*
Copyright or © or Copr. Bio++ Development Team, (2010)

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

#ifndef _MAFALIGNMENTPARSER_H_
#define _MAFALIGNMENTPARSER_H_

#include "MafIterator.h"
#include "../Alphabet/CaseMaskedAlphabet.h"

//From the STL:
#include <iostream>

namespace bpp {

/**
 * @brief MAF file parser.
 * 
 * This class is a (draft) attempt to parse synteny block from Maf file.
 *
 * The MAF format is documented on the UCSC Genome Browser website:
 * @url http://genome.ucsc.edu/FAQ/FAQformat.html#format5
 *
 * @author Julien Dutheil
 */
class MafAlignmentParser:
  public MafIterator
{
  private:
    std::istream* stream_;
    bool mask_;
    CaseMaskedAlphabet cmAlphabet_;
    bool firstBlock_;

  public:
    MafAlignmentParser(std::istream* stream, bool parseMask = false) :
      stream_(stream), mask_(parseMask), cmAlphabet_(&AlphabetTools::DNA_ALPHABET), firstBlock_(true) {}

  private:
    //Recopy is forbidden!
    MafAlignmentParser(const MafAlignmentParser& maf): stream_(0), mask_(maf.mask_), cmAlphabet_(&AlphabetTools::DNA_ALPHABET), firstBlock_(maf.firstBlock_) {}
    MafAlignmentParser& operator=(const MafAlignmentParser& maf) { stream_ = 0; mask_ = maf.mask_; firstBlock_ = maf.firstBlock_; return *this; }

  public:
    MafBlock* nextBlock() throw (Exception);

};

} // end of namespace bpp.

#endif //_MAFALIGNMENTPARSER_H_

