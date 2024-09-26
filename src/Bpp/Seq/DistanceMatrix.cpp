// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "DistanceMatrix.h"

using namespace std;

using namespace bpp;

size_t DistanceMatrix::getNameIndex(const std::string& name) const
{
	for (size_t i = 0; i < names_.size(); ++i)
	{
		if (names_[i] == name)
			return i;
	}
	throw Exception("DistanceMatrix::getNameIndex. Name not found: '" + name + "'.");
}
