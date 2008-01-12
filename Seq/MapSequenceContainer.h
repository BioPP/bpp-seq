//
// File MapSequenceContainer.h
// Authors : Guillaume Deuchst
//           Julien Dutheil
//           Sylvain Gaillard
// Last modification : Friday June 25 2004
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

#ifndef _MAPSEQUENCECONTAINER_H_
#define _MAPSEQUENCECONTAINER_H_

#include "Sequence.h"
#include "AbstractSequenceContainer.h"
#include "Alphabet.h"

#include <string>
#include <map>

using namespace std;

namespace bpp
{

/**
 * @brief MapSequenceContainer class
 *
 * Sequences are stored using a key string, in a map object.
 * Sequences are ordered according to the key order (defined by the < operator).
 * 
 */

class MapSequenceContainer:
  public AbstractSequenceContainer
{
	protected:
		map<string, Sequence *> _sequences;
        
	public:
		MapSequenceContainer(const map<string, Sequence *> & ms, const Alphabet * alpha);
		MapSequenceContainer(const Alphabet * alpha);

		MapSequenceContainer(const MapSequenceContainer & msc);
		MapSequenceContainer & operator = (const MapSequenceContainer & msc);

		virtual ~MapSequenceContainer();

	public:
		/**
		 * @brief Get a sequence.
		 *
		 * @param key The key of the sequence to retrieve.
		 * @return The sequence associated to the given key.
		 * @throw SequenceNotFoundException If no sequence is associated to the given key.
		 */
		const Sequence * getSequenceByKey(const string & key)  const throw (SequenceNotFoundException);

		/**
		 * @brief Set a sequence.
		 *
		 * @param key The key of the sequence.
		 * @param sequence The new sequence that will be associated to the key.
		 * @param checkNames Tell is the sequence name must be checked.
		 * @throw SequenceNotFoundException If no sequence is associated to the given key.
		 */
		void setSequenceByKey(const string & key , const Sequence & sequence, bool checkNames = true) throw (SequenceNotFoundException);

		/**
		 * @brief Remove a sequence.
		 * 
		 * @param key The key of the sequence.
		 * @return The sequence previously associated to the given key.
		 * @throw SequenceNotFoundException If no sequence is associated to the given key.
		 */
		Sequence * removeSequenceByKey(const string & key) throw (SequenceNotFoundException);

		/**
		 * @brief Delete a sequence.
		 * 
		 * @param key The key of the sequence.
		 * @throw SequenceNotFoundException If no sequence is associated to the given key.
		 */
		void deleteSequenceByKey(const string & key) throw (SequenceNotFoundException);

		/**
		 * @brief Add a sequence and key.
		 * 
		 * @param key The key of the new sequence.
		 * @param sequence The new sequence that will be associated to the key.
		 * @param checkNames Tell is the sequence name must be checked.
		 */
		void addSequence(const string & key, const Sequence & sequence, bool checkNames = true) throw (Exception);

		/**
		 * @return All sequences keys.
		 */
		vector<string> getKeys() const;

		/**
		 * @return The key of a given sequence specified by its position in the container.
		 * @param pos The index of the sequence.
		 * @throw IndexOutOfBoundsException If pos is not a valid index.
		 */
		string getKey(unsigned int pos) const throw (IndexOutOfBoundsException);

		/**
		 * @return The key of a given sequence specified by its name.
		 * @param name The name of the sequence.
		 * @throw SequenceNotFoundException If no sequence was found with the given name.
		 */
		string getKey(const string & name) const throw (SequenceNotFoundException);

		/**
		 * @name The clonable interface
		 * 
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
		SequenceContainer * createEmptyContainer() const;

    int & valueAt(const string & sequenceName, unsigned int elementIndex) throw (SequenceNotFoundException, IndexOutOfBoundsException)
    {
      return (* getSequenceInner(sequenceName))[elementIndex];
    }
    const int & valueAt(const string & sequenceName, unsigned int elementIndex) const throw (SequenceNotFoundException, IndexOutOfBoundsException)
    {
      return (* getSequence(sequenceName))[elementIndex];
    }
    int & operator()(const string & sequenceName, unsigned int elementIndex)
    {
      return (* getSequenceInner(sequenceName))[elementIndex];
    }
    const int & operator()(const string & sequenceName, unsigned int elementIndex) const
    {
      return (* getSequence(sequenceName))[elementIndex];
    }

    int & valueAt(unsigned int sequenceIndex, unsigned int elementIndex) throw (IndexOutOfBoundsException)
    {
      return (* getSequenceInner(sequenceIndex))[elementIndex];
    }
    const int & valueAt(unsigned int sequenceIndex, unsigned int elementIndex) const throw (IndexOutOfBoundsException)
    {
      return (* getSequence(sequenceIndex))[elementIndex];
    }    
    int & operator()(unsigned int sequenceIndex, unsigned int elementIndex)
    {
      return (* getSequenceInner(sequenceIndex))[elementIndex];
    }
    const int & operator()(unsigned int sequenceIndex, unsigned int elementIndex) const
    {
      return (* getSequence(sequenceIndex))[elementIndex];
    }    
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
		Sequence * getSequenceInner(unsigned int i)      throw (IndexOutOfBoundsException);
		Sequence * getSequenceInner(const string & name) throw (SequenceNotFoundException);
		/** @} */
};

} //end of namespace bpp.

#endif // _MAPSEQUENCECONTAINER_H_

