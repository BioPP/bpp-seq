// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/NumTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>

#include "PhredPoly.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

PhredPoly::PhredPoly(double ratio) : ratio_(ratio) {
}

/******************************************************************************/

bool PhredPoly::nextSequence(istream& input, Sequence& seq) const
{
	if (!input)
	{
		throw IOException ("PhredPoly::read: fail to open stream");
	}

	string temp, name, sequence = ""; // Initialization
	bool flag = false;

	// Read first line
	if (!input.eof())
	{
		getline(input, temp, '\n'); // Copy current line in temporary string
		StringTokenizer st(temp, " ");
		name = st.getToken(0);
	}

	auto alphaPtr = seq.getAlphabet();

	// Main loop : for all other lines
	while (!input.eof())
	{
		getline(input, temp, '\n'); // Copy current line in temporary string
		StringTokenizer st(temp, " ");
		if (st.numberOfRemainingTokens() == 12)
		{
			double a = TextTools::toDouble(st.getToken(3));
			double b = TextTools::toDouble(st.getToken(7));
			if (a < b)
			{
				NumTools::swap(a, b);
			}
			vector<string> v;
			v.push_back(st.getToken(0)); // Get the called base
			if (b / a > this->ratio_)
			{
				v.push_back(st.getToken(4)); // Get the uncalled base if relative picks areas are similar
			}
			sequence += alphaPtr->getGeneric(v);
		}
	}
	if (name == "")
	{
		throw Exception("PhredPoly::read: sequence without name!");
	}
	else
	{
		seq.setName(name);
		seq.setContent(sequence);
		flag = true;
	}
	return flag;
}

/******************************************************************************/
