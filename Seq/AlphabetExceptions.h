//
// File: AlphabetExceptions.h
// Created by: Julien Dutheil
// Created on: Mon Nov  3 16:41:53 2003
//

/*
Copyright ou � ou Copr. CNRS, (17 Novembre 2004) 

Julien.Dutheil@univ-montp2.fr

Ce logiciel est un programme informatique servant � fournir des classes
pour l'analyse de s�quences.

Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilit� au code source et des droits de copie,
de modification et de redistribution accord�s par cette licence, il n'est
offert aux utilisateurs qu'une garantie limit�e.  Pour les m�mes raisons,
seule une responsabilit� restreinte p�se sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les conc�dants successifs.

A cet �gard  l'attention de l'utilisateur est attir�e sur les risques
associ�s au chargement,  � l'utilisation,  � la modification et/ou au
d�veloppement et � la reproduction du logiciel par l'utilisateur �tant 
donn� sa sp�cificit� de logiciel libre, qui peut le rendre complexe � 
manipuler et qui le r�serve donc � des d�veloppeurs et des professionnels
avertis poss�dant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invit�s � charger  et  tester  l'ad�quation  du
logiciel � leurs besoins dans des conditions permettant d'assurer la
s�curit� de leurs syst�mes et ou de leurs donn�es et, plus g�n�ralement, 
� l'utiliser et l'exploiter dans les m�mes conditions de s�curit�. 

Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accept� les
termes.
*/

/*
Copyright or � or Copr. CNRS, (November 17, 2004)

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

#ifndef _ALPHABETEXCEPTIONS_H_
#define _ALPHABETEXCEPTIONS_H_

// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <vector>
using namespace std;

class Alphabet;

/**
 * @brief The alphabet exception base class.
 * @see Exception
 */
class AlphabetException : public Exception {

	protected:
		const Alphabet * alphabet;
			
	public: // Class constructors.
		//AlphabetException(const char *   text, const Alphabet * alpha = NULL);
		/**
		 * @brief Build a new AlphabetException.
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		AlphabetException(const string & text, const Alphabet * alpha = NULL);
	
		// Class destructor
		~AlphabetException() throw () {}
        
	public:
		/**
		 * @brief Get the alphabet that threw the exception.
		 * @return a const pointer toward the alphabet.
		 */
		virtual const Alphabet * getAlphabet() const;
};

/**
 * @brief An alphabet exception throw when trying to specify a bad char to the alphabet.
 */
class BadCharException : public AlphabetException {

	protected:
		string c;
	
	public:	// Class constructor
		//BadCharException(const string badChar, const char *   text = "", const Alphabet * alpha = NULL);
		/**
		 * @brief Build a new BadCharException.
		 * @param badChar The faulty character.
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		BadCharException(const string badChar, const string & text = "", const Alphabet * alpha = NULL);
	
		// Class destructor
		~BadCharException() throw() {};
	
	public:
		/**
		 * @brief Get the character that threw the exception.
		 * @return the faulty character.
		 */
		virtual string getBadChar() const;
};

/**
 * @brief An alphabet exception throw when trying to specify a bad int to the alphabet.
 */
class BadIntException : public AlphabetException {

	protected:
		int i;
	
	public:	// Class constructor
		//BadIntException(int badInt, const char *   text = "", const Alphabet * alpha = NULL);
		/**
		 * @brief Build a new BadIntException.
		 * @param badInt The faulty integer.
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha A const pointer toward the alphabet that threw the exception.
		 */
		BadIntException(int badInt, const string & text = "", const Alphabet * alpha = NULL);
	
		// Class destructor
		~BadIntException() throw() {}

	public:
		/**
		 * @brief Get the integer that threw the exception.
		 * @return the faulty integer.
		 */
		virtual int getBadInt() const;
};

/**
 * @brief Exception thrown when two alphabet don't match.
 *
 * Typically, this may occur when you try to add a bad sequence to a container,
 * or concatenate two kinds of sequences, and so on.</p>
 */
class AlphabetMismatchException : public Exception {

	protected:
		const Alphabet * alphabet1, * alphabet2;
	
	public:
    
		/**
		 * @brief Build a new AlphabetMismatchException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha1 A const pointer toward the first alphabet.
		 * @param alpha2 A const pointer toward the second alphabet, i.e. the one which does not match with the first.
		 */
		AlphabetMismatchException(const char *   text = "", const Alphabet * alpha1 = NULL, const Alphabet * alpha2 = NULL);
        
		/**
		 * @brief Build a new AlphabetMismatchException object.
         *
		 * @param text A message to be passed to the exception hierarchy.
		 * @param alpha1 A const pointer toward the first alphabet.
		 * @param alpha2 A const pointer toward the second alphabet, i.e. the one which does not match with the first.
		 */
		AlphabetMismatchException(const string & text = "", const Alphabet * alpha1 = NULL, const Alphabet * alpha2 = NULL);
	
		~AlphabetMismatchException() throw();

	public:
		/**
		 * @brief Get the alphabets that do not match.
         *
		 * @return a vector of pointers toward the alphabets.
		 */
		vector<const Alphabet *> getAlphabets() const;
};

#endif	//_ALPHABETEXCEPTIONS_H_

