// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "DefaultNucleotideScore.h"

// from the STL:
#include <string>

using namespace std;
using namespace bpp;

DefaultNucleotideScore::DefaultNucleotideScore(const NucleicAlphabet* alphabet) :
	distanceMatrix_(4, 4),
	alpha_(alphabet)
{
	// Load the matrix:
	distanceMatrix_(0, 0) = 10;
	distanceMatrix_(0, 1) = -3;
	distanceMatrix_(0, 2) = -1;
	distanceMatrix_(0, 3) = -4;

	distanceMatrix_(1, 0) = -3;
	distanceMatrix_(1, 1) = 9;
	distanceMatrix_(1, 2) = -5;
	distanceMatrix_(1, 3) = 0;

	distanceMatrix_(2, 0) = -1;
	distanceMatrix_(2, 1) = -5;
	distanceMatrix_(2, 2) = 7;
	distanceMatrix_(2, 3) = -3;

	distanceMatrix_(3, 0) = -4;
	distanceMatrix_(3, 1) = 0;
	distanceMatrix_(3, 2) = -3;
	distanceMatrix_(3, 3) = 8;
}

double DefaultNucleotideScore::getIndex(int state1, int state2) const
{
	if (!alpha_->isUnresolved(state1) && !alpha_->isUnresolved(state2))
		return distanceMatrix_(
			alpha_->getStateIndex(state1) - 1,
			alpha_->getStateIndex(state2) - 1);

	vector<int> states1 = alpha_->getAlias(state1);
	vector<int> states2 = alpha_->getAlias(state2);
	double score = -5;
	double tmp_score;
	for (size_t i = 0; i < states1.size(); i++)
	{
		for (size_t j = 0; j < states2.size(); j++)
		{
			tmp_score = getIndex(states1[i], states2[j]);
			if (tmp_score > score)
				score = tmp_score;
		}
	}
	return score / static_cast<double>(states1.size() + states2.size() - 1);
}

double DefaultNucleotideScore::getIndex(const std::string& state1, const std::string& state2) const
{
	return distanceMatrix_(
		alpha_->getStateIndex(state1) - 1,
		alpha_->getStateIndex(state2) - 1);
}

const Matrix<double>& DefaultNucleotideScore::getIndexMatrix() const
{
	return distanceMatrix_;
}
