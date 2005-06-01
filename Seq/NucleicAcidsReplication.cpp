//
// File: NucleicAcidsReplication.cpp
// Created by: Julien Dutheil
// Created on: Fri May 20 14:40 2005
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

#include "NucleicAcidsReplication.h"

NucleicAcidsReplication::NucleicAcidsReplication(const NucleicAlphabet * nuc1, const NucleicAlphabet * nuc2) : _nuc1(nuc1), _nuc2(nuc2)
{
	_trans[-1] = -1;
	_trans[0] = 3;
	_trans[1] = 2;
	_trans[2] = 1;
	_trans[3] = 0;

	_trans[4] = 9;
	_trans[5] = 8;
	_trans[6] = 6;
	_trans[7] = 7;
	_trans[8] = 5;
	_trans[9] = 4;

	_trans[10] = 13;
	_trans[11] = 12;
	_trans[12] = 11;
	_trans[13] = 10;

	_trans[14] = 14;
}

int NucleicAcidsReplication::translate(int state) const throw (BadIntException)
{
	_nuc1 -> intToChar(state);
	return _trans[state];
}

string NucleicAcidsReplication::translate(const string & state) const throw (BadCharException)
{
	int i = _nuc1 -> charToInt(state);
	return _nuc2 -> intToChar(_trans[i]);
}

Sequence * NucleicAcidsReplication::translate(const Sequence & sequence) const throw (AlphabetMismatchException)
{
	if(sequence.getAlphabet() -> getAlphabetType() != getSourceAlphabet() -> getAlphabetType())
		throw AlphabetMismatchException("AbstractTranslator::translate", getSourceAlphabet(), getTargetAlphabet());
	Sequence * tSeq = new Sequence(sequence.getName(), "", sequence.getComments(), getTargetAlphabet());
	for(unsigned int i = 0; i < sequence.size(); i++) {
		tSeq -> addElement(translate(sequence.getValue(i)));
	}
	tSeq -> setSense(! tSeq -> getSense());
	return tSeq;
}


int NucleicAcidsReplication::reverse(int state) const throw (BadIntException) 
{
	_nuc2 -> intToChar(state);
	return _trans[state];
}

string NucleicAcidsReplication::reverse(const string & state) const throw (BadCharException)
{
	int i = _nuc2 -> charToInt(state);
	return _nuc1 -> intToChar(_trans[i]);
}

Sequence * NucleicAcidsReplication::reverse(const Sequence & sequence) const throw (AlphabetMismatchException, Exception)
{
	if(sequence.getAlphabet() -> getAlphabetType() != getTargetAlphabet() -> getAlphabetType())
		throw AlphabetMismatchException("AbstractReverseTranslator::reverse", getSourceAlphabet(), getTargetAlphabet());
	Sequence * rSeq = new Sequence(sequence.getName(), "", sequence.getComments(), getSourceAlphabet());
	for(unsigned int i = 0; i < sequence.size(); i++) {
		rSeq -> addElement(reverse(sequence.getValue(i)));
	}
	rSeq -> setSense(! rSeq -> getSense());
	return rSeq;
}

