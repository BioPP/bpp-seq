/*
 * File MapSequenceContainer.h
 * Authors : Guillaume Deuchst <GDeuchst@ifrance.com>
 *           Sylvain Gaillard <yragael2001@yahoo.fr>
 * Last modification : Friday June 25 2004
 */

// Secured inclusion of header's file
#ifndef _MAPSEQUENCECONTAINER_H_
#define _MAPSEQUENCECONTAINER_H_

#include "Sequence.h"
#include "AbstractSequenceContainer.h"
#include "Alphabet.h"

#include <string>
#include <map>

using namespace std;

/**
 * @brief MapSequenceContainer class
 * 
 * Host sequence container's alphabet
 * Define specific methods to map sequences containers manipulation
 *
 * Note : although a sequence can be deleted directly from sequence container,
 * using specific methods (removeSequence and deleteSequence) to do it is IMPERATIVE (because they do
 * verifications, and they properly delete pointer toward sequence)
 *
 * This class can be instanciated (i.e. SequenceContainer sc * = new MapSequenceContainer(seqMap, alphabet);)
 */

class MapSequenceContainer: public AbstractSequenceContainer
{
	protected:
		map<string, Sequence *> _sequences;
        
	public:
		// Class constructors
		MapSequenceContainer(const map<string, Sequence *> & ms, const Alphabet * alpha);
		MapSequenceContainer(const Alphabet * alpha);

		// Copy constructors
		MapSequenceContainer(const MapSequenceContainer & msc);
		MapSequenceContainer & operator = (const MapSequenceContainer & msc);

		// Class destructor
		~MapSequenceContainer();

	public:
		/**
		 * @brief Get a sequence.
		 */
		const Sequence * getSequenceByKey(const string & key)  const throw (SequenceNotFoundException);

		/**
		 * @brief Set a sequence.
		 */
		void setSequenceByKey(const string & key , const Sequence & sequence, bool checkNames = true) throw (SequenceNotFoundException);

		/**
		 * @brief Remove a sequence.
		 */
		Sequence * removeSequenceByKey(const string & key);

		/**
		 * @brief Delete a sequence.
		 */
		void deleteSequenceByKey(const string & key);

		/**
		 * @brief Add a sequence.
		 */
		void addSequence(const string & key, const Sequence & sequence, bool checkNames = true) throw (Exception);

		/**
		 * @brief Get the sequences keys.
		 */
		vector<string> getKeys() const;

		/**
		 * @brief Get a key at a position.
		 */
		string getKey(unsigned int pos) const throw (IndexOutOfBoundsException);

		/**
		 * @brief Get a sequence key.
		 */
		string getKey(const string & name) const throw (SequenceNotFoundException);

		/**
		 * @name The clonable interface
		 * @{
		 */
		Clonable * clone() const;
		/**
		 * @}
		 */

		/**
		 * @name The SequenceContainer interface implementation:
		 *
		 * @{
		 */
		const Sequence * getSequence(const string & name) const throw (SequenceNotFoundException);
		void setSequence(const string & name, const Sequence & sequence, bool checkName = true) throw (SequenceNotFoundException);
		Sequence * removeSequence(const string & name) throw (SequenceNotFoundException);
		void deleteSequence(const string & name) throw (SequenceNotFoundException);
		unsigned int getNumberOfSequences() const;
		void clear();
		SequenceContainer * getEmptyContainer() const;
		/** @} */

		/**
		 * @name The OrderedSequenceContainer interface implementation:
		 *
		 * @{
		 */
		const Sequence * getSequence(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		unsigned int     getSequencePosition(const string & name) const throw (SequenceNotFoundException);
		void             setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName = true) throw (IndexOutOfBoundsException);
		Sequence *    removeSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		void          deleteSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		void setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException);
		vector<string> getSequencesNames() const;
		void setSequencesNames(const vector<string> & names, bool checkNames) throw (Exception);
		/** @} */

		/**
		 * @name AbstractSequenceContainer methods.
		 *
		 * @{
		 */
		Sequence * getSequence(unsigned int i)      throw (IndexOutOfBoundsException);
		Sequence * getSequence(const string & name) throw (SequenceNotFoundException);
		/** @} */
};

#endif // _MAPSEQUENCECONTAINER_H_
