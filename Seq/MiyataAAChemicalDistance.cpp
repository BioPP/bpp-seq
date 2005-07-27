//
// File: MiyataAAChemicalDistance.cpp
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Feb 21 17:42 2005
//

// from the STL:
#include <string>

#include "MiyataAAChemicalDistance.h"
#include <NumCalc/NumTools.h>
using namespace NumTools;

MiyataAAChemicalDistance::MiyataAAChemicalDistance()
{
	// Build the alphabet:
	_alpha = new ProteicAlphabet();
	
	// Load the matrix:
	_distanceMatrix.resize(20, 20);
	#include "__MiyataMatrixCode"
}
MiyataAAChemicalDistance::~MiyataAAChemicalDistance() { delete _alpha; }

double MiyataAAChemicalDistance::getIndex(int state1, int state2) const 
throw (BadIntException) {
	if(state1 < 0 || state1 > 19) throw BadIntException(state1, "MiyataAAChemicalDistance::getIndex(). Invalid state1.", _alpha);
	if(state2 < 0 || state2 > 19) throw BadIntException(state2, "MiyataAAChemicalDistance::getIndex(). Invalid state2.", _alpha);
	double d = _distanceMatrix(state1, state2);
	return _sym ? NumTools::abs<double>(d) : d;
}

double MiyataAAChemicalDistance::getIndex(const string & state1, const string & state2) const
throw (BadCharException) {
	double d = _distanceMatrix(_alpha -> charToInt(state1), _alpha -> charToInt(state2));
	return _sym ? NumTools::abs(d) : d;
}

