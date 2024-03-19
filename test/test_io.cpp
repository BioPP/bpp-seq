// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Seq/Alphabet/AlphabetTools.h>
#include <Bpp/Seq/Io/Fasta.h>
#include <Bpp/Seq/Io/Mase.h>
#include <Bpp/Seq/Io/Clustal.h>
#include <Bpp/Seq/Io/Phylip.h>
#include <iostream>

using namespace bpp;
using namespace std;

int main()
{
  // This program reads a protein alignment generated using SimProt
  // [http://www.uhnresearch.ca/labs/tillier/simprotWEB/] in various file formats
  shared_ptr<const Alphabet> alpha = AlphabetTools::PROTEIN_ALPHABET;
  Fasta fasta;
  auto sites1 = fasta.readAlignment("example.fasta", alpha);
  // test number of seq
  cout << "example.fasta contains " << sites1->getNumberOfSequences() << " sequences" << endl;
  if (sites1->getNumberOfSequences() != 100)
  {
    return 1;
  }

  Mase mase;
  auto sites2 = mase.readAlignment("example.mase", alpha);
  Clustal clustal;
  auto sites3 = clustal.readAlignment("example.aln", alpha);
  Phylip phylip(true, false);
  auto sites4 = phylip.readAlignment("example.ph", alpha);
  Phylip phylip3(true, true);
  auto sites5 = phylip3.readAlignment("example.ph3", alpha);

  cout << "Fasta:    " << sites1->getNumberOfSequences() << "\t" << sites1->getNumberOfSites() << endl;
  cout << "Mase:     " << sites2->getNumberOfSequences() << "\t" << sites2->getNumberOfSites() << endl;
  cout << "Clustal:  " << sites3->getNumberOfSequences() << "\t" << sites3->getNumberOfSites() << endl;
  cout << "Phylip:   " << sites4->getNumberOfSequences() << "\t" << sites4->getNumberOfSites() << endl;
  cout << "Phylip 3: " << sites5->getNumberOfSequences() << "\t" << sites5->getNumberOfSites() << endl;

  // Test:
  bool test = sites1->getNumberOfSequences() == sites2->getNumberOfSequences()
      && sites1->getNumberOfSequences() == sites3->getNumberOfSequences()
      && sites1->getNumberOfSequences() == sites4->getNumberOfSequences()
      && sites1->getNumberOfSequences() == sites5->getNumberOfSequences()
      && sites1->getNumberOfSites()     == sites2->getNumberOfSites()
      && sites1->getNumberOfSites()     == sites3->getNumberOfSites()
      && sites1->getNumberOfSites()     == sites4->getNumberOfSites()
      && sites1->getNumberOfSites()     == sites5->getNumberOfSites();

  cout << (test ? "Succeeded." : "Failed.") << endl;
  return test ? 0 : 1;
}
