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
#include "../ProbabilisticSequence.h"
#include "AbstractSequenceContainer.h"
#include "VectorMappedContainer.h"

// From the STL:
#include <algorithm>
#include <vector>
#include <memory>

namespace bpp
{
/**
 * @brief The VectorSequenceContainer class.
 *
 * This is the simplest implementation of the SequenceContainer interface.
 * std::shared_ptr<Sequence> are stored in a std::vector, as well as in a std::map.
 */
template<class SequenceType>
class TemplateVectorSequenceContainer :
  public AbstractTemplateSequenceContainer<SequenceType, std::string>
{
protected:
  VectorMappedContainer<SequenceType> sequenceVectorMap_;

public:

  /**
   * @brief Build a container with pointers to sequence objects.
   *
   * The keys of the map are set to the names of the sequences.
   *
   * @param alphabet The alphabet of the container.
   * @param vs       A vector of smart pointers toward sequence objects.
   */
  TemplateVectorSequenceContainer(
      std::shared_ptr<const Alphabet> alphabet,
      std::vector<std::unique_ptr<SequenceType>>& vs):
    AbstractTemplateSequenceContainer<SequenceType, std::string>(alphabet),
    sequenceVectorMap_()
  {
    for (auto& seqPtr : vs) {
      addSequence(seqPtr->getName(), seqPtr);
    }
  }

  /**
   * @brief Build an empty container that will contain sequences of a particular alphabet.
   *
   * @param alphabet The alphabet of the container.
   */
  TemplateVectorSequenceContainer(std::shared_ptr<const Alphabet> alphabet) :
      AbstractTemplateSequenceContainer<SequenceType>(alphabet),
      sequenceVectorMap_()
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
  TemplateVectorSequenceContainer(
      const TemplateVectorSequenceContainer<SequenceType>& vsc):
    AbstractTemplateSequenceContainer<SequenceType, std::string>(vsc),
    sequenceVectorMap_()
  {
    size_t max = vsc.getNumberOfSequences();
    for (size_t i = 0; i < max; ++i)
    {
      auto seqPtr = std::unique_ptr<SequenceType>(vsc.sequence(i).clone());
      addSequence(vsc.sequenceKey(i), seqPtr);
    }
  }



  /**
   * @brief Copy from a SequenceContainer.
   *
   * @param sc The SequenceContainer to copy into this container.
   */
  TemplateVectorSequenceContainer(const TemplateSequenceContainerInterface<SequenceType, std::string>& sc):
    AbstractTemplateSequenceContainer<SequenceType>(sc.getAlphabet()),
    sequenceVectorMap_()
  {
    size_t max = sc.getNumberOfSequences();
    for (size_t i = 0; i < max; ++i)
    {
      auto seqPtr = std::unique_ptr<SequenceType>(sc.sequence(i).clone());
      addSequence(sc.sequenceKey(i), seqPtr);
    }
    setComments(sc.getComments());
  }


  /** @} */

  /**
   * @brief Assign from a VectorSequenceContainer.
   *
   * @param vsc The VectorSequenceContainer to copy into this container.
   */
  TemplateVectorSequenceContainer& operator=(const TemplateVectorSequenceContainer& vsc)
  {
    clear();
    AbstractTemplateSequenceContainer<SequenceType>::operator=(vsc);
    size_t max = vsc.getNumberOfSequences();
    for (size_t i = 0; i < max; ++i)
    {
      auto seqPtr = std::unique_ptr<SequenceType>(vsc.sequence(i).clone());
      addSequence(vsc.sequenceKey(i), seqPtr);
    }
    return *this;
  }

  /**
   * @brief Copy from a SequenceContainer.
   *
   * @param sc The SequenceContainer to copy into this container.
   */
  TemplateVectorSequenceContainer& operator=(const TemplateSequenceContainerInterface<SequenceType, std::string>& sc)
  {
    clear();
    AbstractTemplateSequenceContainer<SequenceType>::operator=(sc);
    size_t max = sc.getNumberOfSequences();
    for (size_t i = 0; i < max; ++i)
    {
      auto seqPtr = std::unique_ptr<SequenceType>(sc.sequence(i).clone());
      addSequence(sc.sequenceKey(i), seqPtr);
    }
    setComments(sc.getComments());
    return *this;
  }

  virtual ~TemplateVectorSequenceContainer() {}
  
  void clear() override
  {
    sequenceVectorMap_.clear();
  }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  TemplateVectorSequenceContainer<SequenceType>* clone() const override { return new TemplateVectorSequenceContainer<SequenceType>(*this); }
  /** @} */



  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */
  using AbstractTemplateSequenceContainer<SequenceType, std::string>::getAlphabet;
  using AbstractTemplateSequenceContainer<SequenceType, std::string>::getComments;
  using AbstractTemplateSequenceContainer<SequenceType, std::string>::setComments;

  TemplateVectorSequenceContainer<SequenceType>* createEmptyContainer() const override 
  {
    auto alphaPtr = getAlphabet();
    TemplateVectorSequenceContainer<SequenceType>* vsc = new TemplateVectorSequenceContainer<SequenceType>(alphaPtr);
    vsc->setComments(getComments());
    return vsc;
  }
 
  double getStateValueAt(size_t sitePosition, const std::string& sequenceKey, int state) const override
  {
    return sequence(sequenceKey).getStateValueAt(sitePosition, state);
  }

  double operator()(size_t sitePosition, const std::string& sequenceKey, int state) const override
  {
    return sequence(sequenceKey)(sitePosition, state);
  }

  double getStateValueAt(size_t sitePosition, size_t sequencePosition, int state) const override
  {
    return sequence(sequencePosition).getStateValueAt(sitePosition, state);
  }

  double operator()(size_t sitePosition, size_t sequencePosition, int state) const override
  {
    return sequence(sequencePosition)(sitePosition, state);
  }
 
  size_t getNumberOfSequences() const override
  { 
    return sequenceVectorMap_.getSize();
  }
 
  std::vector<std::string> getSequenceKeys() const override
  { 
    return sequenceVectorMap_.getObjectNames();
  }

  void setSequenceKeys(const std::vector<std::string>& sequenceKeys) override
  {
    sequenceVectorMap_.setObjectNames(sequenceKeys);
  }

  const std::string& sequenceKey(size_t sequencePosition) const override
  { 
    return sequenceVectorMap_.getObjectName(sequencePosition);
  }

  size_t getSequencePosition(const std::string& sequenceKey) const override
  {
    return sequenceVectorMap_.getObjectPosition(sequenceKey);
  }

  const typename SequenceType::ElementType& valueAt(const std::string& sequenceKey, size_t elementPosition) const override
  {
    return sequence(sequenceKey)[elementPosition];
  }

  const typename SequenceType::ElementType& valueAt(size_t sequencePosition, size_t elementPosition) const override
  {
    return sequence(sequencePosition)[elementPosition];
  }

  bool hasSequence(const std::string& sequenceKey) const override
  {
    return sequenceVectorMap_.hasObject(sequenceKey);
  }

  const SequenceType& sequence(const std::string& sequenceKey) const override
  {
    return *sequenceVectorMap_.getObject(sequenceKey);
  }

  void setSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequencePtr) override
  {
    setSequence(getSequencePosition(sequenceKey), sequencePtr);
  }

  void addSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequencePtr) override
  {
    if (sequencePtr->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::addSequence : Alphabets do not match.", getAlphabet(), sequencePtr->getAlphabet());

    std::shared_ptr<SequenceType> sequencePtr2(sequencePtr.release(), SwitchDeleter<SequenceType>());
    sequenceVectorMap_.appendObject(sequencePtr2, sequenceKey, true);
  }
 
  std::unique_ptr<SequenceType> removeSequence(const std::string& sequenceKey) override
  {
    std::shared_ptr<SequenceType> ptr = sequenceVectorMap_.removeObject(sequenceKey);
    std::get_deleter<SwitchDeleter<SequenceType>, SequenceType>(ptr)->off();
    return(std::unique_ptr<SequenceType>(ptr.get())); //Not elegant but safe because of the complete encapsulation.
  }

  void deleteSequence(const std::string& sequenceKey) override
  {
    sequenceVectorMap_.deleteObject(sequenceKey);
  }

  void deleteSequence(size_t sequencePosition) override
  {
    sequenceVectorMap_.deleteObject(sequencePosition);
  }

  const SequenceType& sequence(size_t sequencePosition) const override
  {
    return *sequenceVectorMap_.getObject(sequencePosition);
  }

  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr) override
  {
    if (sequencePtr->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", getAlphabet(), sequencePtr->getAlphabet());

    sequenceVectorMap_.addObject(std::move(sequencePtr), sequencePosition, sequenceKey(sequencePosition), false);
  }

  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr, const std::string& sequenceKey) override
  {
    if (sequencePtr->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::setSequence : Alphabets don't match", getAlphabet(), sequencePtr->getAlphabet());

    sequenceVectorMap_.addObject(std::move(sequencePtr), sequencePosition, sequenceKey, false);
  }

  void insertSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequencePtr, const std::string& sequenceKey) override
  {
    if (sequencePtr->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSequenceContainer::insertSequence : Alphabets don't match", getAlphabet(), sequencePtr->getAlphabet());

    sequenceVectorMap_.insertObject(std::move(sequencePtr), sequencePosition, sequenceKey);
  }

  std::unique_ptr<SequenceType> removeSequence(size_t sequencePosition) override
  {
    std::shared_ptr<SequenceType> ptr = sequenceVectorMap_.removeObject(sequencePosition);
    std::get_deleter<SwitchDeleter<SequenceType>, SequenceType>(ptr)->off();
    return(std::unique_ptr<SequenceType>(ptr.get())); //Not elegant but safe because of the complete encapsulation.
  }

 
  std::vector<std::string> getSequenceNames() const override
  {
    size_t nbSeq = getNumberOfSequences();
    std::vector<std::string> vs(nbSeq);
    for (size_t i = 0; i < nbSeq; ++i) {
      vs[i] = sequence(i).getName();
    }
    return vs;
  }

  void setSequenceNames(const std::vector<std::string>& names, bool updateKeys) override
  {
    if (names.size() != getNumberOfSequences())
      throw DimensionException("VectorSequenceContainer::setSequenceNames : bad number of names", names.size(), getNumberOfSequences());
    for (size_t i = 0; i < names.size(); ++i)
    {
      getSequence_(i).setName(names[i]);
    }
    if (updateKeys) {
      sequenceVectorMap_.setObjectNames(names);
    }
  }

  std::vector<Comments> getSequenceComments() const override
  {
    size_t nbSeq = getNumberOfSequences();
    std::vector<Comments> vs(nbSeq);
    for (size_t i = 0; i < nbSeq; ++i) {
      vs[i] = sequence(i).getComments();
    }
    return vs;
  }

  /** @} */



  /**
   * @brief Add a sequence to the container, using its name as a key.
   *
   * @param sequence  The sequence to add.
   */
  virtual void addSequence(std::unique_ptr<SequenceType>& sequencePtr)
  {
    addSequence(sequencePtr->getName(), sequencePtr);
  }
 
protected:

  virtual SequenceType& getSequence_(size_t sequencePosition)
  {
    return *sequenceVectorMap_.getObject(sequencePosition);
  }


  /**
   * @brief getSequence with given key
   *
   */
  
  virtual SequenceType& getSequence_(const std::string& sequenceKey)
  {
    return *sequenceVectorMap_.getObject(sequenceKey);
  }

  /** @} */

};

//Aliases:
using VectorSequenceContainer = TemplateVectorSequenceContainer<Sequence>;
using ProbabilisticVectorSequenceContainer = TemplateVectorSequenceContainer<ProbabilisticSequence>;

} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_VECTORSEQUENCECONTAINER_H
