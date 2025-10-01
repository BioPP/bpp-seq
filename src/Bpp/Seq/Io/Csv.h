// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_CSV_H
#define BPP_SEQ_IO_CSV_H


#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "../Sequence.h"
#include "AbstractIAlignment.h"
#include "AbstractISequence.h"
#include "AbstractOSequence.h"
#include "ISequenceStream.h"
#include "OSequenceStream.h"


namespace bpp
{
/**
 * @brief The Csv sequence file format.
 *
 * Read and write from/to coma separated values files : sequences are
 * set in columns, each position on a line separated by a tabulation
 * 
 */

  class Csv :
    public AbstractISequence,
    public AbstractIAlignment,
    public AbstractOSequence2,
    public virtual ISequenceStream,
    public virtual OSequenceStream
  {
  protected:
    /**
     * @brief The separator between columns
     */
    std::string sep_; // Separator of columns

    /**
     * @brief header   Tell if the first line must be used as column names,  default true.
     */

    bool header_;
    
     /**
     * @brief Use a column as rowNames. If positive, use the specified column to compute rownames, otherwise use default;
     */
    
    int rowNames_; // column number with rownames in input

  public:
    /**
     * @brief Build a new Csv object.
     *
     */

    Csv(const std::string sep = ",", bool header = true, int rowNames = -1) : sep_(sep), header_(header), rowNames_(rowNames) {}

    // Class destructor
    virtual ~Csv()
    {}

  public:
    /**
     * @name The AbstractISequence interface.
     *
     * @{
     */
    void appendSequencesFromStream(std::istream& input, SequenceContainerInterface& sc) const override;
    /** @} */

    /**
     * @name The AbstractIAlignment interface.
     *
     * @{
     */
    void appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& sc) const override
    {
      appendSequencesFromStream(input, sc); // This may raise an exception if sequences are not aligned!
    }
    /** @} */

    /**
     * @name The OSequence interface.
     *
     * @{
     */
    void writeSequences(std::ostream& output, const SequenceContainerInterface& sc) const override;

    void writeSequences(const std::string& path, const SequenceContainerInterface& sc, bool overwrite = true) const override
    {
      AbstractOSequence::writeSequences(path, sc, overwrite);
    }
    /** @} */

    /**
     * @name The IOSequence interface.
     *
     * @{
     */
    const std::string getFormatName() const override
    {
      return "CSV file";
    }

    const std::string getFormatDescription() const override
    {
      return "Sequence name as column names in one line, sites set per line, values tabulated separated.";
    }
    
    /** @} */

    /**
     * @name The ISequenceStream interface.
     *
     * @{
     */
    bool nextSequence(std::istream& input, Sequence& seq) const override
    {
      return true; // does nothing
    }
    
    /** @} */

    /**
     * @name The OSequenceStream interface.
     *
     * @{
     */
    void writeSequence(std::ostream& output, const Sequence& seq) const override {};
    
    /** @} */

  };
} // end of namespace bpp.
#endif // BPP_SEQ_IO_CSV_H
