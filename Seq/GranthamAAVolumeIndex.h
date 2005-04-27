//
// File: GranthamAAVolumeIndex.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Tue Apr 21 2005
//

/*
Copyright ou © ou Copr. Julien Dutheil, (17 Novembre 2004) 

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
Copyright or © or Copr. Julien Dutheil, (November 17, 2004)

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

#ifndef _GRANTHAMAAVOLUMEINDEX_H_
#define _GRANTHAMAAVOLUMEINDEX_H_

#include "AlphabetIndex1.h"
#include "ProteicAlphabet.h"

class GranthamAAVolumeIndex: public AlphabetIndex1<double> {

	private:
		vector<double> _polarity;
		const ProteicAlphabet * _alpha;

	public:
		GranthamAAVolumeIndex() {
			_alpha = new ProteicAlphabet();
			_polarity.resize(20);
			_polarity[ 0] =  31.; //A
			_polarity[ 1] = 124.; //R
			_polarity[ 2] =  56.; //N
			_polarity[ 3] =  54.; //D
			_polarity[ 4] =  55.; //C
			_polarity[ 5] =  85.; //Q
			_polarity[ 6] =  83.; //E
			_polarity[ 7] =   3.; //G
			_polarity[ 8] =  96.; //H
			_polarity[ 9] = 111.; //I
			_polarity[10] = 111.; //L
			_polarity[11] = 119.; //K
			_polarity[12] = 105.; //M
			_polarity[13] = 132.; //F
			_polarity[14] =  32.5; //P
			_polarity[15] =  32.; //S
			_polarity[16] =  61.; //T
			_polarity[17] = 170.; //W
			_polarity[18] = 136.; //Y
			_polarity[19] =  84.; //V
		}

		~GranthamAAVolumeIndex() {
			delete _alpha;
		}

	public:
		double getIndex(int state) const throw (BadIntException) {
			if(state < 0 || state > 19) throw BadIntException(state, "GranthamAAVolumeIndex::getIndex(). Invalid state.", _alpha);
			return _polarity[state];
		}
		
		double getIndex(const string & state) const throw (BadCharException) {
			return _polarity[_alpha -> charToInt(state)];
		}

		const Alphabet * getAlphabet() const { return _alpha; }

		Clonable * clone() const { return new GranthamAAVolumeIndex(); }
};

#endif //_GRANTHAMAAVOLUMEINDEX_H_

