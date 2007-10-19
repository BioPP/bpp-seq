//
// File: SymbolListTools.cpp
// Created by: Julien Dutheil
// Created on: Wed Apr 9 2004
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

#include "SymbolListTools.h"
#include "AlphabetTools.h"

map<int, unsigned int> SymbolListTools::getCounts(const SymbolList & list)
{
  map<int, unsigned int> c;
  vector<int> seq = list.getContent();
  for(unsigned int i = 0; i < seq.size(); i++) c[seq[i]]++;
  return c;
}

map<int, double> SymbolListTools::getCounts(const SymbolList & list, bool resolveUnknowns)
{
	map<int, double> c;
	vector<int> seq = list.getContent();
  if(!resolveUnknowns)
  {
    for(unsigned int i = 0; i < seq.size(); i++) c[seq[i]]++;
  }
  else
  {
	  for(unsigned int i = 0; i < seq.size(); i++) 
    {
      vector<int> alias = list.getAlphabet()->getAlias(seq[i]);
      double n = (double)alias.size();
      for(unsigned int j = 0; j < alias.size(); j++)
      {
        c[alias[j]] += 1./n ;
      }
    }
  }
	return c;
}

map<int, double> SymbolListTools::getFrequencies(const SymbolList & list, bool resolveUnknowns)
{
	map<int, double> f;
	double n = (double)list.size();
  map<int, double> counts = getCounts(list, resolveUnknowns);
  for(map<int, double>::iterator i = counts.begin(); i != counts.end(); i++)
  {
    f[i->first] = i -> second / n;
  }
  return f;
}

double SymbolListTools::getGCContent(const SymbolList & list, bool ignoreUnresolved, bool ignoreGap) throw (AlphabetException)
{
  const Alphabet * alphabet = list.getAlphabet();
  if(!AlphabetTools::isNucleicAlphabet(alphabet))
    throw AlphabetException("SymbolListTools::getGCContent. Method only works on nucleotides.", alphabet);
  double gc = 0;
  double total = 0;
  for(unsigned int i = 0; i < list.size(); i++) {
    int state = list.getValue(i);
    if(state > -1) { // not a gap
      if(state == 1 || state == 2) { // G or C
        gc++;
        total++;
      } else if(state == 0 || state == 3) { // A, T or U
        total++;
      } else { // Unresolved character
        if(!ignoreUnresolved) {
          total++;
          switch(state) {
            case(7): gc++; break;// G or C
            case(4): gc+=0.5; break;// A or C
            case(5): gc+=0.5; break;// A or G
            case(6): gc+=0.5; break;// C or T
            case(9): gc+=0.5; break;// G or T
            case(10): gc+=2./3.; break;// A or C or G
            case(11): gc+=1./3.; break;// A or C or T
            case(12): gc+=1./3.; break;// A or G or T
            case(13): gc+=2./3.; break;// C or G or T
            case(14): gc+=0.5; break;// A or C or G or T
          }
        }
      }
    } else {
      if(!ignoreGap) total++;
    }
  }
  return total != 0 ? gc/total : 0;
}

unsigned int SymbolListTools::getNumberOfDistinctPositions(const SymbolList & l1, const SymbolList & l2) throw (AlphabetMismatchException)
{
	if(l1.getAlphabet() -> getAlphabetType() != l2.getAlphabet() -> getAlphabetType()) throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
	unsigned int n = min(l1.size(), l2.size());
	unsigned int count = 0;
	for(unsigned int i = 0; i < n; i++) {
		if(l1[i] != l2[i]) count++;
	}
	return count;
}

unsigned int SymbolListTools::getNumberOfPositionsWithoutGap(const SymbolList & l1, const SymbolList & l2) throw (AlphabetMismatchException)
{
	if(l1.getAlphabet() -> getAlphabetType() != l2.getAlphabet() -> getAlphabetType()) throw AlphabetMismatchException("SymbolListTools::getNumberOfDistinctPositions.", l1.getAlphabet(), l2.getAlphabet());
	unsigned int n = min(l1.size(), l2.size());
	unsigned int count = 0;
	for(unsigned int i = 0; i < n; i++) {
		if(l1[i] != -1 && l2[i] != -1) count++;
	}
	return count;
}

void SymbolListTools::changeGapsToUnknownCharacters(SymbolList & l)
{
  int unknownCode = l.getAlphabet()->getUnknownCharacterCode();
  for(unsigned int i = 0; i < l.size(); i++)
  {
    if(l.getAlphabet()->isGap(l[i])) l[i] = unknownCode;
  }
}

void SymbolListTools::changeUnresolvedCharactersToGaps(SymbolList & l)
{
  int gapCode = l.getAlphabet()->getGapCharacterCode();
  for(unsigned int i = 0; i < l.size(); i++)
  {
    if(l.getAlphabet()->isUnresolved(l[i])) l[i] = gapCode;
  }
}


