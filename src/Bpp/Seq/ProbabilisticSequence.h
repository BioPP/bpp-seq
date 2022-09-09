//
// File: ProbabilisticSequence.h
// Authors:
//   Murray Patterson
// Created: 2015-10-04 00:00:00
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

#ifndef BPP_SEQ_PROBABILISTICSEQUENCE_H
#define BPP_SEQ_PROBABILISTICSEQUENCE_H


#include "CoreSequence.h"
#include "ProbabilisticSymbolList.h"
#include <Bpp/Numeric/Table.h>

// From the STL :
#include <string>

namespace bpp
{
  /**
   * @brief The probabilistic sequence interface.
   *
   * This is a general purpose container, containing an ordered list of
   * elements.  The states represented by the elements is defined by an
   * alphabet object.
   *
   * ProbabilisticSequence objects also contain a name attribute and
   * potentially several comment lines.
   *
   * @see Alphabet
   */
  class ProbabilisticSequenceInterface:
    public virtual CoreSequenceInterface,
    public virtual ProbabilisticSymbolListInterface
  {
  public:
    typedef std::vector<double> ElementType;

  public:
    virtual ~ProbabilisticSequenceInterface() {}

  public:
    virtual ProbabilisticSequenceInterface* clone() const = 0;
    
  };
    
/**
 * @brief A basic implementation of the ProbabilisticSequence interface.
 *
 * This is a general purpose container, containing an ordered list of
 * elements.  The states represented by the elements is defined by an
 * alphabet object, which is passed to the contructor by a pointer.
 *
 * ProbabilisticSequence objects also contain a name attribute and
 * potentially several comment lines.
 *
 * @see Alphabet
 */
class ProbabilisticSequence :
  public virtual ProbabilisticSequenceInterface,
  public AbstractCoreSequence,
  public virtual ProbabilisticSymbolList //Diamond inheritence
{
  public:
    /**
     * @brief Empty constructor : build a void ProbabilisticSequence with just an Alphabet
     *
     * One can use it safely for all types of Alphabet in order to build
     * an empty ProbabilisticSequence, i.e., without name nor sequence
     * data.
     *
     * @param alphabet A pointer to the Alphabet to be used with this ProbabilisticSequence.
     */
    ProbabilisticSequence(std::shared_ptr<const Alphabet>& alphabet):
      ProbabilisticSymbolList(alphabet), AbstractCoreSequence()
    {}

    /**
     * @brief Direct constructor : build a ProbabilisticSequence object from a vector.
     *
     * @param name The sequence name.
     * @param sequence The entire sequence to parsed as a vector.
     * @param alphabet A pointer to the alphabet associated with this sequence.
     * @throws Exception if the content is internally inconsistent, or is inconsistent with the specified alphabet.
     */
    ProbabilisticSequence(
        const std::string& name,
	const std::vector<ProbabilisticSequence::SymbolType>& sequence,
	std::shared_ptr<const Alphabet>& alphabet):
      ProbabilisticSymbolList(sequence, alphabet), AbstractCoreSequence(name)
    {}

    /**
     * @brief Direct constructor : build a ProbabilisticSequence object from Table<double>.
     *
     * One can use it safely for RNA, DNA and protein sequences.
     *
     * @param name The sequence name.
     * @param sequence The entire sequence to parsed as a Table<double>.
     * @param alphabet A pointer to the alphabet associated with this sequence.
     * @throws Exception if the content is internally inconsistent, or is inconsistent with the specified alphabet.
     */
    ProbabilisticSequence(
        const std::string& name,
	const DTable& sequence,
	std::shared_ptr<const Alphabet>& alphabet):
      ProbabilisticSymbolList(sequence, alphabet),
      AbstractCoreSequence(name)
    {}

    /**
     * @brief Direct constructor : build a ProbabilisticSequence object from a vector.
     *
     * @param name The sequence name.
     * @param sequence The entire sequence to parsed as a vector.
     * @param comments Comments to add to the sequence.
     * @param alphabet A pointer to the alphabet associated with this sequence.
     * @throws Exception if the content is internally inconsistent, or is inconsistent with the specified alphabet.
     */
    ProbabilisticSequence(
	const std::string& name,
	const std::vector<ProbabilisticSequence::SymbolType>& sequence,
	const Comments& comments,
	std::shared_ptr<const Alphabet>& alphabet):
      ProbabilisticSymbolList(sequence, alphabet),
      AbstractCoreSequence(name, comments) 
    {}

    /**
     * @brief Direct constructor : build a ProbabilisticSequence object from a DTable.
     *
     * One can use it safely for RNA, DNA and protein sequences.
     *
     * @param name The sequence name.
     * @param sequence The entire sequence to parsed as a DTable.
     * @param comments Comments to add to the sequence.
     * @param alphabet A pointer to the alphabet associated with this sequence.
     * @throws Exception if the content is internally inconsistent, or is inconsistent with the specified alphabet.
     */
    ProbabilisticSequence(
	const std::string& name,
	const DTable& sequence,
	const Comments& comments,
	std::shared_ptr<const Alphabet>& alphabet):
      ProbabilisticSymbolList(sequence, alphabet),
      AbstractCoreSequence(name, comments) 
    {}

    /**
     * @brief The copy constructor.
     */
    ProbabilisticSequence(const ProbabilisticSequence& sequence) :
      ProbabilisticSymbolList(sequence),
      AbstractCoreSequence(sequence)
    {}


    /**
     * @brief The assignment operator.
     *
     * @return A reference to the assigned ProbabilisticSequence.
     */
    ProbabilisticSequence& operator=(const ProbabilisticSequence& sequence)
    {
      ProbabilisticSymbolList::operator=(sequence);
      AbstractCoreSequence::operator=(sequence);
      return *this;
    }


    /**
     * @name The Clonable interface
     *
     * @{
     */
    ProbabilisticSequence* clone() const override
    { 
      return new ProbabilisticSequence(*this);
    }

    /**
     * @}
     */

    // class destructor
    virtual ~ProbabilisticSequence() {}

  public:
    void setToSizeR(size_t newSize) override {} // leave empty for now

    void setToSizeL(size_t newSize) override {}

    double getStateValueAt(size_t sitePosition, int state) const override
    {
      if (sitePosition  >= size()) throw IndexOutOfBoundsException("ProbabilisticSequence::getStateValueAt.", sitePosition, 0, size() - 1);
      return (*this)[sitePosition][static_cast<size_t>(state)];
    }
  };
} // end of namespace bpp
#endif // BPP_SEQ_PROBABILISTICSEQUENCE_H
