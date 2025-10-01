// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/KeyvalTools.h>
#include <memory>
#include <string>

#include "BppOAlignmentReaderFormat.h"
#include "Clustal.h"
#include "Csv.h"
#include "Dcse.h"
#include "Fasta.h"
#include "FastaCsv.h"
#include "Mase.h"
#include "NexusIoSequence.h"
#include "Pasta.h"
#include "Phylip.h"

using namespace bpp;
using namespace std;

unique_ptr<IAlignment> BppOAlignmentReaderFormat::read(const std::string& description)
{
  unparsedArguments_.clear();
  string format = "";
  KeyvalTools::parseProcedure(description, format, unparsedArguments_);
  unique_ptr<IAlignment> iAln;
  if (format == "Mase")
  {
    iAln.reset(new Mase());
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
      throw Exception("BppOAlignmentReaderFormat::read. Invalid argument 'order' for phylip format: " + order);

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
        throw Exception("BppOAlignmentReaderFormat::read. Invalid argument 'split' for phylip format: " + split);
    }
    else if (type == "classic")
      extended = false;
    else
      throw Exception("BppOAlignmentReaderFormat::read. Invalid argument 'type' for phylip format: " + type);

    iAln.reset(new Phylip(extended, sequential, 100, split));
  }
  else if (format == "Csv")
  {
    string sep = ApplicationTools::getStringParameter("sep", unparsedArguments_, ",", "", true, warningLevel_);
    bool header = ApplicationTools::getBooleanParameter("header", unparsedArguments_, true, "", true, warningLevel_);
    int rownames = ApplicationTools::getIntParameter("rownames", unparsedArguments_, -1, "", true, warningLevel_);
    iAln.reset(new Csv(sep, header, rownames));
  }
  else if (format == "Fasta")
  {
    bool strictNames = ApplicationTools::getBooleanParameter("strict_names", unparsedArguments_, false, "", true, warningLevel_);
    bool extended    = ApplicationTools::getBooleanParameter("extended", unparsedArguments_, false, "", true, warningLevel_);
    iAln.reset(new Fasta(100, true, extended, strictNames));
  }
  else if (format == "FastaCsv")
  {
    bool strictNames = ApplicationTools::getBooleanParameter("strict_names", unparsedArguments_, false, "", true, warningLevel_);
    bool extended    = ApplicationTools::getBooleanParameter("extended", unparsedArguments_, false, "", true, warningLevel_);
    iAln.reset(new FastaCsv(100, extended, strictNames));
  }
  else if (format == "Clustal")
  {
    unsigned int extraSpaces = ApplicationTools::getParameter<unsigned int>("extraSpaces", unparsedArguments_, 0, "", true, warningLevel_);
    iAln.reset(new Clustal(true, extraSpaces));
  }
  else if (format == "Dcse")
  {
    iAln.reset(new DCSE());
  }
  else if (format == "Nexus")
  {
    iAln.reset(new NexusIOSequence());
  }
  else
  {
    throw IOException("Sequence format '" + format + "' unknown.");
  }

  return iAln;
}


unique_ptr<IProbabilisticAlignment> BppOAlignmentReaderFormat::readProbabilistic(const std::string& description)
{
  unparsedArguments_.clear();
  string format = "";
  KeyvalTools::parseProcedure(description, format, unparsedArguments_);
  unique_ptr<IProbabilisticAlignment> iAln;

  if (format == "Pasta")
  {
    bool strictNames = ApplicationTools::getBooleanParameter("strict_names", unparsedArguments_, false, "", true, warningLevel_);
    bool extended    = ApplicationTools::getBooleanParameter("extended", unparsedArguments_, false, "", true, warningLevel_);
    iAln.reset(new Pasta(100, extended, strictNames));
  }
  else
  {
    throw IOException("Probabilistic Sequence format '" + format + "' unknown.");
  }

  return iAln;
}
