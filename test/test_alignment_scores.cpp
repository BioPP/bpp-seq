//
// File: test_alignment_scores.cpp
// Created by: Julien Dutheil
// Created on: Wed Dec 14 16:35 2011
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
