//
// File AlignedSequenceContainer.h
// Created by: Guillaume Deuchst
// //             Julien Dutheil
// Last modification : Friday August 22 2003
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

#ifndef _ALIGNEDSEQUENCECONTAINER_H_
#define _ALIGNEDSEQUENCECONTAINER_H_

#include "Site.h"
#include "SiteContainer.h"
#include "OrderedSequenceContainer.h"
#include "VectorSequenceContainer.h"

// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <string>
using namespace std;

namespace bpp
{

/**
 * @brief Aligned sequences container.
 *
 * This class inherits from the VectorSequenceContainer and add site access.
 * Sequence addition methods are re-defined to check for sequence lengths.
 * Sequence access is in \f$O(1)\f$, and site access in \f$O(n)\f$, where
 * \f$n\f$ is the number of sequences in the container.
 *
 * See VectorSiteContainer for an alternative implementation.
 * 
 * @see VectorSequenceContainer, Sequence, Site, VectorSiteContainer
 */
class AlignedSequenceContainer:
  public virtual VectorSequenceContainer,
  public virtual SiteContainer
{
  protected:
    // Integer vector that contains sites's positions
    vector<int> positions;
    
    unsigned int length; // Number of sites for verifications before sequence's insertion in sequence container
  
    /**
     * This is used in order to implement the SiteContainer interface.
     * A SiteContainer is expected to work on Site objects, but this class
     * -- since it is a VectorSequenceContainer -- has its data sored as
     * Sequence object. When the SiteContainer method getSite() is invoked
     * it creates a new Site object and send the address of it.
     * To avoid memory leaks, this object is put into a vector so that it can be
     * destroyed when the container is destroyed.
     */
    mutable vector<Site *> sites;
    
  public:
    /**
     * @brief Build a new empty container with the specified alphabet.
     *
     * @param alpha The alphabet to use.
     */
    AlignedSequenceContainer(const Alphabet * alpha);
    /**
     * @brief Copy constructor.
     *
     * @param asc The container to copy.
     */
    AlignedSequenceContainer(const AlignedSequenceContainer & asc);
    /**
     * @brief Convert any SiteContainer object into a AlignedSequenceContainer object.
     *
     * @param sc The container to copy.
     */
    AlignedSequenceContainer(const SiteContainer &  sc);
    /**
     * @brief Try to coerce an OrderedSequenceContainer object into an AlignedSequenceContainer object.
     *
     * Sequences in osc will be considered alligned, and have the same number of sites.
     * 
     * @param osc The ordered container to coerce.
     * @throw SequenceNotAlignedException If sequences in osc do not have the same length.
     */
    AlignedSequenceContainer(const OrderedSequenceContainer & osc) throw (SequenceNotAlignedException);

    AlignedSequenceContainer & operator = (const AlignedSequenceContainer & asc);
    AlignedSequenceContainer & operator = (const            SiteContainer &  sc);
    AlignedSequenceContainer & operator = (const OrderedSequenceContainer & osc) throw (SequenceNotAlignedException);

    virtual ~AlignedSequenceContainer();

  public:

    /**
     * @name The Clonable interface.
     *
     * @{
     */
    AlignedSequenceContainer * clone() const { return new AlignedSequenceContainer(*this); }
    /** @} */

    /**
     * @name The SiteContainer interface implementation:
     *
     * @{
     */
    const Site * getSite(unsigned int siteIndex) const throw (IndexOutOfBoundsException) ;
    void         setSite(unsigned int siteIndex, const Site & site, bool checkPosition = true) throw (Exception) ;
    Site *    removeSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) ;
    void      deleteSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) ;
    void addSite(const Site & site, bool checkPosition = true) throw (Exception);
    void addSite(const Site & site, unsigned int siteIndex, bool checkPosition = true) throw (Exception);
    unsigned int getNumberOfSites() const;
    Vint getSitePositions() const;
    void reindexSites();
    void clear();
    SequenceContainer * createEmptyContainer() const;
    /** @} */

    /**
     * @name Redefinition of VectorSequenceContainer methods, to check for sequence lengths.
     *
     * @{
     */
    void setSequence(const string & name, const Sequence & sequence, bool checkName = true) throw (Exception);
    void setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName = true) throw (Exception);

    void addSequence(const Sequence & sequence, bool checkName = true) throw (Exception);
    void addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName = true) throw (Exception);
    /** @} */
    
  
  protected:
    /**
     * @brief Check sequence's size before insertion in sequence container.
     * 
     * @param sequence The sequence to check.
     * @return True if sequence length = number of sites in container.
     */
    bool checkSize(const Sequence & sequence);
};

} //end of namespace bpp.

#endif // _ALIGNEDSEQUENCECONTAINER_H_

