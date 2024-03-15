// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/TextTools.h>

#include "NexusTools.h"

using namespace bpp;
using namespace std;

std::string NexusTools::getNextNonCommentLine(std::istream& input)
{
  string line = TextTools::removeSurroundingWhiteSpaces(FileTools::getNextLine(input));
  bool test = true;
  unsigned int countOpen = 0;
  unsigned int countClosed = 0;
  while (test)
  {
    if (line[0] == '[')
    {
      countOpen++;
    }
    if (line[line.size() - 1] == ']')
    {
      countClosed++;
    }
    if (countOpen > 0)
      line = TextTools::removeSurroundingWhiteSpaces(FileTools::getNextLine(input));
    if (countOpen == countClosed)
      test = false;
  }
  return line;
}


bool NexusTools::getNextCommand(std::istream& input, std::string& name, std::string& arguments, bool lineBrk)
{
  // Checking if the stream is readable
  if (!input)
  {
    throw IOException ("NexusTools::getNextCommand(). Failed to read from stream");
  }

  string line = TextTools::removeSurroundingWhiteSpaces(getNextNonCommentLine(input));
  if (TextTools::startsWith(line, "BEGIN"))
  {
    return false;
  }

  // Check if the command stands on one line:
  bool commandComplete = TextTools::endsWith(line, ";");
  if (commandComplete)
    line = line.substr(0, line.size() - 1);
  // Get the command name, as the first block:
  string::size_type limit = line.find(" ");
  if (limit == string::npos)
  {
    name = line;
    arguments = "";
    if (commandComplete)
    {
      // Command with no argument:
      return true;
    }
  }
  else
  {
    name = line.substr(0, limit);
    arguments = line.substr(limit + 1);
  }
  // Then parse the next lines:
  while (!commandComplete)
  {
    if (input.eof())
    {
      throw IOException ("NexusTools::getNextCommand(). Reached end of file before the end of the command could be found");
    }
    line = TextTools::removeSurroundingWhiteSpaces(getNextNonCommentLine(input));
    commandComplete = TextTools::endsWith(line, ";");
    if (commandComplete)
      line = line.substr(0, line.size() - 1);
    if (lineBrk)
      arguments += "\n";
    arguments += line;
  }
  return true;
}
