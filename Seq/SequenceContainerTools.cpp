//
// File: SequenceContainerTools.h
// Created by: jdutheil <jdutheil@Deedlit>
// Created on: Sat Oct  4 09:18:34 2003
//

#include "SequenceContainerTools.h"
#include "VectorSequenceContainer.h"

// From the STL:
#include <iostream>
using namespace std;

/******************************************************************************/	
	
SequenceContainer * SequenceContainerTools::createContainerOfSpecifiedSize(const Alphabet * alphabet, int size) {
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

bool SequenceContainerTools::sequencesHaveTheSameLength(const SequenceContainer & sequences) {
	vector<string> seqNames = sequences.getSequencesNames();
	if(seqNames.size() <= 1) return true;
	unsigned int length = sequences.getSequence(seqNames[0]) -> size();
	for(unsigned int i = 1; i < seqNames.size(); i++) {
		if(sequences.getSequence(seqNames[i]) -> size() != length) return false;
	}
	return true;
}

/******************************************************************************/

map<int, double> SequenceContainerTools::getFrequencies(const SequenceContainer & sequences) {
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
