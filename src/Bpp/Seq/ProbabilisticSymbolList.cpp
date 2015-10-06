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

using namespace bpp;
using namespace std;

/****************************************************************************************/

// Auxiliary methods used by several class methods

// get the set of states represented by the elements of this list, as
// defined by the alphabet object, which are the non-gap resolved
// characters of this alphabet
std::vector<std::string> const getRepresentedStates(const Alphabet * alpha)

// check to see if an element is internally consistent, i.e., that it
// is a list of decimal numbers that sums up to 1
void checkElement(const std::vector<std::string> & element) throw (Exception);

/****************************************************************************************/

BasicProbabilisticSymbolList::BasicProbabilisticSymbolList(const Alphabet * alpha) :
  alphabet_(alpha), content_(alpha->getSize())
{
  std::vector<std::string> states = getRepresentedStates(getAlphabet());
  content_.setColumnNames(states);
}

BasicProbabilisticSymbolList::BasicProbabilisticSymbolList(const DataTable & list, const Alphabet * alpha) throw (Exception) :
  alphabet_(alpha), content_(0)
{
  setContent(list);
}

/****************************************************************************************/

BasicProbabilisticSymbolList::BasicProbabilisticSymbolList(const ProbabilisticSymbolList & list) :
  alphabet_(list.getAlphabet()), content_(list.getContent()) {}

BasicProbabilisticSymbolList::BasicProbabilisticSymbolList(const BasicProbabilisticSymbolList & list) :
  alphabet_(list.alphabet_), content_(list.content_) {}

BasicProbabilisticSymbolList & operator=(const ProbabilisticSymbolList & list)
{
  alphabet_ = list.getAlphabet();
  setContent(list.getContent());
  return * this;
}

BasicProbabilisticSymbolList & operator=(const BasicProbabilisticSymbolList & list)
{
  alphabet_ = list.alphabet_;
  content_ = list.content_;
  return * this;
}

/****************************************************************************************/

void BasicProbabilisticSymbolList::setContent(const DataTable & list) throw (Exception)
{

  // first, if table has column names, we run these names against the
  // alphabet to see if they match.  Note: we ignore row names -- they
  // serve us no purpose here
  if(list.hasColumnNames()) {

    // loop through names in DataTable.  Note: getColumnNames could
    // throw a NoTableColumnNamesException, but we don't try to catch
    // this because we did a check above for hasColumnNames
    const std::vector<std::string> & a = list.getColumnNames();
    for(std::size_t i = 0; i < a.size(); ++i)
      if(!getAlphabet()->isCharInAlphabet(a[i]))
        throw BadCharException(a[i], "ProbabilisticSite::setData", getAlphabet());
  }
  else { // DataTable has no column names

    // hence, we first check if width of DataTable is not larger than
    // the alphabet size.  Note: getSize returns the number of
    // *resolved* characters (which is what we want)
    if(list.getNumberOfColumns() != getAlphabet()->getSize())
      throw DimensionException("ProabilisticSite::setData", list.getNumberOfColumns(), getAlphabet()->getSize());
  }

  // the above check passes (in either case), and so now we do a pass
  // over the table to ensure that each entry is internally consistent
  for(std::size_t i = 0; i < list.getNumberOfRows(); ++i) {
    try {
      checkElement(list.getRow(i));
    }
    catch(Exception e) {
      throw Exception(std::string("ProbabilisticSite::setData. ") + e.what());
    }
  }

  content_ = list; // final check passes, content_ becomes DataTable

  // now, we work with the columns of our DataTable, in the case that
  // it has no column names
  if(!list.hasColumnNames()) {

    // we associate the columns of DataTable with the (non-gap
    // resolved) characters of the alphabet ... this will work with,
    // e.g., binary alphabets and DNA alphabets
    std::vector<std::string> states = getRepresentedStates(getAlphabet());

    // set the names.  Note: that setColumnNames can throw both
    // DimensionException and DuplicatedTableColumnNameException.
    // There should never be a DimensionException because we check
    // above for size.  The fact that Alphabet already disallows
    // duplicated characters ensures no
    // DuplicatedTableColumnNameException
    content_.setColumnNames(states);
  }
}

/****************************************************************************************/

void BasicProbabilisticSymbolList::addElement(const std::vector<std::string> & element) throw (Exception)
{

  // first we check if the 'row' is not larger than the size of the
  // alphabet, hence the number of columns of content DataTable
  if(element.size() > getAlphabet()->getSize())
    throw DimensionException("ProabilisticSite::addElement. ", element.size(), getAlphabet()->getSize());

  // next, we check if element to add is internally consistent
  try {
    checkElement(element);
  }
  catch(Exception e) {
    throw Exception(std::string("ProbabilisticSite::addElement. ") + e.what());
  }

  // now we add this 'row', to the content DataTable, padding the end
  // with 0's should its length be smaller than the alphabet, hence
  // the number of columns in this content DataTable
  if(element.size() < getAlphabet->getSize()) {
    std::vector<std::string> padded_element(element);
    padded_element.resize(getAlphabet->getSize(),"0");

    // Note that addRow can throw both DimensionException and
    // TableRowNamesException.  Above, we have controlled for all
    // possible DimensionException, so we need not check for this.
    // Since the construction of BasicProbabilisticSymbolList ensures
    // a DataTable with no row names, a TableRowNamesExceptoin cannot
    // happen, so we need not check for this
    content_.addRow(padded_element);
  }
  else {
    content_.addRow(element);
  }
}

/****************************************************************************************/

// Auxiliary methods used by several class methods

std::vector<std::string> const getRepresentedStates(const Alphabet * alpha)
{

  std::vector<std::string> states;
  for(std::vector<std::string>::const_iterator s = alpha->getSupportedChars().begin(); s != alpha->getSupportedChars().end(); ++s) {
    // ensure that we take only the non-gap resolved characters
    if(!alpha->isGap(*s) && !alpha->isUnresolved(*s))
      states.push_back(*s);
  }

  return states;
}

void checkElement(const std::vector<std::string> & e) throw (Exception)
{
  double sum = 0;

  // take the sum (where toDouble ensures that v[s] is a decimal number)
  for(std::vector<std::string>::const_iterator s = e.begin(); s != e.end(); ++s)
    sum += TextTools::toDouble(*s,'.','E');

  // then we ensure that the sum is 1
  double tol = 0.000001; // but within a tolerance
  double lowerbound = double(1) - tol;
  double upperbound = double(1) + tol;
  if(sum < lowerbound or sum > upperbound)
    throw Exception("Probabilities do not sum up to 1");
}
