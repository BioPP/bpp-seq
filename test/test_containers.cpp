// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Seq/Alphabet/RNA.h>
#include <Bpp/Seq/Container/VectorSiteContainer.h>
#include <Bpp/Seq/Container/CompressedVectorSiteContainer.h>
#include <Bpp/Seq/Container/SiteContainerTools.h>
#include <iostream>

using namespace bpp;
using namespace std;

int main() {
  //ProteicAlphabet* alpha = new ProteicAlphabet;
  shared_ptr<const Alphabet> alpha = AlphabetTools::RNA_ALPHABET;
  auto sites = make_unique<VectorSiteContainer>(alpha);

  auto seq1 = make_unique<Sequence>("seq1", "----AUGCCG---GCGU----UUU----G--G-CCGACGUGUUUU--", alpha);
  auto seq2 = make_unique<Sequence>("seq2", "---GAAGGCG---G-GU----UUU----GC-GACCGACG--UUUU--", alpha);
  sites->addSequence(seq1->getName(), seq1);
  sites->addSequence(seq2->getName(), seq2);
  cout << sites->getNumberOfSites() << endl;
  cout << sites->sequence("seq1").toString() << endl;
  cout << sites->sequence("seq2").toString() << endl;
  SiteContainerTools::removeGapOnlySites(*sites);
  cout << endl;
  
  cout << sites->getNumberOfSites() << endl;
  cout << sites->sequence("seq1").toString() << endl;
  cout << sites->sequence("seq2").toString() << endl;

  if (sites->getNumberOfSites() != 30)
    throw Exception("Bad removal of gap only sites");

  SiteContainerTools::removeGapSites(*sites, 0.);
  cout << endl;
  
  cout << sites->getNumberOfSites() << endl;
  cout << sites->sequence("seq1").toString() << endl;
  cout << sites->sequence("seq2").toString() << endl;
  
  if (sites->getNumberOfSites() != 24)
    throw Exception("Bad removal of gap sites");

  cout << endl;
  CompressedVectorSiteContainer cvs(*sites);

  cout << "Compressed sequence" << endl;
  
  cout << "Number of unique sites : " << cvs.getNumberOfUniqueSites() << endl;
  if (cvs.getNumberOfUniqueSites() != 6) {
    throw Exception("Bad compression of sites");
  }
  if (cvs.getSequenceNames() != sites->getSequenceNames()) {
    throw Exception("Sequence names got lost when compressing sites!");
  }
  cout << cvs.getNumberOfSites() << endl;
  cout << cvs.sequence("seq1").toString() << endl;
  cout << cvs.sequence("seq2").toString() << endl;

  return (sites->getNumberOfSites() == 24 ? 0 : 1);
}
