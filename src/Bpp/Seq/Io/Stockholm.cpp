// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>

#include "../StringSequenceTools.h"
#include "Stockholm.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

void Stockholm::writeAlignment(ostream& output, const SiteContainerInterface& sc) const
{
	if (!output)
		throw IOException("Stockholm::writeAlignment: can't write to ostream output");

	output << "# STOCKHOLM 1.0" << endl;
	// Loop for all general comments
	for (size_t i = 0; i < sc.getComments().size(); ++i)
	{
		output << "#=GF CC " << sc.getComments()[i] << endl;
	}

	// Main loop : for all sequences in vector container
	vector<string> names = sc.getSequenceNames();
	size_t maxSize = 0;
	for (size_t i = 0; i < names.size(); ++i)
	{
		names[i] = TextTools::removeWhiteSpaces(names[i]);
		if (names[i].size() > maxSize)
			maxSize = names[i].size();
	}
	if (maxSize > 255)
		maxSize = 255;
	for (size_t i = 0; i < sc.getNumberOfSequences(); ++i)
	{
		output << TextTools::resizeRight(names[i], maxSize) << " " << sc.sequence(i).toString() << endl;
	}
	output << "//" << endl;
}

/******************************************************************************/
