//
// File: VectorSiteContainer.h
// Created by: Julien Dutheil
// Created on: Mon Oct  6 11:50:40 2003
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

#ifndef _VECTORSITECONTAINER_H_
#define _VECTORSITECONTAINER_H_

#include "Site.h"
#include "SiteContainer.h"
#include "AbstractSequenceContainer.h"
#include "AlignedSequenceContainer.h"
#include "OrderedSequenceContainer.h"

// From the NumCalc library:
#include <NumCalc/VectorTools.h>

// From the STL library:
#include <string>
#include <vector>
#include <iostream>

using namespace std;

namespace bpp
{

/**
 * @brief The VectorSiteContainer class.
 *
 * Sites are stored in a vector of pointers.
 * Site access is hence in \f$O(1)\f$, and sequence access in \f$O(l)\f$, where
 * \f$l\f$ is the number of sites in the container.
 *
 * See AlignedSequenceContainer for an alternative implementation.
 * 
 * @see Sequence, Site, AlignedSequenceContainer
 */
class VectorSiteContainer: 
  public AbstractSequenceContainer,   //This container implements the SequenceContainer interface
                                      //and use the AbstractSequenceContainer adapter.
  public virtual SiteContainer        //This container is a SiteContainer.
{
  protected:
    vector<Site     *> _sites;
    vector<string   *> _names;
    vector<Comments *> _comments; //Sequences comments.
    mutable vector<Sequence *> _sequences; //To store pointer toward sequences retrieves (cf. AlignedSequenceContainer).
  
  public:
    /**
     * @brief Build a new container from a set of sites.
     *
     * @param vs A vector of sites.
     * @param alpha The common alphabet for all sites.
     * @param checkPositions Check for the redundancy of site position tag. This may turn to be very time consuming!
     * @throw Exception If sites differ in size or in alphabet.
     */
    VectorSiteContainer(const vector<const Site *> & vs, const Alphabet * alpha, bool checkPositions = true) throw (Exception);
    /**
     * @brief Build a new empty container with specified size.
     *
     * @param size Number of sequences in the container.
     * @param alpha The alphabet for this container.
     */
    VectorSiteContainer(unsigned int size, const Alphabet * alpha);
    /**
     * @brief Build a new empty container with specified sequence names.
     *
     * @param names Sequence names. This will set the number of sequences in the container.
     * @param alpha The alphabet for this container.
     */
    VectorSiteContainer(const vector<string> & names, const Alphabet * alpha);

    /**
     * @brief Build a new empty container.
     *
     * @param alpha The alphabet for this container.
     */
    VectorSiteContainer(const Alphabet * alpha);

    VectorSiteContainer(const      VectorSiteContainer & vsc);
    VectorSiteContainer(const            SiteContainer &  sc);
    VectorSiteContainer(const OrderedSequenceContainer & osc);
    VectorSiteContainer(const        SequenceContainer &  sc);

    VectorSiteContainer& operator = (const      VectorSiteContainer & vsc);
    VectorSiteContainer& operator = (const            SiteContainer &  sc);
    VectorSiteContainer& operator = (const OrderedSequenceContainer & osc);
    VectorSiteContainer& operator = (const        SequenceContainer &  sc);

    virtual ~VectorSiteContainer() { clear(); }

  public:

    /**
     * @name The Clonable interface.
     *
     * @{
     */
    VectorSiteContainer * clone() const { return new VectorSiteContainer(*this); }
    /** @} */

    /**
     * @name The SiteContainer interface implementation:
     *
     * @{
     */
    const Site * getSite(unsigned int siteIndex) const throw (IndexOutOfBoundsException);
    void         setSite(unsigned int siteIndex, const Site & site, bool checkPosition = true) throw (Exception);
    Site *    removeSite(unsigned int siteIndex) throw (IndexOutOfBoundsException);
    void      deleteSite(unsigned int siteIndex) throw (IndexOutOfBoundsException);
    void         addSite(const Site & site,                         bool checkPosition = true) throw (Exception);
    void         addSite(const Site & site, unsigned int siteIndex, bool checkPosition = true) throw (Exception);
    unsigned int getNumberOfSites() const;
    const vector<int> getPositions() const;
    void reindexSites();
    Vint getSitePositions() const;
    /** @} */

    //Theses methods are implemented for this class:
    
    /**
     * @name The SequenceContainer interface.
     *
     * @{
     */
    void setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException);

    // Method to get a sequence object from sequence container
    const Sequence * getSequence(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
    const Sequence * getSequence(const string & name       ) const throw (SequenceNotFoundException);

    // Methods to get position of a sequence in sequence container from his name
    // This method is used by delete and remove methods
    unsigned int getSequencePosition(const string & name) const throw (SequenceNotFoundException);

    Sequence * removeSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
    Sequence * removeSequence(const string & name       ) throw (SequenceNotFoundException);

    void deleteSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
    void deleteSequence(const string & name       ) throw (SequenceNotFoundException);
    
    unsigned int getNumberOfSequences() const;

    vector<string> getSequencesNames() const;

    void setSequencesNames(const vector<string> & names, bool checkNames = true) throw (Exception);

    void clear();  
    
    SequenceContainer * createEmptyContainer() const;

    int & valueAt(const string & sequenceName, unsigned int elementIndex) throw (SequenceNotFoundException, IndexOutOfBoundsException)
    {
      if(elementIndex >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(string, unsigned int).", elementIndex, 0, getNumberOfSites() - 1);
      return (* _sites[elementIndex])[getSequencePosition(sequenceName)];
    }
    const int & valueAt(const string & sequenceName, unsigned int elementIndex) const throw (SequenceNotFoundException, IndexOutOfBoundsException)
    {
      if(elementIndex >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(string, unsigned int).", elementIndex, 0, getNumberOfSites() - 1);
      return (* _sites[elementIndex])[getSequencePosition(sequenceName)];
    }
    int & operator()(const string & sequenceName, unsigned int elementIndex)
    {
      return (* _sites[elementIndex])[getSequencePosition(sequenceName)];
    }
    const int & operator()(const string & sequenceName, unsigned int elementIndex) const
    {
      return (* _sites[elementIndex])[getSequencePosition(sequenceName)];
    }

    int & valueAt(unsigned int sequenceIndex, unsigned int elementIndex) throw (IndexOutOfBoundsException)
    {
      if(sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(unsigned int, unsigned int).", sequenceIndex, 0, getNumberOfSequences() - 1);          
      if(elementIndex  >= getNumberOfSites())     throw IndexOutOfBoundsException("VectorSiteContainer::operator(unsigned int, unsigned int).", elementIndex, 0, getNumberOfSites() - 1);
      return (* _sites[elementIndex])[sequenceIndex];
    }
    const int & valueAt(unsigned int sequenceIndex, unsigned int elementIndex) const throw (IndexOutOfBoundsException)
    {
      if(sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(unsigned int, unsigned int).", sequenceIndex, 0, getNumberOfSequences() - 1);          
      if(elementIndex  >= getNumberOfSites())     throw IndexOutOfBoundsException("VectorSiteContainer::operator(unsigned int, unsigned int).", elementIndex, 0, getNumberOfSites() - 1);
      return (* _sites[elementIndex])[sequenceIndex];
    }
    int & operator()(unsigned int sequenceIndex, unsigned int elementIndex)
    {
      return (* _sites[elementIndex])[sequenceIndex];
    }
    const int & operator()(unsigned int sequenceIndex, unsigned int elementIndex) const
    {
      return (* _sites[elementIndex])[sequenceIndex];
    }
     /** @} */

    void addSequence(const Sequence & sequence,                             bool checkName = true) throw (Exception);
    void addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName = true) throw (Exception);
    
    void setSequence(const string & name,        const Sequence & sequence, bool checkName) throw (Exception);
    void setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName) throw (Exception);


  protected:
    // Create n void sites:
    void realloc(unsigned int n);
};

} //end of namespace bpp.

#endif  //_VECTORSITECONTAINER_H_

