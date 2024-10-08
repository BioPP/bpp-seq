// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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
class ProbabilisticSequenceInterface :
  public virtual CoreSequenceInterface,
  public virtual ProbabilisticSymbolListInterface
{
public:
  typedef std::vector<double> ElementType;

public:
  virtual ~ProbabilisticSequenceInterface()
  {}

public:
  virtual ProbabilisticSequenceInterface* clone() const = 0;
};

/**
 * @brief A basic implementation of the ProbabilisticSequence interface.
 *
 * This is a general purpose container, containing an ordered list of
 * elements.  The states represented by the elements is defined by an
 * alphabet object, which is passed to the constructor by a pointer.
 *
 * ProbabilisticSequence objects also contain a name attribute and
 * potentially several comment lines.
 *
 * @see Alphabet
 */
class ProbabilisticSequence :
  public virtual ProbabilisticSequenceInterface,
  public AbstractCoreSequence,
  public virtual ProbabilisticSymbolList // Diamond inheritance
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
  ProbabilisticSequence(std::shared_ptr<const Alphabet>& alphabet) :
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
      std::shared_ptr<const Alphabet>& alphabet) :
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
      std::shared_ptr<const Alphabet>& alphabet) :
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
      std::shared_ptr<const Alphabet>& alphabet) :
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
      std::shared_ptr<const Alphabet>& alphabet) :
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
  virtual ~ProbabilisticSequence()
  {}

public:
  /*
   * @brief append or remove lines at the bottom (ie end of the
   * sequence) to fit newSize.
   *
   * Additional characters are gaps, ie vectors of 0
   */

  void setToSizeR(size_t newSize) override;

  /*
   * @brief append or remove lines at the top (ie start of the
   * sequence) to fit newSize.
   *
   * Additional characters are gaps, ie vectors of 0
   */

  void setToSizeL(size_t newSize) override;

  double getStateValueAt(size_t sitePosition, int state) const override
  {
    if (sitePosition  >= size()) throw IndexOutOfBoundsException("ProbabilisticSequence::getStateValueAt.", sitePosition, 0, size() - 1);
    return (*this)[sitePosition][getAlphabet()->getStateIndex(state) - 1];
  }
};
} // end of namespace bpp
#endif // BPP_SEQ_PROBABILISTICSEQUENCE_H
