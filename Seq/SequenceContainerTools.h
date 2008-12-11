//
// File: SequenceContainerTools.h
// Created by: Julien Dutheil
//             Sylvain Gaillard
// Created on: Sat Oct  4 09:18:34 2003
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

#ifndef _SEQUENCECONTAINERTOOLS_H_
#define _SEQUENCECONTAINERTOOLS_H_

// From the STL:
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "SequenceContainer.h"
#include "OrderedSequenceContainer.h"

namespace bpp
{

typedef vector<unsigned int> SequenceSelection;

/**
 * @brief Utilitary methods dealing with sequence containers.
 */
class SequenceContainerTools
{

  public:
    SequenceContainerTools() {}
     virtual ~SequenceContainerTools() {}
  
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
     * @brief Remove all sequences that are not in a given selection from a given container.
     *
     * A new VectorSequenceContainer is created with specified sequences.
     * The destruction of the container is up to the user.
     * Sequences are specified by their position, beginning at 0.
     * Redundant selection is not checked, so be careful with what you're doing!
     *
     * @param sequences The container from wich sequences are to be taken.
     * @param selection The positions of all sequences to retrieve.
     * @return A new container with all selected sequences.
     */
    static void keepOnlySelectedSequences(OrderedSequenceContainer & sequences, const SequenceSelection & selection);
    
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

    /**
     * @brief Append all the sequences of a SequenceContainer to the end of another.
     *
     * This function is a template because of the non existance of the
     * addSequence() methode in the SequenceContainer interface (see the doc
     * of SequenceContainer for more details).
     * The type of the template must be the type of the SequenceContainer which
     * receives the sequences. This SequenceContainer <b>must have</b> an
     * addSequence() methode like:
     * <code>
     * void addSequence(const Sequence &sequence, bool checkNames=true);
     * </code>
     * @param seqCont1 The SequenceContainer in which the sequences will be added.
     * @param seqCont2 The SequenceContainer from which the sequences are taken.
     */
    template<class T>static void append(T & seqCont1, const SequenceContainer & seqCont2)
    throw (Exception)
    {
      try
      {
        vector<string> seqNames = seqCont2.getSequencesNames();
        for(unsigned int i = 0; i < seqNames.size(); i++)
          seqCont1.addSequence(* seqCont2.getSequence(seqNames[i]));
      }
      catch (Exception e)
      {
        throw e;
      }
    }
    
    /**
     * @brief Concatenate the sequences from two containers.
     *
     * This function is a template because of the non existence of the
     * addSequence() method in the SequenceContainer interface (see the doc
     * of SequenceContainer for more details).
     * The type of the template must be the type of the SequenceContainer which
     * receives the sequences. This SequenceContainer <b>must have</b> an
     * addSequence() method like:<br>
     * @code
     * void addSequence(const Sequence &sequence, bool checkNames=true);
     * @endcode
     * @param seqCont1 First container.
     * @param seqCont2 Second container. This container must contain sequences with the same names as in seqcont1.
     * Additional sequences will be ignored.
     * @param seqCont Output sequence container to which concatenated sequences will be added.
     * @throw AlphabetMismatchException If the alphabet in the 3 containers do not match.
     */
    template<class T>static void merge(const SequenceContainer& seqCont1, const SequenceContainer& seqCont2, T& seqCont)
    throw (Exception)
    {
      if(seqCont1.getAlphabet()->getAlphabetType() != seqCont2.getAlphabet()->getAlphabetType())
        throw AlphabetMismatchException("SequenceContainerTools::merge.", seqCont1.getAlphabet(), seqCont2.getAlphabet());

      try
      {
        vector<string> seqNames = seqCont1.getSequencesNames();
        for(unsigned int i = 0; i < seqNames.size(); i++)
        {
          Sequence tmp = *seqCont1.getSequence(seqNames[i]);
          tmp.append(seqCont2.getSequence(seqNames[i])->getContent());
          seqCont.addSequence(tmp);
        }
      }
      catch (Exception e)
      {
        throw e;
      }
    }

    /**
     * @brief Convert a SequenceContainer with a new alphabet.
     *
     * This function is a template because of the non existance of the
     * addSequence() method in the SequenceContainer interface (see the doc
     * of SequenceContainer for more details).
     * The type of the template must be the type of the SequenceContainer which
     * receives the sequences. This SequenceContainer <b>must have</b> an
     * addSequence() method like:<br>
     * <code>
     * void addSequence(const Sequence &sequence, bool checkNames=true);
     * </code>
     * It can be used with a VectorSequenceContainer or a VectorSiteContainer
     * but not with a MapSequenceContainer.
     * @param seqCont The container to convert.
     * @param alphabet The alphabet into the container will be converted.
     * @return A new container.
     */
    template<class T> static T* convertAlphabet(const T & seqCont, const Alphabet *alphabet)
    throw (Exception)
    {  
      try {
        T *seqContNew = new T(alphabet);      
        vector<string> seqNames = seqCont.getSequencesNames();
        for(unsigned int i = 0; i < seqNames.size(); i++)
        {
          Sequence seq = Sequence(seqNames[i], seqCont.toString(seqNames[i]), alphabet);
          seqContNew->addSequence(seq);
        }
        return seqContNew;
      }
      catch (Exception e)
      {
        throw e;
      }
    }

};

} //end of namespace bpp.

#endif  //_SEQUENCECONTAINERTOOLS_H_

