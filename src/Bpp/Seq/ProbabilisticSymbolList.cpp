//
// File: ProbabilisticSymbolList.cpp
// Created by: Murray Patterson
// Created on: Mon Oct 5 2015
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

#include "ProbabilisticSymbolList.h"
#include "Alphabet/AlphabetTools.h"
#include <Bpp/Text/TextTools.h>

using namespace bpp;
using namespace std;

/****************************************************************************************/

BasicProbabilisticSymbolList::BasicProbabilisticSymbolList(const Alphabet * alpha) :
  alphabet_(alpha), content_(alpha->getResolvedChars().size(),0)
{
  content_.setRowNames(alphabet_->getResolvedChars());
}

BasicProbabilisticSymbolList::BasicProbabilisticSymbolList(const DataTable & list, const Alphabet * alpha) :
  alphabet_(alpha), content_(alpha->getResolvedChars().size(),0)
{
  content_.setRowNames(alpha->getResolvedChars());
  setContent(list);
}

/****************************************************************************************/

BasicProbabilisticSymbolList::BasicProbabilisticSymbolList(const ProbabilisticSymbolList & list) :
  alphabet_(list.getAlphabet()), content_(list.getContent()) {}

BasicProbabilisticSymbolList::BasicProbabilisticSymbolList(const BasicProbabilisticSymbolList & list) :
  alphabet_(list.alphabet_), content_(list.content_) {}

BasicProbabilisticSymbolList & BasicProbabilisticSymbolList::operator=(const ProbabilisticSymbolList & list)
{
  alphabet_ = list.getAlphabet();
  setContent(list.getContent());
  return * this;
}

BasicProbabilisticSymbolList & BasicProbabilisticSymbolList::operator=(const BasicProbabilisticSymbolList & list)
{
  alphabet_ = list.alphabet_;
  content_ = list.content_;
  return * this;
}

/****************************************************************************************/

void BasicProbabilisticSymbolList::setContent(const DataTable & list)
{

  // first, if table has column names, we ensure that this is
  // identical to the resolved characters the alphabet (even the
  // ordering must be the same).  Note: we ignore row names -- they
  // serve us no purpose here
  if(list.hasRowNames()) {

    // first we check if the column names has the same size as the
    // resolved characters of the alphabet.  Note: getRowNames
    // could throw a NoTableRowNamesException, but we don't try to
    // catch this because we did a check above for hasRowNames
    if(list.getRowNames().size() != alphabet_->getResolvedChars().size())
      throw DimensionException("BasicProbabilisticSymbolList::setContent. ", list.getRowNames().size(), alphabet_->getResolvedChars().size());

    // above check passes : they are of the same size, now we check if
    // they are identical
    std::vector<std::string> column_names = list.getRowNames();
    std::vector<std::string> resolved_chars = alphabet_->getResolvedChars();
    for(std::size_t i = 0; i < list.getRowNames().size(); ++i)
      if(column_names[i] != resolved_chars[i])
	throw Exception("BasicProbabilisticSymbolList::setContent. Row names / resolved characters of alphabet mismatch at " + TextTools::toString(column_names[i]) + " and " + TextTools::toString(resolved_chars[i]) + ".");
  }
  else { // DataTable has no column names

    // hence, we first check if width of DataTable is not larger than
    // the resolved characters of the alphabet
    if(list.getNumberOfRows() != alphabet_->getResolvedChars().size())
      throw DimensionException("BasicProabilisticSymbolList::setContent. ", list.getNumberOfRows(), alphabet_->getResolvedChars().size());
  }


  content_ = list; // final check passes, content_ becomes DataTable

  // now, we work with the columns of our DataTable, in the case that
  // it has no column names
  if(!list.hasRowNames()) {

    // we set the columns of DataTable to the resolved characters of
    // the alphabet ... this will work with, e.g., binary alphabets
    // and DNA alphabets.  Note: that setRowNames can throw both
    // DimensionException and DuplicatedTableRowNameException.
    // There should never be a DimensionException because we check
    // above for size.  The fact that Alphabet already disallows
    // duplicated characters ensures no
    // DuplicatedTableRowNameException
    content_.setRowNames(alphabet_->getResolvedChars());
  }
}

/****************************************************************************************/

void BasicProbabilisticSymbolList::setContent(const std::vector<std::vector<double> >& list)
{
  if (list.size()==0)
    return;

  // first we check if the column names has the same size as the
  // resolved characters of the alphabet.  Note: getRowNames
  // could throw a NoTableRowNamesException, but we don't try to
  // catch this because we did a check above for hasRowNames

  if(list[0].size() != alphabet_->getResolvedChars().size())
    throw DimensionException("BasicProbabilisticSymbolList::setContent. ", list[0].size(), alphabet_->getResolvedChars().size());

  content_ = list; // final check passes, content_ becomes DataTable
  content_.setRowNames(alphabet_->getResolvedChars());
}

/****************************************************************************************/

void BasicProbabilisticSymbolList::addElement(const std::vector<double> & element)
{
  // now we add this 'row', to the content DataTable, padding the end
  // with 0's should its length be smaller than the width of this DataTable
  if(element.size() < content_.getNumberOfRows()) {
    std::vector<double> padded_element(element);
    
    padded_element.resize(content_.getNumberOfRows(),0.);

    // Note that addColumn can throw both DimensionException and
    // TableColumnNamesException.  Above, we have controlled for all
    // possible DimensionException, so we need not check for this.
    // Since the construction of BasicProbabilisticSymbolList ensures
    // a DataTable with no row names, a TableColumnNamesException cannot
    // happen, so we need not check for this
    content_.addColumn(padded_element);
  }
  else {
    content_.addColumn(element);
  }
}

void  BasicProbabilisticSymbolList::addElement(size_t pos, const std::vector<double> & element)
{
  // now we add this 'row', to the content DataTable, padding the end
  // with 0's should its length be smaller than the width of this DataTable
  if(element.size() < content_.getNumberOfRows()) {
    std::vector<double> padded_element(element);
    
    padded_element.resize(content_.getNumberOfRows(),0.);

    // Note that addColumn can throw both DimensionException and
    // TableColumnNamesException.  Above, we have controlled for all
    // possible DimensionException, so we need not check for this.
    // Since the construction of BasicProbabilisticSymbolList ensures
    // a DataTable with no row names, a TableColumnNamesException cannot
    // happen, so we need not check for this
    content_.addColumn(padded_element, (int)pos);
  }
  else {
    content_.addColumn(element, (int)pos);
  }
}

void  BasicProbabilisticSymbolList::setElement(size_t pos, const std::vector<double> & element)
{
  // now we change this 'row', to the content DataTable, padding the end
  // with 0's should its length be smaller than the width of this DataTable
  if(element.size() < content_.getNumberOfRows()) {
    std::vector<double> padded_element(element);
    
    padded_element.resize(content_.getNumberOfRows(),0.);

    // Note that addColumn can throw both DimensionException and
    // TableColumnNamesException.  Above, we have controlled for all
    // possible DimensionException, so we need not check for this.
    // Since the construction of BasicProbabilisticSymbolList ensures
    // a DataTable with no row names, a TableColumnNamesException cannot
    // happen, so we need not check for this
    content_.setColumn(padded_element, pos);
  }
  else {
    content_.setColumn(element, pos);
  }
}


