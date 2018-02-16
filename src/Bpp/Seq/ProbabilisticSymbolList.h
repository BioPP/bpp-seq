//
// File: ProbabilisticSymbolList.h
// Created by: Murray Patterson
// Created on: Sun Oct 4 2015
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#ifndef _PROBABILISTICSYMBOLLIST_H_
#define _PROBABILISTICSYMBOLLIST_H_

#include "Alphabet/Alphabet.h"
#include <Bpp/Clonable.h>
#include <Bpp/Numeric/Table.h>

#include "CoreSymbolList.h"
#include "CruxSymbolListSite.h"

// From the STL :
#include <string>
#include <vector>

namespace bpp
{

  class ProbabilisticSymbolList:
    virtual public CoreSymbolList<std::vector<double> >
  {
  public:

    ProbabilisticSymbolList() 
    {
    }
  
    /**
     * @name The Colonable interface
     *
     * @{
     */
    ProbabilisticSymbolList * clone() const = 0;

    /**
     * @}
     */

    // class destructor
    virtual ~ProbabilisticSymbolList() {}
  
  public:
    typedef Table<double> DataTable;

    virtual const DataTable& getTable() const = 0;
  };

/**
 * @brief A basic ProbabilisticSymbolList object.
 *
 * This is a general purpose container, containing an ordered list of
 * elements.  The states represented by the elements is defined by an
 * alphabet object, which is passed to the list constructor by a
 * pointer.
 *
 * @see Alphabet
 */

class BasicProbabilisticSymbolList :
  public virtual ProbabilisticSymbolList
{
public :

  typedef Table<double> DataTable;

private :

  /**
   * @brief The Alphabet attribute must be initialized in the constructor and then can never be changed.
   *
   * To apply another alphabet to the list requires creating another
   * list.
   */
  const Alphabet * alphabet_;

protected :

  /**
   * @brief The list content.
   */
  DataTable content_;

public :

  /**
   * @brief Build a new void BasicProbabilisticSymbolList object with the specified alphabet.
   *
   * @param alpha the alphabet to use.
   */

  BasicProbabilisticSymbolList(const Alphabet * alpha);

  /**
   * @brief Build a new BasicProbabilisticSymbolList object with the specified alphabet.
   *
   * @param list The content of the site.
   * @param alpha The alphabet to use.
   * @throw If the content is internally inconsistent, or is inconsistent with the specified alphabet.
   */
  BasicProbabilisticSymbolList(const DataTable & list, const Alphabet * alpha);

  /**
   * @brief The generic copy constructor.
   */
  BasicProbabilisticSymbolList(const ProbabilisticSymbolList & list);

  BasicProbabilisticSymbolList(const CruxSymbolList& list);

  /**
   * @brief The copy constructor.
   */
  BasicProbabilisticSymbolList(const BasicProbabilisticSymbolList & list);

  /**
   * @brief The generic assignment operator.
   */

  BasicProbabilisticSymbolList & operator=(const ProbabilisticSymbolList & list);

  /**
   * @brief The assignement operator.
   */
  BasicProbabilisticSymbolList & operator=(const BasicProbabilisticSymbolList & list);

  /**
   * @name The Clonable interface
   *
   * @{
   */
  BasicProbabilisticSymbolList * clone() const { return new BasicProbabilisticSymbolList(* this); }

  /**
   * @}
   */

  // class destructor
  virtual ~BasicProbabilisticSymbolList() {}

public :

  const Alphabet * getAlphabet() const { return alphabet_; }
  
  size_t size() const { return static_cast<size_t>(content_.getNumberOfColumns()); }
  
  void setContent(const std::vector<std::vector<double> >& list);

  void setContent(const DataTable& list);

  /*
   * @brief String output, as a concatenate of:
   *  alphabet character(value)
   *
   * with letters ordered alphabetically. Value is written with
   * scientific format, and precision 8.
   *
   */
  
  std::string toString() const;

  void addElement(const std::vector<double> & element);

  void addElement(size_t pos, const std::vector<double> & element);

  void setElement(size_t pos, const std::vector<double> & element);

  const std::vector<double>& getElement(size_t pos) const
  {
    return content_.getColumn(pos);
  }

  virtual void deleteElement(size_t pos) { content_.deleteColumn(pos); }

  virtual void deleteElements(size_t pos, size_t len) 
  {
    content_.deleteRows(pos,len);
  }
  
  const std::vector<std::vector<double> >& getContent() const { return content_.getData(); }

  const DataTable& getTable() const { return content_; }

  const std::vector<double>& getValue(size_t pos) const
  {
    return getElement(pos);
  }

  const std::vector<double>& operator[](size_t i) const
  {
    return content_.getColumn(i);
  }

  std::vector<double>& operator[](size_t i)
  {
    return content_.getColumn(i);
  }

  double getStateValueAt(size_t siteIndex, int state) const
  {
    if (siteIndex >= content_.getNumberOfColumns())
      throw IndexOutOfBoundsException("ProbabilisticSymbolList::getStateValueAt.", siteIndex, 0, content_.getNumberOfColumns() - 1);
    return content_.getColumn(siteIndex)[getAlphabet()->getStateIndex(state)-1];
  }
    
  double operator()(size_t siteIndex, int state) const 
  {
    return content_.getColumn(siteIndex)[getAlphabet()->getStateIndex(state)-1];
  }
  
  void shuffle(){};
};

} // end of namespace bpp

#endif // _PROBABILISTICSYMBOLLIST_H
