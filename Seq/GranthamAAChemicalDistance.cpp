//
// File: GranthamAAChemicalDistance.cpp
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Feb 21 17:42 2005
//

// from the STL:
#include <string>

#include "GranthamAAChemicalDistance.h"

GranthamAAChemicalDistance::GranthamAAChemicalDistance() throw (IOException)
{
	// Build the alphabet:
	_alpha = new ProteicAlphabet();
	
	// Load the matrix:
	_distanceMatrix = Matrix(20, 20);
	#include "__GranthamMatrixCode.cpp"
}
GranthamAAChemicalDistance::~GranthamAAChemicalDistance() { delete _alpha; }

double GranthamAAChemicalDistance::getIndex(int state1, int state2) const 
throw (BadIntException) {
	if(state1 < 0 || state1 > 19) throw BadIntException(state1, "GranthamAAChemicalDistance::getIndex(). Invalid state1.", _alpha);
	if(state2 < 0 || state2 > 19) throw BadIntException(state2, "GranthamAAChemicalDistance::getIndex(). Invalid state2.", _alpha);
	double d = _distanceMatrix(state1, state2);
	return _sym ? abs(d) : d;
}

double GranthamAAChemicalDistance::getIndex(const string & state1, const string & state2) const
throw (BadCharException) {
	double d = _distanceMatrix(_alpha -> charToInt(state1), _alpha -> charToInt(state2));
	return _sym ? abs(d) : d;
}

