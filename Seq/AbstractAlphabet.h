//
// File: AbstractAlphabet.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Tue Jul 22 2003
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

#ifndef _ABSTRACTALPHABET_H_
#define _ABSTRACTALPHABET_H_

#include "Alphabet.h"

// From Utils:
#include <Utils/Exceptions.h>

using namespace std;

namespace bpp
{

/**
 * @brief A partial implementation of the Alphabet interface.
 *
 * It defines a sletter structure used to store information for each state, and
 * contains a vector of this structure. All methods are based uppon this vector,
 * but do not provide any method to initialize it.
 * This is up to each constructor of the derived classes.
 *
 * @see Alphabet
 */
class AbstractAlphabet:
  public Alphabet
{
	protected:
		/**
		 * @brief sletter structure: an alphabet is a vector of sletter
		 */
		struct sletter
    {
           
			/**
			 * @brief State number id (i.e. -1 for gap (-))
			 */
			int num;

			/**
			 * @brief State string id. May be more than one char (for instance: codons)!
			 */
	    string letter;

			/**
			 * @brief Abbreviation for amino-acids (not used in nucleic alphabets).
			 */
    	string abbr;
			
			/**
			 * @brief Full name of the state (i.e. Adenine)
			 */
    	string name;
      
      bool operator == (AbstractAlphabet::sletter & l2)
      {
        return name == l2.name;
      }

		};
		
	protected:
		
		/**
		 * @brief Alphabet : vector of structure sletter.
		 */
		vector<sletter> alphabet;

    /**
     * @name Available codes
     *
     * These vectors will be computed the first time you call the getAvailableInts or getAvailableChars method.
     *
     * @{
     */
    mutable vector<string> _charList;
    mutable vector<int> _intList;
    /** @} */

	public:
		
		AbstractAlphabet() {}
		virtual ~AbstractAlphabet() {}
	
	public:
    
    /**
		 * @name Implement these methods from the Alphabet interface.
		 *
		 * @{
		 */
		unsigned int getNumberOfChars() const { return alphabet.size(); }
		string getName(const string & state) const throw (BadCharException);
		string getName(int state) const throw (BadIntException);
		int charToInt(const string & state) const throw (BadCharException);
		string intToChar(int state) const throw (BadIntException);
		bool isIntInAlphabet(int state) const;
		bool isCharInAlphabet(const string & state) const;
		vector<int> getAlias(int state) const throw (BadIntException);
		vector<string> getAlias(const string & state) const throw (BadCharException);
    int    getGeneric(const vector<int   > & states) const throw (BadIntException);
    string getGeneric(const vector<string> & states) const throw (AlphabetException);
    const vector<int> & getSupportedInts() const;
    const vector<string> & getSupportedChars() const;
    int getGapCharacterCode() const { return -1; }
    bool isGap(int state) const { return state == -1; }
    bool isGap(const string & state) const { return charToInt(state) == -1; }
		/** @} */
};

} //end of namespace bpp.

#endif // _ABSTRACTALPHABET_H_

