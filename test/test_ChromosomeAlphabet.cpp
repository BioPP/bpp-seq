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

#include <Bpp/Seq/Alphabet/IntegerAlphabet.h>
#include <Bpp/Seq/Alphabet/ChromosomeAlphabet.h>
#include <Bpp/Seq/Alphabet/BinaryAlphabet.h>
#include <Bpp/Seq/Alphabet/DNA.h>
#include <Bpp/Seq/Alphabet/LexicalAlphabet.h>
#include <Bpp/Seq/Io/AbstractISequence.h>
#include <Bpp/Seq/Io/ISequence.h>
#include <Bpp/Seq/Container/VectorSequenceContainer.h>
#include <Bpp/Seq/Container/VectorSiteContainer.h>
#include <Bpp/Seq/Alphabet/NucleicAlphabet.h>
#include <Bpp/Seq/Io/chrFasta.h>
#include <iostream>
#define MAX_CHR_ALPHA 25
#define MIN_CHR_ALPHA 1

using namespace bpp;
using namespace std;

VectorSiteContainer* resizeAlphabetForSequenceContainer(VectorSequenceContainer* vsc, unsigned int maxChrAlpha, ChromosomeAlphabet* alphaInitial);
VectorSiteContainer* getCharacterData(const std :: string &path);
void testOutput(VectorSiteContainer* vsc);

/********************************************************************************************************************************/
void testOutput(VectorSiteContainer* vsc){
    size_t numOfSequences = vsc->getNumberOfSequences();
    vector <string> sequenceNames = vsc->getSequencesNames();
    for (size_t i = 0; i < numOfSequences; i++){
        BasicSequence seq = vsc->getSequence(sequenceNames[i]);
        std::string strValue = seq.getChar(0);
        int intValue = seq.getValue(0);
        std::cout <<"**************"<<endl;
        std::cout <<seq.getName() <<endl;
        std::cout << "string rep: " << strValue << endl;
        std::cout << "int rep: " << intValue << endl;
    }
    
}

/********************************************************************************************************************************/
VectorSiteContainer* getCharacterData (const string& path){
    ChromosomeAlphabet* alphaInitial = new ChromosomeAlphabet(MIN_CHR_ALPHA, MAX_CHR_ALPHA);
    VectorSequenceContainer* initialSetOfSequences = chrFasta::readSequencesFromFile(path, alphaInitial);
    size_t numOfSequences = initialSetOfSequences->getNumberOfSequences();
    vector <string> sequenceNames = initialSetOfSequences->getSequencesNames();

    unsigned int maxNumberOfChr = 1; //the minimal number of chromosomes cannot be zero
    //unsigned int minNumOfChr = MAX_CHR_ALPHA;

    for (size_t i = 0; i < numOfSequences; i++){
        BasicSequence seq = initialSetOfSequences->getSequence(sequenceNames[i]);
        int character = seq.getValue(0);
        if (character == -1){
            continue;
        }

        if (character == static_cast<int>(MAX_CHR_ALPHA)+1){
            continue;
        }
        // if it is a composite state
        if (character > static_cast<int>(MAX_CHR_ALPHA) +1){
            const std::vector<int> compositeCharacters = alphaInitial->getSetOfStatesForAComposite(character);
            for (size_t j = 0; j < compositeCharacters.size(); j++){
                if ((unsigned int) compositeCharacters[j] > maxNumberOfChr){
                    maxNumberOfChr = compositeCharacters[j];
                }
            }
            continue;
        }
        if ((unsigned int) character > maxNumberOfChr){
            maxNumberOfChr = character;
        }
        // if ((unsigned int) character < minNumOfChr){
        //     minNumOfChr = character;
        // }

    }
    VectorSiteContainer* vsc = resizeAlphabetForSequenceContainer(initialSetOfSequences, maxNumberOfChr, alphaInitial);
    delete initialSetOfSequences;
    delete alphaInitial;
    return vsc;
}
/*********************************************************************************************************************************/
VectorSiteContainer* resizeAlphabetForSequenceContainer(VectorSequenceContainer* vsc, unsigned int maxChrAlpha, ChromosomeAlphabet* alphaInitial){
    size_t numOfSequences = vsc->getNumberOfSequences();
    vector <string> sequenceNames = vsc->getSequencesNames();
    ChromosomeAlphabet* new_alphabet = new ChromosomeAlphabet(MIN_CHR_ALPHA, maxChrAlpha);
    // fill with composite values
    if (alphaInitial->getNumberOfCompositeStates() > 0){
        const std::map <int, std::map<int, double>> compositeStates = alphaInitial->getCompositeStatesMap();
        std::map <int, std::map<int, double>>::const_iterator it = compositeStates.begin();
        while (it != compositeStates.end()){
            int compositeState = it->first;
            std::string charComposite = alphaInitial->intToChar(compositeState);
            new_alphabet->setCompositeState(charComposite);
            it++;
        }
    }
    VectorSiteContainer* resized_alphabet_site_container = new VectorSiteContainer(new_alphabet);
    for (size_t i = 0; i < numOfSequences; i++){
        BasicSequence seq = vsc->getSequence(sequenceNames[i]);
        BasicSequence new_seq = BasicSequence(seq.getName(), seq.getChar(0), new_alphabet);
        resized_alphabet_site_container->addSequence(new_seq);

    }
    return resized_alphabet_site_container;
}

/*********************************************************************************************************************************/

int main() {
  string path = "/home/anats/Documents/data/examples/example.fasta";
  VectorSiteContainer* vsc = getCharacterData(path);
  testOutput(vsc);

  delete vsc;
  return 0;
}
