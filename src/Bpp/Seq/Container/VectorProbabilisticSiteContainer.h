//
// File: VectorProbabilisticSiteContainer.h
// Authors:
//   Murray Patterson
// Created: 2015-10-19 00:00:00
//

/*
  Copyright or Â© or Copr. CNRS, (November 17, 2004)
  
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

#ifndef BPP_SEQ_CONTAINER_VECTORPROBABILISTICSITECONTAINER_H
#define BPP_SEQ_CONTAINER_VECTORPROBABILISTICSITECONTAINER_H


#include "../ProbabilisticSite.h"
#include "AbstractProbabilisticSequenceContainer.h"
#include "AlignedValuesContainer.h"
#include "OrderedSequenceContainer.h"
#include "ProbabilisticSiteContainer.h"
#include "VectorMappedContainer.h"
#include "VectorPositionedContainer.h"

// From the STL :
#include <vector>

namespace bpp
{
/**
 * @brief The VectorProbabilisticSiteContainer class.
 *
 * ProbabilisticSites are stored in a std::vector of pointers.
 * ProbabilisticSite access is hence in \f$O(1)\f$, and sequence
 * access in \f$O(l)\f$, where \f$l\f$ is the number of sites in the
 * container.
 *
 * This is a modified copy of VectorSiteContainer with the minimum
 * changes necessary for it to work with bppML and bppAncestor
 *
 * @see ProbabilisticSequence, ProbabilisticSite, VectorSiteContainer
 */

class VectorProbabilisticSiteContainer :
    public AbstractProbabilisticSequenceContainer,
// This container implements the SequenceContainer interface
// and use the AbstractSequenceContainer adapter.
    public virtual ProbabilisticSiteContainer,
// This container is a SiteContainer.
    virtual public VectorPositionedContainer<ProbabilisticSite>,
    virtual public VectorMappedContainer<ProbabilisticSequence>
{
public:
  /**
   * @brief Build a new emtpy container.
   *
   */

  VectorProbabilisticSiteContainer(const Alphabet* alpha);

  /**
   * @brief Build a new container from a set of sites.
   *
   * @param vs A std::vector of sites. Those sites are cloned in the container.
   * @param alpha The common alphabet for all sites.
   * @param checkPositions Check for the redundancy of site position tag. This may turn to be very time consuming!
   * @throw Exception If sites differ in size or in alphabet.
   */

  VectorProbabilisticSiteContainer(const std::vector<const CruxSymbolListSite*>& vs, const Alphabet* alpha, bool checkPositions = true);

  /**
   * @brief Build a new container from an AlignedValuesContainer.
   *
   */

  VectorProbabilisticSiteContainer(const AlignedValuesContainer& avc);

  /**
   * @brief Build a new empty container with specified sequence names.
   *
   * @param names Sequence names. This will set the number of sequences in the container.
   * @param alpha The alphabet for this container.
   */

  VectorProbabilisticSiteContainer(const std::vector<std::string>& names, const Alphabet* alpha);

  /**
   * @name The Clonable interface.
   *
   * @{
   */

  /**
   * @brief Copy contructors
   *
   * Sites are cloned.
   *
   */

  VectorProbabilisticSiteContainer(const VectorProbabilisticSiteContainer& vpsc);

  VectorProbabilisticSiteContainer(const OrderedSequenceContainer& osc);


  VectorProbabilisticSiteContainer& operator=(const VectorProbabilisticSiteContainer& vpsc);

  VectorProbabilisticSiteContainer* clone() const { return new VectorProbabilisticSiteContainer(*this); }

  /**
   * @}
   */

  // class destructor
  virtual ~VectorProbabilisticSiteContainer() {}

public:
  size_t getNumberOfSequences() const { return VectorMappedContainer<ProbabilisticSequence>::getSize(); }

  /*
   * @brief get Objects
   *
   * @{
   */
  const std::shared_ptr<ProbabilisticSite> getSite(size_t i) const
  {
    if (i >= getNumberOfSites())
      throw IndexOutOfBoundsException("VectorProbabilisticSiteContainer::getProbabilisticSite.", i, 0, getNumberOfSites() - 1);

    return VectorPositionedContainer<ProbabilisticSite>::getObject(i);
  }

  std::shared_ptr<ProbabilisticSite> getSite(size_t i)
  {
    if (i >= getNumberOfSites())
      throw IndexOutOfBoundsException("VectorProbabilisticSiteContainer::getProbabilisticSite.", i, 0, getNumberOfSites() - 1);

    return VectorPositionedContainer<ProbabilisticSite>::getObject(i);
  }

  /**
   * @brief Get Sequence from a position in the container.
   *
   * @param i  index of the sequence
   * @return A read-only reference to the selected sequence.
   */
  const ProbabilisticSequence& getSequence(std::size_t i) const;

  /**
   * @brief Get Sequence from its name in the container.
   * @param name Name of the sequence
   * @return A read-only reference to the selected sequence.
   */
  const ProbabilisticSequence& getSequence(const std::string& name) const;
  
  bool hasSequence(const std::string& name) const
  {
    // Look for sequence name:
    return VectorMappedContainer<ProbabilisticSequence>::hasObject(name);
  }

  size_t getSequencePosition(const std::string& name) const
  {
    // Look for sequence name:
    return VectorMappedContainer<ProbabilisticSequence>::getObjectPosition(name);
  }

  std::vector<std::string> getSequencesNames() const
  {
    return VectorMappedContainer<ProbabilisticSequence>::getObjectsNames();
  }

  void setSequencesNames(const std::vector<std::string>& names, bool checkNames = true)
  {
    VectorMappedContainer<ProbabilisticSequence>::setObjectsNames(names);
  }


  void setComments(size_t sequenceIndex, const Comments& comments)
  {
    VectorMappedContainer<ProbabilisticSequence>::getObject(sequenceIndex)->setComments(comments);
  }


  /*
   * @}
   *
   */


  /**
   * @name SequencedValuesContainer methods.
   *
   * @{
   */

  /*
   * @brief get value of a state in a position
   * @param siteIndex  index of the site in the container
   * @param sequenceIndex index of the looked value in the site
   * @param state  state alphabet
   */
  double getStateValueAt(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    if (siteIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorProbabilisticSiteContainer::getStateValueAt.", siteIndex, 0, getNumberOfSites() - 1);
    return (*getSite(siteIndex))[getSequencePosition(sequenceName)][(size_t)state];
  }

  double operator()(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    return (*getSite(siteIndex))[getSequencePosition(sequenceName)][(size_t)state];
  }

  /*
   * @}
   *
   */

  /**
   * @name OrderedValuesContainer methods.
   *
   * @{
   */

  /*
   * @brief get value of a state in a position
   * @param siteIndex  index of the site in the container
   * @param sequenceIndex index of the looked value in the site
   * @param state  state alphabet
   */
  double getStateValueAt(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorProbabilisticSiteContainer::getStateValueAt.", sequenceIndex, 0, getNumberOfSequences() - 1);
    if (siteIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorProbabilisticSiteContainer::getStateValueAt.", siteIndex, 0, getNumberOfSites() - 1);
    return (*getSite(siteIndex))[sequenceIndex][(size_t)state];
  }

  double operator()(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    return (*getSite(siteIndex))[sequenceIndex][(size_t)state];
  }

  /*
   * @}
   *
   */

  /*
   * @brief Append a Site. The shared_ptr is shared.
   *
   */

  void addSite(std::shared_ptr<ProbabilisticSite> site, bool checkPosition = false);

  /*
   * @brief Add a Sequence of sites. Sites are copied.
   *
   */

  void addSequence(const ProbabilisticSequence& sequence, bool checkName = true);

  void addSequence(const Sequence& sequence, bool checkName = true);

  void setSequence(const std::string& name, const ProbabilisticSequence& sequence, bool checkName);

  void setSequence(size_t sequenceIndex, const ProbabilisticSequence& sequence, bool checkName);

  void clear();

  VectorProbabilisticSiteContainer* createEmptyContainer() const;

  /*
   * @name From AlignedValuesContainer interface
   *
   * @{
   */
  void deleteSites(size_t siteIndex, size_t length)
  {
    VectorPositionedContainer<ProbabilisticSite>::deleteObjects(siteIndex, length);
  }

  size_t getNumberOfSites() const
  {
    return VectorPositionedContainer<ProbabilisticSite>::getSize();
  }

  void reindexSites();

  Vint getSitePositions() const;

  void setSitePositions(Vint vPositions);

protected:
  // Create n void sites:
  void realloc(size_t n);

  /** @} */
};
} // end of namespace bpp
#endif // BPP_SEQ_CONTAINER_VECTORPROBABILISTICSITECONTAINER_H
