//
// File: CodonAlphabet.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 17:41:56 2003
//

/*
Copyright ou © ou Copr. CNRS, (17 Novembre 2004) 

Julien.Dutheil@univ-montp2.fr

Ce logiciel est un programme informatique servant à fournir des classes
pour l'analyse de séquences.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

Julien.Dutheil@univ-montp2.fr

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

#ifndef _CODONALPHABET_H_
#define _CODONALPHABET_H_

#include "AbstractAlphabet.h"
#include "NucleicAlphabet.h"

// From the STL:
#include <string>

using namespace std;

/**
 * @brief The abstract bae class for codon alphabets.
 * 
 * Since codons are made of 3 nucleic bases (RNA or DNA), this class has a
 * NucleicAlphabet field used to check char description. This nucleic alphabet
 * is passed to the constructor.
 * This class also adds some util specific to codon manipulation.
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
		 * @brief <Builds a new codon alphabet from a nucleic alphabet.
		 * 
		 * @param alpha The nucleic alphabet to be used.
		 */
		CodonAlphabet(const NucleicAlphabet * alpha) : AbstractAlphabet(), nucAlpha(alpha) {}
	
		virtual ~CodonAlphabet() {}
	
	public:	// These methods are redefined from AbstractAlphabet:
	
		string getName(const string & letter) const throw (BadCharException);
		int charToInt(const string & letter) const throw (BadCharException);
		string getAlphabetType() const { return "Codon alphabet"; }
	
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
