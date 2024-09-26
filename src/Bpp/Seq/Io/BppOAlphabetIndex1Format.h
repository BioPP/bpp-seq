// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_BPPOALPHABETINDEX1FORMAT_H
#define BPP_SEQ_IO_BPPOALPHABETINDEX1FORMAT_H

#include <Bpp/Io/IoFormat.h>

#include "../AlphabetIndex/AlphabetIndex1.h"
#include "../GeneticCode/GeneticCode.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief AlphabetIndex1 I/O in BppO format.
 *
 * Enables the instantiation of AlphabetIndex1 objects according to
 * the BppO syntax (see the Bio++ Program Suite
 * manual for a detailed description of this syntax).
 *
 */
class BppOAlphabetIndex1Format :
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
BppOAlphabetIndex1Format(
	std::shared_ptr<const Alphabet>& alphabet,
	const std::string& message,
	bool verbose = true) :
	alphabet_(alphabet),
	gencode_(0),
	message_(message),
	verbose_(verbose)
{
}

BppOAlphabetIndex1Format(
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

BppOAlphabetIndex1Format(const BppOAlphabetIndex1Format& format) :
	alphabet_(format.alphabet_),
	gencode_(format.gencode_),
	message_(format.message_),
	verbose_(format.verbose_) {
}

BppOAlphabetIndex1Format& operator=(const BppOAlphabetIndex1Format& format)
{
	alphabet_ = format.alphabet_;
	gencode_ = format.gencode_;
	message_  = format.message_;
	verbose_  = format.verbose_;
	return *this;
}

virtual ~BppOAlphabetIndex1Format() {
}

public:
const std::string getFormatName() const {
	return "BppO";
}

const std::string getFormatDescription() const {
	return "Bpp Options format.";
}

const std::string getDataType() const {
	return "AlphabetIndex1";
}

/**
 * @brief Read a AlphabetIndex1 object from a string.
 *
 * @param description A string describing the index in the keyval syntax.
 * @return A new AlphabetIndex1 object according to options specified.
 * @throw Exception if an error occurred.
 */
std::unique_ptr<AlphabetIndex1> read(const std::string& description);
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_BPPOALPHABETINDEX1FORMAT_H
