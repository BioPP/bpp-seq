//
// File: VectorSiteContainer.h
// Created by: jdutheil <jdutheil@Deedlit>
// Created on: Mon Oct  6 11:50:40 2003
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
 

#ifndef _VECTORSITECONTAINER_H_
#define _VECTORSITECONTAINER_H_

#include "Site.h"
#include "SiteContainer.h"
#include "AbstractSequenceContainer.h"
#include "AlignedSequenceContainer.h"
#include "OrderedSequenceContainer.h"

// From the NumCalc library:
#include <NumCalc/VectorTools.h>

// From the STL library:
#include <string>
#include <vector>
#include <iostream>

/**
 * This class does the same as AlignedSequenceContainer in the SeqLib package.
 * However, sequences are stored as a vector of sites.
 * This means that sequences are obviously aligned.
 * Methods dealing with sites are in o(N), whereas methods dealing with sequences
 * are in o(N²). This is the contrary of the AlignedSequenceContainer class.
 * Since phylogenetic methods deal wit sites, consider using this class instead
 * of the former one.
 */

class VectorSiteContainer : 
	public AbstractSequenceContainer,   //This container implements the SequenceContainer interface
				                  						//and use the AbstractSequenceContainer adapter.
	public SiteContainer				//This container is a SiteContainer.
{
	
	protected:
		vector<Site     *> _sites;
		vector<string   *> _names;
		vector<Comments *> _comments; //Sequences comments.
		mutable vector<Sequence *> _sequences; //To store pointer toward sequences retrieves (cf. AlignedSequenceContainer).
	
	public:
		VectorSiteContainer(const vector<const Site *> & vs, const Alphabet * alpha) throw (Exception);
		VectorSiteContainer(unsigned int size, const Alphabet * alpha);
		VectorSiteContainer(const Alphabet * alpha);

		VectorSiteContainer(const      VectorSiteContainer & vsc);
		VectorSiteContainer(const            SiteContainer &  sc);
		VectorSiteContainer(const OrderedSequenceContainer & osc);

		VectorSiteContainer& operator = (const      VectorSiteContainer & vsc);
		VectorSiteContainer& operator = (const            SiteContainer &  sc);
		VectorSiteContainer& operator = (const OrderedSequenceContainer & osc);

		~VectorSiteContainer();

	public:

		/**
		 * @name The Clonable interface.
		 *
		 * @{
		 */
		Clonable * clone() const;
		/** @} */

		/**
		 * @name The SiteContainer interface implementation:
		 *
		 * @{
		 */
		const Site * getSite(unsigned int siteIndex) const throw (IndexOutOfBoundsException);
		void         setSite(unsigned int siteIndex, const Site & site, bool checkPosition = true) throw (Exception);
		Site *    removeSite(unsigned int siteIndex) throw (IndexOutOfBoundsException);
		void      deleteSite(unsigned int siteIndex) throw (IndexOutOfBoundsException);
		void         addSite(const Site & site,                         bool checkPosition = true) throw (Exception);
		void         addSite(const Site & site, unsigned int siteIndex, bool checkPosition = true) throw (Exception);
		unsigned int getNumberOfSites() const;
		const vector<int> getPositions() const;
		void reindexSites();
		Vint getSitePositions() const;
		/** @} */

		/**
		 * @name The SequenceContainer interface implementation:
		 *
		 * Redefined AbstractSequenceContainer methods (to stop ambiguity of multi-inheritance)
		 * Only add- and setSequence methods are overloaded.
		 *
		 * @{
		 */

		const Alphabet * getAlphabet() const {
			return AbstractSequenceContainer::getAlphabet();
		}

		string getName(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return AbstractSequenceContainer::getName(sequenceIndex);
		}

		vector<int> getContent(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return AbstractSequenceContainer::getContent(sequenceIndex);
		}

		vector<int> getContent(const string & name) const throw (SequenceNotFoundException) {
			return AbstractSequenceContainer::getContent(name);
		}

		string toString(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return AbstractSequenceContainer::toString(sequenceIndex);
		}

		string toString(const string & name) const throw (SequenceNotFoundException) {
			return AbstractSequenceContainer::toString(name);
		}

		Comments getComments(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return AbstractSequenceContainer::getComments(sequenceIndex);
		}

		Comments getComments(const string & name) const throw (SequenceNotFoundException) {
			return AbstractSequenceContainer::getComments(name);
		}

		void setComments(const string & name, const Comments & comments) throw (SequenceNotFoundException) {
			AbstractSequenceContainer::setComments(name, comments);
		}

		Comments getGeneralComments() const {
			return AbstractSequenceContainer::getGeneralComments();
		}

		void setGeneralComments(const Comments & comments) {
			AbstractSequenceContainer::setGeneralComments(comments);
		}

		void deleteGeneralComments() {
			AbstractSequenceContainer::deleteGeneralComments();
		}

		//Theses methods are implemented for this class:
		
		void setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException);

		// Method to get a sequence object from sequence container
		const Sequence * getSequence(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		const Sequence * getSequence(const string & name       ) const throw (SequenceNotFoundException);

		// Methods to get position of a sequence in sequence container from his name
		// This method is used by delete and remove methods
		unsigned int getSequencePosition(const string & name) const throw (SequenceNotFoundException);

		void setSequence(const string & name,        const Sequence & sequence, bool checkName) throw (Exception);
		void setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName) throw (Exception);

		Sequence * removeSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		Sequence * removeSequence(const string & name       ) throw (SequenceNotFoundException);

		void deleteSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		void deleteSequence(const string & name       ) throw (SequenceNotFoundException);

		void addSequence(const Sequence & sequence,                             bool checkName = true) throw (Exception);
		void addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName = true) throw (Exception);

		unsigned int getNumberOfSequences() const;

		vector<string> getSequencesNames() const;

		void setSequencesNames(const vector<string> & names, bool checkNames = true) throw (Exception);

		void clear();

	protected:
		// Create n void sites:
		void realloc(unsigned int n);
};


#endif	//_VECTORSITECONTAINER_H_
