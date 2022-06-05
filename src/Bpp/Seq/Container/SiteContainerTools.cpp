//
// File: SiteContainerTools.cpp
// Authors:
//   Julien Dutheil
//   Sylvain GlÃ©min
// Created: 2003-12-12 18:55:06
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#include <Bpp/App/ApplicationTools.h>

#include "../Alphabet/AlphabetTools.h"
#include "../CodonSiteTools.h"
#include "../SequenceTools.h"
#include "../SymbolListTools.h"
#include "SequenceContainerTools.h"
#include "SiteContainerIterator.h"
#include "SiteContainerTools.h"
#include "VectorProbabilisticSiteContainer.h"
#include "VectorSiteContainer.h"

using namespace bpp;

// From the STL:
#include <vector>
#include <deque>
#include <string>

using namespace std;

/******************************************************************************/

unique_ptr<Sequence> SiteContainerTools::getConsensus(const BasicSiteContainer& sc, const std::string& name, bool ignoreGap, bool resolveUnknown)
{
  Vint consensus;
  SimpleSiteContainerIterator<Site> ssi(sc);
  const Site& site;
  while (ssi.hasMoreSites())
  {
    site = ssi.nextSite();
    map<int, double> freq;
    SiteTools::getFrequencies(site, freq, resolveUnknown);
    double max = 0;
    int cons = -1; // default result
    if (ignoreGap) {
      for (auto it : freq) {
        if (it->second > max && it->first != -1) {
          max = it->second;
          cons = it->first;
        }
      }
    } else {
      for (auto it : freq) {
        if (it->second > max) {
          max = it->second;
          cons = it->first;
        }
      }
    }
    consensus.push_back(cons);
  }
  auto seqConsensus = make_unique<BasicSequence>(name, consensus, sc.getAlphabet());
  return seqConsensus;
}

/******************************************************************************/

void SiteContainerTools::changeGapsToUnknownCharacters(BasicSiteContainer& sites)
{
  int unknownCode = sites.getAlphabet()->getUnknownCharacterCode();
  for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
    unique_ptr<Site> site(sites.getSite(i).clone());
    if (SiteTools::hasGap(site)) {
      for (size_t j = 0; j < sites.getNumberOfSequences(); ++j) {
        if (sites.getAlphabet()->isGap((*site)[j])) {
          (*site)[j] = unknownCode;
        }
      }
      sites.setSite(i, site, false);
    }
  }
}

/******************************************************************************/

void SiteContainerTools::changeGapsToUnknownCharacters(ProbabilisticSiteContainer& sites)
{
  int unknownCode = sites.getAlphabet()->getUnknownCharacterCode();
  for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
    unique_ptr<ProbabilisticSite> site(sites.getSite(i).clone());
    if (SiteTools::hasGap(site)) {
      for (size_t j = 0; j < sites.getNumberOfSequences(); ++j) {
        vector<double>& element = (*site)[j];
        if (VectorTools::sum(element) <= NumConstants::TINY()) {
          VectorTools::fill(element, 0.);
	}
      }
      sites.setSite(i, site, false);
    }
  }
}

/******************************************************************************/

void SiteContainerTools::changeUnresolvedCharactersToGaps(BasicSiteContainer& sites)
{
  // NB: use iterators for a better algorithm?
  int gapCode = sites.getAlphabet()->getGapCharacterCode();
  for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
    unique_ptr<Site> site(sites.getSite(i).clone());
    if (SiteTools::hasUnresolved(site)) {
      for (size_t j = 0; j < sites.getNumberOfSequences(); ++j) {
        if (sites.getAlphabet()->isUnresolved((*site)[j]))
          (*site)[j] = gapCode;
      }
      sites.setSite(i, site, false);
    }
  }
}

/******************************************************************************/

unique_ptr<BasicSiteContainer> SiteContainerTools::resolveDottedAlignment(
  const BasicSiteContainer& dottedAln,
  std::shared_ptr<const Alphabet>& resolvedAlphabet)
{
  if (!AlphabetTools::isDefaultAlphabet(dottedAln.getAlphabet()))
    throw AlphabetException("SiteContainerTools::resolveDottedAlignment. Alignment alphabet should of class 'DefaultAlphabet'.", dottedAln.getAlphabet());

  // First we look for the reference sequence:
  size_t n = dottedAln.getNumberOfSequences();
  if (n == 0)
    throw Exception("SiteContainerTools::resolveDottedAlignment. Input alignment contains no sequence.");

  const Sequence* refSeq = 0;
  for (size_t i = 0; i < n; ++i) // Test each sequence
  {
    const Sequence& seq = dottedAln.getSequence(i);
    bool isRef = true;
    for (size_t j = 0; isRef && j < seq.size(); ++j) // For each site in the sequence
    {
      if (seq.getChar(j) == ".")
        isRef = false;
    }
    if (isRef) // We found the reference sequence!
    {
      refSeq = &seq;
    }
  }
  if (!refSeq)
    throw Exception("SiteContainerTools::resolveDottedAlignment. No reference sequence was found in the input alignment.");

  // Now we build a new VectorSiteContainer:
  auto sites = make_unique<BasicVectorSiteContainer>(dottedAln.getSequenceKeys(), resolvedAlphabet);
  sites->setComments(dottedAln.getComments());

  // We add each site one by one:
  size_t m = dottedAln.getNumberOfSites();
  string state;
  for (size_t i = 0; i < m; ++i)
  {
    string resolved = refSeq->getChar(i);
    const Site& site = dottedAln.getSite(i);
    auto resolvedSite = make_unique<Site>(resolvedAlphabet, site.getPosition());
    for (size_t j = 0; j < n; ++j)
    {
      state = site.getChar(j);
      if (state == ".")
      {
        state = resolved;
      }
      resolvedSite.addElement(state);
    }
    // Add the new site:
    sites->addSite(resolvedSite, false);
  }

  // Return result:
  return sites;
}

/******************************************************************************/

std::map<size_t, size_t> SiteContainerTools::getSequencePositions(const Sequence& seq)
{
  int gapCode = seq.getAlphabet()->getGapCharacterCode();
  map<size_t, size_t> tln;
  if (seq.size() == 0)
    return tln;
  size_t count = 0;
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (seq[i] != gapCode)
    {
      count++;
      tln[i + 1] = count;
    }
  }
  return tln;
}

/******************************************************************************/

std::map<size_t, size_t> SiteContainerTools::getAlignmentPositions(const Sequence& seq)
{
  int gapCode = seq.getAlphabet()->getGapCharacterCode();
  map<size_t, size_t> tln;
  if (seq.size() == 0)
    return tln;
  size_t count = 0;
  for (size_t i = 0; i < seq.size(); ++i)
  {
    if (seq[i] != gapCode)
    {
      count++;
      tln[count] = i + 1;
    }
  }
  return tln;
}

/******************************************************************************/

std::map<size_t, size_t> SiteContainerTools::translateAlignment(const Sequence& seq1, const Sequence& seq2)
{
  if (seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::translateAlignment", seq1.getAlphabet(), seq2.getAlphabet());
  map<size_t, size_t> tln;
  if (seq1.size() == 0)
    return tln;
  unsigned int count1 = 0;
  unsigned int count2 = 0;
  if (seq2.size() == 0)
    throw Exception("SiteContainerTools::translateAlignment. Sequences do not match at position " + TextTools::toString(count1 + 1) + " and " + TextTools::toString(count2 + 1) + ".");
  int state1 = seq1[count1];
  int state2 = seq2[count2];
  bool end = false;
  while (!end)
  {
    while (state1 == -1)
    {
      count1++;
      if (count1 < seq1.size())
        state1 = seq1[count1];
      else
        break;
    }
    while (state2 == -1)
    {
      count2++;
      if (count2 < seq2.size())
        state2 = seq2[count2];
      else
        break;
    }
    if (state1 != state2)
      throw Exception("SiteContainerTools::translateAlignment. Sequences do not match at position " + TextTools::toString(count1 + 1) + " and " + TextTools::toString(count2 + 1) + ".");
    tln[count1 + 1] = count2 + 1; // Count start at 1
    if (count1 == seq1.size() - 1)
      end = true;
    else
    {
      if (count2 == seq2.size() - 1)
      {
        state1 = seq1[++count1];
        while (state1 == -1)
        {
          count1++;
          if (count1 < seq1.size())
            state1 = seq1[count1];
          else
            break;
        }
        if (state1 == -1)
          end = true;
        else
          throw Exception("SiteContainerTools::translateAlignment. Sequences do not match at position " + TextTools::toString(count1 + 1) + " and " + TextTools::toString(count2 + 1) + ".");
      }
      else
      {
        state1 = seq1[++count1];
        state2 = seq2[++count2];
      }
    }
  }
  return tln;
}

/******************************************************************************/

std::map<size_t, size_t> SiteContainerTools::translateSequence(const BasicSiteContainer& sequences, size_t i1, size_t i2)
{
  const Sequence& seq1 = sequences.getSequence(i1);
  const Sequence& seq2 = sequences.getSequence(i2);
  map<size_t, size_t> tln;
  size_t count1 = 0; // Sequence 1 counter
  size_t count2 = 0; // Sequence 2 counter
  int state1;
  int state2;
  for (size_t i = 0; i <  sequences.getNumberOfSites(); i++)
  {
    state1 = seq1[i];
    if (state1 != -1)
      count1++;
    state2 = seq2[i];
    if (state2 != -1)
      count2++;
    if (state1 != -1)
    {
      tln[count1] = (state2 == -1 ? 0 : count2);
    }
  }
  return tln;
}

/******************************************************************************/

std::unique_ptr<BasicAlignedSequenceContainer> SiteContainerTools::alignNW(
  const Sequence& seq1,
  const Sequence& seq2,
  const AlphabetIndex2& s,
  double gap)
{
  if (seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::alignNW", seq1.getAlphabet(), seq2.getAlphabet());
  if (seq1.getAlphabet()->getAlphabetType() != s.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::alignNW", seq1.getAlphabet(), s.getAlphabet());
  // Check that sequences have no gap!
  unique_ptr<Sequence> s1(seq1.clone());
  SequenceTools::removeGaps(*s1);
  unique_ptr<Sequence> s2(seq2.clone());
  SequenceTools::removeGaps(*s2);

  // 1) Initialize matrix:
  RowMatrix<double> m(s1->size() + 1, s2->size() + 1);
  RowMatrix<char>   p(s1->size(), s2->size());
  double choice1, choice2, choice3, mx;
  char px;
  for (size_t i = 0; i <= s1->size(); i++)
  {
    m(i, 0) = static_cast<double>(i) * gap;
  }
  for (size_t j = 0; j <= s2->size(); j++)
  {
    m(0, j) = static_cast<double>(j) * gap;
  }
  for (size_t i = 1; i <= s1->size(); i++)
  {
    for (size_t j = 1; j <= s2->size(); j++)
    {
      choice1 = m(i - 1, j - 1) + static_cast<double>(s.getIndex((*s1)[i - 1], (*s2)[j - 1]));
      choice2 = m(i - 1, j) + gap;
      choice3 = m(i, j - 1) + gap;
      mx = choice1; px = 'd'; // Default in case of equality of scores.
      if (choice2 > mx)
      {
        mx = choice2; px = 'u';
      }
      if (choice3 > mx)
      {
        mx = choice3; px = 'l';
      }
      m(i, j) = mx;
      p(i - 1, j - 1) = px;
    }
  }

  // 2) Get alignment:
  deque<int> a1, a2;
  size_t i = s1->size(), j = s2->size();
  char c;
  while (i > 0 && j > 0)
  {
    c = p(i - 1, j - 1);
    if (c == 'd')
    {
      a1.push_front((*s1)[i - 1]);
      a2.push_front((*s2)[j - 1]);
      i--;
      j--;
    }
    else if (c == 'u')
    {
      a1.push_front((*s1)[i - 1]);
      a2.push_front(-1);
      i--;
    }
    else
    {
      a1.push_front(-1);
      a2.push_front((*s2)[j - 1]);
      j--;
    }
  }
  while (i > 0)
  {
    a1.push_front((*s1)[i - 1]);
    a2.push_front(-1);
    i--;
  }
  while (j > 0)
  {
    a1.push_front(-1);
    a2.push_front((*s2)[j - 1]);
    j--;
  }
  s1->setContent(vector<int>(a1.begin(), a1.end()));
  s2->setContent(vector<int>(a2.begin(), a2.end()));
  auto asc = make_unique<BasicAlignedSequenceContainer>(s1->getAlphabet());
  asc->addSequence(s1->getName(), s1);
  asc->addSequence(s2->getName(), s2);
  return asc;
}

/******************************************************************************/

unique_ptr<BasicAlignedSequenceContainer> SiteContainerTools::alignNW(
  const Sequence& seq1,
  const Sequence& seq2,
  const AlphabetIndex2& s,
  double opening,
  double extending)
{
  if (seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::alignNW", seq1.getAlphabet(), seq2.getAlphabet());
  if (seq1.getAlphabet()->getAlphabetType() != s.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::alignNW", seq1.getAlphabet(), s.getAlphabet());
  // Check that sequences have no gap!
  unique_ptr<Sequence> s1(seq1.clone());
  SequenceTools::removeGaps(*s1);
  unique_ptr<Sequence> s2(seq2.clone());
  SequenceTools::removeGaps(*s2);

  // 1) Initialize matrix:
  RowMatrix<double> m(s1->size() + 1, s2->size() + 1);
  RowMatrix<double> v(s1->size() + 1, s2->size() + 1);
  RowMatrix<double> h(s1->size() + 1, s2->size() + 1);
  RowMatrix<char>   p(s1->size(), s2->size());

  double choice1, choice2, choice3, mx;
  char px;
  m(0, 0) = 0.;
  for (size_t i = 0; i <= s1->size(); i++)
  {
    v(i, 0) = log(0.);
  }
  for (size_t j = 0; j <= s2->size(); j++)
  {
    h(0, j) = log(0.);
  }
  for (size_t i = 1; i <= s1->size(); i++)
  {
    m(i, 0) = h(i, 0) = opening + static_cast<double>(i) * extending;
  }
  for (size_t j = 1; j <= s2->size(); j++)
  {
    m(0, j) = v(0, j) = opening + static_cast<double>(j) * extending;
  }

  for (size_t i = 1; i <= s1->size(); i++)
  {
    for (size_t j = 1; j <= s2->size(); j++)
    {
      choice1 = m(i - 1, j - 1) + s.getIndex((*s1)[i - 1], (*s2)[j - 1]);
      choice2 = h(i - 1, j - 1) + opening + extending;
      choice3 = v(i - 1, j - 1) + opening + extending;
      mx = choice1; // Default in case of equality of scores.
      if (choice2 > mx)
      {
        mx = choice2;
      }
      if (choice3 > mx)
      {
        mx = choice3;
      }
      m(i, j) = mx;

      choice1 = m(i, j - 1) + opening + extending;
      choice2 = h(i, j - 1) + extending;
      mx = choice1; // Default in case of equality of scores.
      if (choice2 > mx)
      {
        mx = choice2;
      }
      h(i, j) = mx;

      choice1 = m(i - 1, j) + opening + extending;
      choice2 = v(i - 1, j) + extending;
      mx = choice1; // Default in case of equality of scores.
      if (choice2 > mx)
      {
        mx = choice2;
      }
      v(i, j) = mx;

      px = 'd';
      if (v(i, j) > m(i, j))
        px = 'u';
      if (h(i, j) > m(i, j))
        px = 'l';
      p(i - 1, j - 1) = px;
    }
  }

  // 2) Get alignment:
  deque<int> a1, a2;
  size_t i = s1->size(), j = s2->size();
  char c;
  while (i > 0 && j > 0)
  {
    c = p(i - 1, j - 1);
    if (c == 'd')
    {
      a1.push_front((*s1)[i - 1]);
      a2.push_front((*s2)[j - 1]);
      i--;
      j--;
    }
    else if (c == 'u')
    {
      a1.push_front((*s1)[i - 1]);
      a2.push_front(-1);
      i--;
    }
    else
    {
      a1.push_front(-1);
      a2.push_front((*s2)[j - 1]);
      j--;
    }
  }
  while (i > 0)
  {
    a1.push_front((*s1)[i - 1]);
    a2.push_front(-1);
    i--;
  }
  while (j > 0)
  {
    a1.push_front(-1);
    a2.push_front((*s2)[j - 1]);
    j--;
  }
  s1->setContent(vector<int>(a1.begin(), a1.end()));
  s2->setContent(vector<int>(a2.begin(), a2.end()));
  auto asc = make_unique<BasicAlignedSequenceContainer>(s1->getAlphabet());
  asc->addSequence(s1->getName(), s1);
  asc->addSequence(s2->getName(), s2);
  return asc;
}

/******************************************************************************/

const string SiteContainerTools::SIMILARITY_ALL         = "all sites";
const string SiteContainerTools::SIMILARITY_NOFULLGAP   = "no full gap";
const string SiteContainerTools::SIMILARITY_NODOUBLEGAP = "no double gap";
const string SiteContainerTools::SIMILARITY_NOGAP       = "no gap";

/******************************************************************************/

double SiteContainerTools::computeSimilarity(const Sequence& seq1, const Sequence& seq2, bool dist, const std::string& gapOption, bool unresolvedAsGap)
{
  if (seq1.size() != seq2.size())
    throw SequenceNotAlignedException("SiteContainerTools::computeSimilarity.", &seq2);
  if (seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::computeSimilarity.", seq1.getAlphabet(), seq2.getAlphabet());

  std::shared_ptr<const Alphabet> alpha = seq1.getAlphabet();
  unsigned int s = 0;
  unsigned int t = 0;
  for (size_t i = 0; i < seq1.size(); i++)
  {
    int x = seq1[i];
    int y = seq2[i];
    int gapCode = alpha->getGapCharacterCode();
    if (unresolvedAsGap)
    {
      if (alpha->isUnresolved(x))
        x = gapCode;
      if (alpha->isUnresolved(y))
        y = gapCode;
    }
    if (gapOption == SIMILARITY_ALL)
    {
      t++;
      if (x == y && !alpha->isGap(x) && !alpha->isGap(y))
        s++;
    }
    else if (gapOption == SIMILARITY_NODOUBLEGAP)
    {
      if (!alpha->isGap(x) || !alpha->isGap(y))
      {
        t++;
        if (x == y)
          s++;
      }
    }
    else if (gapOption == SIMILARITY_NOGAP)
    {
      if (!alpha->isGap(x) && !alpha->isGap(y))
      {
        t++;
        if (x == y)
          s++;
      }
    }
    else
      throw Exception("SiteContainerTools::computeSimilarity. Invalid gap option: " + gapOption);
  }
  double r = (t == 0 ? 0. : static_cast<double>(s) / static_cast<double>(t));
  return dist ? 1 - r : r;
}

/******************************************************************************/

std::unique_ptr<DistanceMatrix> SiteContainerTools::computeSimilarityMatrix(
		const SiteContainer& sites,
		bool dist,
		const std::string& gapOption,
		bool unresolvedAsGap)
{
  size_t n = sites.getNumberOfSequences();
  auto mat = make_unique<DistanceMatrix>(sites.getSequenceNames());
  string pairwiseGapOption = gapOption;
  SiteContainer* sites2;
  if (gapOption == SIMILARITY_NOFULLGAP)
  {
    if (unresolvedAsGap)
    {
      SiteContainer* tmp = removeGapOrUnresolvedOnlySites(sites);
      sites2 = new AlignedSequenceContainer(*tmp);
      delete tmp;
    }
    else
    {
      SiteContainer* tmp = removeGapOnlySites(sites);
      sites2 = new AlignedSequenceContainer(*tmp);
      delete tmp;
    }
    pairwiseGapOption = SIMILARITY_ALL;
  }
  else
  {
    sites2 = new AlignedSequenceContainer(sites);
  }

  for (size_t i = 0; i < n; i++)
  {
    (*mat)(i, i) = dist ? 0. : 1.;
    const Sequence* seq1 = &sites2->getSequence(i);
    for (size_t j = i + 1; j < n; j++)
    {
      const Sequence* seq2 = &sites2->getSequence(j);
      (*mat)(i, j) = (*mat)(j, i) = computeSimilarity(*seq1, *seq2, dist, pairwiseGapOption, unresolvedAsGap);
    }
  }
  delete sites2;
  return mat;
}

/******************************************************************************/

void SiteContainerTools::getSequencePositions(const BasicSiteContainer& sites, Matrix<size_t>& positions)
{
  positions.resize(sites.getNumberOfSequences(), sites.getNumberOfSites());
  int gapCode = sites.getAlphabet()->getGapCharacterCode();
  for (size_t i = 0; i < sites.getNumberOfSequences(); ++i)
  {
    const Sequence& seq = sites.getSequence(i);
    size_t pos = 0;
    for (size_t j = 0; j < sites.getNumberOfSites(); ++j)
    {
      if (seq[j] != gapCode)
      {
        ++pos;
        positions(i, j) = pos;
      }
      else
      {
        positions(i, j) = 0;
      }
    }
  }
}

/******************************************************************************/

vector<int> SiteContainerTools::getColumnScores(const Matrix<size_t>& positions1, const Matrix<size_t>& positions2, int na)
{
  if (positions1.getNumberOfRows() != positions2.getNumberOfRows())
    throw Exception("SiteContainerTools::getColumnScores. The two input alignments must have the same number of sequences!");
  vector<int> scores(positions1.getNumberOfColumns());
  for (size_t i = 0; i < positions1.getNumberOfColumns(); ++i)
  {
    // Find an anchor point:
    size_t whichSeq = 0;
    size_t whichPos = 0;
    for (size_t j = 0; j < positions1.getNumberOfRows(); ++j)
    {
      if (positions1(j, i) > 0)
      {
        whichSeq = j;
        whichPos = positions1(j, i);
        break;
      }
    }
    if (whichPos == 0)
    {
      // No anchor found, this alignment column is only made of gaps. We assign a score of 'na' and move to the next column.
      scores[i] = na;
      continue;
    }
    // We look for the anchor in the reference alignment:
    size_t i2 = 0;
    bool found = false;
    for (size_t j = 0; !found && j < positions2.getNumberOfColumns(); ++j)
    {
      if (positions2(whichSeq, j) == whichPos)
      {
        i2 = j;
        found = true;
      }
    }
    if (!found)
    {
      throw Exception("SiteContainerTools::getColumnScores(). Position " + TextTools::toString(whichPos) + " of sequence " + TextTools::toString(whichSeq) + " not found in reference alignment. Please make sure the two indexes are built from the same data!");
    }
    // Now we compare all pairs of sequences between the two positions:
    bool test = true;
    for (size_t j = 0; test && j < positions1.getNumberOfRows(); ++j)
    {
      test = (positions1(j, i) == positions2(j, i2));
    }
    scores[i] = test ? 1 : 0;
  }
  return scores;
}

/******************************************************************************/

vector<double> SiteContainerTools::getSumOfPairsScores(const Matrix<size_t>& positions1, const Matrix<size_t>& positions2, double na)
{
  if (positions1.getNumberOfRows() != positions2.getNumberOfRows())
    throw Exception("SiteContainerTools::getColumnScores. The two input alignments must have the same number of sequences!");
  vector<double> scores(positions1.getNumberOfColumns());
  for (size_t i = 0; i < positions1.getNumberOfColumns(); ++i)
  {
    // For all positions in alignment 1...
    size_t countAlignable = 0;
    size_t countAligned   = 0;
    for (size_t j = 0; j < positions1.getNumberOfRows(); ++j)
    {
      // Get the corresponding column in alignment 2:
      size_t whichPos = positions1(j, i);
      if (whichPos == 0)
      {
        // No position for this sequence here.
        continue;
      }
      // We look for the position in the second alignment:
      size_t i2 = 0;
      bool found = false;
      for (size_t k = 0; !found && k < positions2.getNumberOfColumns(); ++k)
      {
        if (positions2(j, k) == whichPos)
        {
          i2 = k;
          found = true;
        }
      }
      if (!found)
      {
        throw Exception("SiteContainerTools::getColumnScores(). Position " + TextTools::toString(whichPos) + " of sequence " + TextTools::toString(j) + " not found in reference alignment. Please make sure the two indexes are built from the same data!");
      }

      // Now we check all other positions and see if they are aligned with this one:
      for (size_t k = j + 1; k < positions1.getNumberOfRows(); ++k)
      {
        size_t whichPos2 = positions1(k, i);
        if (whichPos2 == 0)
        {
          // Empty position
          continue;
        }
        countAlignable++;
        // check position in alignment 2:
        if (positions2(k, i2) == whichPos2)
          countAligned++;
      }
    }
    scores[i] = countAlignable == 0 ? na : static_cast<double>(countAligned) / static_cast<double>(countAlignable);
  }
  return scores;
}

/******************************************************************************/
