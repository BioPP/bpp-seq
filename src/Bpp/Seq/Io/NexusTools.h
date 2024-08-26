// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_NEXUSTOOLS_H
#define BPP_SEQ_IO_NEXUSTOOLS_H


// From the STL:
#include <iostream>
#include <Bpp/Exceptions.h>

namespace bpp
{
/**
 * @brief Tools for parsing Nexus files.
 *
 * The Nexus format is described in the following paper:
 * Maddison D, Swofford D, and Maddison W (1997), _Syst Biol_ 46(4):590-621
 *
 * @author Julien Dutheil
 */
class NexusTools
{
public:
/**
 * @param input The input stream.
 * @return A string containing the next line in the file wichi is not empty and is no a comment line.
 */
static std::string getNextNonCommentLine(std::istream& input);

/**
 * @brief parse the next command name within a block.
 *
 * @param input     [in]  The input stream.
 * @param name      [out] Will contain the name of the command.
 * @param arguments [out] Will contain the arguments of the commands, as raw data. The arguments will not be parsed.
 * @param lineBrk   [in]  Tell is the line break should be preserved in the arguments.
 * @return Whether a command was found in the current block.
 * @throw IOException In case of bad format.
 */
static bool getNextCommand(std::istream& input, std::string& name, std::string& arguments, bool lineBrk = true);
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_NEXUSTOOLS_H
