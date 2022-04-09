//
// File: VectorSequenceContainer.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
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

#ifndef BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H
#define BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H

#include <Bpp/Exceptions.h>

#include "../Alphabet/Alphabet.h"
#include "../Sequence.h"
#include "AbstractSequenceContainer.h"
#include "VectorMappedContainer.h"

// From the STL:
#include <algorithm>
#include <vector>

namespace bpp
{
/**
 * @brief The VectorSequenceContainer class.
 *
 * This is the simplest implementation of the SequenceContainer interface.
 * std::shared_ptr<Sequence> are stored in a std::vector, as well as in a std::map.
 */
template<class SequenceType>
class VectorSequenceContainer :
  virtual public AbstractSequenceContainer<SequenceType, std::string>,
  virtual private VectorMappedContainer<SequenceType>
{
public:

  /**
   * @brief Build a container with pointers to sequence objects.
   *
   * The keys of the map are set to the names of the sequences.
   *
   * @param alphabet The alphabet of the container.
   * @param vs       A vector of smart pointers toward sequence objects.
   */
  VectorSequenceContainer(
      const std::shared_ptr<const Alphabet> alphabet,
      const std::vector< std::unique_ptr<SequenceType> >& vs):
    AbstractSequenceContainer<SequenceType, std::string>(alphabet),
    VectorMappedContainer<SequenceType>()
  {
    for (auto seq: vs)
    {
      addSequence(seq);
    }
  }

  /**
   * @brief Build an empty container that will contain sequences of a particular alphabet.
   *
   * @param alphabet The alphabet of the container.
   */
  VectorSequenceContainer(const std::shared_ptr<const Alphabet> alphabet) :
      AbstractSequenceContainer<SequenceType, std::string>(alphabet),
      VectorMappedContainer<SequenceType>()
  {}

  /**
   * @name Copy contructors:
   *
   * @{
   */

  /**
   * @brief Copy from a VectorSequenceContainer.
   *
   * @param vsc The VectorSequenceContainer to copy into this container.
   */
  VectorSequenceContainer(
      const VectorSequenceContainer<SequenceType, std::string>& vsc):
    AbstractSequenceContainer<SequenceType, std::string>(vsc),
    VectorMappedContainer<SequenceType>()
  {
    size_t max = vsc.getNumberOfSequences();
    for (size_t i = 0; i < max; i++)
    {
      addSequence(vsc.getSequenceKey(i), vsc.getSequence(i));
    }
  }



  /**
   * @brief Copy from a SequenceContainer.
   *
   * @param sc The SequenceContainer to copy into this container.
   */
  VectorSequenceContainer(const SequenceContainer<SequenceType, std::string>& sc):
    AbstractSequenceContainer(sc.getAlphabet()),
    VectorMappedContainer<Sequence>()
  {
    size_t max = vsc.getNumberOfSequences();
    for (size_t i = 0; i < max; i++)
    {
      addSequence(vsc.getSequenceKey(i), vsc.getSequence(i));
    }
    setComments(sc.getComments());
  }


  /** @} */

  /**
   * @brief Assign from a VectorSequenceContainer.
   *
   * @param vsc The VectorSequenceContainer to copy into this container.
   */
  VectorSequenceContainer& operator=(const VectorSequenceContainer& vsc)
  {
    clear();
    AbstractSequenceContainer::operator=(vsc);
    size_t max = vsc.getNumberOfSequences();
    for (size_t i = 0; i < max; i++)
    {
      addSequence(vsc.getSequenceKey(i), vsc.getSequence(i));
    }
    return *this;
  }

  /**
   * @brief Copy from a SequenceContainer.
   *
   * @param sc The SequenceContainer to copy into this container.
   */
  VectorSequenceContainer& operator=(const SequenceContainer& sc)
  {
    clear();
    AbstractSequenceContainer::operator=(sc);
    size_t max = vsc.getNumberOfSequences();
    for (size_t i = 0; i < max; i++)
    {
      addSequence(vsc.getSequenceKey(i), vsc.getSequence(i));
    }
    setComments(sc.getComments());
    return *this;
  }

  void clear()
  {
    VectorMappedContainer<Sequence>::clear();
  }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  VectorSequenceContainer* clone() const { return new VectorSequenceContainer(*this); }
  /** @} */



  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */
  VectorSequenceContainer* createEmptyContainer() const 
  {
    VectorSequenceContainer* vsc = new VectorSequenceContainer(getAlphabet(), getComments());
    return vsc;
  }
 
  double getStateValueAt(size_t sitePosition, const std::string& sequenceKey, int state) const
  {
    return getSequence(sequenceName).getStateValueAt(sitePosition, state);
  }

  double operator()(size_t sitePosition, const std::string& sequenceKey, int state) const
  {
    return getSequence(sequenceName)(sitePosition, state);
  }

  double getStateValueAt(size_t sitePosition, size_t sequencePosition, int state) const
  {
    if (sequencePosition >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSequenceContainer::getStateValueAt.", sequencePosition, 0, getNumberOfSequences() - 1);
    const Sequence& seq = getSequence(sequencePosition);

    if (sitePosition >= seq.size())
      throw IndexOutOfBoundsException("VectorSequenceContainer::getStateValueAt.", sitePosition, 0, seq.size() - 1);

    return getAlphabet()->isResolvedIn(seq[sitePosition], state) ? 1. : 0.;
  }

  double operator()(size_t sitePosition, size_t sequencePosition, int state) const
  {
    return getAlphabet()->isResolvedIn(getSequence(sequencePosition)[sitePosition], state) ? 1. : 0.;
  }
 
  std::vector<std::string> getSequenceKeys() const { return getObjectNames(); }

  size_t getNumberOfSequences() const { return getSize(); }
 
  size_t getSequencePosition(const std::string& sequenceKey) const
  {
    return getObjectPosition(sequenceKey);
  }

  const int& valueAt(const std::string& sequenceKey, size_t elementPosition) const
  {
    return getSequence(sequenceKey)[elementPosition];
  }

  const int& operator()(const std::string& sequenceKey, size_t elementPosition) const
  {
    return getSequence(sequenceKey)[elementPosition];
  }

  const int& valueAt(size_t sequencePosition, size_t elementPosition) const
  {
    return getSequence(sequencePosition)[elementPosition];
  }

  const int& operator()(size_t sequencePosition, size_t elementPosition) const
  {
    return getSequence(sequencePosition)[elementPosition];
  }


  bool hasSequence(const std::string& sequenceKey) const
  {
    return hasObject(sequenceKey);
  }

  const SequenceType& getSequence(const std::string& sequenceKey) const
  {
    return *getObject(sequenceKey);
  }

  void setSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequence, bool updateKey = false)
  {
    setSequence(getSequencePosition(sequenceKey), sequence, updateKey);
  }

  std::unique_ptr<SequenceType> removeSequence(const std::string& sequenceKey)
  {
    std::shared_ptr<SequenceType> ptr = removeObject(sequenceKey);
    return(make_unique<SequenceType>(ptr.release())); //Not elegant but safe because of the private inheritence.
  }

  void deleteSequence(const std::string& sequenceKey)
  {
    deleteObject(sequenceKey);
  }

  void deleteSequence(size_t sequencePosition)
  {
    deleteObject(sequencePosition);
  }

  const SequenceType& getSequence(size_t sequencePosition) const
  {
    return *getObject(sequencePosition);
  }

  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequence, bool updateKey)
  {
    if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", getAlphabet(), sequence->getAlphabet());

    addObject(std::shared_ptr<SequenceType>(sequence), sequencePosition, updateKey ? sequence->getName() : getSequenceKey(sequencePosition), false);
  }

  std::unique_ptr<SequenceType> removeSequence(size_t sequencePosition)
  {
    std::shared_ptr<SequenceType> ptr = removeObject(sequencePosition);
    return(make_unique<SequenceType>(ptr.release())); //Not elegant but safe because of the private inheritence.
  }

  void addSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequence)
  {
    if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::addSequence : Alphabets do not match.", getAlphabet(), sequence->getAlphabet());

    appendObject(std::shared_ptr<SequenceType>(sequence), key, false);
  }
 
  std::vector<std::string> getSequenceNames() const
  {
    size_t nbSeq = getNumberOfSequences();
    std::vector<std::string> vs(nbSeq);
    for (size_t i = 0; i < nbSeq; ++i) {
      vs[i] = getSequence(i).getName();
    }
    return vs;
  }


  void setSequenceNames(const std::vector<std::string>& names, bool updateKeys)
  {
    if (names.size() != getNumberOfSequences())
      throw DimensionException("VectorSequenceContainer::setSequenceNames : bad number of names", names.size(), getNumberOfSequences(), getNumberOfSequences());
    for (size_t i = 0; i < names.size(); i++)
    {
      getSequence_(i).setName(names[i]);
    }
    if (updateKey) {
      setObjectNames(names);
    }
  }

  /** @} */



  /**
   * @brief Add a sequence to the container, using its name as a key.
   *
   * @param sequence  The sequence to add.
   */
  void addSequence(const std::unique_ptr<SequenceType> sequencePtr)
  {
    if (sequence.getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::addSequence: Alphabets do not match.", getAlphabet(), sequencePtr->getAlphabet());

    appendObject(sequencePtr, sequencePtr->getName(), false);
  }
 
protected:

  SequenceType& getSequence_(size_t sequencePosition)
  {
    return *getObject(sequencePosition);
  }


  /**
   * @brief getSequence with given key
   *
   */
  
  SequenceType& getSequence_(const std::string& sequenceKey)
  {
    return *getObject(key);
  }

  /** @} */

 };

  //Aliases:
  using BasicVectorSequenceContainer = VectorSequenceContainer<Sequence>;
  using ProbabilisticVectorSequenceContainer = VectorSequenceContainer<ProbabilisticSequence>;

} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H
