//
// File: MaseTools.cpp
// Created by: Julien Dutheil
// Created on: Tue Apr  1 09:16:59 2003
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

#include "MaseTools.h"

#include <iostream>

using namespace std;

#include "VectorSequenceContainer.h"
#include "AlignedSequenceContainer.h"
#include "SequenceContainerTools.h"

// From Utils:
#include <Utils/StringTokenizer.h>
#include <Utils/TextTools.h>

using namespace bpp;

SiteSelection MaseTools::getSiteSet(const Comments & maseFileHeader, const string & setName) throw (IOException)
{
	SiteSelection selection;
	for(unsigned int i = 0; i < maseFileHeader.size(); i++) {
		string current = maseFileHeader[i];
		
    string::size_type index = current.find("# of");
		if(index < current.npos) {
			StringTokenizer st(string(current.begin() + index + 4, current.end()), " \t\n\f\r=;");
			st.nextToken(); //skip next word: may be 'regions' or 'segments' or else ;-)
			unsigned int numberOfSegments = TextTools::toInt(st.nextToken());
			string name;
			while(st.hasMoreToken()) {
				name += st.nextToken();
			}
			if(name == setName) {
				//cout << numberOfSegments << " segments found." << endl;
				//Then look for the set definition:
				i++;//next line.
                unsigned int counter = 0;
				while(i < maseFileHeader.size()) {
					current = maseFileHeader[i++];
					StringTokenizer st2(current);
	 				//st.nextToken(); //Skip ';;'
					while(st2.hasMoreToken()) {
						StringTokenizer st3(st2.nextToken(), ",");
						unsigned int begin = TextTools::toInt(st3.nextToken());
						unsigned int end   = TextTools::toInt(st3.nextToken());
                        //WARNING!!! In the mase+ format, sites are numbered from 1 to nbSites,
                        //Whereas in SiteContainer the index begins at 0.
						for(unsigned int j = begin; j <= end; j++) selection.push_back(j - 1);//bounds included.
                        counter++;
						if(counter == numberOfSegments) return selection;
					}
				}
			}
		}
	}
	if(selection.size() == 0) {
		throw IOException("Site set " + setName + " has not been found in the sequence file.");
	}
	return selection;
}

/******************************************************************************/

SequenceSelection MaseTools::getSequenceSet(const Comments & maseFileHeader, const string & setName) throw (IOException)
{
	SequenceSelection selection;
	for(unsigned int i = 0; i < maseFileHeader.size(); i++) {
		string current = maseFileHeader[i];

    string::size_type index = current.find("@ of");
		if(index < current.npos) {
			StringTokenizer st(string(current.begin() + index + 4, current.end()), " \t\n\f\r=;");
			st.nextToken(); //skip next word: may be 'sequences' or else ;-)
			unsigned int numberOfSequences = TextTools::toInt(st.nextToken());
			string name;
			while(st.hasMoreToken()) {
				name += st.nextToken();
			}
            unsigned int counter = 0;
			if(name == setName) {
				//cout << numberOfSequences << " segments found." << endl;
				//Then look for the set definition:
				i++;//next line.
				while(i < maseFileHeader.size()) {
					current = maseFileHeader[i++];
					StringTokenizer st2(current, ",");
                    while(st2.hasMoreToken()) {
                        int seqIndex = TextTools::toInt(st2.nextToken());
                        //WARNING!!! In the mase+ format, sequences are numbered from 1 to nbSequences,
                        //Whereas in SequenceContainer the index begins at 0.
						selection.push_back(seqIndex - 1);//bounds included.
                        counter++;
						if(counter == numberOfSequences) return selection;
					}
				}
			}
		}
	}
	if(selection.size() == 0) {
		throw IOException("Sequence set " + setName + " has not been found in the sequence file.");
	}
	return selection;
}

/******************************************************************************/

SiteContainer * MaseTools::getSelectedSites(
	const SiteContainer & sequences,
	const string & setName) throw (IOException)
{
    SiteSelection ss = getSiteSet(sequences.getGeneralComments(), setName);
    return SiteContainerTools::getSelectedSites(sequences, ss);
}

/******************************************************************************/

SequenceContainer * MaseTools::getSelectedSequences(
    const OrderedSequenceContainer & sequences,
    const string & setName) throw (IOException)
{
    SequenceSelection ss = getSequenceSet(sequences.getGeneralComments(), setName);
    return SequenceContainerTools::getSelectedSequences(sequences, ss);
}

/******************************************************************************/

map<string, unsigned int> MaseTools::getAvailableSiteSelections(const Comments & maseHeader)
{
  map<string, unsigned int> selections;
  for(unsigned int i = 0; i < maseHeader.size(); i++) {
		string current = maseHeader[i];

    string::size_type index = current.find("# of");
		if(index < current.npos) {
			StringTokenizer st(string(current.begin() + index + 4, current.end()), " \t\n\f\r=;");
			st.nextToken(); //skip next word: may be 'sequences' or else ;-)
			unsigned int numberOfSegments = TextTools::toInt(st.nextToken());
			string name = st.nextToken();
			while(st.hasMoreToken()) {
				name += " " +st.nextToken();
			}
			unsigned int counter = 0;
			unsigned nbSites = 0;
			while(i < maseHeader.size()) {
				i++;
				current = maseHeader[i];
				StringTokenizer st2(current);
				//st.nextToken(); //Skip ';;'
				while(st2.hasMoreToken()) {
					StringTokenizer st3(st2.nextToken(), ",");
					unsigned int begin = TextTools::toInt(st3.nextToken());
					unsigned int end   = TextTools::toInt(st3.nextToken());
					counter++;
					nbSites += end - begin + 1;
				}
				if(counter == numberOfSegments) {
					selections[name] = nbSites;
					break;
				}
			}
    }
	}
  return selections;
}

/******************************************************************************/

map<string, unsigned int> MaseTools::getAvailableSequenceSelections(const Comments & maseHeader)
{
	map<string, unsigned int> selections;
	for(unsigned int i = 0; i < maseHeader.size(); i++) {
		string current = maseHeader[i];

    string::size_type index = current.find("@ of");
		if(index < current.npos) {
      StringTokenizer st(string(current.begin() + index + 4, current.end()), " \t\n\f\r=;");
      st.nextToken(); //skip next word: may be 'sequences' or else ;-)
      unsigned int numberOfSequences = TextTools::fromString<unsigned int>(st.nextToken());
      string name = st.nextToken();
      while(st.hasMoreToken()) {
        name += st.nextToken();
      }
      selections[name] = numberOfSequences;
    }
  }
  return selections;
}

/******************************************************************************/

unsigned int MaseTools::getPhase(const Comments & maseFileHeader, const string &setName) throw (Exception)
{
	unsigned int phase = 0;
  string::size_type index = 0;
	for(unsigned int i = 0; i < maseFileHeader.size(); i++) {
		string current = maseFileHeader[i];

		index = current.find("# of");
		if(index < current.npos) {
			StringTokenizer st(string(current.begin() + index + 12 , current.end()), " \t\n\f\r=;");
			//unsigned int numberOfSegments = TextTools::toInt(st.nextToken());
			//cout << "Number of regions: " << st.nextToken() << endl;
			string name;
			while(st.hasMoreToken()) {
				name = st.nextToken();
				//cout << "Name of regions: " << name << endl;
			}
			if(name == setName) {
				return phase;
			}
		}

		index = current.find("/codon_start");
		if(index < current.npos) {
			StringTokenizer st(string(current.begin() + index + 12, current.end()), " \t\n\f\r=;");
			phase = TextTools::toInt(st.nextToken());
		}
	}
	throw Exception("PolymorphismSequenceContainer::getPhase: no /codon_start found, or site selection missing.");
}

/******************************************************************************/

