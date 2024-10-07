// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>

#include "GenBank.h"

using namespace bpp;
using namespace std;

/****************************************************************************************/

void GenBank::appendSequencesFromStream(std::istream& input, SequenceContainerInterface& vsc) const
{
  if (!input)
  {
    throw IOException ("GenBank::read: fail to open file");
  }

  string temp, name, sequence = ""; // Initialization
  auto alphaPtr = vsc.getAlphabet();

  // Main loop : for all file lines
  while (!input.eof())
  {
    getline(input, temp, '\n'); // Copy current line in temporary string

    if (temp.size() >= 9 && temp.substr(0, 9) == "ACCESSION")
    {
      name = TextTools::removeSurroundingWhiteSpaces(temp.substr(10));
      StringTokenizer st(name, " ");
      name = st.nextToken();
      // cout << name << endl;
    }
    if (temp.size() >= 6 && temp.substr(0, 6) == "ORIGIN")
    {
      sequence = "";
      getline(input, temp, '\n'); // Copy current line in temporary string
      while (!input.eof() && temp.size() > 2 && temp.substr(0, 2) != "//")
      {
        sequence += TextTools::removeWhiteSpaces(temp.substr(10));
        getline(input, temp, '\n'); // Copy current line in temporary string
      }
      if (name == "")
        throw Exception("GenBank::read(). Sequence with no ACCESSION number!");
      auto seq = make_unique<Sequence>(name, sequence, alphaPtr);
      vsc.addSequence(seq->getName(), seq);
      name = "";
    }
  }
}

/****************************************************************************************/
