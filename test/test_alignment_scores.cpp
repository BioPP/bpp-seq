// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Seq/Alphabet/RNA.h>
#include <Bpp/Seq/Container/VectorSiteContainer.h>
#include <Bpp/Seq/Container/SiteContainerTools.h>
#include <Bpp/Seq/SiteTools.h>
#include <iostream>

using namespace bpp;
using namespace std;

int main() {
  shared_ptr<const Alphabet> alpha = AlphabetTools::RNA_ALPHABET;
  auto sites = make_unique<VectorSiteContainer>(alpha);
  auto seq1 = make_unique<Sequence>("seq1", "----AUGCCG---GCGU----UUU----G--G-CCGACGUGUUUU--", alpha);
  auto seq2 = make_unique<Sequence>("seq2", "---GAAGGCG---G-GU----UUU----GC-GACCGACG--UUUU--", alpha);
  auto seq3 = make_unique<Sequence>("seq3", "---GAA-CCG---G-GU----UUU----VC-GACCGGAG--UUUU--", alpha);
  sites->addSequence(seq1->getName(), seq1);
  sites->addSequence(seq2->getName(), seq2);
  sites->addSequence(seq3->getName(), seq3);

  //Create alignment indexes:
  RowMatrix<size_t> index1;
  SiteContainerTools::getSequencePositions(*sites, index1);

  vector<int> scores = SiteContainerTools::getColumnScores(index1, index1);
  VectorTools::print(scores);
  for (size_t i = 0; i < sites->getNumberOfSites(); ++i) {
    if (SiteTools::isGapOnly(sites->site(i))) {
      if (scores[i] != 0) return 1;
    } else {
      if (scores[i] != 1) return 1;
    }
  }

  auto sites2 = make_unique<VectorSiteContainer>(alpha);
  auto seq21 = make_unique<Sequence>("seq1", "----AUGCCGGCGU-UUUG--G-CCGACGUGUUUU", alpha);
  auto seq22 = make_unique<Sequence>("seq2", "---GAAGGCGG-GUU-UUGC-GACCGAC--GUUUU", alpha);
  auto seq23 = make_unique<Sequence>("seq3", "---GAA-CCGG-GUUU-UVC-GACCGGA--GUUUU", alpha);
  sites2->addSequence(seq21->getName(), seq21);
  sites2->addSequence(seq22->getName(), seq22);
  sites2->addSequence(seq23->getName(), seq23);

  RowMatrix<size_t> index2;
  SiteContainerTools::getSequencePositions(*sites2, index2);

  vector<int> scores12 = SiteContainerTools::getColumnScores(index1, index2);
  VectorTools::print(scores12);

  //Just a simple test, please check output by eye for better evaluation!
  if (scores12.size() != index1.getNumberOfColumns()) return 1;

  vector<int> scores21 = SiteContainerTools::getColumnScores(index2, index1);
  VectorTools::print(scores21);

  if (scores21.size() != index2.getNumberOfColumns()) return 1;
  
  vector<double> sp12 = SiteContainerTools::getSumOfPairsScores(index1, index2);
  VectorTools::print(sp12);

  vector<double> sp21 = SiteContainerTools::getSumOfPairsScores(index2, index1);
  VectorTools::print(sp21);

  return 0;
}
