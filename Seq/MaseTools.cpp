//
// File: MaseTools.h
// Created by: Julien Dutheil <julien.dutheil@ens-lyon.fr>
// Created on: Tue Apr  1 09:16:59 2003
//

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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

SiteSelection MaseTools::getSiteSet(const Comments & maseFileHeader, const string & setName) throw (IOException)
{
	SiteSelection selection;
	for(unsigned int i = 0; i < maseFileHeader.size(); i++) {
		string current = maseFileHeader[i];
		
		unsigned int index = current.find("# of");
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

		unsigned int index = current.find("@ of");
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

		unsigned int index = current.find("# of");
		if(index < current.npos) {
            StringTokenizer st(string(current.begin() + index + 4, current.end()), " \t\n\f\r=;");
            st.nextToken(); //skip next word: may be 'sequences' or else ;-)
			unsigned int numberOfSegments = TextTools::toInt(st.nextToken());
			string name;
			while(st.hasMoreToken()) {
				name += st.nextToken();
			}
			i++;//next line.
            unsigned int counter = 0;
            unsigned nbSites = 0;
			while(i < maseHeader.size()) {
				current = maseHeader[i++];
				StringTokenizer st2(current);
	 			//st.nextToken(); //Skip ';;'
				while(st2.hasMoreToken()) {
					StringTokenizer st3(st2.nextToken(), ",");
					unsigned int begin = TextTools::toInt(st3.nextToken());
					unsigned int end   = TextTools::toInt(st3.nextToken());
                    counter++;
                    nbSites += end - begin + 1;
					if(counter == numberOfSegments) break;
				}
                selections[name] = nbSites;
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

		unsigned int index = current.find("@ of");
		if(index < current.npos) {
            StringTokenizer st(string(current.begin() + index + 4, current.end()), " \t\n\f\r=;");
            st.nextToken(); //skip next word: may be 'sequences' or else ;-)
            unsigned int numberOfSequences = TextTools::fromString<unsigned int>(st.nextToken());
            string name;
            while(st.hasMoreToken()) {
                name += st.nextToken();
            }
            selections[name] = numberOfSequences;
        }
    }
    return selections;
}

/******************************************************************************/
