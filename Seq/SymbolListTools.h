//
// File: SymbolListTools.h
// Created by: Julien Dutheil
// Created on: Wed Apr 9 2004
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

#ifndef SYMBOLLISTTOOLS_H
#define SYMBOLLISTTOOLS_H

#include "SymbolList.h"
#include "AlphabetExceptions.h"
// From the STL:
#include <map>
using namespace std;

class SymbolListTools {

	public: 
		~SymbolListTools() {}

	public:
		static map<int, unsigned int> getCounts(const SymbolList & list);
		static map<int, double> getFrequencies(const SymbolList & list);
		/**
 		 * @brief Get the number of distinct positions.
		 *
		 * The comparison in achieved from position 0 to the minimum size of the two vectors.
		 *
		 * @param l1 SymbolList 1.
		 * @param l2 SymbolList 2.
		 * @return The number of distinct positions.
		 * @throw AlphabetMismatchException if the two lists have not the same alphabet type.
		 */
		static unsigned int getNumberOfDistinctPositions(const SymbolList & l1, const SymbolList & l2) throw (AlphabetMismatchException);
		
		/**
 		 * @brief Get the number of positions without gap.
		 *
		 * The comparison in achieved from position 0 to the minimum size of the two vectors.
		 *
		 * @param l1 SymbolList 1.
		 * @param l2 SymbolList 2.
		 * @return The number of positions without gap.
		 * @throw AlphabetMismatchException if the two lists have not the same alphabet type.
		 */
		static unsigned int getNumberOfPositionsWithoutGap(const SymbolList & l1, const SymbolList & l2) throw (AlphabetMismatchException);

};

#endif
