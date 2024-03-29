// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>
#include <fstream>

#include "../StringSequenceTools.h"
#include "../Container/SequenceContainer.h"
#include "Pasta.h"

using namespace bpp;
using namespace std;

/********************************************************************************/

bool Pasta::nextSequence(istream& input, ProbabilisticSequence& seq, bool hasLabels, const vector<size_t>& permutationMap) const
{
  if (!input)
    throw IOException("Pasta::nextSequence : can't read from istream input");

  string seqname = "";
  vector<double> tokens;
  Comments seqcmts;
  short seqcpt = 0;
  string linebuffer = "";
  char c;

  while (!input.eof())
  {
    c = static_cast<char>(input.peek());
    if (input.eof())
      c = '\n';

    // detect the beginning of a sequence
    if (c == '>')
    {
      // stop if we find a new sequence
      if (seqcpt++)
        break;
    }

    getline(input, linebuffer);

    if (c == '>')
    {
      // get the sequence name line
      seqname = string(linebuffer.begin() + 1, linebuffer.end());
    }

    if (c != '>' && !TextTools::isWhiteSpaceCharacter(c))
    {
      // sequence content : probabilities for each site are space-separated
      StringTokenizer st(linebuffer, " \t\n", false, false);
      while (st.hasMoreToken())
      {
        double t;
        stringstream ss(st.nextToken());
        ss >> t;
        tokens.push_back(t);
      }
    }
  }

  bool res = (!input.eof());

  // Sequence name and comments isolation (identical to that of Fasta)
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

  /* finally, deal with the content */

  // there is a header that specifies to which character each
  // probability is associated
  auto size = seq.getAlphabet()->getSize();
  if (hasLabels)
  {
    DataTable content(size, 0);
    vector<double>::const_iterator i = tokens.begin();
    while (i != tokens.end())
    {
      // junk up the tokens into groups of alphabetsize, and permute
      // according to how the header is permuted
      vector<double> row(size, 0.);
      for (const auto j:permutationMap)
      {
        if (i == tokens.end())
          throw Exception("Pasta::nextSequence : input is incomplete");
        row[(size_t)j] = *i;
        ++i;
      }

      content.addColumn(row);
    }
    // finally set the content
    seq.setContent(content.getData());
  }
  // o.w., we assume that each probability is that a (binary)
  // character is 1
  else
  {
    DataTable content(2, 0);

    // fill in pairs of probabilities that (binary) character is 0,
    // resp. 1
    for (const auto& i : tokens)
    {
      // the following will throw an exception if v[i] is not a properly
      // formatted double : a check that we want to have

      vector<double> pair_v{1. - i, i};
      content.addColumn(pair_v);
    }

    // finally, we set the content of the sequence to the above.
    // Since any number format exception was taken care of above, as
    // well as that each sequence must be of the same length by
    // construction of a DataTable object, the only thing left that
    // could go wrong is that p(0) + p(1) != 1 : a check that is done
    // in the call of the function below
    seq.setContent(content.getData());
  }

  return res;
}

/********************************************************************************/

void Pasta::appendAlignmentFromStream(istream& input, ProbabilisticSequenceContainerInterface& container) const
{
  if (!input)
    throw IOException("Pasta::appendAlignmentFromStream: can't read from istream input");

  char c = '\n';
  char last_c;
  bool header = false;
  bool hasSeq = true;
  string line = "";
  Comments cmts;
  auto alphaPtr = container.getAlphabet();

  // labels for the states
  bool hasLabels = false;
  vector<string> labels;
  vector<size_t> permutationMap;

  while (!input.eof() && hasSeq)
  {
    last_c = c;
    input.get(c);

    // detect the header
    if (extended_ && c == '#')
    {
      header = true;
      continue;
    }

    // detect the end of the header
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

    // capture the header
    if (header)
    {
      line.append(1, c);
    }

    // detect/get labels for the states
    if (!header && c != '>')
    {
      hasLabels = true;
      input.putback(c);
      c = last_c;

      getline(input, line);
      StringTokenizer st(line, " \t\n", false, false);

      while (st.hasMoreToken())
      {
        string s = st.nextToken();
        labels.push_back(s);
      }

      /* check labels against alphabet of the container */
      vector<string> resolved_chars = container.getAlphabet()->getResolvedChars();

      // build permutation map on the content, error should one exist
      for (const auto&  i : labels)
      {
        bool found = false;

        for (size_t j = 0; j < resolved_chars.size(); ++j)
        {
          if (i == resolved_chars[j])
          {
            if (found)
              throw Exception("Pasta::appendSequencesFromStream. Label " + i + " found twice.");

            permutationMap.push_back(j);
            found = true;
          }
        }

        if (!found)
        {
          string states = "<";
          for (const auto&  i2 :  resolved_chars)
          {
            states += " " + i2;
          }
          states += " >";
          throw Exception("Pasta::appendSequencesFromStream. Label " + i + " is not found in alphabet " + states + ".");
        }
      }
    }

    // detect the sequence
    if (c == '>' && last_c == '\n')
    {
      input.putback(c);
      c = last_c;
      auto tmpseq = make_unique<ProbabilisticSequence>(alphaPtr); // add probabilistic sequences instead
      hasSeq = nextSequence(input, *tmpseq, hasLabels, permutationMap);
      container.addSequence(tmpseq->getName(), tmpseq);
    }
  }
  if (extended_ && cmts.size())
  {
    container.setComments(cmts);
  }
}


/********************************************************************************/

void Pasta::writeSequence(ostream& output, const ProbabilisticSequence& seq, bool header) const
{
  if (!output)
    throw IOException("Pasta::writeSequence : can't write to ostream output");

  // The alphabet
  if (header)
  {
    vector<string> resolved_chars = seq.getAlphabet()->getResolvedChars();
    for (auto state : resolved_chars)
    {
      output << state << "\t";
    }
    output << endl;
  }

  // sequence name
  output << ">" << seq.getName();

  // sequence comments
  if (extended_)
  {
    for (size_t i = 0; i < seq.getComments().size(); ++i)
    {
      output << " \\" << seq.getComments()[i];
    }
  }
  output << endl;

  StlOutputStreamWrapper outs(&output);

  // sequence content
  const vector<vector<double>>& data = seq.getContent();

  for (auto i : data)
  {
    VectorTools::print(i, outs, "\t");
  }
}

/********************************************************************************/

void Pasta::writeSequence(ostream& output, const Sequence& seq, bool header) const
{
  if (!output)
    throw IOException("Pasta::writeSequence : can't write to ostream output");

  // The alphabet
  if (header)
  {
    vector<string> resolved_chars = seq.getAlphabet()->getResolvedChars();
    for (auto state : resolved_chars)
    {
      output << state << "\t";
    }
    output << endl;
  }

  // sequence name
  output << ">" << seq.getName();

  // sequence comments
  if (extended_)
  {
    for (size_t i = 0; i < seq.getComments().size(); ++i)
    {
      output << " \\" << seq.getComments()[i];
    }
  }
  output << endl;

  // sequence content

  int nbc = (int)seq.getAlphabet()->getResolvedChars().size();

  for (size_t i = 0; i < seq.size(); ++i)
  {
    for (int s = 0; s < nbc; ++s)
    {
      output << seq.getStateValueAt(i, (int)s);
      output << "\t";
    }
    output << endl;
  }
}
