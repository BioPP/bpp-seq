// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/KeyvalTools.h>
#include <memory>
#include <string>

#include "BppOSequenceStreamReaderFormat.h"
#include "Fasta.h"

using namespace bpp;
using namespace std;

unique_ptr<ISequenceStream> BppOSequenceStreamReaderFormat::read(const std::string& description)
{
  unparsedArguments_.clear();
  string format = "";
  KeyvalTools::parseProcedure(description, format, unparsedArguments_);
  unique_ptr<ISequenceStream> iSeq;
  if (format == "Fasta")
  {
    bool strictNames = ApplicationTools::getBooleanParameter("strict_names", unparsedArguments_, false, "", true, false);
    bool extended    = ApplicationTools::getBooleanParameter("extended", unparsedArguments_, false, "", true, false);
    iSeq = make_unique<Fasta>(100, true, extended, strictNames);
  }
  else
  {
    throw IOException("Sequence format '" + format + "' unknown.");
  }

  return iSeq;
}
