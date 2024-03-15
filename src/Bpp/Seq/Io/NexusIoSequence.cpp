// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/KeyvalTools.h>
#include <Bpp/Text/TextTools.h>

#include "../Alphabet/AlphabetTools.h"
#include "../Container/SiteContainerTools.h"
#include "NexusIoSequence.h"
#include "NexusTools.h"

using namespace bpp;

// From the STL:
#include <sstream>

using namespace std;

/******************************************************************************/

const std::vector<std::string> NexusIOSequence::splitNameAndSequence_(const std::string& s) const
{
  vector<string> v(2);
  string::size_type index = s.find(" ");
  if (index == string::npos)
    throw Exception("NexusIOSequence::splitNameAndSequence_(). No sequence name found.");
  v[0] = TextTools::removeSurroundingWhiteSpaces(s.substr(0, index));
  v[1] = TextTools::removeFirstWhiteSpaces(s.substr(index + 1));
  return v;
}


/******************************************************************************/

void NexusIOSequence::appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& vsc) const
{
  // Checking the existence of specified file
  if (!input)
  {
    throw IOException ("NexusIOSequence::read(). Fail to open file");
  }

  // Look for the DATA block:
  string line = "";
  while (TextTools::toUpper(line) != "BEGIN DATA;")
  {
    if (input.eof())
      throw Exception("NexusIOSequence::appendFromStream(). No data block was found.");
    line = TextTools::removeSurroundingWhiteSpaces(FileTools::getNextLine(input));
  }

  // Look for the DIMENSIONS command:
  string cmdName = "", cmdArgs = "";
  while (cmdName != "DIMENSIONS")
  {
    if (input.eof())
      throw Exception("NexusIOSequence::appendFromStream(). No DIMENSIONS command was found.");
    NexusTools::getNextCommand(input, cmdName, cmdArgs);
    cmdName = TextTools::toUpper(cmdName);
  }
  map<string, string> args;
  KeyvalTools::multipleKeyvals(cmdArgs, args, " ");
  map<string, string> argsUp;
  for (map<string, string>::iterator it = args.begin(); it != args.end(); it++)
  {
    argsUp[TextTools::toUpper(it->first)] = it->second;
  }
  if (argsUp["NTAX"] == "")
    throw Exception("NexusIOSequence::appendFromStream(). DIMENSIONS command does not have a NTAX argument.");
  size_t ntax = TextTools::to<size_t>(argsUp["NTAX"]);

  // Look for the FORMAT command:
  while (cmdName != "FORMAT")
  {
    if (input.eof())
      throw Exception("NexusIOSequence::appendFromStream(). No FORMAT command was found.");
    NexusTools::getNextCommand(input, cmdName, cmdArgs);
    cmdName = TextTools::toUpper(cmdName);
  }
  if (TextTools::hasSubstring(cmdArgs, "TRANSPOSE"))
    throw Exception("NexusIOSequence::appendFromStream(). TRANSPOSE option is not supported.");

  // Check if the alignment is dotted or not:
  bool matchChar = TextTools::hasSubstring(TextTools::toUpper(cmdArgs), "MATCHCHAR");

  auto alphaPtr = std::dynamic_pointer_cast<const Alphabet>(AlphabetTools::DEFAULT_ALPHABET);
  auto alignment = make_unique<AlignedSequenceContainer>(alphaPtr);

  // Look for the MATRIX command:
  line = "";
  while (!TextTools::startsWith(TextTools::toUpper(line), "MATRIX"))
  {
    if (input.eof())
      throw Exception("NexusIOSequence::appendFromStream(). No MATRIX command was found.");
    line = TextTools::removeSurroundingWhiteSpaces(FileTools::getNextLine(input));
  }
  line = FileTools::getNextLine(input);

  vector<string> names, seqs;
  // Read first block:
  bool commandFinished = false;
  for (size_t i = 0; i < ntax && !input.eof(); ++i)
  {
    if (TextTools::endsWith(line, ";"))
    {
      if (i < ntax - 1)
        throw IOException("NexusIOSequence::appendFromStream. Early end of MATRIX command, some sequences are missing.");
      else
      {
        commandFinished = true;
        line = line.substr(0, line.size() - 1); // Remove trailing semi-colon.
      }
    }
    vector<string> v = splitNameAndSequence_(line);
    names.push_back(v[0]);
    seqs.push_back(v[1]);
    line = FileTools::getNextLine(input);
  }

  // Then read all other blocks:
  commandFinished = TextTools::removeSurroundingWhiteSpaces(line) == ";"; // In case the end of command is on a separate line.
  while (!commandFinished)
  {
    for (size_t i = 0; i < ntax && !input.eof(); ++i)
    {
      if (TextTools::endsWith(line, ";"))
      {
        if (i < ntax - 1)
          throw IOException("NexusIOSequence::appendFromStream. Early end of MATRIX command, some sequences are missing.");
        else
        {
          commandFinished = true;
          line = line.substr(0, line.size() - 1); // Remove trailing semi-colon.
        }
      }

      vector<string> v = splitNameAndSequence_(line);
      if (v[0] != names[i])
        throw IOException("NexusIOSequence::appendFromStream. Bad file, the sequences are not in the same order in interleaved blocks, or one taxon is missing.");
      seqs[i] += v[1];
      line = FileTools::getNextLine(input);
      commandFinished = TextTools::removeSurroundingWhiteSpaces(line) == ";"; // In case the end of command is on a separate line.
    }
  }
  for (size_t i = 0; i < names.size(); ++i)
  {
    auto seqPtr = make_unique<Sequence>(names[i], seqs[i], alphaPtr);
    alignment->addSequence(seqPtr->getName(), seqPtr);
  }

  if (matchChar)
  {
    // Now we resolve the alignment:
    auto resolvedAlignment =
      SiteContainerTools::resolveDottedAlignment(*alignment, alphaPtr);
    for (size_t i = 0; i < resolvedAlignment->getNumberOfSequences(); ++i)
    {
      auto seqPtr = unique_ptr<Sequence>(resolvedAlignment->sequence(i).clone());
      vsc.addSequence(seqPtr->getName(), seqPtr);
    }
  }
  else
  {
    for (size_t i = 0; i < alignment->getNumberOfSequences(); ++i)
    {
      auto seqPtr = unique_ptr<Sequence>(alignment->sequence(i).clone());
      vsc.addSequence(seqPtr->getName(), seqPtr);
    }
  }
}

/******************************************************************************/

const std::string NexusIOSequence::getFormatName() const { return "Nexus"; }

/******************************************************************************/

const std::string NexusIOSequence::getFormatDescription() const
{
  return "Nexus file format.";
}

/******************************************************************************/
