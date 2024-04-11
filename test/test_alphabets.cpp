// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Seq/Alphabet/DNA.h>
#include <Bpp/Seq/Alphabet/RNA.h>
#include <Bpp/Seq/Alphabet/ProteicAlphabet.h>
#include <Bpp/Seq/Alphabet/DefaultAlphabet.h>
#include <Bpp/Seq/Alphabet/CodonAlphabet.h>
#include <Bpp/Seq/Alphabet/AllelicAlphabet.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>
#include <iostream>

using namespace bpp;
using namespace std;

int main()
{
  // This is a very simple test that instantiate all alphabet classes.
  auto dna = std::make_shared<DNA>();
  auto rna = std::make_shared<RNA>();
  auto pro = std::make_shared<ProteicAlphabet>();
  auto def = std::make_shared<DefaultAlphabet>();
  auto cdn = std::make_shared<CodonAlphabet>(rna);
  auto allelic = std::make_shared<AllelicAlphabet>(dna, 4);

  // Testing functions:
  if (!AlphabetTools::isDNAAlphabet(*dna))
    return 1;
  if (!AlphabetTools::isRNAAlphabet(*rna))
    return 1;
  if (!AlphabetTools::isNucleicAlphabet(*dna))
    return 1;
  if (!AlphabetTools::isNucleicAlphabet(*rna))
    return 1;
  if (!AlphabetTools::isProteicAlphabet(*pro))
    return 1;
  if (!AlphabetTools::isCodonAlphabet(*cdn))
    return 1;

  for (size_t i = 0; i < allelic->getNumberOfStates(); i++)
  {
    cerr << i << " -> " << allelic->getStateAt(i).getNum() << " -> " << allelic->getStateAt(i).getLetter() << endl;
  }

  return 0;
}
