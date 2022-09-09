//
// File: Pasta.h
// Authors:
//   Murray Patterson
// Created: 2015-10-20 00:00:00
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

#ifndef BPP_SEQ_IO_PASTA_H
#define BPP_SEQ_IO_PASTA_H

#include <Bpp/Numeric/Table.h>

#include "../Container/VectorSiteContainer.h"
#include "../ProbabilisticSequence.h"
#include "../Container/AlignmentData.h"
#include "AbstractIAlignment.h"
#include "AbstractISequence.h"
#include "AbstractOAlignment.h"
#include "AbstractOSequence.h"

namespace bpp
{
/**
 * @brief The Pasta sequence file format.
 *
 * Read and write from/to Pasta files -- a format that is more general
 * than the Fasta format : while the Fasta format contains sequence
 * information in the form of character states at each site of the
 * sequence, the Pasta format contains sequence information in the
 * form of probability of presence of each character state at each
 * site.  See implementation of methods below for more details
 */
class Pasta :
  public AbstractIProbabilisticAlignment,
  public AbstractOProbabilisticAlignment,
  public AbstractOSequence2
{
protected:
  /**
   * @brief The maximum number of chars to be written on a line.
   */
  unsigned int charsByLine_; // Number of chars by line (output only)

  bool extended_;            // If using HUPO-PSI extensions
  bool strictNames_;         // If name is between '>' and first space

public:
  typedef Table<double> DataTable;

  /**
   * @brief Build a new Pasta object.
   *
   * @param charsByLine Number of characters per line when writing files.
   * @param extended Tell if we should read general comments and sequence comments in HUPO-PSI format.
   * @param strictSequenceNames Tells if the sequence names should be restricted to the characters between '>' and the first blank one.
   */
  Pasta(unsigned int charsByLine = 100, bool extended = false, bool strictSequenceNames = false) : charsByLine_(charsByLine), extended_(extended), strictNames_(strictSequenceNames) {}

  // class destructor
  virtual ~Pasta() {}

public:
  /**
   * @brief Get the format name
   *
   * @return format name
   */
  const std::string getFormatName() const override { return "PASTA file"; }
  const std::string getFormatDescription() const override
  {
    return "By rows: alphabet, then Sequence name (preceded by >) in one line, and rows of sequence content.";
  }

  /**
   * @name The "ISequenceStream interface"
   *
   * @{
   */
  bool nextSequence(std::istream& input, ProbabilisticSequence& seq, bool hasLabels, const std::vector<size_t>& permutationMap) const;

  /**
   * @}
   */

  void writeSequence(std::ostream& output, const ProbabilisticSequence& seq, bool header) const;

  void writeSequence(std::ostream& output, const Sequence& seq, bool header) const;

  /**
   * @name The OSequence interface"
   *
   * @{
   */
   void writeSequences(std::ostream& output, const SequenceContainerInterface& sc) const override
   {
     for (size_t i = 0; i < sc.getNumberOfSequences(); ++i) {
       writeSequence(output, sc.getSequence(i), i == 0);
     }
   }
   
   using AbstractOSequence::writeSequences;

  /**
   * @}
   */

   /**
   * @name The "I/OProbabilisticSequence interface"
   *
   * @{
   */
  void appendAlignmentFromStream(std::istream& input, ProbabilisticSequenceContainerInterface& psc) const override;

  void writeAlignment(std::ostream& output, const ProbabilisticSiteContainerInterface& psc) const override
  {
    if (!output)
      throw IOException("Pasta::write: can't write to ostream output");
  
    // Main loop : for all sequences in vector container
  
    bool first = true;
    for (size_t i = 0; i < psc.getNumberOfSequences(); ++i)
    {
      writeSequence(output, psc.getSequence(i), first);
      first = false;
    }
  }
  /**
   * @}
   */
  using AbstractOSequence2::writeAlignment;
  
  const std::string getDataType() const override { return "(Probabilistic) sequence container"; }

};
} // end of namespace bpp
#endif // BPP_SEQ_IO_PASTA_H
