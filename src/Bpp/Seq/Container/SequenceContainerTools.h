//
// File: SequenceContainerTools.h
// Authors:
//   Julien Dutheil
//   Sylvain Gaillard
// Created: 2003-10-04 09:18:34
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_CONTAINER_SEQUENCECONTAINERTOOLS_H
#define BPP_SEQ_CONTAINER_SEQUENCECONTAINERTOOLS_H



// From the STL:
#include <string>
#include <vector>
#include <map>
#include <memory>

#include <Bpp/Numeric/VectorTools.h>
#include "../SymbolListTools.h"
#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"
#include "../Alphabet/CodonAlphabet.h"

namespace bpp
{
using SequenceSelection = std::vector<size_t>;
using SiteSelection = std::vector<size_t>;

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
   * @name Work with sequence names
   *
   * Note that in case the container is indexed by sequence names, methods working directly on sequence keys will be more efficient!
   *
   * @{
   */

  /**
   * @brief Tells whether a sequence with the given name is present in the container.
   *
   * @param sc The sequence container to check.
   * @param name The query sequence name.
   */
  template<class SequenceType, class HashType>
  static bool hasSequenceWithName(const TemplateSequenceContainerInterface<SequenceType, HashType>& sc, const std::string& name)
  {
    size_t nbSeq = sc.getNumberOfSequences();
    for (size_t i = 0; i < nbSeq; ++i) {
      if (sc.sequence(i).getName() == name) {
        return true;
      }
    }
    return false;
  }

  /** @} */
  
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
  template<class SequenceType, class HashType>
  static std::unique_ptr< TemplateSequenceContainerInterface<SequenceType, HashType> > createContainerOfSpecifiedSize(std::shared_ptr<const Alphabet>& alphabet, size_t size)
  {
    auto vsc = std::make_unique< TemplateVectorSequenceContainer<SequenceType> >(alphabet);
    for (size_t i = 0; i < size; ++i)
    {
      vsc->addSequence(SequenceType(TextTools::toString(i), "", alphabet), false);
    }
    return vsc;
  }



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
  template<class SequenceType, class HashType>
  static std::unique_ptr< TemplateSequenceContainerInterface<SequenceType, HashType> > createContainerWithSequenceNames(
    std::shared_ptr<const Alphabet>& alphabet,
    const std::vector<std::string>& seqNames)
  {
    auto sc = createContainerOfSpecifiedSize<SequenceType, HashType>(alphabet, seqNames.size());
    sc->setSequenceNames(seqNames, true);
    return sc;
  }


  /**
   * @brief Generic function which creates a new container from another one,
   * by specifying the class of sequence to be stored.
   *
   * Compared to several copy constructors, this function allows to change the class of
   * the inner sequence class used for storing sequences.
   * The function used the addSequence method, so that it can also be used to
   * concatenate containers.
   *
   * @param input The container to copy.
   * @param output The container where new sequences will be appended.
   */
  template<class ContFrom, class ContTo, class SequenceType>
  static void convertContainer(const ContFrom& input, ContTo& output)
  {
    for (size_t i = 0; i < input.getNumberOfSequences(); ++i)
    {
      auto seq = std::make_unique<SequenceType>(input.sequence(i));
      output.addSequence(seq->getName(), seq);
    }
  }

  /**
   * @brief Add a specified set of sequences from a container to another.
   *
   * Sequences are specified by their position, beginning at 0.
   * Name verification will be performed, only if the output container is not empty,
   * based on the assumption that the container passed as argument is a correct one.
   * Redundant selection is not checked, so be careful with what you're doing!
   *
   * @author Julien Dutheil
   *
   * @param sequences The container from wich sequences are to be taken.
   * @param selection The positions of all sequences to retrieve.
   * @param outputCont A container where the selection should be added.
   * @throw Exception In case of bad sequence name, alphabet mismatch, etc.
   */
  template<class SequenceType, class HashType>
  static void getSelectedSequences(
    const TemplateSequenceContainerInterface<SequenceType, HashType>& sequences,
    const SequenceSelection& selection,
    TemplateSequenceContainerInterface<SequenceType, HashType>& outputCont)
  {
    for (size_t position : selection)
    {
      auto seq = std::make_unique<SequenceType>(sequences.sequence(position));
      outputCont.addSequence(seq->getName(), seq);
    }
  }


  /**
   * @brief Add a specified set of sequences from a container to another.
   *
   * Sequences are specified by their names.
   * Name verification will be performed, only if the output container is not empty,
   * based on the assumption that the container passed as argument is a correct one.
   * Redundant selection is not checked, so be careful with what you're doing!
   *
   * @author Julien Dutheil
   *
   * @param sequences The container from wich sequences are to be taken.
   * @param selection The names of all sequences to retrieve.
   * @param outputCont A container where the selection should be added.
   * @param strict If yes, trying to select a sequence that is not present
   * will raise an exception. If no, only available sequence will be added.
   * @throw Exception In case of bad sequence name, alphabet mismatch, etc.
   */
  template<class SequenceType, class HashType>
  static void getSelectedSequences(
      const TemplateSequenceContainerInterface<SequenceType, HashType>& sequences,
      const std::vector<std::string>& selection,
      TemplateSequenceContainerInterface<SequenceType, HashType>& outputCont,
      bool strict = true)
  {
    for (const std::string& key : selection) {
      if (strict) {
        auto seq = std::make_unique<SequenceType>(sequences.sequence(key));
        outputCont.addSequence(seq->getName(), seq);
      } else {
        if (sequences.hasSequence(key)) {
          auto seq = std::make_unique<SequenceType>(sequences.sequence(key));
          outputCont.addSequence(seq->getName(), seq);
	}
      }
    }
  }


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
  template<class SequenceType, class HashType>
  static void keepOnlySelectedSequences(
      TemplateSequenceContainerInterface<SequenceType, HashType>& sequences,
      const SequenceSelection& selection)
  {
    std::vector<std::string> keys = sequences.getSequenceKeys();
    std::vector<std::string> selectedKeys = VectorTools::extract<std::string>(keys, selection);
    std::vector<std::string> keysToRemove;
    VectorTools::diff(keys, selectedKeys, keysToRemove);
    for (const std::string& key : keysToRemove)
    {
      // We need to do this because after removal the indices will not be the same!
      // another solution would be to sort decreasingly the indices...
      sequences.removeSequence(key);
    }
  }



  /**
   * @brief Check if all sequences in a SequenceContainer have the same length.
   *
   * @param sc The container to check.
   * @return True is all sequence have the same length.
   */
  template<class SequenceType, class HashType>
  static bool sequencesHaveTheSameLength(
      const TemplateSequenceContainerInterface<SequenceType,
      HashType>& sc)
  {
    size_t ns = sc.getNumberOfSequences();
    if (ns <= 1)
      return true;
    size_t length = sc.sequence(0).size();
    for (size_t i = 1; i < ns; ++i)
    {
      if (sc.sequence(i).size() != length)
        return false;
    }
    return true;
  }



  /**
   * @brief Compute base counts
   *
   * Example of usage: getting the GC count from a sequence container.
   * <code>
   *   map<int, unsigned int> counts;
   *  SequenceContainerTools::getCounts(myContainer, counts); //My container is previously defined.
   *   int GCcontent = counts[1] + counts[2] ;
   * </code>
   *
   * States are stored as their int code.
   */
  static void getCounts(const SequenceContainerInterface& sc, std::map<int, unsigned int>& f)
  {
    for (size_t i = 0; i < sc.getNumberOfSequences(); ++i) {
      const Sequence& seq = sc.sequence(i);
      for (size_t j = 0; j < seq.size(); ++j) {
        f[seq[j]]++;
      }
    }
  }



  /**
   * @brief Compute base frequencies of a BasicSequenceContainer.
   *
   * Example of usage: getting the GC content from a sequence container.
   * <code>
   *  map<int, double> freqs;
   *  SequenceContainerTools::getFrequencies(myContainer, freqs); //My container is previously defined.
   *   double GCcontent = (freqs[1] + freqs[2]) / (freqs[0] + freqs[1] + freqs[2] + freqs[3]);
   * </code>
   *
   * States are stored as their int code.
   */
  static void getFrequencies(
		  const SequenceContainerInterface& sc,
		  std::map<int, double>& f,
		  double pseudoCount = 0)
  {
    double n = 0;
    for (size_t i = 0; i < sc.getNumberOfSequences(); ++i) {
      const Sequence& seq = sc.sequence(i);
      SymbolListTools::getCounts(seq, f, true);
      n += static_cast<double>(seq.size());
    }

    if (pseudoCount != 0) {
      std::shared_ptr<const Alphabet> pA = sc.getAlphabet();
      for (int i = 0; i < static_cast<int>(pA->getSize()); ++i) {
        f[i] += pseudoCount;
      }
      n += pseudoCount * static_cast<double>(pA->getSize());
    }

    for (auto& i : f) {
      i.second = i.second / n;
    }
  }



  /**
   * @brief Compute base frequencies of a ProbabilisticSequenceContainer.
   *
   * Example of usage: getting the GC content from a sequence container.
   * <code>
   *  map<int, double> freqs;
   *  SequenceContainerTools::getFrequencies(myContainer, freqs); //My container is previously defined.
   *   double GCcontent = (freqs[1] + freqs[2]) / (freqs[0] + freqs[1] + freqs[2] + freqs[3]);
   * </code>
   *
   * States are stored as their int code.
   */
  static void getFrequencies(
		  const ProbabilisticSequenceContainerInterface& sc,
		  std::map<int, double>& f,
		  double pseudoCount = 0)
  {
    double n = 0;
    for (size_t i = 0; i < sc.getNumberOfSequences(); ++i) {
      const ProbabilisticSequence& seq = sc.sequence(i);
      SymbolListTools::getCounts(seq, f, true);
      n += static_cast<double>(seq.size());
    }

    if (pseudoCount != 0) {
      std::shared_ptr<const Alphabet> pA = sc.getAlphabet();
      for (int i = 0; i < static_cast<int>(pA->getSize()); ++i) {
        f[i] += pseudoCount;
      }
      n += pseudoCount * static_cast<double>(pA->getSize());
    }

    for (auto& i : f) {
      i.second = i.second / n;
    }
  }



  /**
   * @brief Compute base frequencies of an object implementing the SequenceDataInterface.
   *
   * This method will try to cast the input data and call the corresponding method is any.
   * An exception will be thrown if the cast failed. 
   */
  static void getFrequencies(
		  const SequenceDataInterface& sc,
		  std::map<int, double>& f,
		  double pseudoCount = 0)
  {
    try {
      getFrequencies(dynamic_cast<const SequenceContainerInterface&>(sc), f, pseudoCount);
      return;
    } catch (std::bad_cast&) {}   
    try {
      getFrequencies(dynamic_cast<const ProbabilisticSequenceContainerInterface&>(sc), f, pseudoCount);
    } catch (std::bad_cast&) {
    throw Exception("SequenceContainerTools::getFrequencies : unsupported SequenceDataInterface implementation.");
    }
  }

  /**
   * @brief Append all the sequences of a SequenceContainer to the end of another.
   *
   * @param seqCont1 The SequenceContainer in which the sequences will be added.
   * @param seqCont2 The SequenceContainer from which the sequences are taken.
   */
  template<class SequenceType, class HashType>
  static void append(
		  TemplateSequenceContainerInterface<SequenceType, HashType>& seqCont1, 
		  const TemplateSequenceContainerInterface<SequenceType, HashType>& seqCont2)
  {
    const auto& keys = seqCont2.getSequenceKeys();
    for (size_t i = 0; i < seqCont2.getNumberOfSequences(); ++i) {
      auto tm=std::unique_ptr<SequenceType>(seqCont2.sequence(i).clone());
      seqCont1.addSequence(keys[i],tm);
    }
  }
  


  /**
   * @brief Concatenate the sequences from two containers.
   *
   *
   * @author Julien Dutheil
   *
   * @param seqCont1 First container.
   * @param seqCont2 Second container. This container must contain sequences with the same keys as in seqcont1.
   * Additional sequences will be ignored.
   * @param outputCont Output sequence container to which concatenated sequences will be added.
   * @throw AlphabetMismatchException If the alphabet in the 3 containers do not match.
   */
  template<class SequenceType, class HashType>
  static void mergeByKey(
		  const TemplateSequenceContainerInterface<SequenceType, HashType>& seqCont1,
		  const TemplateSequenceContainerInterface<SequenceType, HashType>& seqCont2,
		  TemplateSequenceContainerInterface<SequenceType, HashType>& outputCont)
  {
    if (seqCont1.getAlphabet()->getAlphabetType() != seqCont2.getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("SequenceContainerTools::mergeByKey.", seqCont1.getAlphabet(), seqCont2.getAlphabet());

    for (const auto& key: seqCont1.getSequenceKeys())
    {
      auto tmp = std::unique_ptr<SequenceType>(seqCont1.sequence(key).clone());
      tmp->append(seqCont2.sequence(key));
      outputCont.addSequence(key, tmp);
    }
  }

  /**
   * @brief Convert a SequenceContainer to a new alphabet.
   *
   * @param seqCont The container to convert.
   * @param outputCont A container (most likely empty) with an alphabet into which the container will be converted.
   */
  template<class SequenceType, class HashType>
  static void
  convertAlphabet(
		  const TemplateSequenceContainerInterface<SequenceType, HashType>& seqCont,
		  TemplateSequenceContainerInterface<SequenceType, HashType>& outputCont)
  {
    for (size_t i = 0; i < seqCont.getNumerOfSequence(); ++i) {
      std::string seqName = seqCont.getSequenceName(i);
      std::string seqKey = seqCont.getSequenceKey(i);
      auto seq = std::unique_ptr<SequenceType>(new SequenceType(seqName, seqCont.toString(i), outputCont.getAlphabet()));
      outputCont.addSequence(seqKey, seq);
    }
  }

  /**
   * @brief Extract a certain position (1, 2 or 3) from a container of codon sequences and returns the resulting nucleotide container.
   *
   * @param sequences The input sequence container, with codon alphabet.
   * @param pos       The codon position to retrieve.
   * @return          A SequenceContainer with a nucleotide alphabet.
   * @throw AlphabetException If input sequences are not registered with a codon alphabet.
   */
  template<class SequenceType>
  static std::unique_ptr< TemplateSequenceContainerInterface<SequenceType> >
  getCodonPosition(
		  const TemplateSequenceContainerInterface<SequenceType, std::string>& sequences,
		  size_t pos)
  {
    auto calpha = std::dynamic_pointer_cast<const CodonAlphabet>(sequences.getAlphabet());
    if (!calpha)
      throw AlphabetException("SequenceContainerTools::getCodonPosition. Input sequences should be of type codon.", sequences.getAlphabet());
    auto newcont = std::make_unique< TemplateVectorSequenceContainer<SequenceType> >(calpha->getNucleicAlphabet());
    for (size_t i = 0; i < sequences.getNumberOfSequences(); ++i) {
      const SequenceType& seq = sequences.sequence(i);
      std::vector<int> newseq(seq.size());
      for (size_t j = 0; j < seq.size(); ++j) {
        newseq[i] = calpha->getNPosition(seq[i], pos);
      }
      std::shared_ptr<const bpp::Alphabet> na = calpha->getNucleicAlphabet();
      auto s = std::make_unique<SequenceType>(seq.getName(), newseq, seq.getComments(), na);
      newcont->addSequence(sequences.getSequenceKeys()[i], s);
    }
    return newcont;
  }


};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCECONTAINERTOOLS_H
