//
// File: AAIndex1Entry.h
// Created by: Julien Dutheil
// Created on: Fri Jan 19 17:07 2007
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

#ifndef _AAINDEX1ENTRY_H_
#define _AAINDEX1ENTRY_H_

#include "AlphabetIndex1.h"
#include "ProteicAlphabet.h"

namespace bpp
{

/**
 * @brief Create a AlphabetIndex1 object from an AAIndex2 entry.
 */
class AAIndex1Entry:
  public AlphabetIndex1<double>
{
	private:
		vector<double> _property;
		const ProteicAlphabet * _alpha;

	public:
    /**
     * @brief Create a new AAIndex1Entry from an input stream.
     *
     * @param input The input stream to use.
     * @throw IOException if the stream content does not follow the AAIndex1 database entry format.
     */
		AAIndex1Entry(istream & input) throw (IOException);

    AAIndex1Entry(const AAIndex1Entry & index)
    {
      _property = index._property;
      _alpha = new ProteicAlphabet();
    }

    AAIndex1Entry & operator=(const AAIndex1Entry & index)
    {
      _property = index._property;
      _alpha = new ProteicAlphabet();
      return *this;
    }

		virtual ~AAIndex1Entry() { delete _alpha;	}
		
    AAIndex1Entry * clone() const { return new AAIndex1Entry(*this); }

	public:
		double getIndex(int state) const throw (BadIntException)
		{
			if(state < 0 || state > 19) throw BadIntException(state, "KleinAANetChargeIndex::getIndex(). Invalid state.", _alpha);
			return _property[state];
		}
		
		double getIndex(const string & state) const throw (BadCharException)
		{
			return _property[_alpha->charToInt(state)];
		}

    vector<double> * getIndexVector() const { return new vector<double>(_property); }

		const Alphabet * getAlphabet() const { return _alpha; }

};

} //end of namespace bpp.

#endif //_AAINDEX1ENTRY_H_

