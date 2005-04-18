//
// File: MiyataAAChemicalDistance.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Feb 21 17:42 2005
//

#ifndef _MIYATAAACHEMICALDISTANCE_H_
#define _MIYATAAACHEMICALDISTANCE_H_

// from the STL:
#include <string>
using namespace std;

#include "AlphabetIndex2.h"
#include "ProteicAlphabet.h"
#include "AlphabetExceptions.h"

// From Utils:
#include <Utils/Exceptions.h>
// From NumCalc:
#include <NumCalc/Matrix.h>

// From the MTL:
//#include <mtl/matrix.h>
//using namespace mtl;
//typedef matrix<double>::type Matrix;
typedef RowMatrix<double> Mat;


/**
 * @brief One dimensionnal alphabet index interface.
 *
 * Derivatives of this interface implement properties for a single state.
 */
class MiyataAAChemicalDistance: public AlphabetIndex2<double> {

	private:
		Mat _distanceMatrix;
		const ProteicAlphabet * _alpha;
		bool _sym;

	public:
		MiyataAAChemicalDistance() throw (IOException);
		~MiyataAAChemicalDistance();

	public:
		double getIndex(int state1, int state2) const throw (BadIntException);
		double getIndex(const string & state1, const string & state2) const throw (BadCharException);

	public:
		void setSymmetric(bool yn) { _sym = yn; }
		bool isSymmetric() const { return _sym; }

};

#endif //_MIYATAAACHEMICALDISTANCE_H_

