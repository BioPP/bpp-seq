//
// File: SimpleIndexDistance.h
// Created by: Julien Dutheil
// Created on: Tue Apr 21 2005
//

/*
Copyright or © or Copr. Julien Dutheil, (November 17, 2004)

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

#ifndef _SIMPLEINDEXDISTANCE_H_
#define _SIMPLEINDEXDISTANCE_H_

// from the STL:
#include <string>
using namespace std;

#include "AlphabetIndex1.h"
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
 * @brief Simple dissimilarity distance.
 *
 * Take a one-dimensional index end return the difference between the
 * indexes of two states.
 */
template<class T>
class SimpleIndexDistance:
  public AlphabetIndex2<T>
{

	private:
		AlphabetIndex1<T> * _index;
		bool _sym;

	public:
		SimpleIndexDistance(const AlphabetIndex1<T> & index) { _index = (AlphabetIndex1<T> *)index.clone(); }
		virtual ~SimpleIndexDistance() { delete _index; }

	public:
		T getIndex(int state1, int state2) const throw (BadIntException)
		{
			if(state1 < 0 || state1 > 19) throw BadIntException(state1, "SimpleIndexDistance::getIndex(). Invalid state1.", _index -> getAlphabet());
			if(state2 < 0 || state2 > 19) throw BadIntException(state2, "SimpleIndexDistance::getIndex(). Invalid state2.", _index -> getAlphabet());
			T d = _index -> getIndex(state2) - _index -> getIndex(state1);
			return _sym ? NumTools::abs<double>(d) : d;
		}
		
		T getIndex(const string & state1, const string & state2) const throw (BadCharException)
    {
			T d = _index -> getIndex(state2) - _index -> getIndex(state1);
			return _sym ? NumTools::abs<double>(d) : d;
		}

		const Alphabet * getAlphabet() const { return _index -> getAlphabet(); }

		Clonable * clone() const { return new SimpleIndexDistance<T>(* _index); }

    Matrix<T> * getIndexMatrix() const
    {
	    RowMatrix<T> * m = new RowMatrix<T>(20, 20);
		  for(unsigned int i = 0; i < 20; i++) {
			  for(unsigned int j = 0; j < 20; j++) {
			    (* m)(i,j) = getIndex(i,j);
			  }
		  }
	    return m;
    }
    
	public:
		void setSymmetric(bool yn) { _sym = yn; }
		bool isSymmetric() const { return _sym; }
    /**
     * @return The AlphabetIndex1 object associated to this object.
     */
    const AlphabetIndex1<T> * getAlphabetIndex1() const { return _index; }
    /**
     * @return The AlphabetIndex1 object associated to this object.
     */
    AlphabetIndex1<T> * getAlphabetIndex1() { return _index; }

};

} //end of namespace bpp.

#endif //_SIMPLEINDEXDISTANCE_H_

