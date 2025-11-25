// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>
#include <fstream>

#include "../StringSequenceTools.h"
#include "FastaCsv.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

bool FastaCsv::nextSequence(istream& input, Sequence& seq) const
{
  if (!input)
    throw IOException("FastaCsv::nextSequence: can't read from istream input");
  string seqname = "";
  std::vector<string> content;
  Comments seqcmts;
  short seqcpt = 0;
  string linebuffer = "";
  char c;
  while (!input.eof())
  {
    c = static_cast<char>(input.peek());
    if (input.eof())
      c = '\n';

    // Sequence beginning detection
    if (c == '>')
    {
      // Stop if find a new sequence
      if (seqcpt++)
        break;
    }
    getline(input, linebuffer);
    if (c == '>')
    {
      // Get the sequence name line
      seqname = string(linebuffer.begin() + 1, linebuffer.end());
    }
    if (c != '>' && !TextTools::isWhiteSpaceCharacter(c))
    {
      // Sequence content
      auto lc = TextTools::removeWhiteSpaces(linebuffer);
      StringTokenizer st(lc, ",", false);
      while (st.hasMoreToken())
      {
        content.push_back(st.nextToken());
      }
    }
  }

  seqname = TextTools::removeWhiteSpaces(seqname);

  bool res = (!input.eof());
  // Sequence name and comments isolation
  if (strictNames_ || extended_)
  {
    size_t pos = seqname.find_first_of(" \t\n");
    string seqcmt;
    if (pos != string::npos)
    {
      seqcmt = seqname.substr(pos + 1);
      seqname = seqname.substr(0, pos);
    }
    if (extended_)
    {
      StringTokenizer st(seqcmt, " \\", true, false);
      while (st.hasMoreToken())
      {
        seqcmts.push_back(st.nextToken());
      }
    }
    else
    {
      seqcmts.push_back(seqcmt);
    }
    seq.setComments(seqcmts);
  }
  seq.setName(seqname);
  seq.setContent(content);
  return res;
}

/******************************************************************************/

void FastaCsv::writeSequence(ostream& output, const Sequence& seq) const
{
  if (!output)
    throw IOException("FastaCsv::writeSequence: can't write to ostream output");
  // Sequence name
  output << ">" << seq.getName();
  // Sequence comments
  if (extended_)
  {
    for (unsigned int i = 0; i < seq.getComments().size(); i++)
    {
      output << " \\" << seq.getComments()[i];
    }
  }
  output << endl;
  // Sequence content
  string buffer; // use a buffer to format sequence with states > 1 char
  for (size_t i = 0; i < seq.size() - 1; ++i)
  {
    buffer += seq.getChar(i) + ",";
    if (buffer.size() >= charsByLine_)
    {
      output << string(buffer.begin(), buffer.begin() + charsByLine_) << endl;
      buffer.erase(0, charsByLine_);
    }
  }
  buffer += seq.getChar(seq.size() - 1);

  output << string(buffer.begin(), buffer.end()) << endl;
}

/******************************************************************************/

void FastaCsv::appendSequencesFromStream(istream& input, SequenceContainerInterface& vsc) const
{
  if (!input)
    throw IOException("FastaCsv::appendFromStream: can't read from istream input");
  char c = '\n';
  char last_c;
  bool header = false;
  bool hasSeq = true;
  string line = "";
  Comments cmts;
  while (!input.eof() && hasSeq)
  {
    last_c = c;
    input.get(c);
    // Header detection
    if (extended_ && c == '#')
    {
      header = true;
      continue;
    }
    // Header end detection
    if (c == '\n')
    {
      if (extended_ && header)
      {
        if (line[0] == '\\')
        {
          line.erase(line.begin());
          cmts.push_back(line);
        }
        line = "";
        header = false;
      }
      continue;
    }
    // Header capture
    if (header)
    {
      line.append(1, c);
    }
    // Sequence detection
    if (c == '>' && last_c == '\n')
    {
      input.putback(c);
      c = last_c;
      auto alphaPtr = vsc.getAlphabet();
      auto tmpseq = make_unique<Sequence>("", "", alphaPtr);
      hasSeq = nextSequence(input, *tmpseq);
      vsc.addSequence(tmpseq->getName(), tmpseq);
    }
  }
  if (extended_ && cmts.size())
  {
    vsc.setComments(cmts);
  }
}

/******************************************************************************/

void FastaCsv::writeSequences(ostream& output, const SequenceContainerInterface& sc) const
{
  if (!output)
    throw IOException("FastaCsv::write: can't write to ostream output");

  if (extended_)
  {
    // Loop for all general comments
    for (size_t i = 0; i < sc.getComments().size(); ++i)
    {
      output << "#\\" << sc.getComments()[i] << endl;
    }
    output << endl;
  }

  // Main loop : for all sequences in vector container
  vector<string> names = sc.getSequenceNames();
  for (size_t i = 0; i < names.size(); ++i)
  {
    writeSequence(output, sc.sequence(names[i]));
  }
}

/******************************************************************************/
