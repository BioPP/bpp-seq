/*
 * File AlignedSequenceContainer.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Friday August 22 2003
*/

#ifndef _ALIGNEDSEQUENCECONTAINER_H_
#define _ALIGNEDSEQUENCECONTAINER_H_

#include "Site.h"
#include "SiteContainer.h"
#include "OrderedSequenceContainer.h"
#include "VectorSequenceContainer.h"

// From Utils:
#include <Utils/Exceptions.h>

/* AlignedSequenceContainer class
 * Define specific methods to sites containers manipulation
 * 
 * This class can be instanciated (i.e. AlignedSequenceContainer asc * = new AlignedSequenceContainer(vsc);)
*/

class AlignedSequenceContainer : public VectorSequenceContainer, public SiteContainer
{
	protected:
		// Integer vector that contains sites's positions
		vector<int> positions;
		
		unsigned int length; // Number of sites for verifications before sequence's insertion in sequence container
	
		/* This is used in order to implement the SiteContainer interface.
		 * A SiteContainer is expected to work on Site objects, but this class
		 * -- since it is a VectorSequenceContainer -- has its data sored as
		 * Sequence object. When the SiteContainer method getSite() is invoked
		 * it creates a new Site object and send the adress of it.
		 * To avoid memory leaks, this object is put into a vector so that it can be
		 * destroyed when the container is destroyed.
		 */
		mutable vector<Site *> sites;
		
	public:
		// Class constructors
		AlignedSequenceContainer(const Alphabet * alpha);
		AlignedSequenceContainer(const AlignedSequenceContainer & asc);
		AlignedSequenceContainer(const            SiteContainer &  sc);
		AlignedSequenceContainer(const OrderedSequenceContainer & osc);

		AlignedSequenceContainer & operator = (const AlignedSequenceContainer & asc);
		AlignedSequenceContainer & operator = (const            SiteContainer &  sc);
		AlignedSequenceContainer & operator = (const OrderedSequenceContainer & osc) throw (SequenceNotAlignedException);

		// Class destructor
		~AlignedSequenceContainer();

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
  	const Site * getSite(unsigned int siteIndex) const throw (IndexOutOfBoundsException) ;
		void         setSite(unsigned int siteIndex, const Site & site, bool checkPosition = true) throw (Exception) ;
		Site *    removeSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) ;
		void      deleteSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) ;
		void addSite(const Site & site, bool checkPosition = true) throw (Exception);
		void addSite(const Site & site, unsigned int siteIndex, bool checkPosition = true) throw (Exception);
		unsigned int getNumberOfSites() const;
		Vint getSitePositions() const;
		void reindexSites();
		/** @} */

	// Redefined VectorSequenceContainer methods (to stop ambiguity of multi-inheritance)
	// Only add- and setSequence methods are overloaded.
	public:
		// Method to get sequence container's alphabet
		const Alphabet * getAlphabet() const {
			return AbstractSequenceContainer::getAlphabet();
		}

		// Method to get name of a sequence in sequence container
		string getName(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return VectorSequenceContainer::getName(sequenceIndex);
		}

		// Method to get content of a sequence in sequence container
		vector<int> getContent(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return VectorSequenceContainer::getContent(sequenceIndex);
		}
		vector<int> getContent(const string & name) const throw (SequenceNotFoundException) {
			return VectorSequenceContainer::getContent(name);
		}

		// Convert to string
		string toString(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return VectorSequenceContainer::toString(sequenceIndex);
		}
		string toString(const string & name) const throw (SequenceNotFoundException) {
			return VectorSequenceContainer::toString(name);
		}

		// Method to get commentaries of a sequence in sequence container
		Comments getComments(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return VectorSequenceContainer::getComments(sequenceIndex);
		}
		Comments getComments(const string & name) const throw (SequenceNotFoundException) {
			return VectorSequenceContainer::getComments(name);
		}

		// Method to replace commentaries of a sequence in sequence container
		void setComments(const string & name, const Comments & comments) throw (SequenceNotFoundException) {
			VectorSequenceContainer::setComments(name, comments);
		}
		

		void setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException) {
			VectorSequenceContainer::setComments(sequenceIndex, comments);
		}

	// Method to get an sequence object from sequence container
		const Sequence * getSequence(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException) {
			return VectorSequenceContainer::getSequence(sequenceIndex);
		}
		const Sequence * getSequence(const string & name) const throw (SequenceNotFoundException) {
			return VectorSequenceContainer::getSequence(name);
		}

		// Methods to get position of a sequence in sequence container from his name
		// This method is used by delete and remove methods
		unsigned int getSequencePosition(const string & name) const throw (SequenceNotFoundException) {
			return VectorSequenceContainer::getSequencePosition(name);
		}

		// Method to replace a sequence in sequence container
		void setSequence(const string & name, const Sequence & sequence, bool checkName = true) throw (Exception);
		void setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName = true) throw (Exception);

		// Methods to extract (and remove) a sequence from sequence container
		Sequence * removeSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException) {
			return VectorSequenceContainer::removeSequence(sequenceIndex);
		}
		Sequence * removeSequence(const string & name) throw (SequenceNotFoundException) {
			return VectorSequenceContainer::removeSequence(name);
		}

		// Methods to delete a sequence from sequence container
		void deleteSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException) {
			VectorSequenceContainer::deleteSequence(sequenceIndex);
		}
		void deleteSequence(const string & name) throw (SequenceNotFoundException) {
			VectorSequenceContainer::deleteSequence(name);
		}

		// Methods to add sequence in sequence container
		// CheckName : boolean for enable or disable sequence's name existence checking
		void addSequence(const Sequence & sequence, bool checkName = true) throw (Exception);
		void addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName = true) throw (Exception);

		// Method to get number of sequences contained in sequence container
		unsigned int getNumberOfSequences() const {
			return VectorSequenceContainer::getNumberOfSequences();
		}

		// Method to get all names of sequences in sequence container
		vector<string> getSequencesNames() const {
			return VectorSequenceContainer::getSequencesNames();
		}

		// Method to replace all names of sequences in sequence container
		// CheckName : boolean for enable or disable sequence's name existence checking
		void setSequencesNames(const vector<string> & names, bool checkNames = true) throw (Exception) {
			VectorSequenceContainer::setSequencesNames(names, checkNames);
		}

		// Methods to get, replace and delete sequence container's commentaries
		Comments getGeneralComments() const {
			return VectorSequenceContainer::getGeneralComments();
		}
		void setGeneralComments(const Comments & comments) {
			VectorSequenceContainer::setGeneralComments(comments);
		}
		void deleteGeneralComments() {
			VectorSequenceContainer::deleteGeneralComments();
		}

		// Method to delete all sequences in sequence container
		void clear();
		
		SequenceContainer * getEmptyContainer() const;
	
	protected:
		// Protected method to verify sequence's size before insertion in sequence container
		bool checkSize(const Sequence & sequence);
};

#endif // _ALIGNEDSEQUENCECONTAINER_H_
