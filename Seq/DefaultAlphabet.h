
#ifndef DEFAULTALPHABET_H
#define DEFAULTALPHABET_H

#include "AbstractAlphabet.h"

/**
 * @brief The DefaultAlphabet class.
 *
 * This alphabet should match virtually any type of sequences.
 * This should be used by who does not care of the sequence type.
 */
class DefaultAlphabet : public AbstractAlphabet
{
	protected:
		static const string chars;
		
	public:
		// class constructor
		DefaultAlphabet();

		// class destructor
		~DefaultAlphabet() {}

	public:
		unsigned int getSize() const;
		unsigned int getNumberOfTypes() const;
		string getAlphabetType() const;
};

#endif
