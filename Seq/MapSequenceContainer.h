/*
 * File MapSequenceContainer.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Wednesday July 30 2003
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

/* MapSequenceContainer class
 * Host sequence container's alphabet
 * Define specific methods to map sequences containers manipulation
 *
 * Note : although a sequence can be deleted directly from sequence container,
 * using specific methods (removeSequence and deleteSequence) to do it is IMPERATIVE (because they do
 * verifications, and they properly delete pointer toward sequence)
 *
 * This class can be instanciated (i.e. SequenceContainer vsc * = new MapSequenceContainer(seqMap, alphabet);)
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
         * @name The SequenceContainer interface implementation:
         *
         * @{
         */
		const Sequence * getSequence(const string & name) const throw (SequenceNotFoundException);
		void setSequence(const string & name, const Sequence & sequence, bool checkNames = true) throw (SequenceNotFoundException);
		Sequence * removeSequence(const string & name) throw (SequenceNotFoundException);
		void deleteSequence(const string & name) throw (SequenceNotFoundException);
		unsigned int getNumberOfSequences() const;
		void clear();
        /** @} */

        /**
         * @name The OrderedSequenceContainer interface implementation:
         *
         * @{
         */
		const Sequence * getSequence(unsigned int i) const throw (IndexOutOfBoundsException);
		unsigned int getSequencePosition(const string & name) const throw (SequenceNotFoundException);
		void setSequence(unsigned int i, const Sequence & sequence, bool checkNames = true) throw (IndexOutOfBoundsException);
		Sequence * removeSequence(unsigned int i) throw (IndexOutOfBoundsException);
		void deleteSequence(unsigned int i) throw (IndexOutOfBoundsException);
        /** @} */

        /**
         * @name The MapSequenceContainer specific methods:
         *
         * @{
         */
		const Sequence * getSequenceByKey(const string & key)  const throw (SequenceNotFoundException);
		void setSequenceByKey(const string & key , const Sequence & sequence, bool checkNames = true) throw (SequenceNotFoundException);
		Sequence * removeSequenceByKey(const string & key);
		void deleteSequenceByKey(const string & key);
		void addSequence(const string & key, const Sequence & sequence, bool checkNames = true) throw (Exception);
		vector<string> getKeys() const;
        string getKey(unsigned int i) const throw (IndexOutOfBoundsException);
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
