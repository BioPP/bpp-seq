// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>

#include "Clustal.h"

using namespace bpp;

// From the STL:
#include <iostream>
#include <iomanip>
using namespace std;

void Clustal::appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& sc) const
{
  // Checking the existence of specified file
  if (!input)
  {
    throw IOException ("Clustal::read : fail to open file");
  }

  auto alphaPtr = sc.getAlphabet();
  vector<unique_ptr<Sequence>> sequences;

  string lineRead("");

  Comments comments(1);
  comments[0] = FileTools::getNextLine(input); // First line gives file generator.

  lineRead = FileTools::getNextLine(input); // This is the first sequence of the first block.

  string::size_type beginSeq = 0;
  unsigned int count = 0;
  for (size_t i = lineRead.size(); i > 0; i--)
  {
    char c = lineRead[i - 1];
    if (c == ' ')
    {
      count++;
      if (count == nbSpacesBeforeSeq_)
      {
        beginSeq = i - 1 + nbSpacesBeforeSeq_;
        break;
      }
    }
    else
      count = 0;
  }
  if (beginSeq == 0)
    throw IOException("Clustal::read. Bad input file.");

  unsigned int countSequences = 0;

  // Read first sequences block:
  bool test = true;
  do
  {
    sequences.push_back(make_unique<Sequence>(TextTools::removeSurroundingWhiteSpaces(lineRead.substr(0, beginSeq - nbSpacesBeforeSeq_)), lineRead.substr(beginSeq), alphaPtr));
    getline(input, lineRead, '\n');
    countSequences++;
    test = !TextTools::isEmpty(lineRead) && !TextTools::isEmpty(lineRead.substr(0, beginSeq - nbSpacesBeforeSeq_));
  }
  while (input && test);

  // Read other blocks
  lineRead = FileTools::getNextLine(input); // Read first sequence of next block.
  while (!TextTools::isEmpty(lineRead))
  {
    // Read next block:
    for (unsigned int i = 0; i < countSequences; ++i)
    {
      // Complete sequences
      if (TextTools::isEmpty(lineRead))
        throw IOException("Clustal::read. Bad input file.");
      sequences[i]->append(lineRead.substr(beginSeq));
      getline(input, lineRead, '\n');
    }
    // At this point, lineRead is the first line after the current block.
    lineRead = FileTools::getNextLine(input);
  }

  for (unsigned int i = 0; i < countSequences; ++i)
  {
    sc.addSequence(sequences[i]->getName(), sequences[i]);
  }
  sc.setComments(comments);
}

void Clustal::writeAlignment(std::ostream& output, const SiteContainerInterface& sc) const
{
  output << "CLUSTAL W (1.81) multiple sequence alignment" << endl;
  output << endl;
  if (sc.getNumberOfSequences() == 0)
    return;

  vector<string> text;
  size_t length = 0;
  for (size_t i = 0; i < sc.getNumberOfSequences(); ++i)
  {
    const Sequence& seq = sc.sequence(i);
    if (seq.getName().size() > length)
      length = seq.getName().size();
    text.push_back(sc.sequence(i).toString());
  }
  length += nbSpacesBeforeSeq_;
  for (unsigned int j = 0; j < text[0].size(); j += charsByLine_)
  {
    for (unsigned int i = 0; i < sc.getNumberOfSequences(); ++i)
    {
      output << TextTools::resizeRight(sc.sequence(i).getName(), length);
      output << text[i].substr(j, charsByLine_) << endl;
    }
    output << endl;
  }
}
