//
// File: test_io.cpp
// Created by: Julien Dutheil
// Created on: Mon Nov 01 10:16 2010
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

#include <Bpp/Seq/Alphabet.all>
#include <Bpp/Seq/Io.all>
#include <iostream>

using namespace bpp;
using namespace std;

int main() {
  //This program reads a protein alignment generated using SimProt
  //[http://www.uhnresearch.ca/labs/tillier/simprotWEB/] in various file formats
  ProteicAlphabet* alpha = new ProteicAlphabet;
  Fasta fasta;
  const SequenceContainer* seqcont1 = fasta.read("example.fasta", alpha);
  //test number of seq
  cout << "example.fasta contains " << seqcont1->getNumberOfSequences() << " sequences" << endl;
  if (seqcont1->getNumberOfSequences() != 100) {
    return 1;
  }

  Mase mase;
  const SequenceContainer* seqcont2 = mase.read("example.mase", alpha);
  Clustal clustal;
  const SequenceContainer* seqcont3 = clustal.read("example.aln", alpha);
  Phylip phylip(true, false);
  const SequenceContainer* seqcont4 = phylip.read("example.ph", alpha);
  Phylip phylip3(true, true);
  const SequenceContainer* seqcont5 = phylip3.read("example.ph3", alpha);

  //Convert to alignments:
  const SiteContainer* sites1 = new VectorSiteContainer(*seqcont1);
  const SiteContainer* sites2 = new VectorSiteContainer(*seqcont2);
  const SiteContainer* sites3 = new VectorSiteContainer(*seqcont3);
  const SiteContainer* sites4 = new VectorSiteContainer(*seqcont4);
  const SiteContainer* sites5 = new VectorSiteContainer(*seqcont5);

  cout << sites1->getNumberOfSequences() << "\t" << sites1->getNumberOfSites() << endl;
  cout << sites2->getNumberOfSequences() << "\t" << sites2->getNumberOfSites() << endl;
  cout << sites3->getNumberOfSequences() << "\t" << sites3->getNumberOfSites() << endl;
  cout << sites4->getNumberOfSequences() << "\t" << sites4->getNumberOfSites() << endl;
  cout << sites5->getNumberOfSequences() << "\t" << sites5->getNumberOfSites() << endl;
  
  //Test:
  bool test = sites1->getNumberOfSequences() == sites2->getNumberOfSequences()
           && sites1->getNumberOfSequences() == sites3->getNumberOfSequences()
           && sites1->getNumberOfSequences() == sites4->getNumberOfSequences()
           && sites1->getNumberOfSequences() == sites5->getNumberOfSequences()
           && sites1->getNumberOfSites()     == sites2->getNumberOfSites()
           && sites1->getNumberOfSites()     == sites3->getNumberOfSites()
           && sites1->getNumberOfSites()     == sites4->getNumberOfSites()
           && sites1->getNumberOfSites()     == sites5->getNumberOfSites();

  delete seqcont1;
  delete seqcont2;
  delete seqcont3;
  delete seqcont4;
  delete seqcont5;
  delete sites1;
  delete sites2;
  delete sites3;
  delete sites4;
  delete sites5;

  delete alpha;

  return (test ? 0 : 1);
}
