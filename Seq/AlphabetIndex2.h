//
// File: AlphabetIndex2.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Feb 21 17:42 2005
//

#ifndef _ALPHABETINDEX1_H_
#define _ALPHABETINDEX1_H_

// from the STL:
#include <string>
using namespace std;

/**
 * @brief Two dimensionnal alphabet index interface.
 *
 * Derivatives of this interface implement distances between two states.
 */
template <class T>
class AlphabetIndex2 {

	public:
		AlphabetIndex2() {}
		virtual ~AlphabetIndex2() {}

	public:
		/**
		 * @brief Get the index associated to a pair of states.
		 *
		 * @param state1 First state to consider, as a int value.
		 * @param state2 Second state to consider, as a int value.
		 * @return The index associated to the pair of states
		 */
		virtual T getIndex(int state1, int state2) const = 0;
		
		/**
		 * @brief Get the index associated to a pair of states.
		 *
		 * @param state1 First state to consider, as a string value.
		 * @param state1 Second state to consider, as a string value.
		 * @return The index associated to the pair of states
		 */
		virtual T getIndex(const string & state1, const string & state2) const = 0;

		// @TODO:
		// Matrix<T, T> getIndexMatrix() const = 0;

};

#endif //_ALPHABETINDEX2_H_

