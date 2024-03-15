// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

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
       writeSequence(output, sc.sequence(i), i == 0);
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
      writeSequence(output, psc.sequence(i), first);
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
