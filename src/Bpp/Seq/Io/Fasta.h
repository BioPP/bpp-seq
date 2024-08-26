// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_FASTA_H
#define BPP_SEQ_IO_FASTA_H


#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "../Sequence.h"
#include "AbstractIAlignment.h"
#include "AbstractISequence.h"
#include "AbstractOSequence.h"
#include "ISequenceStream.h"
#include "OSequenceStream.h"
#include "SequenceFileIndex.h"

namespace bpp
{
/**
 * @brief The fasta sequence file format.
 *
 * Read and write from/to Fasta files.
 */
class Fasta :
	public AbstractISequence,
	public AbstractIAlignment,
	public AbstractOSequence2,
	public virtual ISequenceStream,
	public virtual OSequenceStream
{
protected:
/**
 * @brief The maximum number of chars to be written on a line.
 */
unsigned int charsByLine_;   // Number of char by line (output only)
bool checkNames_;            // If names must be checked in container
bool extended_;              // If using HUPO-PSI extensions
bool strictNames_;           // If name is between '>' and first space

public:
/**
 * @brief Build a new Fasta object.
 *
 * @param charsByLine Number of character per line when writing files.
 * @param checkSequenceNames  Tells if the names in the file should be checked for unicity (slower, in o(n*n) where n is the number of sequences).
 * @param extended Tells if we should read general comments and sequence comments in HUPO-PSI format.
 * @param strictSequenceNames Tells if the sequence names should be restricted to the characters between '>' and the first blank one.
 */
Fasta(unsigned int charsByLine = 100, bool checkSequenceNames = true, bool extended = false, bool strictSequenceNames = false) : charsByLine_(charsByLine), checkNames_(checkSequenceNames), extended_(extended), strictNames_(strictSequenceNames) {
}

// Class destructor
virtual ~Fasta() {
}

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
const std::string getFormatName() const override {
	return "FASTA file";
}

const std::string getFormatDescription() const override
{
	return "Sequence name (preceded by >) in one line, sequence content, no comments";
}
/** @} */

/**
 * @name The ISequenceStream interface.
 *
 * @{
 */
bool nextSequence(std::istream& input, Sequence& seq) const override;
/** @} */

/**
 * @name The OSequenceStream interface.
 *
 * @{
 */
void writeSequence(std::ostream& output, const Sequence& seq) const override;
/** @} */

/**
 * @return true if the names are to be checked when reading sequences from files.
 */
bool checkNames() const {
	return checkNames_;
}

/**
 * @brief Tell whether the sequence names should be checked when reading from files.
 *
 * @param yn whether the sequence names should be checked when reading from files.
 */
void checkNames(bool yn) {
	checkNames_ = yn;
}

/**
 * @return true if the sequence name is restricted to be between '>' and the first space character.
 */
bool strictNames() const {
	return strictNames_;
}

/**
 * @brief Tell wethed the sequence name should be restrected to the first non blank characters.
 *
 * @param yn whether the sequence names should be restrected.
 */
void strictNames(bool yn) {
	strictNames_ = yn;
}

/**
 * @brief The SequenceFileIndex class for Fasta format
 * @author Sylvain Gaillard
 */
class FileIndex : SequenceFileIndex
{
public:
FileIndex() : index_(), fileSize_(0) {
}
~FileIndex() {
}
void build(const std::string& path)
{
	build(path, false);
}
/**
 * @brief Constructor
 *
 * @param path The path to the file.
 * @param strictSequenceNames Tells if the sequence names should be restricted to the characters between '>' and the first blank one.
 */
void build(const std::string& path, const bool strictSequenceNames);
std::streampos getSequencePosition(const std::string& id) const;
size_t getNumberOfSequences() const
{
	return index_.size();
}
/**
 * @brief Read the index from a file
 */
void read(const std::string& path);
/**
 * @brief Write the index to a file
 */
void write(const std::string& path);
/**
 * @brief Get a sequence given its ID
 */
void getSequence(const std::string& seqid, Sequence& seq, const std::string& path) const;
void getSequence(const std::string& seqid, Sequence& seq, const std::string& path, const bool strictSequenceNames) const;

private:
std::map<std::string, std::streampos> index_;
std::streampos fileSize_;
};
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_FASTA_H
