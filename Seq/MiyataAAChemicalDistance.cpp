//
// File: MiyataAAChemicalDistance.cpp
// Created by: Julien Dutheil
// Created on: Mon Feb 21 17:42 2005
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for sequences analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

// from the STL:
#include <string>

using namespace std;

#include "MiyataAAChemicalDistance.h"
#include <NumCalc/NumTools.h>

using namespace bpp;

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
	double d = _distanceMatrix(_alpha->charToInt(state1), _alpha->charToInt(state2));
	return _sym ? NumTools::abs(d) : d;
}

Matrix<double> * MiyataAAChemicalDistance::getIndexMatrix() const
{
	RowMatrix<double> * m = new RowMatrix<double>(_distanceMatrix);
	if(_sym)
  {
		for(unsigned int i = 0; i < 20; i++)
    {
			for(unsigned int j = 0; j < 20; j++)
      {
				(* m)(i,j) = NumTools::abs<double>((*m)(i,j));
			}
		}
	}
	return m;
}

