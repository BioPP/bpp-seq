/*
 * File AbstractAlphabet.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
 */

// Secured inclusion of header's file
#ifndef _ABSTRACTALPHABET_H_
#define _ABSTRACTALPHABET_H_

#include "Alphabet.h"

// From Utils:
#include <Utils/Exceptions.h>

/**
 * @brief A low level implementation of the Alphabet interface.
 *
 * This class implements common method to all alphabets.
 * It define a sletter structure used to store information for each state and a
 * contains a vector of this structure. All methods are based uppon this vector,
 * but does not provide any method to initialized it. This must be done by each
 * constructor of the derived classes.
 *
 * @see Alphabet
 */
class AbstractAlphabet : public Alphabet
{
	protected:
		// sletter structure : an alphabet is a vector of sletter
		struct sletter {
            
			/**
			 * @brief Base or acid n° (i.e. -1 for gap (-))
			 */
			int num;

			/**
			 * @brief The char of base or amino-acid. May be more than one char (codons)!
			 */
	    	string letter;

			/**
			 * @brief Abbreviation for amino-acids (not used in nucleic alphabets).
			 */
	    	string abbr;

			/**
			 * @brief Full name of base or acid (i.e. Adenine)
			 */
    		string name;
		};
		friend bool operator == (AbstractAlphabet::sletter &, AbstractAlphabet::sletter &);

	protected:
		
		/**
		 * @brief Alphabet : vector of struct sletter (defined in Alphabet.h).
		 */
		vector<sletter> alphabet;

    public: // Class destructor
		~AbstractAlphabet() {}
	
	public:
    
    /**
		 * @name Implement these methods from the Alphabet interface.
		 *
		 * @{
		 */
		unsigned int getNumberOfChars() const;
		string getName(const string & letter) const throw (BadCharException);
		string getName(int            letter) const throw (BadIntException);
		int charToInt(const string & letter) const throw (BadCharException);
		string intToChar(int letter) const throw (BadIntException);
		bool  isIntInAlphabet(      int      letter) const;
		bool isCharInAlphabet(const string & letter) const;
		vector<int   > getAlias(      int      state) const throw (BadIntException);
		vector<string> getAlias(const string & state) const throw (BadCharException);
		/** @} */
};

bool operator == (AbstractAlphabet::sletter &, AbstractAlphabet::sletter &);

#endif // _ABSTRACTALPHABET_H_
