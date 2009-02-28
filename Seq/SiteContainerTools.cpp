//
// File: SiteContainerTools.cpp
// Created by: Julien Dutheil
//             Sylvain Glémin
// Created on: Fri Dec 12 18:55:06 2003
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

#include "SiteContainerTools.h"
#include "VectorSiteContainer.h"
#include "SiteIterator.h"
#include "SiteTools.h"
#include "AlphabetTools.h"
#include "SequenceTools.h"

using namespace bpp;

// From the STL:
#include <vector>
#include <deque>
#include <string>

using namespace std;

/******************************************************************************/

SiteContainer * SiteContainerTools::getSitesWithoutGaps(const SiteContainer & sites)
{
  vector<string> seqNames = sites.getSequencesNames();
  VectorSiteContainer * noGapCont = new VectorSiteContainer(seqNames.size(), sites.getAlphabet());
  noGapCont->setSequencesNames(seqNames, false);
  NoGapSiteIterator ngsi(sites);
  while(ngsi.hasMoreSites())
  {
    noGapCont->addSite(* ngsi.nextSite());
  }
  return noGapCont;
}

/******************************************************************************/

SiteContainer * SiteContainerTools::getCompleteSites(const SiteContainer & sites)
{
  vector<string> seqNames = sites.getSequencesNames();
  VectorSiteContainer * noGapCont = new VectorSiteContainer(seqNames.size(), sites.getAlphabet());
  noGapCont->setSequencesNames(seqNames, false);
  CompleteSiteIterator csi(sites);
  while(csi.hasMoreSites())
  {
    noGapCont->addSite(* csi.nextSite());
  }
  return noGapCont;
}

/******************************************************************************/

SiteContainer * SiteContainerTools::getSelectedSites(
    const SiteContainer & sequences,
    const SiteSelection & selection)
{
  vector<string> seqNames = sequences.getSequencesNames();
  VectorSiteContainer * sc = new VectorSiteContainer(seqNames.size(), sequences.getAlphabet());
  sc->setSequencesNames(seqNames, false);
  for(unsigned int i = 0; i < selection.size(); i++)
  {
    sc->addSite(* sequences.getSite(selection[i]), false);
   //We do not check names, we suppose that the container passed as an argument is correct.
   //WARNING: what if selection contains many times the same indice? ...
  }
  sc->setGeneralComments(sequences.getGeneralComments());
  return sc;
}

/******************************************************************************/

const Sequence * SiteContainerTools::getConsensus(const SiteContainer & sc, string name, bool ignoreGap, bool resolveUnknown)
{
  Vint consensus;
  SimpleSiteIterator ssi(sc);
  const Site * site;
  while(ssi.hasMoreSites())
  {
    site = ssi.nextSite();
    map<int, double> freq;
    SiteTools::getFrequencies(*site, freq, resolveUnknown);
    double max = 0;
    int cons = -1; //default result
    if(ignoreGap)
    {
      for(map<int, double>::iterator it = freq.begin(); it != freq.end(); it++)
      {
        if(it->second > max && it->first != -1)
        {
          max = it->second;
          cons = it->first;
        }
      }
    }
    else
    {
      for(map<int, double>::iterator it = freq.begin(); it != freq.end(); it++)
      {
        if(it->second > max)
        {
          max = it->second;
          cons = it->first;
        }
      }
    }
    consensus.push_back(cons);
  }
  const Sequence * seqConsensus = new Sequence(name, consensus, sc.getSequence(0)->getAlphabet());
  return seqConsensus;
}

/******************************************************************************/

void SiteContainerTools::changeGapsToUnknownCharacters(SiteContainer & sites)
{
  //NB: use iterators for a better algorithm? 
  int unknownCode = sites.getAlphabet()->getUnknownCharacterCode();
  for(unsigned int i = 0; i < sites.getNumberOfSites(); i++)
  {
    for(unsigned int j = 0; j < sites.getNumberOfSequences(); j++)
    {
      int * element = & sites(j,i);
      if(sites.getAlphabet()->isGap(*element)) *element = unknownCode;
    }
  }
}

/******************************************************************************/

void SiteContainerTools::changeUnresolvedCharactersToGaps(SiteContainer & sites)
{
  //NB: use iterators for a better algorithm? 
  int gapCode = sites.getAlphabet()->getGapCharacterCode();
  for(unsigned int i = 0; i < sites.getNumberOfSites(); i++)
  {
    for(unsigned int j = 0; j < sites.getNumberOfSequences(); j++)
    {
      int * element = & sites(j,i);
      if(sites.getAlphabet()->isUnresolved(*element)) *element = gapCode;
    }
  }
}

/******************************************************************************/

SiteContainer * SiteContainerTools::removeGapOnlySites(const SiteContainer & sites)
{
  vector<string> seqNames = sites.getSequencesNames();
  VectorSiteContainer * noGapCont = new VectorSiteContainer(seqNames.size(), sites.getAlphabet());
  noGapCont->setSequencesNames(seqNames, false);
  for(unsigned int i = 0; i < sites.getNumberOfSites(); i++)
  {
    const Site * site = sites.getSite(i);
    if(!SiteTools::isGapOnly(*site)) noGapCont->addSite(* site);
  }
  return noGapCont;
}

/******************************************************************************/

SiteContainer * SiteContainerTools::removeGapOrUnresolvedOnlySites(const SiteContainer & sites)
{
  vector<string> seqNames = sites.getSequencesNames();
  VectorSiteContainer * noGapCont = new VectorSiteContainer(seqNames.size(), sites.getAlphabet());
  noGapCont->setSequencesNames(seqNames, false);
  for(unsigned int i = 0; i < sites.getNumberOfSites(); i++)
  {
    const Site * site = sites.getSite(i);
    if(!SiteTools::isGapOrUnresolvedOnly(*site)) noGapCont->addSite(* site);
  }
  return noGapCont;
}

/******************************************************************************/

SiteContainer * SiteContainerTools::resolveDottedAlignment(
    const SiteContainer & dottedAln,
    const Alphabet * resolvedAlphabet) throw (AlphabetException, Exception)
{
  if(!AlphabetTools::isDefaultAlphabet(dottedAln.getAlphabet()))
    throw AlphabetException("SiteContainerTools::resolveDottedAlignment. Alignment alphabet should of class 'DefaultAlphabet'.", dottedAln.getAlphabet());
  
  //First we look for the reference sequence:
  unsigned int n = dottedAln.getNumberOfSequences();
  if(n == 0) throw Exception("SiteContainerTools::resolveDottedAlignment. Input alignment contains no sequence.");

  const Sequence * refSeq = NULL;
  for(unsigned int  i = 0; i < n; i++) //Test each sequence
  {
    const Sequence * seq = dottedAln.getSequence(i);
    bool isRef = true;
    for(unsigned int j = 0; isRef && j < seq->size(); j++) //For each site in the sequence
    {
      if(seq->getChar(j) == ".") isRef = false;
    }
    if(isRef) //We found the reference sequence!
    {
      refSeq = new Sequence(*seq);
    }
  }
  if(!refSeq) throw Exception("SiteContainerTools::resolveDottedAlignment. No reference sequence was found in the input alignment.");

  //Now we build a new VectorSiteContainer:
  VectorSiteContainer * sites = new VectorSiteContainer(n, resolvedAlphabet);

  //We add each site one by one:
  unsigned int m = dottedAln.getNumberOfSites();
  string state;
  for(unsigned int i = 0; i < m; i++)
  {
    string resolved = refSeq->getChar(i);
    const Site * site = dottedAln.getSite(i);
    Site resolvedSite(resolvedAlphabet, site->getPosition());
    for(unsigned int j = 0; j < n; j++)
    {
      state = site->getChar(j);
      if(state == ".")
      {
        state = resolved;
      }
      resolvedSite.addElement(state);
    }
    //Add the new site:
    sites->addSite(resolvedSite);
  }

  //Seq sequence names:
  sites->setSequencesNames(dottedAln.getSequencesNames());
  
  //Delete the copied sequence:
  delete refSeq;
  
  //Return result:
  return sites;
}

/******************************************************************************/

map<unsigned int, unsigned int> SiteContainerTools::getSequencePositions(const Sequence & seq)
{
  map<unsigned int, unsigned int> tln;
  if(seq.size() == 0) return tln;
  unsigned int count = 0;
  for(unsigned int i = 0; i < seq.size(); i++)
  {
    if(seq[i] != -1)
    {
      count++;
      tln[i+1] = count;
    }
  }
  return tln;
}

/******************************************************************************/

map<unsigned int, unsigned int> SiteContainerTools::getAlignmentPositions(const Sequence & seq)
{
  map<unsigned int, unsigned int> tln;
  if(seq.size() == 0) return tln;
  unsigned int count = 0;
  for(unsigned int i = 0; i < seq.size(); i++)
  {
    if(seq[i] != -1)
    {
      count++;
      tln[count] = i+1;
    }
  }
  return tln;
}

/******************************************************************************/

map<unsigned int, unsigned int> SiteContainerTools::translateAlignment(const Sequence & seq1, const Sequence & seq2)
  throw (AlphabetMismatchException, Exception)
{
  if(seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::translateAlignment", seq1.getAlphabet(), seq2.getAlphabet());
  map<unsigned int, unsigned int> tln;
  if(seq1.size() == 0) return tln;
  unsigned int count1 = 0;
  unsigned int count2 = 0;
  if(seq2.size() == 0) throw Exception("SiteContainerTools::translateAlignment. Sequences do not match at position " + TextTools::toString(count1+1) + " and " + TextTools::toString(count2+1) + ".");
  int state1 = seq1[count1];
  int state2 = seq2[count2];
  bool end = false;
  while(!end)
  {
    while(state1 == -1) 
    {
      count1++;
      if(count1 < seq1.size()) state1 = seq1[count1];
      else break;
    }
    while(state2 == -1)
    {
      count2++;
      if(count2 < seq2.size()) state2 = seq2[count2];
      else break;
    }
    if(state1 != state2) throw Exception("SiteContainerTools::translateAlignment. Sequences do not match at position " + TextTools::toString(count1+1) + " and " + TextTools::toString(count2+1) + ".");
    tln[count1+1] = count2+1;//Count start at 1
    if(count1 == seq1.size() - 1) end = true;
    else
    {
      if(count2 == seq2.size() - 1)
      {
        state1 = seq1[++count1];
        while(state1 == -1)
        {
          count1++;
          if(count1 < seq1.size()) state1 = seq1[count1];
          else break;
        }
        if(state1 == -1) end = true;
        else throw Exception("SiteContainerTools::translateAlignment. Sequences do not match at position " + TextTools::toString(count1+1) + " and " + TextTools::toString(count2+1) + ".");
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

map<unsigned int, unsigned int> SiteContainerTools::translateSequence(const SiteContainer & sequences, unsigned int i1, unsigned int i2)
{
  const Sequence * seq1 = sequences.getSequence(i1);
  const Sequence * seq2 = sequences.getSequence(i2);
  map<unsigned int, unsigned int> tln;
  unsigned int count1 = 0; //Sequence 1 counter
  unsigned int count2 = 0; //Sequence 2 counter
  int state1;
  int state2;
  for(unsigned int i = 0; i <  sequences.getNumberOfSites(); i++)
  {
    state1 = (*seq1)[i];
    if(state1 != -1) count1++;
    state2 = (*seq2)[i];
    if(state2 != -1) count2++;
    if(state1 != -1)
    {
      tln[count1] = (state2 == -1 ? 0 : count2);
    }
  }
  return tln;
}

/******************************************************************************/
    
AlignedSequenceContainer * SiteContainerTools::alignNW(
    const Sequence & seq1,
    const Sequence & seq2,
    const AlphabetIndex2<double> & s,
    double gap)
throw (AlphabetMismatchException)
{
  if(seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::alignNW", seq1.getAlphabet(), seq2.getAlphabet());
  if(seq1.getAlphabet()->getAlphabetType() != s.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::alignNW", seq1.getAlphabet(), s.getAlphabet());
  //Check that sequences have no gap!
  Sequence * s1 = SequenceTools::removeGaps(seq1);
  Sequence * s2 = SequenceTools::removeGaps(seq2);
  
  //1) Initialize matrix:
  RowMatrix<double> m(s1->size() + 1, s2->size() + 1);
  RowMatrix<char>   p(s1->size(), s2->size());
  double choice1, choice2, choice3, mx;
  char px;
  for(unsigned int i = 0; i <= s1->size(); i++) m(i,0) = i * gap;
  for(unsigned int j = 0; j <= s2->size(); j++) m(0,j) = j * gap;
  for(unsigned int i = 1; i <= s1->size(); i++)
  {
    for(unsigned int j = 1; j <= s2->size(); j++)
    {
      choice1 = m(i-1,j-1) + s.getIndex((*s1)[i-1], (*s2)[j-1]);
      choice2 = m(i-1,j) + gap;
      choice3 = m(i,j-1) + gap;
      mx = choice1; px = 'd'; //Default in case of equality of scores.
      if(choice2 > mx) { mx = choice2; px = 'u'; }
      if(choice3 > mx) { mx = choice3; px = 'l'; }
      m(i,j) = mx;
      p(i-1,j-1) = px;
    }
  }

  //2) Get alignment:
  deque<int> a1, a2;
  unsigned int i = s1->size(), j = s2->size();
  char c;
  while(i > 0 && j > 0)
  {
    c = p(i-1, j-1);
    if(c == 'd')
    {
      a1.push_front((*s1)[i-1]);
      a2.push_front((*s2)[j-1]);
      i--;
      j--;
    }
    else if(c == 'u')
    {
      a1.push_front((*s1)[i-1]);
      a2.push_front(-1);
      i--;
    }
    else
    {
      a1.push_front(-1);
      a2.push_front((*s2)[j-1]);
      j--;
    }
  }
  while (i > 0)
  {
    a1.push_front((*s1)[i-1]);
    a2.push_front(-1);
    i--;
  }
  while (j > 0)
  {
    a1.push_front(-1);
    a2.push_front((*s2)[j-1]);
    j--;
  }
  s1->setContent(vector<int>(a1.begin(), a1.end()));
  s2->setContent(vector<int>(a2.begin(), a2.end()));
  AlignedSequenceContainer * asc = new AlignedSequenceContainer(s1->getAlphabet());
  asc->addSequence(*s1, false);
  asc->addSequence(*s2, false);//Do not check for sequence names.
  delete s1;//Sequence are recopied in the container, we can delete these...
  delete s2;
  return asc;
}

/******************************************************************************/
    
AlignedSequenceContainer * SiteContainerTools::alignNW(
    const Sequence & seq1,
    const Sequence & seq2,
    const AlphabetIndex2<double> & s,
    double opening,
    double extending)
throw (AlphabetMismatchException)
{
  if(seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::alignNW", seq1.getAlphabet(), seq2.getAlphabet());
  if(seq1.getAlphabet()->getAlphabetType() != s.getAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("SiteContainerTools::alignNW", seq1.getAlphabet(), s.getAlphabet());
  //Check that sequences have no gap!
  Sequence * s1 = SequenceTools::removeGaps(seq1);
  Sequence * s2 = SequenceTools::removeGaps(seq2);
  
  //1) Initialize matrix:
  RowMatrix<double> m(s1->size() + 1, s2->size() + 1);
  RowMatrix<double> v(s1->size() + 1, s2->size() + 1);
  RowMatrix<double> h(s1->size() + 1, s2->size() + 1);
  RowMatrix<char>   p(s1->size(), s2->size());
  
  double choice1, choice2, choice3, mx;
  char px;
  m(0,0) = 0.;
  for(unsigned int i = 0; i <= s1->size(); i++) v(i,0) = log(0.);
  for(unsigned int j = 0; j <= s2->size(); j++) h(0,j) = log(0.);
  for(unsigned int i = 1; i <= s1->size(); i++) m(i,0) = h(i,0) = opening + i*extending;
  for(unsigned int j = 1; j <= s2->size(); j++) m(0,j) = v(0,j) = opening + j*extending;
  
  for(unsigned int i = 1; i <= s1->size(); i++)
  {
    for(unsigned int j = 1; j <= s2->size(); j++)
    {
      choice1 = m(i-1,j-1) + s.getIndex((*s1)[i-1], (*s2)[j-1]);
      choice2 = h(i-1,j-1) + opening + extending;
      choice3 = v(i-1,j-1) + opening + extending;
      mx = choice1; //Default in case of equality of scores.
      if(choice2 > mx) { mx = choice2; }
      if(choice3 > mx) { mx = choice3; }
      m(i,j) = mx;

      choice1 = m(i,j-1) + opening + extending;
      choice2 = h(i,j-1) + extending;
      mx = choice1; //Default in case of equality of scores.
      if(choice2 > mx) { mx = choice2; }
      h(i,j) = mx;

      choice1 = m(i-1,j) + opening + extending;
      choice2 = v(i-1,j) + extending;
      mx = choice1; //Default in case of equality of scores.
      if(choice2 > mx) { mx = choice2; }
      v(i,j) = mx;

      px = 'd';
      if(v(i,j) > m(i,j)) px='u';
      if(h(i,j) > m(i,j)) px='l';
      p(i-1,j-1) = px;
    }
  }

  //2) Get alignment:
  deque<int> a1, a2;
  unsigned int i = s1->size(), j = s2->size();
  char c;
  while(i > 0 && j > 0)
  {
    c = p(i-1, j-1);
    if(c == 'd')
    {
      a1.push_front((*s1)[i-1]);
      a2.push_front((*s2)[j-1]);
      i--;
      j--;
    }
    else if(c == 'u')
    {
      a1.push_front((*s1)[i-1]);
      a2.push_front(-1);
      i--;
    }
    else
    {
      a1.push_front(-1);
      a2.push_front((*s2)[j-1]);
      j--;
    }
  }
  while (i > 0)
  {
    a1.push_front((*s1)[i-1]);
    a2.push_front(-1);
    i--;
  }
  while (j > 0)
  {
    a1.push_front(-1);
    a2.push_front((*s2)[j-1]);
    j--;
  }
  s1->setContent(vector<int>(a1.begin(), a1.end()));
  s2->setContent(vector<int>(a2.begin(), a2.end()));
  AlignedSequenceContainer * asc = new AlignedSequenceContainer(s1->getAlphabet());
  asc->addSequence(*s1, false);
  asc->addSequence(*s2, false);//Do not check for sequence names.
  delete s1;//Sequence are recopied in the container, we can delete these...
  delete s2;
  return asc;
}

/******************************************************************************/

VectorSiteContainer * SiteContainerTools::bootstrapSites(const SiteContainer & sites)
{
  VectorSiteContainer * sample = new VectorSiteContainer(sites.getSequencesNames(), sites.getAlphabet());
  unsigned int nbSites = sites.getNumberOfSites();
  for(unsigned int i = 0; i < nbSites; i++)
  {
    unsigned int pos = (unsigned int)RandomTools::giveIntRandomNumberBetweenZeroAndEntry(nbSites);
    sample->addSite(*sites.getSite(pos), false);
  }
  return sample;
}

/******************************************************************************/

const string SiteContainerTools::SIMILARITY_ALL         = "all sites";
const string SiteContainerTools::SIMILARITY_NOFULLGAP   = "no full gap";
const string SiteContainerTools::SIMILARITY_NODOUBLEGAP = "no double gap";
const string SiteContainerTools::SIMILARITY_NOGAP       = "no gap";

/******************************************************************************/

double SiteContainerTools::computeSimilarity(const Sequence & seq1, const Sequence & seq2, bool dist, const string & gapOption, bool unresolvedAsGap) throw (SequenceNotAlignedException, AlphabetMismatchException, Exception)
{
  if(seq1.size() != seq2.size()) throw SequenceNotAlignedException("SiteContainerTools::computeSimilarity.", &seq2);
  if(seq1.getAlphabet()->getAlphabetType() != seq2.getAlphabet()->getAlphabetType()) throw AlphabetMismatchException("SiteContainerTools::computeSimilarity.", seq1.getAlphabet(), seq2.getAlphabet());

  const Alphabet * alpha = seq1.getAlphabet();
  unsigned int s = 0;
  unsigned int t = 0;
  for(unsigned int i = 0; i < seq1.size(); i++)
  {
    int x = seq1[i];
    int y = seq2[i];
    if(unresolvedAsGap)
    {
      if(alpha->isUnresolved(x)) x = alpha->getGapCharacterCode();
      if(alpha->isUnresolved(y)) y = alpha->getGapCharacterCode();
    }
    if(gapOption == SIMILARITY_ALL)
    {
      t++;
      if(x == y && !alpha->isGap(x) && !alpha->isGap(y)) s++;
    }
    else if(gapOption == SIMILARITY_NODOUBLEGAP)
    {
      if(!alpha->isGap(x) || !alpha->isGap(y))
      {
        t++;
        if(x == y) s++;
      }
    }
    else if(gapOption == SIMILARITY_NOGAP)
    {
      if(!alpha->isGap(x) && !alpha->isGap(y))
      {
        t++;
        if(x == y) s++;
      }
    }
    else throw Exception("SiteContainerTools::computeSimilarity. Invalid gap option: " + gapOption);
  }
  double r = (t == 0 ? 0. : (double)s / (double)t);
  return dist ? 1 - r : r;
}

/******************************************************************************/

DistanceMatrix * SiteContainerTools::computeSimilarityMatrix(const SiteContainer & sites, bool dist, const string & gapOption, bool unresolvedAsGap)
{
  unsigned int n = sites.getNumberOfSequences();
  DistanceMatrix *mat = new DistanceMatrix(sites.getSequencesNames());
  string pairwiseGapOption = gapOption;
  SiteContainer * sites2;
  if(gapOption == SIMILARITY_NOFULLGAP)
  {
    if(unresolvedAsGap)
    {
      SiteContainer * tmp = removeGapOrUnresolvedOnlySites(sites);
      sites2 = new AlignedSequenceContainer(*tmp);
      delete tmp;
    }
    else
    {
      SiteContainer * tmp = removeGapOnlySites(sites);
      sites2 = new AlignedSequenceContainer(*tmp);
      delete tmp;
    }
    pairwiseGapOption = SIMILARITY_ALL;
  }
  else
  {
    sites2 = new AlignedSequenceContainer(sites);
  }

  for(unsigned int i = 0; i < n; i++)
  {
    (*mat)(i, i) = 1.;
    const Sequence *seq1 = sites2->getSequence(i);
    for(unsigned int j = i + 1; j < n; j++)
    {
      const Sequence *seq2 = sites2->getSequence(j);
      (*mat)(i, j) = (*mat)(j, i) = computeSimilarity(*seq1, *seq2, dist, pairwiseGapOption, unresolvedAsGap);
    }
  }
  delete sites2;
  return mat;
}

/******************************************************************************/

