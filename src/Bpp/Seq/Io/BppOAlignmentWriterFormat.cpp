// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/KeyvalTools.h>
#include <memory>
#include <string>

#include "BppOAlignmentWriterFormat.h"
#include "Clustal.h"
#include "Fasta.h"
#include "Mase.h"
#include "Phylip.h"
#include "Stockholm.h"

using namespace bpp;
using namespace std;

unique_ptr<OAlignment> BppOAlignmentWriterFormat::read(const std::string& description)
{
  unparsedArguments_.clear();
  string format = "";
  KeyvalTools::parseProcedure(description, format, unparsedArguments_);
  unsigned int ncol = ApplicationTools::getParameter<unsigned int>("length", unparsedArguments_, 100, "", true, warningLevel_);
  unique_ptr<OAlignment> oAln;
  if (format == "Fasta")
  {
    oAln.reset(new Fasta(ncol));
  }
  else if (format == "Mase")
  {
    oAln.reset(new Mase(ncol));
  }
  else if (format == "Clustal")
  {
    oAln.reset(new Clustal(ncol));
  }
  else if (format == "Phylip")
  {
    bool sequential = true, extended = true;
    string split = "  ";
    string order = ApplicationTools::getStringParameter("order", unparsedArguments_, "sequential", "", true, warningLevel_);
    if (order == "sequential")
      sequential = true;
    else if (order == "interleaved")
      sequential = false;
    else
      throw Exception("BppOAlignmentWriterFormat::read. Invalid argument 'order' for phylip format: " + order);

    string type = ApplicationTools::getStringParameter("type", unparsedArguments_, "extended", "", true, warningLevel_);
    if (type == "extended")
    {
      extended = true;
      split = ApplicationTools::getStringParameter("split", unparsedArguments_, "spaces", "", true, warningLevel_);
      if (split == "spaces")
        split = "  ";
      else if (split == "tab")
        split = "\t";
      else
        throw Exception("BppOAlignmentWriterFormat::read. Invalid argument 'split' for phylip format: " + split);
    }
    else if (type == "classic")
      extended = false;
    else
      throw Exception("BppOAlignmentWriterFormat::read. Invalid argument 'type' for phylip format: " + type);

    oAln.reset(new Phylip(extended, sequential, ncol, split));
  }
  else if (format == "Stockholm")
  {
    oAln.reset(new Stockholm());
  }
  else
  {
    throw IOException("Sequence format '" + format + "' unknown.");
  }

  return oAln;
}
