//
// File: Translator.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 14:25:25 2003
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

#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include "Alphabet.h"
#include "Sequence.h"

/**
 * @brief This interface is used when translating a sequence from an alphabet to another:
 * it gives the translation rules,
 * eg: RNA -> DNA.
 */
class Translator
{
	public:
		Translator() {}
		virtual ~Translator() {}
	
	public:

		/**
		 * @brief Get the source alphabet.
		 *
		 * @return The source alphabet.
		 */
		virtual const Alphabet * getSourceAlphabet() const = 0;
		
		/**
		 * @brief Get the target alphabet.
		 *
		 * @return The target alphabet.
		 */
		virtual const Alphabet * getTargetAlphabet() const = 0;
		
		/**
		 * @brief Translate a given state coded as a int from source alphabet to target alphabet.
		 *
		 * @param state A state in source alphabet.
		 * @return The corresponding state in target alphabet.
		 * @throw BadIntException If the state is not a proper state for source alphabet.
		 * @throw Exception       Other kind of error, depending on the implementation.
		 */
		virtual int translate(int state) const throw (BadIntException, Exception) = 0;		
		
		/**
		 * @brief Translate a given state coded as a string from source alphabet to target alphabet.
		 *
		 * @param state A state in source alphabet.
		 * @return The corresponding state in target alphabet.
		 * @throw BadCharException If the state is not a proper state for source alphabet.
		 * @throw Exception        Other kind of error, depending on the implementation.
		 */
		virtual string translate(const string & state) const throw (BadCharException, Exception) = 0;
		
		/**
		 * @brief Translate a whole sequence from source alphabet to target alphabet.
		 *
		 * @param sequence A sequence in source alphabet.
		 * @return The corresponding sequence in target alphabet.
		 * @throw AlphabetMismatchException If the sequence alphabet do not match the source alphabet.
		 * @throw Exception                 Other kind of error, depending on the implementation.
		 */	
		virtual Sequence * translate(const Sequence & sequence) const throw (AlphabetMismatchException, Exception) = 0; 
};

/**
 * @brief The same as previous, but can perform the reverse translation,
 * eg: RNA -> DNA and DNA -> RNA;
 */
class ReverseTranslator : public Translator
{
	public:
		ReverseTranslator() {}
		virtual ~ReverseTranslator() {}
	
	public:
		
		/**
		 * @brief Translate a given state coded as a int from target alphabet to source alphabet.
		 *
		 * @param state A state in target alphabet.
		 * @return The corresponding state in source alphabet.
		 * @throw BadIntException If the state is not a proper state for target alphabet.
		 * @throw Exception       Other kind of error, depending on the implementation.
		 */
		virtual int reverse(int state) const throw (BadIntException, Exception) = 0;
		
		/**
		 * @brief Translate a given state coded as a string from target alphabet to source alphabet.
		 *
		 * @param state A state in target alphabet.
		 * @return The corresponding state in source alphabet.
		 * @throw BadCharException If the state is not a proper state for target alphabet.
		 * @throw Exception        Other kind of error, depending on the implementation.
		 */
		virtual string reverse(const string & state) const throw (BadCharException, Exception) = 0;			

		/**
		 * @brief Translate a whole sequence from target alphabet to source alphabet.
		 *
		 * @param sequence A sequence in target alphabet.
		 * @return The corresponding sequence in source alphabet.
		 * @throw AlphabetMismatchException If the sequence alphabet do not match the target alphabet.
		 * @throw Exception                 Other kind of error, depending on the implementation.
		 */	
		virtual Sequence * reverse(const Sequence & sequence) const throw (AlphabetMismatchException, Exception) = 0; 
};

class AbstractTranslator: public Translator
{
	public:
		AbstractTranslator() {}
		virtual ~AbstractTranslator() {}
	
	public:
		virtual int translate(int state) const throw (BadIntException, Exception) = 0;		
		virtual string translate(const string & state) const throw (BadCharException, Exception) = 0;
		virtual Sequence * translate(const Sequence & sequence) const throw (AlphabetMismatchException, Exception) = 0; 
};

class AbstractReverseTranslator: public ReverseTranslator, public AbstractTranslator
{
	public:
		AbstractReverseTranslator() {}
		virtual ~AbstractReverseTranslator() {}
	
	public:
		//These two redeclarations must be here because of the multiple inheritance.
		virtual const Alphabet * getSourceAlphabet() const = 0;
		virtual const Alphabet * getTargetAlphabet() const = 0;
		virtual int reverse(int state) const throw (BadIntException, Exception) = 0;
		virtual string reverse(const string & state) const throw (BadCharException, Exception) = 0;			
		virtual Sequence * reverse(const Sequence & sequence) const throw (AlphabetMismatchException, Exception);
};
#endif	//_TRANSLATOR_H_
