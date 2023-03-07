//
// File: ProbabilisticSymbolList.h
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

#ifndef BPP_SEQ_PROBABILISTICSYMBOLLIST_H
#define BPP_SEQ_PROBABILISTICSYMBOLLIST_H

#include <Bpp/Clonable.h>
#include <Bpp/Numeric/Table.h>

#include "Alphabet/Alphabet.h"
#include "CoreSymbolList.h"

// From the STL :
#include <string>
#include <vector>

namespace bpp
{
/**
 * @brief The ProbabilisticSymbolList interface.
 *
 * This container aims at containing an ordered list of
 * probabilistic sites.
 * It does not inherit from * SympbolList<std::vector<double>>
 * because it does not contain a * vector of sites but a Table of
 * doubles.
 *
 */
class ProbabilisticSymbolListInterface :
  public virtual ProbabilisticCoreSymbolListInterface  
{
public:
  typedef std::vector<double> SymbolType;
  typedef Table<double> DTable;
  
public:
  ProbabilisticSymbolListInterface() {}

  // class destructor
  virtual ~ProbabilisticSymbolListInterface() {}

public:

  virtual const DTable& getTable() const = 0;
  
  using ProbabilisticCoreSymbolListInterface::setContent;
  
  virtual void setContent(const DTable& list) = 0;
};

/**
 * @brief ProbabilisticSymbolList object.
 *
 * This is a general purpose container, containing an ordered list of
 * elements.  The states represented by the elements are defined by an
 * alphabet object, which is passed to the list constructor by a
 * pointer.
 *
 * @see Alphabet
 */
class ProbabilisticSymbolList :
  public virtual ProbabilisticSymbolListInterface
{

private:
  /**
   * @brief The Alphabet attribute must be initialized in the constructor and then can never be changed.
   *
   * To apply another alphabet to the list requires creating another
   * list.
   */
  std::shared_ptr<const Alphabet> alphabet_;

protected:
  /**
   * @brief The list content.
   */
  DTable content_;

public:
  /**
   * @brief Build a new void ProbabilisticSymbolList object with the specified alphabet.
   *
   * @param alpha the alphabet to use.
   */
  ProbabilisticSymbolList(std::shared_ptr<const Alphabet>& alpha);

  /**
   * @brief Build a new ProbabilisticSymbolList object with the specified alphabet and contant as a DataTable.
   *
   * @param list The content of the site.
   * @param alpha The alphabet to use.
   * @throw If the content is internally inconsistent, or is inconsistent with the specified alphabet.
   */
  ProbabilisticSymbolList(const DTable& list, std::shared_ptr<const Alphabet>& alpha);

  /**
   * @brief Build a new ProbabilisticSymbolList object with the specified alphabet and contant as a VVdouble.
   *
   * @param list The content of the site.
   * @param alpha The alphabet to use.
   * @throw If the content is internally inconsistent, or is inconsistent with the specified alphabet.
   */
  ProbabilisticSymbolList(const std::vector< std::vector<double> >& list, std::shared_ptr<const Alphabet>& alpha);

  /**
   * @brief The generic copy constructor.
   */
  ProbabilisticSymbolList(const ProbabilisticSymbolListInterface& list);

  ProbabilisticSymbolList(const CruxSymbolListInterface& list);

  /**
   * @brief The copy constructor.
   */
  ProbabilisticSymbolList(const ProbabilisticSymbolList& list);

  /**
   * @brief The generic assignment operator.
   */
  ProbabilisticSymbolList& operator=(const ProbabilisticSymbolListInterface& list);

  /**
   * @brief The assignement operator.
   */
  ProbabilisticSymbolList& operator=(const ProbabilisticSymbolList& list);

  /**
   * @name The Clonable interface
   *
   * @{
   */
  ProbabilisticSymbolList* clone() const override { return new ProbabilisticSymbolList(*this); }

  /**
   * @}
   */

  // class destructor
  virtual ~ProbabilisticSymbolList() {}

public:
  std::shared_ptr<const Alphabet> getAlphabet() const override { return alphabet_; }
  
  const Alphabet& alphabet() const override { return *alphabet_; }

  size_t size() const override { return static_cast<size_t>(content_.getNumberOfColumns()); }

  void setContent(const std::vector<std::vector<double> >& list) override;

  void setContent(const DTable& list) override;

  /*
   * @brief String output, as a concatenate of:
   *  alphabet character(value)
   *
   * with letters ordered alphabetically. Value is written with
   * scientific format, and precision 8.
   */
  std::string toString() const override;

  void addElement(const std::vector<double>& element) override;

  void addElement(size_t pos, const std::vector<double>& element) override;

  void setElement(size_t pos, const std::vector<double>& element) override;

  const std::vector<double>& getElement(size_t pos) const override
  {
    return content_.getColumn(pos);
  }

  virtual void deleteElement(size_t pos) override { content_.deleteColumn(pos); }

  virtual void deleteElements(size_t pos, size_t len) override
  {
    content_.deleteRows(pos, len);
  }

  const std::vector<std::vector<double> >& getContent() const override
  {
    return content_.getData();
  }

  const DTable& getTable() const override
  {
    return content_;
  }

  const std::vector<double>& getValue(size_t pos) const override
  {
    return getElement(pos);
  }

  const std::vector<double>& operator[](size_t pos) const override
  {
    return content_.getColumn(pos);
  }

  std::vector<double>& operator[](size_t pos) override
  {
    return content_.getColumn(pos);
  }

  double getStateValueAt(size_t siteIndex, int state) const override
  {
    if (siteIndex >= content_.getNumberOfColumns())
      throw IndexOutOfBoundsException("ProbabilisticSymbolList::getStateValueAt.", siteIndex, 0, content_.getNumberOfColumns() - 1);
    return content_.getColumn(siteIndex)[getAlphabet()->getStateIndex(state) - 1];
  }

  double operator()(size_t siteIndex, int state) const override
  {
    return content_.getColumn(siteIndex)[getAlphabet()->getStateIndex(state) - 1];
  }

  void shuffle() override
  {
    throw Exception("ProbabilisticSymbolList::shuffle not implemented, yet.");
  }
};
} // end of namespace bpp
#endif // BPP_SEQ_PROBABILISTICSYMBOLLIST_H
