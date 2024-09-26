// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_SEQUENCEFILEINDEX_H
#define BPP_SEQ_IO_SEQUENCEFILEINDEX_H

#include <Bpp/Exceptions.h>
#include <string>


namespace bpp
{
/**
 * @brief Index to retrieve Sequence in a file
 *
 * This class is designed to build an in-memory index of a Sequence file in
 * order to retrieve Sequence given its ID.
 *
 * @author Sylvain Gaillard
 */
class SequenceFileIndex
{
public:
virtual ~SequenceFileIndex() {
}
/**
 * @brief Build the index given a path to the file.
 */
virtual void build(const std::string& path) = 0;

/**
 * @brief Get the position of a Sequence given its ID.
 */
virtual std::streampos getSequencePosition(const std::string& id) const = 0;

/**
 * @brief Get the number of sequences
 */
virtual size_t getNumberOfSequences() const = 0;
};
}
#endif // BPP_SEQ_IO_SEQUENCEFILEINDEX_H
