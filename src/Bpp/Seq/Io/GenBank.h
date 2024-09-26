// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_GENBANK_H
#define BPP_SEQ_IO_GENBANK_H


#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "../Sequence.h"
#include "AbstractISequence.h"

namespace bpp
{
/**
 * @brief The GenBank sequence file format.
 *
 * For now, only read raw sequences. Features are not yet supported.
 */
class GenBank :
	public AbstractISequence
{
public:
/**
 * @brief Build a new GenBank object.
 */
GenBank() {
}

virtual ~GenBank() {
}

public:
/**
 * @name The AbstractISequence interface.
 *
 * @{
 */
void appendSequencesFromStream(std::istream& input, SequenceContainerInterface& sc) const;
/** @} */

/**
 * @name The IOSequence interface.
 *
 * @{
 */
const std::string getFormatName() const {
	return "GenBank file";
}
const std::string getFormatDescription() const
{
	return "Sequences following the GenBank data base format.";
}
/** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_GENBANK_H
