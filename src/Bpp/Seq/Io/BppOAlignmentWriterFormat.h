// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_BPPOALIGNMENTWRITERFORMAT_H
#define BPP_SEQ_IO_BPPOALIGNMENTWRITERFORMAT_H


#include "IoSequenceFactory.h"

namespace bpp
{
/**
 * @brief Sequence I/O in BppO format.
 *
 * Creates a new OAlignment object according to
 * distribution description syntax (see the Bio++ Program Suite
 * manual for a detailed description of this syntax).
 *
 */
class BppOAlignmentWriterFormat :
	public virtual IOFormat
{
private:
std::map<std::string, std::string> unparsedArguments_;
int warningLevel_;

public:
BppOAlignmentWriterFormat(int warningLevel) :
	unparsedArguments_(),
	warningLevel_(warningLevel) {
}

virtual ~BppOAlignmentWriterFormat() {
}

public:
const std::string getFormatName() const {
	return "BppO";
}

const std::string getFormatDescription() const {
	return "Bpp Options format.";
}

const std::string getDataType() const {
	return "Alignment writer";
}

/**
 * @brief Read a OAlignment object from a string.
 *
 * @param description A string describing the reader in the keyval syntax.
 * @return A new OAlignment object according to options specified.
 * @throw Exception if an error occurred.
 */
std::unique_ptr<OAlignment> read(const std::string& description);

/**
 * @return The arguments and their unparsed values from the last call of the read function, if there are any.
 */
virtual const std::map<std::string, std::string>& getUnparsedArguments() const {
	return unparsedArguments_;
}
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_BPPOALIGNMENTWRITERFORMAT_H
