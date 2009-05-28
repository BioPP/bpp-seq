//
// File: WordAlphabet.h
// Created by: Laurent Gueguen
// Created on: Sun Dec 28 2008
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for sequences analysis.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software. You can use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and rights to copy,
modify and redistribute granted by the license, users are provided
only with a limited warranty and the software's author, the holder of
the economic rights, and the successive licensors have only limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading, using, modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean that it is complicated to manipulate, and that also
therefore means that it is reserved for developers and experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards
their requirements in conditions enabling the security of their
systems and/or data to be ensured and, more generally, to use and
operate it in the same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _WORDALPHABET_H_
#define _WORDALPHABET_H_

#include "AbstractAlphabet.h"

// From the STL:
#include <string>
#include <vector>

#include "Sequence.h"

using namespace std;

namespace bpp
{

/**
 * @brief The abstract base class for word alphabets.
 * 
 * These alphabets are compounds of several alphabets. The only
 * constraint on these alphabets is that their words have length one
 * (so it is not possible to make WordAlphabets from other
 * WordAlphabets). The construction is made from a vector of pointers
 * to AbstractAlphabets. 
 *
 * The strings of the WordAlphabet are concatenations of the strings
 * of the Alphabets. They are made from the resolved letters of the
 * Alphabets.
 */
class WordAlphabet:
  public AbstractAlphabet
{
protected:

  vector<const Alphabet* > _VAbsAlph;

public: // Constructor and destructor.
		
  /**
   * @brief Builds a new word alphabet from a vector of Alphabets.
   *
   * @param Valpha The vector of Alphabets to be used.
   */
  WordAlphabet(const vector<const Alphabet* >& Valpha);
  
  /**
   * @brief Builds a new word alphabet from a pointer to number of
   * Alphabets.
   *
   * @param palpha The Pointer to the Alphabet to be used.
   * @param num the length of the words.
   */
  WordAlphabet(const Alphabet* palpha, unsigned int num);
  virtual ~WordAlphabet() {}
  
public:
  
  /**
   * @name Methods redefined from Alphabet
   *
   * @{
   */
  string getName(const string & state) const throw (BadCharException);
  int charToInt(const string & state) const throw (BadCharException);
  unsigned int getSize() const;
  
  /** @} */

  /**
   * @brief Returns is the letters Alphabet in the word are the same
   * type
   * 
   */
  
  bool hasUniqueAlphabet() const;

    /**
   * @brief Returns the length of the word
   * 
   */
  unsigned int getLength() const;

  /**
   * @brief Returns the number of resolved states + one for unresolved
   * 
   */
  unsigned int getNumberOfTypes() const;
  
  string getAlphabetType() const;
  int getUnknownCharacterCode() const;

  bool isUnresolved(int state) const { return state == getUnknownCharacterCode(); }
  bool isUnresolved(const string & state) const { return charToInt(state) == getUnknownCharacterCode(); }

private:
  
  /**
   * @name Inner utilitary functions
   *
   * @{
   */
  bool containsUnresolved(const string & state) const throw (BadCharException);
  bool containsGap(const string & state) const throw (BadCharException);
  void build_();
  /** @} */

  
public:
  
  /**
   * @name Word specific methods
   *
   * @{
   */
  
  /**
   * @brief Get the pointer to the Alphabet  of the n-position.
   * 
   * @param n The position in the word (starting at 0).
   * @return The pointer to the Alphabet of the n-position.
   */
  const Alphabet* getNAlphabet(unsigned int n) const;

  /**
   * @brief Get the int code for a word given the int code of the underlying positions.
   *
   * The int code of each position must match the corresponding alphabet specified at this position.
   * @param vint vector<int>  description for all the positions.
   * @param pos=0 the start position to match in the vector.
   * @return The int code of the word.
   */
  virtual int getWord(vector<int>& vint, int pos=0) const throw (BadIntException);
  
  /**
   * @brief Get the char code for a word given the char code of the
   * underlying positions.
   *
   * The char code of each position must match the corresponding alphabet specified at this position.
   * @param vpos vector<string>  vector description for all the positions.
   * @param pos=0 the start position to match in the vector.
   * @return The string of the word.
   */
  virtual string getWord(const vector<string>& vpos, int pos=0) const throw (BadIntException, BadCharException);
  

  /**
   * @brief Get the int code of the n-position of a word given its int description.
   * 
   * @param word The int description of the word.
   * @param n The position in the word (starting at 0).
   * @return The int description of the n-position of the word.
   */
  virtual int getNPosition(int word, unsigned int n) const throw (BadIntException);
  
  /**
   * @brief Get the int codes of each position of a word given its int description.
   * 
   * @param word The int description of the word.
   * @return The int description of the positions of the codon.
   */
  virtual vector<int> getPositions(int word) const throw (BadIntException);
  
  /**
   * @brief Get the char code of the n-position of a word given its char description.
   * 
   * @param word The char description of the word.
   * @param n The position in the word (starting at 0).
   * @return The char description of the n-position of the word.
   */
  virtual string getNPosition (const string & word, unsigned int n) const throw (BadCharException);
  
  /**
   * @brief Get the char codes of each position of a word given its char description.
   * 
   * @param word The char description of the word.
   * @return The char description of the three positions of the word.
   */
  virtual vector<string> getPositions(const string & word) const throw (BadCharException);

  /**
   * @brief Translate a whole sequence from letters alphabet to words alphabet.
   *
   * @param sequence A sequence in letters alphabet.
   * @param pos the start postion (default 0)
   * @return The corresponding sequence in words alphabet.
   * @throw AlphabetMismatchException If the sequence alphabet do not match the source alphabet.
   * @throw Exception                 Other kind of error, depending on the implementation.
   */
  Sequence * translate(const Sequence & sequence, int pos=0) const throw (AlphabetMismatchException, Exception);

  /**
   * @brief Translate a whole sequence from words alphabet to letters alphabet.
   *
   * @param sequence A sequence in words alphabet.
   * @return The corresponding sequence in letters alphabet.
   * @throw AlphabetMismatchException If the sequence alphabet do not match the target alphabet.
   * @throw Exception                 Other kind of error, depending on the implementation.
   */
  Sequence * reverse(const Sequence & sequence) const throw (AlphabetMismatchException, Exception);
  
  /** @} */
};

} //end of namespace bpp.

#endif	//_WORDALPHABET_H_

