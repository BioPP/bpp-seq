// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_PHREDPOLY_H
#define BPP_SEQ_IO_PHREDPOLY_H


#include "../Sequence.h"
#include "ISequenceStream.h"

namespace bpp
{
/**
 * @brief The poly sequence file format from phred software.
 *
 * This class read DNA sequence from poly files produced by the phred program
 * from the University of Washington.
 * For now, only read raw sequences and do a basic filter on heterozygous site.
 */
class PhredPoly : public ISequenceStream
{
protected:
double ratio_;

public:
/**
 * @brief Build a new PhredPoly object.
 */
PhredPoly(double ratio = 0.8);

virtual ~PhredPoly() {
}

public:
/**
 * @name The AbstractISequence interface.
 *
 * @{
 */
bool nextSequence(std::istream& input, Sequence& seq) const;
/** @} */

/**
 * @name The IOSequence interface.
 *
 * @{
 */
const std::string getDataType() const {
	return "Sequence";
}
const std::string getFormatName() const {
	return "poly file";
}
const std::string getFormatDescription() const
{
	return "Sequences following the poly format as describe in the phred documentation.";
}
/** @} */
};
} // end of namespace bpp
#endif // BPP_SEQ_IO_PHREDPOLY_H
