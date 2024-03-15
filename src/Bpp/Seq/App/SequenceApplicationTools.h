// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_APP_SEQUENCEAPPLICATIONTOOLS_H
#define BPP_SEQ_APP_SEQUENCEAPPLICATIONTOOLS_H

#include <map>
#include <string>
#include <memory>

#include "../Alphabet/Alphabet.h"
#include "../AlphabetIndex/AlphabetIndex1.h"
#include "../AlphabetIndex/AlphabetIndex2.h"
#include "../GeneticCode/GeneticCode.h"
#include "../Container/SequenceContainer.h"
#include "../Container/VectorSiteContainer.h"
#include "../Container/SiteContainerTools.h"
#include "../SiteTools.h"

namespace bpp
{
/**
 * @brief This class provides some common tools for applications.
 *
 * The functions parse some option file, create corresponding objects and send
 * a pointer toward it.
 *
 * The option files are supposed to follow this simple format:
 * @code
 * parameterName = parameterContent
 * @endcode
 * with one parameter per line.
 *
 * @see ApplicationTools
 */
class SequenceApplicationTools
{
public:
  SequenceApplicationTools() {}
  virtual ~SequenceApplicationTools() {}

public:
  /**
   * @brief Build an Alphabet object according to options.
   *
   * Options used are:
   * @verbatim
     - alphabet = [DNA|RNA|Protein], the alphabet type to use.
               = [DNA|RNA|Protein](length=n) a word-alphabet of
                    words with length n
               = [EchinodermMitochondrialCodonAlphabet
   | InvertebrateMitochondrialCodonAlphabet
   | InvertebrateMitochondrialCodonAlphabet
   | StandardCodonAlphabet
   | VertebrateMitochondrialCodonAlphabet]([alphn=NA|RNA])
                     a codon-alphabet
     @endverbatim
   *
   * @param params  The attribute map where options may be found.
   * @param suffix  A suffix to be applied to each attribute name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param verbose Print some info to the 'message' output stream.
   * @param allowGeneric Tell if generic alphabets can be used.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   * @return A new Alphabet object according to options specified.
   */
  static std::unique_ptr<Alphabet> getAlphabet(
    const std::map<std::string, std::string>& params,
    const std::string& suffix = "",
    bool suffixIsOptional = true,
    bool verbose = true,
    bool allowGeneric = false,
    int warn = 1);

  /**
   * @brief Build a GeneticCode object according to options.
   *
   * @param alphabet pointer to the NucleicAlphabet.
   *        !!! the pointer will be owned by the genetic code!!!
   * @param description for the name of the GeneticCode:
   *    [EchinodermMitochondrialGeneticCode
   *    | InvertebrateMitochondrialGeneticCode
   *    | InvertebrateMitochondrialGeneticCode
   *    | StandardGeneticCode
   *    | VertebrateMitochondrialGeneticCode]
   * @return A new GeneticCode object
   * @throw Exception in case of bad description.
   */
  static std::unique_ptr<GeneticCode> getGeneticCode(
    std::shared_ptr<const NucleicAlphabet> alphabet,
    const std::string& description);

  /**
   * @brief Build a AlphabetIndex1 object for a given alphabet.
   *
   * @param alphabet The alphabet to use. This is currently only used for assessing the type of distance allowed.
   * @param description Which distance to use. See the Bio++ Program Suite reference manual for a description of the syntax.
   * @param message To be displayed when parsing.
   * @param verbose Tell if some info should be displayed while parsing.
   * @return A new AlphabetIndex1 object.
   * @throw Exception in case of bad description.
   */
  static std::unique_ptr<AlphabetIndex1> getAlphabetIndex1(
    std::shared_ptr<const Alphabet> alphabet,
    const std::string& description,
    const std::string& message = "Alphabet measure:",
    bool verbose = true);

  static std::unique_ptr<AlphabetIndex1> getAlphabetIndex1(
    std::shared_ptr<const CodonAlphabet> alphabet,
    std::shared_ptr<const GeneticCode> gencode,
    const std::string& description,
    const std::string& message = "Alphabet measure:",
    bool verbose = true);


  /**
   * @brief Build a AlphabetIndex2 object for a given alphabet.
   *
   * @param alphabet The alphabet to use. This is currently only used for assessing the type of distance allowed.
   * @param description Which distance to use. See the Bio++ Program Suite reference manual for a description of the syntax.
   * @param message To be displayed when parsing.
   * @return A new AlphabetIndex2 object.
   * @param verbose Tell if some info should be displayed while parsing.
   * @throw Exception in case of bad description.
   */

  static std::unique_ptr<AlphabetIndex2> getAlphabetIndex2(
    std::shared_ptr<const Alphabet> alphabet,
    const std::string& description,
    const std::string& message = "Alphabet distance:",
    bool verbose = true);

  static std::unique_ptr<AlphabetIndex2> getAlphabetIndex2(
    std::shared_ptr<const CodonAlphabet> alphabet,
    std::shared_ptr<const GeneticCode> gencode,
    const std::string& description,
    const std::string& message = "Alphabet distance:",
    bool verbose = true);

  /**
   * @brief Build a SequenceContainer object according to options.
   *
   * The sequences do not have to be aligned.
   * The supported sequence formats are Fasta, DCSE, Clustal, Mase, Phylip and GenBank.
   *
   * See the Bio++ program suite manual for a full description of the syntax.
   *
   * @param alpha   The alphabet to use in the container.
   * @param params  The attribute map where options may be found.
   * @param suffix  A suffix to be applied to each attribute name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   * @return A new VectorSequenceContainer object according to options specified.
   * @see getSiteContainer to read an alignment.
   */

  static std::unique_ptr<SequenceContainerInterface> getSequenceContainer(
    std::shared_ptr<const Alphabet> alpha,
    const std::map<std::string, std::string>& params,
    const std::string& suffix = "",
    bool suffixIsOptional = true,
    bool verbose = true,
    int warn = 1);

  /**
   * @brief Build a SiteContainer object according to the BppO syntax.
   *
   * Sequences in file must be aligned.
   * The supported sequence formats are Fasta, DCSE, Clustal, Mase and Phylip.
   *
   * See the Bio++ program suite manual for a full description of the syntax.
   *
   * @param alpha   The alphabet to use in the container.
   * @param params  The attribute map where options may be found.
   * @param suffix  A suffix to be applied to each attribute name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   * @return A new VectorSiteContainer object according to the description.
   */
  static std::unique_ptr<VectorSiteContainer> getSiteContainer(
    std::shared_ptr<const Alphabet> alpha,
    const std::map<std::string, std::string>& params,
    const std::string& suffix = "",
    bool suffixIsOptional = true,
    bool verbose = true,
    int warn = 1);

  /**
   * @brief Build a ProbabilisticSiteContainer object according to the BppO syntax.
   *
   * Sequences in file must be aligned.
   * The supported sequence formats are Fasta, DCSE, Clustal, Mase and Phylip.
   *
   * See the Bio++ program suite manual for a full description of the syntax.
   *
   * @param alpha   The alphabet to use in the container.
   * @param params  The attribute map where options may be found.
   * @param suffix  A suffix to be applied to each attribute name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   * @return A new ProbabilisticVectorSiteContainer object according to the description.
   */
  static std::unique_ptr<ProbabilisticVectorSiteContainer> getProbabilisticSiteContainer(
    std::shared_ptr<const Alphabet> alpha,
    const std::map<std::string, std::string>& params,
    const std::string& suffix = "",
    bool suffixIsOptional = true,
    bool verbose = true,
    int warn = 1);

  /**
   * @brief Build multiple SiteContainer objects according to the BppO syntax.
   *
   * Sequences in file must be aligned.
   * The supported sequence formats are Fasta, DCSE, Clustal, Mase and Phylip.
   *
   * See the Bio++ program suite manual for a full description of the syntax.
   *
   * @param alpha   The alphabet to use in the container.
   * @param params  The attribute map where options may be found.
   * @param prefix  A prefix to be applied to each attribute name.
   * @param suffix  A suffix to be applied to each attribute name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   * @return A map of VectorSiteContainer objects according to the description.
   */
  static std::map<size_t, std::unique_ptr<VectorSiteContainer> >
  getSiteContainers(
    std::shared_ptr<const Alphabet> alpha,
    const std::map<std::string, std::string>& params,
    const std::string& prefix = "input.",
    const std::string& suffix = "",
    bool suffixIsOptional = true,
    bool verbose = true,
    int warn = 1);

  /**
   * @brief Build multiple ProbabilisticSiteContainer objects according to the BppO syntax.
   *
   * Sequences in file must be aligned.
   * The supported sequence format is Pasta.
   *
   * See the Bio++ program suite manual for a full description of the syntax.
   *
   * @param alpha   The alphabet to use in the container.
   * @param params  The attribute map where options may be found.
   * @param prefix  A prefix to be applied to each attribute name.
   * @param suffix  A suffix to be applied to each attribute name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   * @return A map of ProbabilisticVectorSiteContainer objects according to the description.
   */
  static std::map<size_t, std::unique_ptr<ProbabilisticVectorSiteContainer> >
  getProbabilisticSiteContainers(
    std::shared_ptr<const Alphabet> alpha,
    const std::map<std::string, std::string>& params,
    const std::string& prefix = "input.",
    const std::string& suffix = "",
    bool suffixIsOptional = true,
    bool verbose = true,
    int warn = 1);


  /**
   * @brief Retrieves selected sequences (by name).
   *
   * This method works on the input container and discard sequences not present in the selection.
   *
   * Recognized options are:
   * - sequence.keep_names = [ all | name1,...,nameN ].
   * - sequence.remove_names = [ none | name1,...,nameN ].
   *
   * @param allSequences The sequence container from which sequences should be filtered.
   * @param params   The attribute map where options may be found.
   * @param suffix   A suffix to be applied to each attribute name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   */
  static void restrictSelectedSequencesByName(
    SequenceContainerInterface& allSequences,
    const std::map<std::string, std::string>& params,
    std::string suffix = "",
    bool suffixIsOptional = true,
    bool verbose = true,
    int warn = 1);


  /**
   * @brief Retrieves sites suitable for the analysis.
   *
   * Options used are:
   * - sequence.sites_to_use = [all|complete|nogap].
   *
   * If the 'complete' option is used, only fully resolve site will be taken
   * into account.
   * If the 'nogap' option is used, only sites without gap will be taken into
   * account.
   * If 'gapAsUnknown' is set to true and the all option is selected, gaps will
   * be changed to 'unknown' character is sequences.
   *
   * - sequence.max_gap_allowed = [57%|30]
   * If a % sign fallow the number, it is taken to be a frequence (in percent).
   * This specify the maximum amount of gaps allowed for each site.
   * Sites not satisfying this amount will be removed.
   * A value of 100% will remove all gap-only sites, a value >100% will keep all sites.
   *
   * @param allSites The site container from which sites must be retrieved.
   * @param params   The attribute map where options may be found.
   * @param suffix   A suffix to be applied to each attribute name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param gapAsUnknown Convert gaps to unknown characters.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   * @return A new VectorSiteContainer object containing sites of interest.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr< TemplateVectorSiteContainer<SiteType, SequenceType> >
  getSitesToAnalyse(
    const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& allSites,
    const std::map<std::string, std::string>& params,
    std::string suffix = "",
    bool suffixIsOptional = true,
    bool gapAsUnknown = true,
    bool verbose = true,
    int warn = 1)
  {
    // Fully resolved sites, i.e. without jokers and gaps:
    std::unique_ptr< TemplateVectorSiteContainer<SiteType, SequenceType> > sitesToAnalyse;

    size_t numSeq = allSites.getNumberOfSequences();

    std::string option = ApplicationTools::getStringParameter("input.sequence.sites_to_use", params, "complete", suffix, suffixIsOptional, warn);
    if (verbose)
      ApplicationTools::displayResult("Sites to use", option);

    if (option == "all")
    {
      sitesToAnalyse = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(allSites);
      size_t nbSites = sitesToAnalyse->getNumberOfSites();

      std::string maxGapOption = ApplicationTools::getStringParameter("input.sequence.max_gap_allowed", params, "100%", suffix, suffixIsOptional, warn);

      double gapCount = 0;

      if (maxGapOption[maxGapOption.size() - 1] == '%')
      {
        double gapFreq = TextTools::toDouble(maxGapOption.substr(0, maxGapOption.size() - 1)) / 100;
        gapCount = gapFreq * (int)numSeq;
      }
      else
        gapCount = TextTools::to<int>(maxGapOption) - NumConstants::TINY();

      if (gapCount < static_cast<double>(numSeq) - NumConstants::TINY())
      {
        if (verbose)
          ApplicationTools::displayTask("Remove sites with gaps", true);
        for (size_t i = nbSites; i > 0; i--)
        {
          if (verbose)
            ApplicationTools::displayGauge(nbSites - i, nbSites - 1, '=');

          if (static_cast<double>(SiteTools::numberOfGaps(sitesToAnalyse->site(i - 1))) > gapCount)
            sitesToAnalyse->deleteSites(i - 1, 1);
        }
        if (verbose)
          ApplicationTools::displayTaskDone();
      }

      std::string maxUnresolvedOption = ApplicationTools::getStringParameter("input.sequence.max_unresolved_allowed", params, "100%", suffix, suffixIsOptional, warn);

      double unresCount = 0;

      if (maxUnresolvedOption[maxUnresolvedOption.size() - 1] == '%')
      {
        double unresFreq = TextTools::toDouble(maxUnresolvedOption.substr(0, maxUnresolvedOption.size() - 1)) / 100;
        unresCount = unresFreq * (int)numSeq;
      }
      else
        unresCount = TextTools::to<double>(maxUnresolvedOption) - NumConstants::TINY();

      nbSites = sitesToAnalyse->getNumberOfSites();

      if (unresCount < static_cast<double>(numSeq) - NumConstants::TINY())
      {
        if (verbose)
          ApplicationTools::displayTask("Remove unresolved sites", true);
        for (size_t i = nbSites; i > 0; i--)
        { 
          if (verbose)
            ApplicationTools::displayGauge(nbSites - i, nbSites - 1, '=');

          if (static_cast<double>(SiteTools::numberOfUnresolved(sitesToAnalyse->site(i - 1))) > unresCount)
            sitesToAnalyse->deleteSites(i - 1, 1);
        }
        if (verbose)
          ApplicationTools::displayTaskDone();
      }
    }
    else if (option == "complete")
    {
      sitesToAnalyse = SiteContainerTools::getCompleteSites(allSites);
      size_t nbSites = sitesToAnalyse->getNumberOfSites();
      if (verbose)
        ApplicationTools::displayResult("Complete sites", TextTools::toString(nbSites));
    }
    else if (option == "nogap")
    {
      sitesToAnalyse = SiteContainerTools::getSitesWithoutGaps(allSites);
      size_t nbSites = sitesToAnalyse->getNumberOfSites();
      if (verbose)
        ApplicationTools::displayResult("Sites without gap", TextTools::toString(nbSites));
    }
    else
    {
      throw Exception("Option '" + option + "' unknown in parameter 'sequence.sites_to_use'.");
    }

    auto ca = std::dynamic_pointer_cast<const CodonAlphabet>(sitesToAnalyse->getAlphabet());
    if (ca)
    {
      option = ApplicationTools::getStringParameter("input.sequence.remove_stop_codons", params, "no", suffix, true, warn);
      if ((option != "") && verbose)
        ApplicationTools::displayResult("Remove Stop Codons", option);

      if (option == "yes")
      {
	std::string codeDesc = ApplicationTools::getStringParameter("genetic_code", params, "Standard", "", true, warn);
	auto nucAlph = ca->getNucleicAlphabet();
	auto gCode = getGeneticCode(nucAlph, codeDesc);
        SiteContainerTools::removeSitesWithStopCodon(*sitesToAnalyse, *gCode);
      }
    }

    if (verbose)
      ApplicationTools::displayResult("Number of sites", sitesToAnalyse->getNumberOfSites());

    return sitesToAnalyse;
  }



  /**
   * @brief Write a sequence file according to options.
   *
   * The supported sequence formats are Fasta and Mase.
   *
   * See the Bio++ program suite manual for a full description of the syntax.
   *
   * @see writeSequenceFile(SiteContainer) for writing alignments, with more output formats.
   *
   * @param sequences The sequences to write.
   * @param params  The attribute map where options may be found.
   * @param suffix  A suffix to be applied to each attribute name.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   */
  static void writeSequenceFile(
    const SequenceContainerInterface& sequences,
    const std::map<std::string, std::string>& params,
    const std::string& suffix = "",
    bool verbose = true,
    int warn = 1);

  /**
   * @brief Write a sequence alignment file according to options.
   *
   * The supported sequence formats are Fasta, Mase and Phylip.
   *
   * See the Bio++ program suite manual for a full description of the syntax.
   *
   * @param sequences The aligned sequences to write.
   * @param params  The attribute map where options may be found.
   * @param suffix  A suffix to be applied to each attribute name.
   * @param verbose Print some info to the 'message' output stream.
   * @param warn Set the warning level (0: always display warnings, >0 display warnings on demand).
   */
  static void writeAlignmentFile(
    const SiteContainerInterface& sequences,
    const std::map<std::string, std::string>& params,
    const std::string& suffix = "",
    bool verbose = true,
    int warn = 1);
};
} // end of namespace bpp.
#endif // BPP_SEQ_APP_SEQUENCEAPPLICATIONTOOLS_H
