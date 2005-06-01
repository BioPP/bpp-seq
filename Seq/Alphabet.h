//
// File: SymbolList.h
// Created by: Guillaume Deuchst
// Created on: Tue Jul 22 2003
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

#ifndef _ALPHABET_H_
#define _ALPHABET_H_

#include <string>
#include <vector>

#include "AlphabetExceptions.h"

using namespace std;

/**
 * @brief The Alphabet interface.
 * Interface for alphabets manipulation.
 * 
 * An alphabet object defines all the states allowed for a particular type of
 * sequence. These states are coded as a string and an integer.
 * The string description is the one found in the text (human comprehensive)
 * description of sequences, typically in sequence files.
 * However, for computionnal needs, this is often more efficient to store the sequences as
 * a vector of integers.
 * The link between the two descriptions is made <i>via</i>
 * the Alphabet classes, and the two methods intToChar() and charToInt().
 * The Alphabet interface also provides other methods, like getting the full name
 * of the states and so on.
 * 
 * The alphabet objects may throw some exceptions derived of the AlphabetException
 * class.
 *
 * @see AlphabetException, BadCharException, BadIntException 
 */
class Alphabet
{
  public:	// Class destructor
		virtual ~Alphabet() {}
	
	public:
    
		/**
		 * @brief Get the complete name of a state given its int description.
		 *
		 * In case of several states with identical number (i.e. N and X for nucleic alphabets),
		 * this method will return the name of the first found in the vector.
		 *
		 * @param letter The int description of the given state.
		 * @return The name of the state.
		 */
		virtual string getName(int letter) const throw (BadIntException)  = 0;
        
		/**
		 * @brief Get the complete name of a state given its string description.
		 *
		 * In case of several states with identical number (i.e. N and X for nucleic alphabets),
		 * this method will return the name of the first found in the vector.
		 *
		 * @param letter The string description of the given state.
		 * @return The name of the state.
		 */
		virtual string getName(const string & letter) const throw (BadCharException) = 0;

		/**
     * @name = Tests
     *
     * @{
     */

		/**
		 * @brief Tell if a state (specified by its int description) is allowed by the
		 * the alphabet.
		 *
		 * @param letter The int description.
		 * @return 'true' if the state in known.
		 */
		virtual bool isIntInAlphabet(int letter) const = 0;
		
		/**
		 * @brief Tell if a state (specified by its string description) is allowed by the
		 * the alphabet.
		 *
		 * @param letter The string description.
		 * @return 'true' if the state in known.
		 */
		virtual bool isCharInAlphabet(const string & letter) const = 0;
	  /** @} */
        
		/**
     * @name Conversion methods
     *
     * @{
     */
		
		/**
		 * @brief Give the string description of a state given its int description.
		 *
		 * @param letter The int description.
		 * @return The string description.
		 */
		virtual string intToChar(int letter) const throw (BadIntException) = 0;
        
		/**
		 * @brief Give the int description of a state given its string description.
		 *
		 * @param letter The string description.
		 * @return The int description.
		 */
		virtual int charToInt(const string & letter) const throw (BadCharException) = 0;
		/** @} */
        
		/**
		 * @name Sizes.
		 *
		 * @{
		 */
		
		/**
		 * @brief Get the number of allowed states in Alphabet (e.g. return 20 for DNA alphabet).
		 * This is the number of characters used for state description.
		 *
		 * @return The total number of states.
		 */
		virtual unsigned int getNumberOfChars() const = 0;
        
		/**
		 * @brief Get the number of <strong>distinct</strong> states in alphabet (e.g. return 15 for DNA alphabet).
		 * This is the number of integers used for state description.
		 *
		 * @return The number of distinct states.
		 */
		virtual unsigned int getNumberOfTypes() const = 0;
        
		/**
		 * @brief Get the number of <strong>resolved</strong> states in the alphabet (e.g. return 4 for DNA alphabet).
		 * This is the method you'll need in most cases.
		 *
		 * @return The number of resolved states.
		 */
		virtual unsigned int getSize() const = 0;
		/** @} */
        
		// Some util:
		
		/**
		 * @brief Get all resolved states that match a generic state.
		 *
		 * If the given state is not a generic code then the outpout vector will contain this sole code.
		 *
		 * @param state The alias to resolve.
		 * @return A vector of resolved states.
		 */
		virtual vector<int> getAlias(int state) const throw (BadIntException) = 0;
		
		/**
		 * @brief Get all resolved states that match a generic state.
		 *
		 * If the given state is not a generic code then the output vector will contain this sole code.
		 *
		 * @param state The alias to resolve.
		 * @return A vector of resolved states.
		 */
		virtual vector<string> getAlias(const string & state) const throw (BadCharException) = 0;

		/**
		 * @brief Identification method.
		 *
		 * Used to tell if two alphabets describe the same type of sequences.
		 * For instance, this method is used by sequence containers to compare two alphabets and
		 * autorize or deny addition of sequences.
		 *
		 * @return A text describing the alphabet.
		 */
		virtual string getAlphabetType() const = 0;
};

#endif // _ALPHABET_H_
