// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_BPPOALPHABETINDEX2FORMAT_H
#define BPP_SEQ_IO_BPPOALPHABETINDEX2FORMAT_H

#include <Bpp/Io/IoFormat.h>

#include "../AlphabetIndex/AlphabetIndex2.h"
#include "../GeneticCode/GeneticCode.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief AlphabetIndex2 I/O in BppO format.
 *
 * Enables the instantiation of AlphabetIndex2 objects according to
 * the BppO syntax (see the Bio++ Program Suite
 * manual for a detailed description of this syntax).
 *
 */
class BppOAlphabetIndex2Format :
	public virtual IOFormat
{
private:
std::shared_ptr<const Alphabet> alphabet_;
std::shared_ptr<const GeneticCode> gencode_;

std::string message_;
bool verbose_;

public:
/**
 * @param alphabet The alphabet for which indices should be built.
 * The alphabet will be used to check that the instantiated index is compatible.
 * @param message Some text describing what the index is intended for.
 * @param verbose Tell if some messages should be printed while parsing.
 */
BppOAlphabetIndex2Format(
	std::shared_ptr<const Alphabet>& alphabet,
	const std::string& message,
	bool verbose = true) :
	alphabet_(alphabet),
	gencode_(0),
	message_(message),
	verbose_(verbose)
{
}

BppOAlphabetIndex2Format(
	std::shared_ptr<const CodonAlphabet>& alphabet,
	std::shared_ptr<const GeneticCode>& gencode,
	const std::string& message,
	bool verbose = true) :
	alphabet_(alphabet),
	gencode_(gencode),
	message_(message),
	verbose_(verbose)
{
}

BppOAlphabetIndex2Format(const BppOAlphabetIndex2Format& format) :
	alphabet_(format.alphabet_),
	gencode_(format.gencode_),
	message_(format.message_),
	verbose_(format.verbose_) {
}

BppOAlphabetIndex2Format& operator=(const BppOAlphabetIndex2Format& format)
{
	alphabet_ = format.alphabet_;
	gencode_ = format.gencode_;
	message_  = format.message_;
	verbose_  = format.verbose_;
	return *this;
}

virtual ~BppOAlphabetIndex2Format() {
}

public:
const std::string getFormatName() const {
	return "BppO";
}

const std::string getFormatDescription() const {
	return "Bpp Options format.";
}

const std::string getDataType() const {
	return "AlphabetIndex2";
}

/**
 * @brief Read a AlphabetIndex2 object from a string.
 *
 * @param description A string describing the index in the keyval syntax.
 * @return A new AlphabetIndex2 object according to options specified.
 * @throw Exception if an error occurred.
 */
std::unique_ptr<AlphabetIndex2> read(const std::string& description);
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_BPPOALPHABETINDEX2FORMAT_H
