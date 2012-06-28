//
// File: IterationListener.h
// Authors: Julien Dutheil
// Created: Wed Jun 27 2012
//

/*
Copyright or © or Copr. Bio++ Development Team, (2012)

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

#ifndef _ITERATIONLISTENER_H_
#define _ITERATIONLISTENER_H_

#include "MafIterator.h"

namespace bpp {

/**
 * @brief Listneer which enable to catch events when parsing a Maf file.
 */
class IterationListener
{
  public:
    virtual ~IterationListener() {}

  public:
    virtual void iterationStarts() = 0;
    virtual void iterationMoves(const MafBlock& currentBlock) = 0;
    virtual void iterationStops() = 0;
};

/**
 * @brief Iteration listener that works with a SequenceStatisticsMafIterator,
 * enabling output of results in a file (partial implementation, format-independent)
 */
class AbstractStatisticsOutputIterationListener:
  public virtual IterationListener
{
  protected:
    SequenceStatisticsMafIterator* statsIterator_;


  public:
    AbstractStatisticsOutputIterationListener(SequenceStatisticsMafIterator* iterator):
      statsIterator_(iterator) {}
    
    AbstractStatisticsOutputIterationListener(const AbstractStatisticsOutputIterationListener& listener):
      statsIterator_(listener.statsIterator_) {}
    
    AbstractStatisticsOutputIterationListener& operator=(const AbstractStatisticsOutputIterationListener& listener)
    {
      statsIterator_ = listener.statsIterator_;
      return *this;
    }

    virtual ~AbstractStatisticsOutputIterationListener() {}

};

/**
 * @brief Iteration listener that works with a SequenceStatisticsMafIterator,
 * enabling output of results in a file in CSv format
 */
class CsvStatisticsOutputIterationListener:
  public AbstractStatisticsOutputIterationListener
{
  private:
    OutputStream* output_;
    std::string sep_;
    std::string refSpecies_;

  public:
    CsvStatisticsOutputIterationListener(SequenceStatisticsMafIterator* iterator, const std::string& refSpecies, OutputStream* output, const std::string& sep = "\t"):
      AbstractStatisticsOutputIterationListener(iterator), output_(output), sep_(sep), refSpecies_(refSpecies) {}
    
    CsvStatisticsOutputIterationListener(const CsvStatisticsOutputIterationListener& listener):
      AbstractStatisticsOutputIterationListener(listener), output_(listener.output_), sep_(listener.sep_), refSpecies_(listener.refSpecies_) {}
    
    CsvStatisticsOutputIterationListener& operator=(const CsvStatisticsOutputIterationListener& listener)
    {
      AbstractStatisticsOutputIterationListener::operator=(listener);
      output_ = listener.output_;
      sep_ = listener.sep_;
      refSpecies_ = listener.refSpecies_;
      return *this;
    }

    virtual ~CsvStatisticsOutputIterationListener() {}

  public:
    virtual void iterationStarts();
    virtual void iterationMoves(const MafBlock& currentBlock);
    virtual void iterationStops() {}
  
};

} //end of namespace bpp.

#endif //_ITERATIONLISTENER_H_

