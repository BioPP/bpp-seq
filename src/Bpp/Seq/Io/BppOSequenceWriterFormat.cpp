// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/KeyvalTools.h>
#include <memory>
#include <string>

#include "BppOSequenceWriterFormat.h"
#include "Fasta.h"
#include "Mase.h"

using namespace bpp;
using namespace std;

unique_ptr<OSequence> BppOSequenceWriterFormat::read(const std::string& description)
{
  unparsedArguments_.clear();
  string format = "";
  KeyvalTools::parseProcedure(description, format, unparsedArguments_);
  unsigned int ncol = ApplicationTools::getParameter<unsigned int>("length", unparsedArguments_, 100, "", true, warningLevel_);
  unique_ptr<OSequence> oSeq;
  if (format == "Fasta")
  {
    oSeq.reset(new Fasta(ncol));
  }
  else if (format == "Mase")
  {
    oSeq.reset(new Mase(ncol));
  }
  else
  {
    throw IOException("Sequence format '" + format + "' unknown.");
  }

  return oSeq;
}
