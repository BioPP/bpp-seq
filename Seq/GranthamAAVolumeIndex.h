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

namespace bpp
{

/**
 * @brief Volume index used in Grantham (1974).
 *
 * @code
 * Database: AAindex1
 * Entry: GRAR740103
 *
 * H GRAR740103
 * D Volume (Grantham, 1974)
 * R LIT:2004143b PMID:4843792
 * A Grantham, R.
 * T Amino acid difference formula to help explain protein evolution
 * J Science 185, 862-864 (1974)
 * I    A/L     R/K     N/M     D/F     C/P     Q/S     E/T     G/W     H/Y     I/V
 *      31.    124.     56.     54.     55.     85.     83.      3.     96.    111.
 *     111.    119.    105.    132.    32.5     32.     61.    170.    136.     84.
 * //
 * @endcode
 */
class GranthamAAVolumeIndex:
  public AlphabetIndex1<double>
{
	private:
		vector<double> _volume;
		const ProteicAlphabet * _alpha;

	public:
		GranthamAAVolumeIndex()
		{
			_alpha = new ProteicAlphabet();
			_volume.resize(20);
			_volume[ 0] =  31.; //A
			_volume[ 1] = 124.; //R
			_volume[ 2] =  56.; //N
			_volume[ 3] =  54.; //D
			_volume[ 4] =  55.; //C
			_volume[ 5] =  85.; //Q
			_volume[ 6] =  83.; //E
			_volume[ 7] =   3.; //G
			_volume[ 8] =  96.; //H
			_volume[ 9] = 111.; //I
			_volume[10] = 111.; //L
			_volume[11] = 119.; //K
			_volume[12] = 105.; //M
			_volume[13] = 132.; //F
			_volume[14] =  32.5;//P
			_volume[15] =  32.; //S
			_volume[16] =  61.; //T
			_volume[17] = 170.; //W
			_volume[18] = 136.; //Y
			_volume[19] =  84.; //V
		}

		virtual ~GranthamAAVolumeIndex() { delete _alpha;	}
		
    GranthamAAVolumeIndex * clone() const { return new GranthamAAVolumeIndex(); }

	public:
		double getIndex(int state) const throw (BadIntException)
		{
			if(state < 0 || state > 19) throw BadIntException(state, "GranthamAAVolumeIndex::getIndex(). Invalid state.", _alpha);
			return _volume[state];
		}
		
		double getIndex(const string & state) const throw (BadCharException)
		{
			return _volume[_alpha->charToInt(state)];
		}

    vector<double> * getIndexVector() const { return new vector<double>(_volume); }

		const Alphabet * getAlphabet() const { return _alpha; }

};

} //end of namespace bpp.

#endif //_GRANTHAMAAVOLUMEINDEX_H_

