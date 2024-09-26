// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_DCSE_H
#define BPP_SEQ_IO_DCSE_H


#include "../Container/AlignedSequenceContainer.h"
#include "../Container/SequenceContainer.h"
#include "../Sequence.h"
#include "AbstractIAlignment.h"

namespace bpp
{
/**
 * @brief Support for the Dedicated Comparative Sequence Editor format.
 *
 * Only the sequence information is retrieved.
 * All structural information is dropped for now.
 *
 * A description of this format may be found here:
 * http://www.psb.ugent.be/rRNA/help/formats/aliformat.html
 */
class DCSE :
	public AbstractIAlignment2
{
public:
DCSE() {
}
virtual ~DCSE() {
}

public:
/**
 * @name The AbstractIAlignment interface.
 *
 * @{
 */
void appendAlignmentFromStream(std::istream& input, SequenceContainerInterface& sc) const override;
/** @} */

/**
 * @name The ISequence interface.
 *
 * As a SiteContainer is a subclass of SequenceContainer, we hereby implement the ISequence
 * interface by downcasting the interface.
 *
 * @{
 */
std::unique_ptr<SequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet> alpha) const override
{
	return readAlignment(input, alpha);
}

std::unique_ptr<SequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet> alpha) const override
{
	return readAlignment(path, alpha);
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
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_DCSE_H
