//
// File: SequenceContainerTools.cpp
// Created by: Julien Dutheil
// Created on: Sat Oct  4 09:18:34 2003
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

#include "SequenceContainerTools.h"
#include "VectorSequenceContainer.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

/******************************************************************************/	
	
SequenceContainer * SequenceContainerTools::createContainerOfSpecifiedSize(const Alphabet * alphabet, int size)
{
	VectorSequenceContainer * vsc = new VectorSequenceContainer(alphabet);
	for(int i = 0; i < size; i++)
		vsc -> addSequence(Sequence("" + i, "", alphabet), false);
	return vsc;
}

/******************************************************************************/

SequenceContainer * SequenceContainerTools::createContainerWithSequenceNames(
  const Alphabet * alphabet,
  const vector<string> & seqNames)
  throw(Exception)
{
	SequenceContainer * sc = createContainerOfSpecifiedSize(alphabet, seqNames.size());
	sc -> setSequencesNames(seqNames, true);
	return sc;
}

/******************************************************************************/

SequenceContainer * SequenceContainerTools::getSelectedSequences(
  const OrderedSequenceContainer & sequences,
  const SequenceSelection & selection)
{
  VectorSequenceContainer * sc = new VectorSequenceContainer(sequences.getAlphabet());
  for(unsigned int i = 0; i < selection.size(); i++) {
    sc -> addSequence(*sequences.getSequence(selection[i]), false);
    //We do not check names, we suppose that the container passed as an argument is correct.
    //WARNING: what if selection contains many times the same indice? ...
  }
  sc -> setGeneralComments(sequences.getGeneralComments());
  return sc;
}

/******************************************************************************/

void SequenceContainerTools::keepOnlySelectedSequences(
  OrderedSequenceContainer & sequences,
  const SequenceSelection & selection)
{
  vector<string> names = sequences.getSequencesNames();
	for(unsigned int i = 0; i < names.size(); i++) {
		// We need to do this because after removal the indices will not be the same!
    // another solution would be to sort decreasingly the indices...
		bool test = false;
		for(unsigned int j = 0; j < selection.size() && !test; j++) {
			test = (selection[j] == i);
		}
    if(!test) sequences.deleteSequence(names[i]);
    //WARNING: what if selection contains several times the same indice? ...
  }
}

/******************************************************************************/

bool SequenceContainerTools::sequencesHaveTheSameLength(const SequenceContainer & sequences)
{
	vector<string> seqNames = sequences.getSequencesNames();
	if(seqNames.size() <= 1) return true;
	unsigned int length = sequences.getSequence(seqNames[0])->size();
	for(unsigned int i = 1; i < seqNames.size(); i++)
  {
		if(sequences.getSequence(seqNames[i])->size() != length) return false;
	}
	return true;
}

/******************************************************************************/

map<int, double> SequenceContainerTools::getFrequencies(const SequenceContainer & sequences)
{
	map<int, double> f;
	int n = 0;
	vector<string> names = sequences.getSequencesNames();
	for(unsigned int j = 0; j < names.size(); j++) {
		vector<int> seq = sequences.getContent(names[j]);
		for(unsigned int i = 0; i < seq.size(); i++) f[seq[i]]++;
		n += seq.size();
	}
	for(map<int, double>::iterator i = f.begin(); i != f.end(); i++) {
		i -> second = i -> second / n;
		//cout << i -> first << "\t" << i -> second << endl;
	}
	return f;
}

/******************************************************************************/

