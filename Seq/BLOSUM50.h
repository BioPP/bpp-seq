//
// File: BLOSUM50.h
// Created by: Julien Dutheil
// Created on: Tue Jan 18 10:28 2007
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

#ifndef _BLOSUM50_H_
#define _BLOSUM50_H_

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

namespace bpp
{

/**
 * @brief BLOSUM 50 Substitution Matrix.
 *   
 * Reference:
 * Henikoff, S. and Henikoff, J.G.
 * Amino acid substitution matrices from protein blocks
 * Proc. Natl. Acad. Sci. USA 89, 10915-10919 (1992)
 *
 * Data from AAIndex2 database, Accession Number HENS920104.
 */
class BLOSUM50: public AlphabetIndex2<double>
{
	private:
		RowMatrix<double> _distanceMatrix;
		const ProteicAlphabet * _alpha;

	public:
		BLOSUM50();
		virtual ~BLOSUM50();

	public:
		/**
		 * @name Methods from the AlphabetIndex2 interface.
		 *
		 * @{
		 */
		double getIndex(int state1, int state2) const throw (BadIntException);
		double getIndex(const string & state1, const string & state2) const throw (BadCharException);
		const Alphabet * getAlphabet() const { return _alpha; };
		BLOSUM50 * clone() const { return new BLOSUM50(); }
		Matrix<double> * getIndexMatrix() const;
		/** @} */

};

} //end of namespace bpp.

#endif //_BLOSUM50_H_

