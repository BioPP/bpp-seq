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

#include "SequenceWithAnnotation.h"
#include "SequenceTools.h"
#include "AlphabetTools.h"
#include "AlignedSequenceContainer.h"
#include "CaseMaskedAlphabet.h"

//From the STL:
#include <iostream>

namespace bpp {

/**
 * @brief A sequence class which is used to store data from MAF files.
 * 
 * It extends the SequenceWithAnnotation class to store MAF-specific features,
 * like the chromosome position.
 * A MAF sequence is necessarily a DNA sequence.
 */
class MafSequence:
  public SequenceWithAnnotation
{
  private:
    unsigned int begin_;
    char         strand_;
    unsigned int size_;
    unsigned int srcSize_;

  public:
    MafSequence(const std::string& name, const std::string& sequence):
      SequenceWithAnnotation(name, sequence, &AlphabetTools::DNA_ALPHABET), begin_(0), strand_(0), size_(0), srcSize_(0)
    {
      size_ = SequenceTools::getNumberOfSites(*this);
    }

    MafSequence(const std::string& name, const std::string& sequence, unsigned int begin, char strand, unsigned int srcSize) :
      SequenceWithAnnotation(name, sequence, &AlphabetTools::DNA_ALPHABET), begin_(begin), strand_(strand), size_(0), srcSize_(srcSize)
    {
      size_ = SequenceTools::getNumberOfSites(*this);
    }

    MafSequence* clone() const { return new MafSequence(*this); }

  public:
    unsigned int start() const { return begin_; }
    unsigned int stop() const { return begin_ + size_ - 1; }
    char getStrand() const { return strand_; }
    unsigned int getGenomicSize() const { return size_; }
    unsigned int getSrcSize() const { return srcSize_; }
};

/**
 * @brief A synteny block data structure, the basic unit of a MAF alignement file.
 *
 * This class basically contains a AlignedSequenceContainer made of MafSequence objects.
 */
class MafBlock
{
  private:
    double score_;
    unsigned int pass_;
    AlignedSequenceContainer alignment_;

  public:
    MafBlock() :
      score_(0),
      pass_(0),
      alignment_(&AlphabetTools::DNA_ALPHABET)
    {}

  public:
    void setScore(double score) { score_ = score; }
    void setPass(unsigned int pass) { pass_ = pass; }
    
    double getScore() const { return score_; }
    unsigned int getPass() const { return pass_; }

    const AlignedSequenceContainer& getAlignment() const { return alignment_; }

    void addSequence(const MafSequence& sequence) { alignment_.addSequence(sequence, false); }

    unsigned int getNumberOfSequences() const { return alignment_.getNumberOfSequences(); }

};

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
class MafAlignmentParser
{
  private:
    std::istream* stream_;
    bool mask_;
    CaseMaskedAlphabet cmAlphabet_;

  public:
    MafAlignmentParser(std::istream* stream, bool parseMask = false) :
      stream_(stream), mask_(parseMask), cmAlphabet_(&AlphabetTools::DNA_ALPHABET) {}

  private:
    //Recopy is forbidden!
    MafAlignmentParser(const MafAlignmentParser& maf): stream_(0), mask_(maf.mask_), cmAlphabet_(&AlphabetTools::DNA_ALPHABET) {}
    MafAlignmentParser& operator=(const MafAlignmentParser& maf) { stream_ = 0; mask_ = maf.mask_; return *this; }

  public:
    MafBlock* nextBlock() throw (Exception);

};

} // end of namespace bpp.

#endif //_MAFALIGNMENTPARSER_H_

