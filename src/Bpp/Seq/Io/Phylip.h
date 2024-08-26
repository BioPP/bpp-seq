// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_PHYLIP_H
#define BPP_SEQ_IO_PHYLIP_H


#include "../Container/AlignedSequenceContainer.h"
#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "../Sequence.h"
#include "AbstractIAlignment.h"
#include "AbstractOAlignment.h"

// From the STL:
#include <iostream>

namespace bpp
{
/**
 * @brief The Phylip & co format.
 *
 * Sequences must be aligned.
 * This format is described on the Phylip package documentation website:
 * http://evolution.genetics.washington.edu/phylip/doc/sequence.html
 */
class Phylip :
	public AbstractIAlignment2,
	public AbstractOAlignment
{
private:
/* this class allows two kinds of Phylip format:
 * traditional, with names limited to 10 chars,
 * and 'extended', defined by PAML, with names separated from sequences by at least 6 white spaces.
 */
bool extended_;
/* tells if sequences are in the seuqential or the interleave format/
 */
bool sequential_;

/**
 * @brief The maximum number of chars to be written on a line.
 */
unsigned int charsByLine_;

std::string namesSplit_;

public:
/**
 * @brief Build a new Phylip file reader.
 *
 * @param extended If true, sequences with names longer than 10 characters are allowed.
 * @param sequential If false, sequences are supposed to be interlaved.
 * @param charsByLine The number of base to display in a row.
 * @param split The string to use to split sequence name from content (only for 'extended' format). This will typically be "  " (two spaces) or "\t" (a tabulation).
 */
Phylip(bool extended = true, bool sequential = true, unsigned int charsByLine = 100, const std::string& split = "  ") :
	extended_(extended), sequential_(sequential), charsByLine_(charsByLine), namesSplit_(split) {
}

virtual ~Phylip() {
}

public:
/**
 * @name The AbstractIAlignment2 interface.
 *
 * @{
 */
void appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& sc) const override;
/** @} */

/**
 * @name The AbstractISequence interface.
 *
 * As a SiteContainer is a subclass of SequenceContainer, we hereby implement the ISequence
 * interface by downcasting the interface.
 *
 * @{
 */
virtual std::unique_ptr<SequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
{
	return readAlignment(input, alpha);
}
virtual std::unique_ptr<SequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
{
	return readAlignment(path, alpha);
}
/** @} */

/**
 * @return The number of sequences contained in the specified file.
 *
 * This methods parses the first line of the phylip file.
 * @param path The path of the file to parse.
 */
unsigned int getNumberOfSequences(const std::string& path) const;

/**
 * @name The AbstractOSequence interface.
 *
 * @{
 */
void writeAlignment(std::ostream& output, const SiteContainerInterface& sc) const override;
void writeAlignment(const std::string& path, const SiteContainerInterface& sc, bool overwrite) const override
{
	AbstractOAlignment::writeAlignment(path, sc, overwrite);
}
/** @} */


/**
 * @name The IOSequence interface.
 *
 * @{
 */
const std::string getFormatName() const override;
const std::string getFormatDescription() const override;
/** @} */

/**
 * @return The string used to split sequence name from content.
 */
const std::string& getSplit() const {
	return namesSplit_;
}

/**
 * @param split The string to be used to split sequence name from content.
 */
void setSplit(const std::string& split) {
	namesSplit_ = split;
}

protected:
// Reading tools:
const std::vector<std::string> splitNameAndSequence(const std::string& s) const;
void readSequential (std::istream& in, SequenceContainerInterface & asc) const;
void readInterleaved(std::istream& in, SequenceContainerInterface & asc) const;
// Writing tools:
std::vector<std::string> getSizedNames(const std::vector<std::string>& names) const;
void writeSequential(std::ostream& out, const SiteContainerInterface& sc) const;
void writeInterleaved(std::ostream& out, const SiteContainerInterface& sc) const;
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_PHYLIP_H
