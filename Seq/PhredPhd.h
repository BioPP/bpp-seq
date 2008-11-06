//
// File: PhredPhd.h
// Created by: Sylvain Gaillard
// Created on: Wed Nov 5 2008
//

/*
Copyright or © or Copr. CNRS, (November 5, 2008)

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

#ifndef _PHREDPHD_H_
#define _PHREDPHD_H_

#include "AbstractISequence.h"
#include "Sequence.h"
#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"

namespace bpp {

  /**
   * @brief The phd sequence file format from phred software.
   *
   * This class read DNA sequence from phd files produced by the phred program
   * from the University of Washington.
   *
   * When the sequence is readed from the file, a sliding window is used to
   * estimate the quality of the sequence at each site. If the mean quality
   * is to bad, the site is set to 'undefined'.
   *
   * The sliding window is defined both at the left and the right of each site.
   *
   */
  class PhredPhd: public AbstractISequence {
    protected:
      double _quality;
      unsigned int _lframe;
      unsigned int _rframe;

    public:

      /**
       * @brief Build a new PhredPhd object.
       *
       * @param quality The mean quality threshold. The state is set to 'undefined' if
       * mean quality is smaller or equal to this value.
       * @param lframe The size of the sliding window at the left of the site.
       * @param rframe The size of the sliding winfow at the right of the site.
       */
      PhredPhd(double quality = 15, unsigned int lframe = 5, unsigned int rframe = 5);

      virtual ~PhredPhd() {}

    public:
      /**
       * @name The AbstractISequence interface.
       *
       * @{
       */
      void appendFromStream(istream & input, VectorSequenceContainer & sc) const throw (Exception);
      /** @} */

      /**
       * @name The IOSequence interface.
       *
       * @{
       */
      const string getFormatName() const { return "phd file"; };
      const string getFormatDescription() const {
        return "Sequences following the phd format as describe in the phred documentation.";
      }
      /** @} */
  };
} //end of namespace bpp

#endif // _PHREDPHD_H_
