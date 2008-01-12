//
// File: KleinAANetChargeIndex.h
// Created by: Julien Dutheil
// Created on: Wed Aug 31 17:48 2005
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

#ifndef _KLEINAANETCHARGEINDEX_H_
#define _KLEINAANETCHARGEINDEX_H_

#include "AlphabetIndex1.h"
#include "ProteicAlphabet.h"

namespace bpp
{

/**
 * @brief Charge of each amino acid.
 *
 * @code
 * Database: AAindex
 * Entry: KLEP840101
 *
 * H KLEP840101
 * D Net charge (Klein et al., 1984)
 * R LIT:1008055 PMID:6547351
 * A Klein, P., Kanehisa, M. and DeLisi, C.
 * T Prediction of protein function from sequence properties: Discriminant 
 * analysis of a data base
 * J Biochim. Biophys. Acta 787, 221-226 (1984)
 * C ZIMJ680104    0.941
 * I    A/L     R/K     N/M     D/F     C/P     Q/S     E/T     G/W     H/Y     I/V
 *       0.      1.      0.     -1.      0.      0.     -1.      0.      0.      0.
 *       0.      1.      0.      0.      0.      0.      0.      0.      0.      0.
 * //
 * @endcode
 */
class KleinAANetChargeIndex:
  public AlphabetIndex1<double>
{
	private:
		vector<double> _charge;
		const ProteicAlphabet * _alpha;

	public:
		KleinAANetChargeIndex()
		{
			_alpha = new ProteicAlphabet();
			_charge.resize(20);
			_charge[ 0] =  0.; //A
			_charge[ 1] =  1.; //R
			_charge[ 2] =  0.; //N
			_charge[ 3] = -1.; //D
			_charge[ 4] =  0.; //C
			_charge[ 5] =  0.; //Q
			_charge[ 6] = -1.; //E
			_charge[ 7] =  0.; //G
			_charge[ 8] =  0.; //H
			_charge[ 9] =  0.; //I
			_charge[10] =  0.; //L
			_charge[11] =  1.; //K
			_charge[12] =  0.; //M
			_charge[13] =  0.; //F
			_charge[14] =  0.; //P
			_charge[15] =  0.; //S
			_charge[16] =  0.; //T
			_charge[17] =  0.; //W
			_charge[18] =  0.; //Y
			_charge[19] =  0.; //V
		}

		virtual ~KleinAANetChargeIndex() { delete _alpha;	}
		
    KleinAANetChargeIndex * clone() const { return new KleinAANetChargeIndex(); }

	public:
		double getIndex(int state) const throw (BadIntException)
		{
			if(state < 0 || state > 19) throw BadIntException(state, "KleinAANetChargeIndex::getIndex(). Invalid state.", _alpha);
			return _charge[state];
		}
		
		double getIndex(const string & state) const throw (BadCharException)
		{
			return _charge[_alpha->charToInt(state)];
		}

    vector<double> * getIndexVector() const { return new vector<double>(_charge); }

		const Alphabet * getAlphabet() const { return _alpha; }

};

} //end of namespace bpp.

#endif //_KLEINAANETCHARGEINDEX_H_

