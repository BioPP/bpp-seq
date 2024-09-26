// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "PhylipDistanceMatrixFormat.h"
#include "IoDistanceMatrixFactory.h"

using namespace bpp;
using namespace std;

const std::string IODistanceMatrixFactory::PHYLIP_FORMAT = "Phylip";

unique_ptr<IDistanceMatrix> IODistanceMatrixFactory::createReader(const std::string& format, bool extended)
{
	if (format == PHYLIP_FORMAT)
		return make_unique<PhylipDistanceMatrixFormat>(extended);
	else
		throw Exception("Format " + format + " is not supported for input.");
}

unique_ptr<ODistanceMatrix> IODistanceMatrixFactory::createWriter(const std::string& format, bool extended)
{
	if (format == PHYLIP_FORMAT)
	{
		return make_unique<PhylipDistanceMatrixFormat>(extended);
	}
	else
		throw Exception("Format " + format + " is not supported for output.");
}
