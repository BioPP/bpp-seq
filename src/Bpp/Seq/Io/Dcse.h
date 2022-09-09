//
// File: Dcse.h
// Authors:
//   Julien Dutheil
// Created: 2004-03-03 00:00:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

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
  DCSE() {}
  virtual ~DCSE() {}

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
  std::unique_ptr<SequenceContainerInterface> readSequences(std::istream& input, std::shared_ptr<const Alphabet>& alpha) const override
  {
    return readAlignment(input, alpha);
  }

  std::unique_ptr<SequenceContainerInterface> readSequences(const std::string& path, std::shared_ptr<const Alphabet>& alpha) const override
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
