//
// File: CodonAlphabet.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 17:41:56 2003
//

#ifndef _CODONALPHABET_H_
#define _CODONALPHABET_H_

#include "AbstractAlphabet.h"
#include "NucleicAlphabet.h"

// From the STL:
#include <string>

using namespace std;

/**
 * @brief <p> The abstract bae class for codon alphabets.
 * Since codons are made of 3 nucleic bases (RNA or DNA), this class has a
 * NucleicAlphabet field used to check char description. This nucleic alphabet
 * is passed to the constructor.
 * This class also adds some util specific to codon manipulation.</p>
 */
class CodonAlphabet : public AbstractAlphabet
{
	public:
		//Constant used for stop codons:
		static const string STOP;
		//Constant used for init codon:
		static const string INIT;
	
	protected:
		const NucleicAlphabet * nucAlpha;
	
	public: // Constructor and destructor.
		/**
		 * @brief <p>Builds a new codon alphabet from a nucleic alphabet.
		 * @param alpha The nucleic alphabet to be used.
		 */
		CodonAlphabet(const NucleicAlphabet * alpha);
	
		virtual ~CodonAlphabet();
	
	public:	// These methods are redefined from AbstractAlphabet:
	
		string getName(const string & letter) const throw (BadCharException);
		int charToInt(const string & letter) const throw (BadCharException);
		string getAlphabetType() const;
	
	private: //Private tools
		bool containsUnresolved(const string & letter) const throw (BadCharException);
		bool containsGap(const string & letter) const throw (BadCharException);
		
	public: //Codon specific methods:
		/**
		 * @brief Get the int code for a codon given the int code of the three underlying positions.
		 *
		 * The int code of each position must match the nucleic alphabet specified for this alphabet.
		 * @param pos1 Int description for position 1.
		 * @param pos2 Int description for position 2.
		 * @param pos3 Int description for position 3.
		 * @return The int code of the codon.
		 */
		virtual int getCodon(int pos1, int pos2, int pos3) const throw (BadIntException);
		/**
		 * @brief Get the char code for a codon given the char code of the three underlying positions.
		 *
		 * The char code of each position must match the nucleic alphabet specified for this alphabet.
		 * NB: This performs pos1 + pos2 + pos3 after checking for each position validity.
		 * @param pos1 Char description for position 1.
		 * @param pos2 Char description for position 2.
		 * @param pos3 Char description for position 3.
		 * @return The Char code of the codon.
		 */
		virtual string getCodon(const string & pos1, const string & pos2, const string & pos3) const throw (BadCharException);
		/**
		 * @brief Get the int code of the first position of a codon given its int description.
		 * 
		 * @param codon The int description of the codon.
		 * @return The int description of the first position of the codon.
		 */
		virtual int getFirstPosition(int codon) const throw (BadIntException);
		/**
		 * @brief Get the int code of the second position of a codon given its int description.
		 * 
		 * @param codon The int description of the codon.
		 * @return The int description of the second position of the codon.
		 */
		virtual int getSecondPosition(int codon) const throw (BadIntException);
		/**
		 * @brief Get the int code of the third position of a codon given its int description.
		 * 
		 * @param codon The int description of the codon.
		 * @return The int description of the third position of the codon.
		 */
		virtual int getThirdPosition(int codon) const throw (BadIntException);
		/**
		 * @brief Get the int codes of each position of a codon given its int description.
		 * 
		 * @param codon The int description of the codon.
		 * @return The int description of the three positions of the codon.
		 */
		virtual vector<int> getPositions(int codon) const throw (BadIntException);

		/**
		 * @brief Get the char code of the first position of a codon given its char description.
		 * 
		 * @param codon The char description of the codon.
		 * @return The char description of the first position of the codon.
		 */
		virtual string getFirstPosition (const string & codon) const throw (BadCharException);
		/**
		 * @brief Get the char code of the second position of a codon given its char description.
		 * 
		 * @param codon The char description of the codon.
		 * @return The char description of the second position of the codon.
		 */
		virtual string getSecondPosition(const string & codon) const throw (BadCharException);
		/**
		 * @brief Get the char code of the third position of a codon given its char description.
		 * 
		 * @param codon The char description of the codon.
		 * @return The char description of the third position of the codon.
		 */
		virtual string getThirdPosition(const string & codon) const throw (BadCharException);
		/**
		 * @brief Get the char codes of each position of a codon given its char description.
		 * 
		 * @param codon The char description of the codon.
		 * @return The char description of the three positions of the codon.
		 */
		virtual vector<string> getPositions(const string & codon) const throw (BadCharException);
		
		/**
		 * @brief Tell whether a particular codon is a stop codon.
		 * 
		 * @param codon The int description of the codon to test.
		 * @return True if the codon is a stop codon.
		 */
		virtual bool isStop(int codon) const = 0;
		/**
		 * @brief Tell whether a particular codon is a stop codon.
		 * 
		 * @param codon The char description of the codon to test.
		 * @return True if the codon is a stop codon.
		 */
		virtual bool isStop(const string & codon) const = 0;
};

#endif	//_CODONALPHABET_H_
