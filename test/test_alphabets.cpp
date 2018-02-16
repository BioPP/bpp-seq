//
// File: test_alphabets.cpp
// Created by: Julien Dutheil
// Created on: Sat Oct 30 17:10 2010
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for numerical calculus. This file is part of the Bio++ project.

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

#include <Bpp/Seq/Alphabet/DNA.h>
#include <Bpp/Seq/Alphabet/RNA.h>
#include <Bpp/Seq/Alphabet/ProteicAlphabet.h>
#include <Bpp/Seq/Alphabet/DefaultAlphabet.h>
#include <Bpp/Seq/Alphabet/CodonAlphabet.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>
#include <iostream>

using namespace bpp;
using namespace std;

int main() {
  //This is a very simple test that instanciate all alpahabet classes.
  NucleicAlphabet* dna = new DNA();
  NucleicAlphabet* rna = new RNA();
  Alphabet* pro = new ProteicAlphabet;
  Alphabet* def = new DefaultAlphabet;
  Alphabet* cdn = new CodonAlphabet(rna->clone());

  //Testing functions:
  if (!AlphabetTools::isDNAAlphabet(dna)) return 1;
  if (!AlphabetTools::isRNAAlphabet(rna)) return 1;
  if (!AlphabetTools::isNucleicAlphabet(dna)) return 1;
  if (!AlphabetTools::isNucleicAlphabet(rna)) return 1;
  if (!AlphabetTools::isProteicAlphabet(pro)) return 1;
  if (!AlphabetTools::isCodonAlphabet(cdn)) return 1;

  delete dna;
  delete rna;
  delete pro;
  delete def;
  delete cdn;

  return (0);
}
