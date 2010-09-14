//
// File: MafAlignmentParser.cpp
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

#include "MafAlignmentParser.h"
#include "SequenceWithQuality.h"
#include "SequenceWithAnnotationTools.h"

//From Utils:
#include <Utils/KeyvalTools.h>
#include <Utils/TextTools.h>

using namespace std;
using namespace bpp;

MafBlock* MafAlignmentParser::nextBlock() throw (Exception)
{
  MafBlock* block = 0;

  string line;
  bool test = true;
  MafSequence* currentSequence = 0;
  while (test)
  {
    getline(*stream_, line, '\n');
    if (TextTools::isEmpty(line))
    {
      if (currentSequence) {
        //Add previous sequence:
        block->addSequence(*currentSequence); //The sequence is copied in the container.
        delete currentSequence;
      }

      //end of paragraph
      test = false;
    }
    else if (line[0] == 'a')
    {
      if (currentSequence) {
        //Add previous sequence:
        block->addSequence(*currentSequence); //The sequence is copied in the container.
        delete currentSequence;
      }
      
      //New block.
      block = new MafBlock();

      map<string, string> args;
      if (line.size() > 2)
      {
        KeyvalTools::multipleKeyvals(line.substr(3), args, " ");

        if (args.find("score") != args.end())
          block->setScore(TextTools::toDouble(args["score"]));
    
        if (args.find("pass") != args.end())
          block->setPass(TextTools::toDouble(args["pass"]));
      }
    }
    else if (line[0] == 's')
    {
      StringTokenizer st(line);
      st.nextToken(); //The 's' tag
      string src = st.nextToken();
      unsigned int start = TextTools::to<unsigned int>(st.nextToken());
      unsigned int size = TextTools::to<unsigned int>(st.nextToken());
      string tmp = st.nextToken();
      if (tmp.size() != 1)
        throw Exception("MafAlignmentParser::nextBlock. Strand specification is incorrect, should be only one character long, found " + TextTools::toString(tmp.size()) + ".");
      char strand = tmp[0];

      unsigned int srcSize = TextTools::to<unsigned int>(st.nextToken());
      if (currentSequence) {
        //Add previous sequence:
        block->addSequence(*currentSequence); //The sequence is copied in the container.
        delete currentSequence;
      }
      const string seq = st.nextToken();
      currentSequence = new MafSequence(src, seq, start, strand, srcSize);
      if (currentSequence->getGenomicSize() != size)
        throw Exception("MafAlignmentParser::nextBlock. Sequence found does not match specified size: " + TextTools::toString(currentSequence->getGenomicSize()) + ", should be " + TextTools::toString(size) + ".");
      
      //Add mask:
      if (mask_) {
        vector<bool> mask(currentSequence->size());
        for (unsigned int i = 0; i < mask.size(); ++i) {
          mask[i] = cmAlphabet_.isMasked(seq[i]);
        }
        currentSequence->addAnnotation(new SequenceMask(mask));
      }
    }
    else if (line[0] == 'q')
    {
      if (!currentSequence)
        throw Exception("MafAlignmentParser::nextBlock(). Quality scores found, but there is currently no sequence!");
      StringTokenizer st(line);
      st.nextToken(); //The 'q' tag
      string name = st.nextToken();
      if (name != currentSequence->getName())
        throw Exception("MafAlignmentParser::nextBlock(). Quality scores found, but with a different name from the previous sequence: " + name + ", should be " + currentSequence->getName() + ".");
      string qstr = st.nextToken();
      //Now parse the score string:
      SequenceQuality* seqQual = new SequenceQuality(qstr.size());
      for (unsigned int i = 0; i < qstr.size(); ++i) {
        char c = qstr[i];
        if (c == '-') {
          seqQual->setScore(i, -1);
        } else if (c == '0' || c == '1' || c == '2' || c== '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
          seqQual->setScore(i, c - '0');
        } else if (c == 'F' || c == 'f') { //Finished
          seqQual->setScore(i, 10);
        } else if (c == '?') {
          seqQual->setScore(i, -2);
        } else {
          throw Exception("MafAlignmentParser::nextBlock(). Unvalid quality score: " + TextTools::toString(c) + ". Should be 0-9, F or '-'.");
        }
      }
      currentSequence->addAnnotation(seqQual);
    }
  }
  return block;
}

void MafAlignmentParser::writeHeader(std::ostream& out) const
{
  out << "##maf version=1 program=Bio++" << endl << endl;
  //There are more options in the header that we may want to support...
}

void MafAlignmentParser::writeBlock(std::ostream& out, const MafBlock& block) const
{
  out << "a";
  if (block.getScore() > -1.)
    out << " score=" << block.getScore();
  if (block.getPass() > 0)
    out << " pass=" << block.getPass();
  out << endl;
  
  //Now we write sequences. First need to count characters for aligning blocks:
  size_t mxcSrc = 0, mxcStart = 0, mxcSize = 0, mxcSrcSize = 0;
  for (unsigned int i = 0; i < block.getNumberOfSequences(); i++) {
    const MafSequence* seq = &block.getSequence(i);
    mxcSrc     = max(mxcSrc    , seq->getName().size());
    mxcStart   = max(mxcStart  , TextTools::toString(seq->start()).size());
    mxcSize    = max(mxcSize   , TextTools::toString(seq->getGenomicSize()).size());
    mxcSrcSize = max(mxcSrcSize, TextTools::toString(seq->getSrcSize()).size());
  }
  //Now print each sequence:
  for (unsigned int i = 0; i < block.getNumberOfSequences(); i++) {
    const MafSequence* seq = new MafSequence(block.getSequence(i));
    out << "s ";
    out << TextTools::resizeRight(seq->getName(), mxcSrc, ' ') << " ";
    out << TextTools::resizeLeft(TextTools::toString(seq->start()), mxcStart, ' ') << " ";
    out << TextTools::resizeLeft(TextTools::toString(seq->getGenomicSize()), mxcSize, ' ') << " ";
    out << seq->getStrand() << " ";
    out << TextTools::resizeLeft(TextTools::toString(seq->getSrcSize()), mxcSrcSize, ' ') << " ";
    //Shall we write the sequence as masked?
    string seqstr = seq->toString();
    if (mask_ && seq->hasAnnotation(SequenceMask::MASK)) {
      const SequenceMask* mask = &dynamic_cast<const SequenceMask&>(seq->getAnnotation(SequenceMask::MASK));
      for (unsigned int j = 0; j < seqstr.size(); ++j) {
        char c = ((*mask)[j] ? tolower(seqstr[j]) : seqstr[j]);
        out << c;
      }
    } else {
      out << seqstr;
    }
    out << endl;
    //Write quality scores if any:
    if (mask_ && seq->hasAnnotation(SequenceQuality::QUALITY_SCORE)) {
      const SequenceQuality* qual = &dynamic_cast<const SequenceQuality&>(seq->getAnnotation(SequenceQuality::QUALITY_SCORE));
      out << "q ";
      out << TextTools::resizeRight(seq->getName(), mxcSrc + mxcStart + mxcSize + mxcSrcSize + 5, ' ') << " ";
      string qualStr;
      for (unsigned int j = 0; j < seq->size(); ++j) {
        double s = (*qual)[j];
        if (s == -1) {
          qualStr += "-";
        } else if (s == -2) {
          qualStr += "?";
        } else if (s >=0 && s < 10) {
          qualStr += TextTools::toString(s);
        } else if (s == 10) {
          qualStr += "F";
        } else {
          throw Exception("MafAlignmentParser::writeBlock. Unsuported score value: " + TextTools::toString(s));
        }
      }
      out << qualStr << endl;
    }
  }
  out << endl;
}

