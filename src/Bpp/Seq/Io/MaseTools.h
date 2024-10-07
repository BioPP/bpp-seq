// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_MASETOOLS_H
#define BPP_SEQ_IO_MASETOOLS_H

#include <Bpp/Exceptions.h>

#include "../Container/SequenceContainer.h"
#include "../Container/SequenceContainerTools.h"
#include "../Container/SiteContainer.h"
#include "../Container/SiteContainerTools.h"

namespace bpp
{
/**
 * @brief Utilitary methods that deal with the Mase format.
 *
 * This class particularly covers the Mase+ format, which allows
 * site and sequence selection.
 * Mase+ tags are in the header of the mase file, which is stored
 * in the 'general comment' section of sequence containers.
 * Most of the methods here hence work on the general comments associated
 * to a container.
 */
class MaseTools
{
public:
  /**
   * @brief Get a site selection from a Mase+ header file.
   *
   * @param maseFileHeader The header of the mase+ file as comments lines.
   * @param setName        The name of the set to retrieve.
   * @throw IOException If the specified set is not found.
   */
  static SiteSelection getSiteSet(const Comments& maseFileHeader, const std::string& setName);

  /**
   * @brief Get a sequence selection from a Mase+ header file.
   *
   * @param maseFileHeader The header of the mase+ file as comments lines.
   * @param setName        The name of the set to retrieve.
   * @throw IOException If the specified set is not found.
   */
  static SequenceSelection getSequenceSet(const Comments& maseFileHeader, const std::string& setName);

  /**
   * @brief Create a new container corresponding to a site set given in the mase+ format.
   *
   * A new VectorSiteContainer is created, whose destruction is up to the user.
   * The container passed as argument must have 'general comments' in the mase+ format.
   * This function calls the getSiteSet() function on the comments and then calls for
   * SiteContainerTools::getSelectedSites() on the selection.
   *
   * @param sequences The container to get the sites from.
   * @param setName   The name of the set to retrieve.
   * @throw IOException If the specified set is not found.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr<TemplateVectorSiteContainer<SiteType, SequenceType>>
  getSelectedSites(
      const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sequences,
      const std::string& setName)
  {
    SiteSelection ss = getSiteSet(sequences.getComments(), setName);
    return SiteContainerTools::getSelectedPositions<SiteType, SequenceType>(sequences, ss);
  }


  /**
   * @brief Create a new container corresponding to a site set given in the mase+ format.
   *
   * A new VectorSequenceContainer is created, whose destruction is up to the user.
   * The container passed as argument must have 'general comments' in the mase+ format.
   * This function calls the getSequenceSet() function on the comments and then calls for
   * SiteContainerTools::getSelectedSequences() on the selection.
   *
   * @param sequences The container to get the sequence from.
   * @param setName   The name of the set to retrieve.
   * @throw IOException If the specified set is not found.
   */
  template<class SequenceType, class HashType>
  static std::unique_ptr<TemplateSequenceContainerInterface<SequenceType, HashType>>
  getSelectedSequences(
      const TemplateSequenceContainerInterface<SequenceType, HashType>& sequences,
      const std::string& setName)
  {
    SequenceSelection ss = getSequenceSet(sequences.getGeneralComments(), setName);
    return SequenceContainerTools::getSelectedSequences<SequenceType, HashType>(sequences, ss);
  }


  /**
   * @brief Get a list of all available site selections.
   *
   * @param maseHeader Comments as described in the Mase+ format specification.
   * @return A vector of selection names.
   */
  static std::map<std::string, size_t> getAvailableSiteSelections(const Comments& maseHeader);

  /**
   * @brief Get a list of all available sequences selections.
   *
   * @param maseHeader Comments as described in the Mase+ format specification.
   * @return A vector of selection names.
   */
  static std::map<std::string, size_t> getAvailableSequenceSelections(const Comments& maseHeader);

  /**
   * @brief Get the phase of a given coding region from a mase+ header.
   *
   * Look for a /codon_start tag with a phase indice and a site selection with name setName.
   *
   * @param maseFileHeader Comments in Mase+ format.
   * @param setName a cds site selection name.
   * @return 1,2 or 3.
   * @throw Exception If no corresponding tag found in file.
   */
  static size_t getPhase(const Comments& maseFileHeader, const std::string& setName);
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_MASETOOLS_H
