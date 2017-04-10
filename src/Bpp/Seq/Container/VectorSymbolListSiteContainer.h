//
// File: VectorSymbolListSiteContainer.h
// Created by: Laurent Gueguen
// Created on: vendredi 31 mars 2017, à 15h 49
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

#ifndef _VECTOR_SYMBOLLIST_SITECONTAINER_H_
#define _VECTOR_SYMBOLLIST_SITECONTAINER_H_

#include "VectorPositionedContainer.h"
#include "VectorMappedContainer.h"

#include "../SymbolListSite.h"
#include "../CoreSequence.h"

// From the STL :
#include <vector>

namespace bpp
{

/**
 * @brief The VectorSymbolListSiteContainer class.
 *
 * SymbolListSites are stored in a std::vector of pointers.
 * SymbolListSite access is hence in \f$O(1)\f$, and sequence
 * access in \f$O(l)\f$, where \f$l\f$ is the number of sites in the
 * container.
 *
 * This is a modified copy of VectorSiteContainer with the minimum
 * changes necessary for it to work with bppML and bppAncestor
 *
 * @see CoreSequence, SymbolListSite, VectorSiteContainer
 */
  
class VectorSymbolListSiteContainer :
    public VectorPositionedContainer<SymbolListSite>,
    public VectorMappedContainer<CoreSequence>,
    public Commentable
{
private:
  const Alphabet* alphabet_;
  
public :

  /**
   * @brief Build a new emtpy container.
   *
   */
  
  VectorSymbolListSiteContainer(const Alphabet * alpha);

  /**
   * @name The Clonable interface.
   *
   * @{
   */

  VectorSymbolListSiteContainer(const VectorSymbolListSiteContainer& vpsc) :
    VectorPositionedContainer<SymbolListSite>(vpsc),
    VectorMappedContainer<CoreSequence>(vpsc),
    Commentable(vpsc),
    alphabet_(vpsc.alphabet_)
  {
  }

  VectorSymbolListSiteContainer& operator=(const VectorSymbolListSiteContainer& vpsc)
  {
    alphabet_ = vpsc.alphabet_;
    VectorPositionedContainer<SymbolListSite>::operator=(vpsc);
    VectorMappedContainer<CoreSequence>::operator=(vpsc);
    Commentable::operator=(vpsc);
    
    return *this;
  }
  
  VectorSymbolListSiteContainer* clone() const { return new VectorSymbolListSiteContainer(*this); }

  /**
   * @}
   */

  // class destructor
  virtual ~VectorSymbolListSiteContainer() {}
  
 public :

  const Alphabet* getAlphabet() const
  {
    return alphabet_;
  }

  size_t getNumberOfSites() const { return VectorPositionedContainer<SymbolListSite>::getSize(); }
  
  size_t getNumberOfSequences() const { return VectorMappedContainer<CoreSequence>::getSize(); }

  /*
   * @brief get Objects
   *
   * @{
   */
  
  const std::shared_ptr<SymbolListSite> getSite(size_t i) const
  {
    if(i >= getNumberOfSites())
      throw IndexOutOfBoundsException("VectorSymbolListSiteContainer::getSymbolListSite.", i, 0, getNumberOfSites() - 1);

    return VectorPositionedContainer<SymbolListSite>::getObject(i);
  }

  std::shared_ptr<SymbolListSite> getSite(size_t i)
  {
    if(i >= getNumberOfSites())
      throw IndexOutOfBoundsException("VectorSymbolListSiteContainer::getSymbolListSite.", i, 0, getNumberOfSites() - 1);

    return VectorPositionedContainer<SymbolListSite>::getObject(i);
  }

  const std::shared_ptr<CoreSequence> getSequence(std::size_t i) const
  {
    return VectorMappedContainer<CoreSequence>::getObject(i);
  }

  std::shared_ptr<CoreSequence> getSequence(std::size_t i)
  {
    return VectorMappedContainer<CoreSequence>::getObject(i);
  }

  /*
   * @}
   *
   */

  /*
   * @brief Add elements
   *
   */
  
  void addSite(std::shared_ptr<SymbolListSite> site, bool checkPosition = false);
  
  void addSequence(std::shared_ptr<CoreSequence> sequence, bool checkName = true);

  void clear();

  void reindexSites();

};

} // end of namespace bpp

#endif  // _VECTOR_SYMBOLLIST_SITECONTAINER_H_
