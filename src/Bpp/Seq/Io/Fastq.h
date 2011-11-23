// 
// File:    Fastq.h
// Author:  Sylvain Gaillard
// Created: 22/11/2011 09:56:24
// 

/*
Copyright or © or Copr. Bio++ Development Team, (November 22, 2011)

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

#ifndef _BPP_SEQ_IO_FASTQ_H_
#define _BPP_SEQ_IO_FASTQ_H_

#include <string>
#include "ISequenceStream.h"
#include "OSequenceStream.h"
#include "../Sequence.h"
#include "../SequenceWithQuality.h"

namespace bpp
{
  /**
   * @brief The fastq sequence file format.
   *
   * @author Sylvain Gaillard
   */
  class Fastq:
    public virtual ISequenceStream,
    public virtual OSequenceStream
  {
    private:
      bool repeatName_;

    public:
      /**
       * @brief Build a new Fastq object.
       *
       * @param repName Tell if the names in the file is repeated (tested
       * on input) or must be repeated (for output).
       */
      Fastq(bool repName = false): repeatName_(repName) {}

      // Class destructor
      virtual ~Fastq() {}

    public:
      /**
       * @name The IOSequence interface.
       *
       * @{
       */
      const std::string getFormatName() const { return "FASTQ file"; }
      const std::string getFormatDescription() const {
        return "Sequence with quality";
      }
      const std::string getDataType() const { return "Sequence with quality"; }
      /** @} */
      bool repeatName() const { return repeatName_; }
      void repeatName(bool yn) { repeatName_ = yn; }

      /**
       * @name The ISequenceStream interface.
       *
       * @{
       */
      /**
       * @copydoc ISequenceStream::nextSequence(std::istream& input, Sequence& seq) const
       * @author Sylvain Gaillard
       *
       * @par Usage
       *
       * @code
       * // Creating a SequenceWithQuality object
       * DNA alpha;
       * SequenceWithQuality seq(&alpha);
       * 
       * // Create a FastQ parser
       * Fastq fq;
       *
       * // Opening the file
       * std::ifstream in("reads.fastq");
       *
       * // Read the sequences
       * while (fq.nextSequence(in, seq)) {
       *   // ... do something with the sequence ...
       * }
       *
       * // Close the file
       * in.close();
       * @endcode
       */
      bool nextSequence(std::istream& input, bpp::Sequence& seq) const throw (Exception);
      /** @} */

      /**
       * @name The OSequenceStream interface.
       *
       * @{
       */
      /**
       * @copydoc ISequenceStream::nextSequence(std::istream& input, Sequence& seq) const
       * @author Sylvain Gaillard
       */
      void writeSequence(std::ostream& output, const bpp::Sequence& seq) const throw (Exception);
      /** @} */
  };
}

#endif // _BPP_SEQ_IO_FASTQ_H_
