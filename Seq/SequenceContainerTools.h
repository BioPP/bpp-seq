//
// File: SequenceContainerTools.h
// Created by: jdutheil <jdutheil@Deedlit>
// Created on: Sat Oct  4 09:18:34 2003
//

#ifndef _SEQUENCECONTAINERTOOLS_H_
#define _SEQUENCECONTAINERTOOLS_H_

// From the STL:
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "SequenceContainer.h"
#include "OrderedSequenceContainer.h"

typedef vector<unsigned int> SequenceSelection;

class SequenceContainerTools
{
	/**
	 * @ brief A few tools for generic SequenceContainers.
	 */

	public:
		 ~SequenceContainerTools() {}
	
	public:
		/**
		 * @brief Create a container with @f$n@f$ void sequences.
		 *
		 * A new VectorSequenceContainer with the specified alphabet is created.
		 * The destruction of this new container is up to the user.
		 * Sequences have name "0", "1",... "n-1" and no content and comments.
		 *
		 * @param alphabet The alphabet to use in the container.
		 * @param size     The number of sequences in the container.
		 * @return A pointer toward a newly created container.
		 */
		static SequenceContainer * createContainerOfSpecifiedSize(const Alphabet * alphabet, int size);

		/**
		 * @brief Create a container with specified names.
		 *
		 * A new VectorSequenceContainer with the specified alphabet is created.
		 * The destruction of this new container is up to the user.
		 * Sequences have the specified names and no content and comments.
		 *
		 * @param alphabet The alphabet to use in the container.
		 * @param seqNames The names of the sequences.
		 * @return A pointer toward a newly created container.
		 * @throw Exception If two sequence names are not unique.
		 */
		static SequenceContainer * createContainerWithSequenceNames(
			const Alphabet * alphabet,
			const vector<string> & seqNames)
			throw (Exception);
			
		/**
		 * @brief Create a new container with a specified set of sequences.
		 *
		 * A new VectorSequenceContainer is created with specified sequences.
		 * The destruction of the container is up to the user.
		 * Sequences are specified by their position, beginning at 0.
		 * No name verification is performed, based on the assumption that
		 * the container passed as an argument is a correct one.
		 * Redundant selection is not checked, so be careful with what you're doing!
		 *
		 * @param sequences The container from wich sequences are to be taken.
		 * @param selection The positions of all sequences to retrieve.
		 * @return A new container with all selected sequences.
		 */
		static SequenceContainer * getSelectedSequences(const OrderedSequenceContainer & sequences, const SequenceSelection & selection);
		
		/**
		 * @brief Check if all sequences in a SequenceContainer have the same length.
		 *
		 * @param sequences The container to check.
		 * @return True is all sequence have the same length.
		 */
		static bool sequencesHaveTheSameLength(const SequenceContainer & sequences);
	
		/**
		 * @brief Compute base frequencies.
		 *
		 * Example of usage: getting the GC content from a sequence container.
		 * <code>
		 *   map<int, double> freqs = SequenceContainerTools::getFrequencies(myContainer); //My container is previously defined.
		 *   double GCcontent = (freqs[1] + freqs[2]) / (freqs[0] + freqs[1] + freqs[2] + freqs[3]);
		 * </code>
		 *
		 * @return A map with ecah state found as a key, and corresponding frequencies as values.
		 * States are stored as their int code.
		 */
		static map<int, double> getFrequencies(const SequenceContainer & sequences);
	
};


#endif	//_SEQUENCECONTAINERTOOLS_H_
