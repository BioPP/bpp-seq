//
// File: GranthamAAVolumeIndex.h
// Created by: Julien Dutheil
// Created on: Tue Apr 21 2005
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

#ifndef _GRANTHAMAAVOLUMEINDEX_H_
#define _GRANTHAMAAVOLUMEINDEX_H_

#include "AlphabetIndex1.h"
#include "ProteicAlphabet.h"

/**
 * @brief Volume index used in Grantham (1974).
 *
 *Database: AAindex1
 *Entry: GRAR740103
 *
 *H GRAR740103
 *D Volume (Grantham, 1974)
 *R LIT:2004143b PMID:4843792
 *A Grantham, R.
 *T Amino acid difference formula to help explain protein evolution
 *J Science 185, 862-864 (1974)
 *I    A/L     R/K     N/M     D/F     C/P     Q/S     E/T     G/W     H/Y     I/V
 *     31.    124.     56.     54.     55.     85.     83.      3.     96.    111.
 *    111.    119.    105.    132.    32.5     32.     61.    170.    136.     84.
 * //
 */
class GranthamAAVolumeIndex: public AlphabetIndex1<double> {

	private:
		vector<double> _polarity;
		const ProteicAlphabet * _alpha;

	public:
		GranthamAAVolumeIndex() {
			_alpha = new ProteicAlphabet();
			_polarity.resize(20);
			_polarity[ 0] =  31.; //A
			_polarity[ 1] = 124.; //R
			_polarity[ 2] =  56.; //N
			_polarity[ 3] =  54.; //D
			_polarity[ 4] =  55.; //C
			_polarity[ 5] =  85.; //Q
			_polarity[ 6] =  83.; //E
			_polarity[ 7] =   3.; //G
			_polarity[ 8] =  96.; //H
			_polarity[ 9] = 111.; //I
			_polarity[10] = 111.; //L
			_polarity[11] = 119.; //K
			_polarity[12] = 105.; //M
			_polarity[13] = 132.; //F
			_polarity[14] =  32.5; //P
			_polarity[15] =  32.; //S
			_polarity[16] =  61.; //T
			_polarity[17] = 170.; //W
			_polarity[18] = 136.; //Y
			_polarity[19] =  84.; //V
		}

		~GranthamAAVolumeIndex() {
			delete _alpha;
		}

	public:
		double getIndex(int state) const throw (BadIntException) {
			if(state < 0 || state > 19) throw BadIntException(state, "GranthamAAVolumeIndex::getIndex(). Invalid state.", _alpha);
			return _polarity[state];
		}
		
		double getIndex(const string & state) const throw (BadCharException) {
			return _polarity[_alpha -> charToInt(state)];
		}

		const Alphabet * getAlphabet() const { return _alpha; }

		Clonable * clone() const { return new GranthamAAVolumeIndex(); }
};

#endif //_GRANTHAMAAVOLUMEINDEX_H_

