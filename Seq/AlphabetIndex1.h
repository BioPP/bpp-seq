//
// File: AlphabetIndex1.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Mon Feb 21 17:42 2005
//

#ifndef _ALPHABETINDEX1_H_
#define _ALPHABETINDEX1_H_

// from the STL:
#include <string>

/**
 * @brief One dimensionnal alphabet index interface.
 *
 * Derivatives of this interface implement properties for a single state.
 */
template <class T>
class AlphabetIndex1 {

	public:
		AlphabetIndex1() {}
		virtual ~AlphabetIndex1() {}

	public:
		/**
		 * @brief Get the index associated to a state.
		 *
		 * @param state The state to consider, as a int value.
		 * @return The index associated to the state
		 */
		virtual T getIndex(int state) const = 0;

		/**
		 * @brief Get the index associated to a state.
		 *
		 * @param state The state to consider, as a string value.
		 * @return The index associated to the state
		 */
		virtual T getIndex(const string & state) const = 0;

};

#endif //_ALPHABETINDEX1_H_

