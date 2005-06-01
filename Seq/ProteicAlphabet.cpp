//
// File: ProteicAlphabet.cpp
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

#include "ProteicAlphabet.h" // class's header file

// From Utils:
#include <Utils/TextTools.h>

// class constructor
ProteicAlphabet::ProteicAlphabet()
{
	// Alphabet size definition
	alphabet.resize(24);

	// Alphabet content definition
	alphabet[0].num = -1;
	alphabet[0].letter = "-";
	alphabet[0].abbr = "GAP";
	alphabet[0].name = "Gap";
	alphabet[1].num = 0;
	alphabet[1].letter = "A";
	alphabet[1].abbr = "ALA";
	alphabet[1].name = "Alanine";
	alphabet[2].num = 1;
	alphabet[2].letter = "R";
	alphabet[2].abbr = "ARG";
	alphabet[2].name = "Arginine";
	alphabet[3].num = 2;
	alphabet[3].letter = "N";
	alphabet[3].abbr = "ASN";
	alphabet[3].name = "Asparagine";
	alphabet[4].num = 3;
	alphabet[4].letter = "D";
	alphabet[4].abbr = "ASP";
	alphabet[4].name = "Asparatic Acid";
	alphabet[5].num = 4;
	alphabet[5].letter = "C";
	alphabet[5].abbr = "CYS";
	alphabet[5].name = "Cysteine";
	alphabet[6].num = 5;
	alphabet[6].letter = "Q";
	alphabet[6].abbr = "GLN";
	alphabet[6].name = "Glutamine";
	alphabet[7].num = 6;
	alphabet[7].letter = "E";
	alphabet[7].abbr = "GLU";
	alphabet[7].name = "Glutamic acid";
	alphabet[8].num = 7;
	alphabet[8].letter = "G";
	alphabet[8].abbr = "GLY";
	alphabet[8].name = "Glycine";
	alphabet[9].num = 8;
	alphabet[9].letter = "H";
	alphabet[9].abbr = "HIS";
	alphabet[9].name = "Histidine";
	alphabet[10].num = 9;
	alphabet[10].letter = "I";
	alphabet[10].abbr = "ILE";
	alphabet[10].name = "Isoleucine";
	alphabet[11].num = 10;
	alphabet[11].letter = "L";
	alphabet[11].abbr = "LEU";
	alphabet[11].name = "Leucine";
	alphabet[12].num = 11;
	alphabet[12].letter = "K";
	alphabet[12].abbr = "LYS";
	alphabet[12].name = "Lysine";
	alphabet[13].num = 12;
	alphabet[13].letter = "M";
	alphabet[13].abbr = "MET";
	alphabet[13].name = "Methionine";
	alphabet[14].num = 13;
	alphabet[14].letter = "F";
	alphabet[14].abbr = "PHE";
	alphabet[14].name = "Phenylalanine";
	alphabet[15].num = 14;
	alphabet[15].letter = "P";
	alphabet[15].abbr = "PRO";
	alphabet[15].name = "Proline";
	alphabet[16].num = 15;
	alphabet[16].letter = "S";
	alphabet[16].abbr = "SER";
	alphabet[16].name = "Serine";
	alphabet[17].num = 16;
	alphabet[17].letter = "T";
	alphabet[17].abbr = "THR";
	alphabet[17].name = "Threonine";
	alphabet[18].num = 17;
	alphabet[18].letter = "W";
	alphabet[18].abbr = "TRP";
	alphabet[18].name = "Tryptophan";
	alphabet[19].num = 18;
	alphabet[19].letter = "Y";
	alphabet[19].abbr = "TYR";
	alphabet[19].name = "Tyrosine";
	alphabet[20].num = 19;
	alphabet[20].letter = "V";
	alphabet[20].abbr = "VAL";
	alphabet[20].name = "Valine";
	alphabet[21].num = 20;
	alphabet[21].letter = "X";
	alphabet[21].abbr = "X";
	alphabet[21].name = "Unresolved acid";
	alphabet[22].num = 21;
	alphabet[22].letter = "B";
	alphabet[22].abbr = "B";
	alphabet[22].name = "N or D";
	alphabet[23].num = 22;
	alphabet[23].letter = "Z";
	alphabet[23].abbr = "Z";
	alphabet[23].name = "Q or E";
}

string ProteicAlphabet::getAbbr(const string & aa) const throw (AlphabetException)
{
	string AA = TextTools::toUpper(aa);
	for (unsigned int i = 0 ; i < getNumberOfChars() ; i++)
		if (alphabet[i].letter == AA) return alphabet[i].abbr;
	throw AlphabetException ("ProteicAlphabet::getAbbr : Unknown specified amino acid");
}

string ProteicAlphabet::getAbbr(int aa) const throw (AlphabetException)
{
	if ((aa >= 0) && (aa < (int)getNumberOfChars())) return alphabet[aa].abbr;
	throw AlphabetException ("ProteicAlphabet::getAbbr : Unknown specified amino acid");
}

