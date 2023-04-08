//
// File: SiteContainerTools.h
// Authors:
//   Julien Dutheil
// Created: 2003-12-12 18:55:06
//

#ifndef BPP_SEQ_CONTAINER_SITECONTAINERTOOLS_H
#define BPP_SEQ_CONTAINER_SITECONTAINERTOOLS_H



/*
   Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)

   This software is a computer program whose purpose is to provide classes
   for sequences analysis.

   This software is governed by the CeCILL  license under French law and
   abiding by the rules of distribution of free software.  You can  use,
   modify and/ or redistribute the software under the terms of the CeCILL
   license as circulated by CEA, CNRS and INRIA at the following URL
   "http://www.cecill.info".

   As a counterpart to the access to the source code and  rights to copy,
   modify and redistribute granted by the license, users are provided only
   with a limited warranty  and the software's author,  the holder of the
   economic rights,  and the successive licensors  have only  limited
   liability.

   In this respect, the user's attention is drawn to the risks associated
   with loading,  using,  modifying and/or developing or reproducing the
   software by the user in light of its specific status of free software,
   that may mean  that it is complicated to manipulate,  and  that  also
   therefore means  that it is reserved for developers  and  experienced
   professionals having in-depth computer knowledge. Users are therefore
   encouraged to load and test the software's suitability as regards their
   requirements in conditions enabling the security of their systems and/or
   data to be ensured and,  more generally, to use and operate it in the
   same conditions as regards security.

   The fact that you are presently reading this means that you have had
   knowledge of the CeCILL license and that you accept its terms.
 */

#include "SiteContainer.h"
#include "VectorSiteContainer.h"
#include "AlignedSequenceContainer.h"
#include "SequenceContainerTools.h"
#include "AlignmentData.h"
#include "../AlphabetIndex/AlphabetIndex2.h"
#include "../DistanceMatrix.h"
#include "../GeneticCode/GeneticCode.h"
#include "../SiteTools.h"
#include "../CodonSiteTools.h"
#include "../Site.h"
#include <Bpp/Numeric/Matrix/Matrix.h>
#include <Bpp/Numeric/Random/RandomTools.h>

// From the STL:
#include <vector>
#include <map>
#include <memory>

namespace bpp
{
typedef std::vector<size_t> SiteSelection;

/**
 * @brief Some utililitary methods to deal with site containers.
 */
class SiteContainerTools
{
public:
  SiteContainerTools() {}
  virtual ~SiteContainerTools() {}

public:
  /**
   * @brief Retrieves sites without gaps.
   *
   * This function builds a new VectorSiteContainer instance with only sites without gaps.
   * The container passed as input is not modified, all sites are copied.
   *
   * @param sites The container to analyse.
   * @return A pointer toward a new SiteContainer with only sites with no gaps.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr<TemplateVectorSiteContainer<SiteType, SequenceType>>
  getSitesWithoutGaps(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites)
  {
    std::vector<std::string> sequenceKeys = sites.getSequenceKeys();
    std::shared_ptr<const Alphabet> alphaPtr = sites.getAlphabet();
    auto selectedSites = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(sequenceKeys, alphaPtr);
    for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
      if (!SiteTools::hasGap(sites.site(i))) { //This calls the method dedicated to basic sites
        std::unique_ptr<SiteType> sitePtr(sites.site(i).clone());
        selectedSites->addSite(sitePtr, false);
      }
    }
    return selectedSites;
  }


 
  /**
   * @brief Retrieves complete sites.
   *
   * This function builds a new VectorSiteContainer instance with only complete sites,
   * i.e. site with fully resolved states (no gap, no unknown caracters).
   * The container passed as input is not modified, all sites are copied.
   *
   * @param sites The container to analyse.
   * @return A pointer toward a new SiteContainer with only complete sites.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr<TemplateVectorSiteContainer<SiteType, SequenceType>>
  getCompleteSites(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites)
  {
    std::vector<std::string> sequenceKeys = sites.getSequenceKeys();
    std::shared_ptr<const Alphabet> alphaPtr = sites.getAlphabet();
    auto selectedSites = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(sequenceKeys, alphaPtr);
    for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
      if (SiteTools::isComplete(sites.site(i))) { //This calls the method dedicated to basic sites
        std::unique_ptr<SiteType> sitePtr(sites.site(i).clone());
        selectedSites->addSite(sitePtr, false);
      }
    }
    return selectedSites;
  }


 	   
  /**
   * @brief Get a site set without gap-only sites.
   *
   * This function builds a new VectorSiteContainer instance without sites with only gaps.
   * The container passed as input is not modified, all sites are copied.
   *
   * @param sites The container to analyse.
   * @return A pointer toward a new SiteContainer.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr<TemplateSiteContainerInterface<SiteType, SequenceType, std::string>>
  removeGapOnlySites(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites)
  {
    if (sites.getNumberOfSequences() == 0)
    throw Exception("SiteContainerTools::removeGapOnlySites. Container is empty.");
    std::vector<std::string> sequenceKeys = sites.getSequenceKeys();
    auto alphaPtr = sites.getAlphabet();
    auto newContainer = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(sequenceKeys, alphaPtr);
    for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
      const Site& site = sites.site(i);
      if (!SiteTools::isGapOnly(site)) {
        auto site2 = std::unique_ptr<SiteType>(site.clone());
        newContainer->addSite(site2, false);
      }
    }
    return newContainer;
  }



  /**
   * @brief Remove gap-only sites from a SiteContainer.
   *
   * @param sites The container where the sites have to be removed.
   */
  template<class SiteType, class SequenceType, class HashType>
  static void removeGapOnlySites(TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites)
  {
    if (sites.getNumberOfSequences() == 0)
      throw Exception("SiteContainerTools::removeGapOnlySites. Container is empty.");

    size_t n = sites.getNumberOfSites();
    size_t i = n;
    while (i > 1) {
      ApplicationTools::displayGauge(n - i + 1, n);
      const SiteType* site = &sites.site(i - 1); //Note (jdutheil 18/12/22: for some reason a ref here does not work, resorting to pointer)
      if (SiteTools::isGapOnly(*site)) {
        size_t end = i;
        while (SiteTools::isGapOnly(*site) && i > 1) {
          --i;
         site = &sites.site(i - 1);
        }
        sites.deleteSites(i, end - i);
      } else {
        --i;
      }
    }
    ApplicationTools::displayGauge(n, n);
    const Site& site = sites.site(0);
    if (SiteTools::isGapOnly(site))
      sites.deleteSite(0);
  }



  /**
   * @brief Get a site set without gap/unresolved-only sites from a SiteContainer.
   *
   * This function build a new VectorSiteContainer instance without sites with only gaps or unresolved characters.
   * The container passed as input is not modified, all sites are copied.
   *
   * @param sites The container to analyse.
   * @return A pointer toward a new SiteContainer.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr<TemplateVectorSiteContainer<SiteType, SequenceType>>
  removeGapOrUnresolvedOnlySites(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites)
  {
    if (sites.getNumberOfSequences() == 0)
      throw Exception("SiteContainerTools::removeGapOrUnresolvedOnlySites. Container is empty.");

    std::vector<std::string> sequenceKeys = sites.getSequenceKeys();
    auto alphaPtr = sites.getAlphabet();
    auto newContainer = std::make_unique<TemplateVectorSiteContainer<SiteType, SequenceType>>(sequenceKeys, alphaPtr);
    for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
      const Site& site = sites.site(i);
      if (!SiteTools::isGapOrUnresolvedOnly(site)) {
        auto site2 = std::unique_ptr<SiteType>(site.clone());
        newContainer->addSite(site2, false);
      }
    }
    return newContainer;
  }



  /**
   * @brief Remove gap/unresolved-only sites from a SiteContainer.
   *
   * @param sites The container where the sites have to be removed.
   */
  template<class SiteType, class SequenceType, class HashType>
  static void removeGapOrUnresolvedOnlySites(TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites)
{
  if (sites.getNumberOfSequences() == 0)
    throw Exception("SiteContainerTools::removeGapOrUnresolvedOnlySites. Container is empty.");

  size_t n = sites.getNumberOfSites();
  size_t i = n;
  while (i > 1) {
    ApplicationTools::displayGauge(n - i + 1, n);
    const SiteType& site = sites.site(i - 1);
    if (SiteTools::isGapOnly(site)) {
      size_t end = i;
      while (SiteTools::isGapOrUnresolvedOnly(site) && i > 1) {
        --i;
        site = &sites.site(i - 1);
      }
      sites.deleteSites(i, end - i);
    } else {
      --i;
    }
  }
  ApplicationTools::displayGauge(n, n);
  const SiteType& site = sites.site(0);
  if (SiteTools::isGapOrUnresolvedOnly(site))
    sites.deleteSite(0);
}

  /**
   * @brief Extract sites, from a SiteContainer, with less than a given amount of gaps.
   *
   * @param sites The container from which the sites have to be removed.
   * @param maxFreqGaps The maximum frequency of gaps in each site.
   * @return A pointer toward a new SiteContainer.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr< TemplateVectorSiteContainer<SiteType, SequenceType> >
  removeGapSites(
      const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites,
      double maxFreqGaps)
  {
    if (sites.getNumberOfSequences() == 0)
      throw Exception("SiteContainerTools::removeGapSites. Container is empty.");

    std::vector<std::string> sequenceKeys = sites.getSequenceKeys();
    auto newContainer = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(sequenceKeys, sites.getAlphabet());
    for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
      std::map<int, double> freq;
      const Site& site = sites.site(i);
      SiteTools::getFrequencies(site, freq);
      if (freq[-1] <= maxFreqGaps) {
        auto site2 = std::make_unique<SiteType>(site.clone());
        newContainer->addSite(site2, false);
      }
    }
    return newContainer;
  }



  /**
   * @brief Remove sites with a given amount of gaps.
   *
   * @param sites The container from which the sites have to be removed.
   * @param maxFreqGaps The maximum frequency of gaps in each site.
   */
  template<class SiteType, class SequenceType, class HashType>
  static void removeGapSites(
		  TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites,
		  double maxFreqGaps)
  {
    if (sites.getNumberOfSequences() == 0)
      throw Exception("SiteContainerTools::removeGapSites. Container is empty.");

    for (size_t i = sites.getNumberOfSites(); i > 0; --i) {
      std::map<int, double> freq;
      SiteTools::getFrequencies(sites.site(i - 1), freq);
      if (freq[-1] > maxFreqGaps) {
        sites.deleteSite(i - 1);
      }
    }
  }



  /**
   * @brief Get a site set without stop codons, if the alphabet is a CodonAlphabet, otherwise throws an Exception.
   *
   * This function build a new BasicSiteContainer instance without sites that have at least a stop codon.
   * The container passed as input is not modified, all sites are copied.
   *
   * @param sites The container to analyse.
   * @param gCode the genetic code to use to determine stop codons.
   * @return A pointer toward a new SiteContainer.
   */
  static std::unique_ptr<SiteContainerInterface> getSitesWithoutStopCodon(
		  const SiteContainerInterface& sites,
		  const GeneticCode& gCode)
  {
    std::shared_ptr<const CodonAlphabet> pca = std::dynamic_pointer_cast<const CodonAlphabet>(sites.getAlphabet());
    if (!pca)
      throw AlphabetException("Not a Codon Alphabet", sites.getAlphabet().get());
    if (sites.getNumberOfSequences() == 0)
      throw Exception("SiteContainerTools::getSitesWithoutStopCodon. Container is empty.");

    std::vector<std::string> sequenceKeys = sites.getSequenceKeys();
    auto alphaP = sites.getAlphabet();
    auto newContainer = std::make_unique<VectorSiteContainer>(sequenceKeys, alphaP);
    for (size_t i = 0; i < sites.getNumberOfSites(); ++i) {
      const Site& site = sites.site(i);
      if (!CodonSiteTools::hasStop(site, gCode)) {
        std::unique_ptr<Site> site2(site.clone());
        newContainer->addSite(site2, false);
      }
    }
    return newContainer;
  }

  /**
   * @brief Remove sites with stop codons, if the alphabet is a CodonAlphabet, otherwise throws an Exception.
   *
   * @param sites The container to analyse.
   * @param gCode the genetic code to use to determine stop codons.
   */
  static void removeSitesWithStopCodon(
		  SiteContainerInterface& sites,
		  const GeneticCode& gCode)
  {
    std::shared_ptr<const CodonAlphabet> pca = std::dynamic_pointer_cast<const CodonAlphabet>(sites.getAlphabet());
    if (!pca)
      throw AlphabetException("Not a Codon Alphabet", sites.getAlphabet().get());
    if (sites.getNumberOfSequences() == 0)
      throw Exception("SiteContainerTools::removeSitesWithStopCodon. Container is empty.");

    for (size_t i = sites.getNumberOfSites(); i > 0; --i) {
      const Site& site = sites.site(i - 1);
      if (CodonSiteTools::hasStop(site, gCode))
        sites.deleteSite(i - 1);
    }
  }

/**
   * @brief Remove sites with stop codons, if the alphabet is a CodonAlphabet, otherwise throws an Exception.
   *
   * Note: this method is currently not implemented for probabilistic objects. An exception is thrown when called.
   * @param sites The container to analyse.
   * @param gCode the genetic code to use to determine stop codons.
   */
  static void removeSitesWithStopCodon(ProbabilisticSiteContainerInterface& sites, const GeneticCode& gCode)
  {
    throw Exception("SiteContainerTools::removeSitesWithStopCodon. Method not supported for probabilistic sequences.");
  }

  /**
   * @brief Extract a specified set of sites.
   *
   * A SiteContainer is filled with specified sites.
   *
   * Sites are specified by their indice, beginning at 0. Sites may be selected multiple times.
   *
   * @param sites       The container from wich sequences are to be taken.
   * @param selection   The positions of all sites to retrieve.
   * @param outputSites A container where to add the selected sites. The container must have the same alphabet, number of sequences and sequence keys from the input container.
   */
  template<class SiteType, class SequenceType, class HashType>
  static void getSelectedSites(
      const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites,
      const SiteSelection& selection,
      TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& outputSites)
  {
    for (auto pos : selection) {
      auto sitePtr = std::unique_ptr<SiteType>(sites.site(pos).clone());
      outputSites.addSite(sitePtr, false);
    }
  }



  /**
   * @brief Create a new container with a specified set of sites.
   *
   * Sites are specified by their indice, beginning at 0. Sites may be selected multiple times.
   *
   * @param sites       The container from wich sequences are to be taken.
   * @param selection   The positions of all sites to retrieve.
   * @return A VectorSiteContainer with the selected sites. Comments from the original container will be copied.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr< TemplateVectorSiteContainer<SiteType, SequenceType> >
  getSelectedSites(
      const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites,
      const SiteSelection& selection)
  {
    auto alphaPtr = sites.getAlphabet();
    auto outputSites = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(sites.getSequenceKeys(), alphaPtr);
    outputSites->setComments(sites.getComments());
    getSelectedSites<SiteType, SequenceType, std::string>(sites, selection, *outputSites);
    return outputSites;
  }



  /**
   * @brief Create a new container with a specified set of sites.
   *
   * Sites are specified by their indice, beginning at 0. Sites may be selected multiple times.
   * This version takes as input a generic AlignmentData object, and will try various casts.
   *
   * @param sites       The container from wich sequences are to be taken.
   * @param selection   The positions of all sites to retrieve.
   * @return A container of the same type as the input one, with the selected sites. Comments from the original container will be copied.
   */
  static std::unique_ptr<AlignmentDataInterface>
  getSelectedSites(
      const AlignmentDataInterface& sites,
      const SiteSelection& selection)
  {
    try {
      auto& sc = dynamic_cast<const SiteContainerInterface&>(sites);
      auto sel = getSelectedSites<Site, Sequence>(sc, selection);
      return move(sel);
    } catch (std::bad_cast& e) {}
   
    try {
      auto& psc = dynamic_cast<const ProbabilisticSiteContainerInterface&>(sites);
      auto sel = getSelectedSites<ProbabilisticSite, ProbabilisticSequence>(psc, selection);
      return move(sel);
    } catch (std::bad_cast& e) {}
    
    throw Exception("SiteContainerTools::getSelectedSites : unsupported container type.");
  }


  /**
   * @brief Extract a specified set of positions.
   *
   * A SiteContainer is filled with the specified positions.
   * Positions are specified by their indice, beginning at 0, and
   * are converted to site positions given the length of the words
   * of the alphabet.
   *
   * @param sites       The container from wich sequences are to be taken.
   * @param selection   The positions to retrieve.
   * @param outputSites A container where to add the selected positions. The container must have the same alphabet, number of sequences and sequence keys from the input container.
   */
  template<class SiteType, class SequenceType, class HashType>
  static void getSelectedPositions(
      const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites,
      const SiteSelection& selection,
      TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& outputSites)
  {
    size_t wsize = sites.getAlphabet()->getStateCodingSize();
    if (wsize > 1) {
      if (selection.size() % wsize != 0)
        throw IOException("SiteContainerTools::getSelectedPositions: Positions selection is not compatible with the alphabet in use in the container.");
      SiteSelection selection2;
      for (size_t i = 0; i < selection.size(); i += wsize) {
        if (selection[i] % wsize != 0)
          throw IOException("SiteContainerTools::getSelectedPositions: Positions selection is not compatible with the alphabet in use in the container.");

        for (size_t j = 1; j < wsize; ++j) {
          if (selection[i + j] != (selection[i + j - 1] + 1))
            throw IOException("SiteContainerTools::getSelectedPositions: Positions selection is not compatible with the alphabet in use in the container.");
        }
        selection2.push_back(selection[i] / wsize);
      }
      getSelectedSites(sites, selection2, outputSites);
    } else {
      getSelectedSites(sites, selection, outputSites);
    }
  }



  /**
   * @brief Create a new container with a specified set of positions.
   *
   * Positions are specified by their indice, beginning at 0, and
   * are converted to site positions given the length of the words
   * of the alphabet.
   *
   * @param sites       The container from wich sequences are to be taken.
   * @param selection   The positions of all sites to retrieve.
   * @return A VectorSiteContainer with the selected positions. Comments from the original container will be copied.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr< TemplateVectorSiteContainer<SiteType, SequenceType> >
  getSelectedPositions(
      const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites,
      const SiteSelection& selection)
  {
    auto alphaPtr = sites.getAlphabet();
    auto outputSites = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(sites.getSequenceKeys(), alphaPtr);
    outputSites->setComments(sites.getComments());
    getSelectedPositions<SiteType, SequenceType, std::string>(sites, selection, *outputSites);
    return outputSites;
  }



  /**
   * @brief create the consensus sequence of the alignment.
   *
   * In case of ambiguity (for instance a AATT site), one state will be chosen arbitrarily.
   *
   * @param sc a site container
   * @param name the name of the sequence object that will be created.
   * @param ignoreGap Tell if gap must be counted or not. If not (true option), only fully gapped sites will result in a gap in the consensus sequence.
   * @param resolveUnknown Tell is unknnown characters must resolved. In a DNA sequence for instance, N will be counted as A=1/4, T=1/4, G=1/4 and C=1/4. Otherwise it will be counted as N=1.
   * If this option is set to true, a consensus sequence will never contain an unknown character.
   * @return A new Sequence object with the consensus sequence.
   */
  static std::unique_ptr<Sequence> getConsensus(
		  const SiteContainerInterface& sc,
		  const std::string& name = "consensus",
		  bool ignoreGap = true,
		  bool resolveUnknown = false);

  /**
   * @brief Change all gaps to unknown state in a SiteContainer, according to its alphabet.
   *
   * For DNA alphabets, this change all '-' to 'N'.
   * For Protein alphabets, this change all '-' to 'X'.
   *
   * @param sites The container to be modified.
   */
  static void changeGapsToUnknownCharacters(SiteContainerInterface& sites);

  /**
   * @brief Change all gaps to unknown state in a ProbabilisticSiteContainer, according to its alphabet.
   *
   * This changes in each sequence all sites that sum to 0 into sites where all values equal 1.
   * @param sites The container to be modified.
   */
  static void changeGapsToUnknownCharacters(ProbabilisticSiteContainerInterface& sites);

  /**
   * @brief Change all unresolved characters to gaps in a SiteContainer, according to its alphabet.
   *
   * For DNA alphabets, this change all 'N', 'M', 'R', etc.  to '-'.
   *
   * @param sites The container to be modified.
   */
  static void changeUnresolvedCharactersToGaps(SiteContainerInterface& sites);

  /**
   * @brief Resolve a container with "." notations.
   *
   * @code
   * ATGCCGTTGG
   * .C...A..C.
   * ..A....C..
   * @endcode
   * will results in
   * @code
   * ATGCCGTTGG
   * ACCCCATTCG
   * ATACCGTCGG
   * @endcode
   * for instance.
   * The first sequence is here called the "reference" sequence.
   * It need not be the first in the container.
   * The alphabet of the input alignment must be an instance of the DefaultAlphabet class, the only one which support dot characters.
   * A new alignment is created and returned, with the specified alphabet.
   *
   * If several sequences that may be considered as reference are found, the first one is used.
   *
   * @param dottedAln The input alignment.
   * @param resolvedAlphabet The alphabet of the output alignment.
   * @return A pointer toward a dynamically created SiteContainer with the specified alphabet (can be a DefaultAlphabet).
   * @throw AlphabetException If the alphabet of the input alignment is not of class DefaultAlphabet, or if one character does not match with the output alphabet.
   * @throw Exception If no reference sequence was found, or if the input alignment contains no sequence.
   */
  static std::unique_ptr<SiteContainerInterface> resolveDottedAlignment(
		  const SiteContainerInterface& dottedAln,
		  std::shared_ptr<const Alphabet>& resolvedAlphabet);

  /**
   * @name Sequences coordinates.
   *
   * @see SequenceWalker For an alternative approach.
   * @{
   */

  /**
   * @brief Get the index of each sequence position in an aligned sequence.
   *
   * If the sequence contains no gap, the translated and the original positions are the same.
   * Position numbers start at 1.
   *
   * @param seq The sequence to translate.
   * @return A map with original sequence positions as keys, and translated positions as values.
   */
  static std::map<size_t, size_t> getSequencePositions(const Sequence& seq);

  /**
   * @brief Get the index of each alignment position in an aligned sequence.
   *
   * If the sequence contains no gap, the translated and the original positions are the same.
   * Position numbers start at 1.
   *
   * @param seq The sequence to translate.
   * @return A map with original alignement positions as keys, and translated positions as values.
   */
  static std::map<size_t, size_t> getAlignmentPositions(const Sequence& seq);

  /**
   * @brief Fill a numeric matrix with the size of the alignment, containing the each sequence position.
   *
   * Positions start at 1, gaps have "position" 0.
   *
   * @param sites The input alignment.
   * @param positions A matrix object which is going to be resized and filled with the corresponding positions.
   * @author Julien Dutheil
   */
  static void getSequencePositions(
		  const SiteContainerInterface& sites,
		  Matrix<size_t>& positions);
  /** @} */

  /**
   * @brief Translate alignement positions from an aligned sequence to the same sequence in a different alignment.
   *
   * Takes each position (starting at 1) in sequence 1, and look for the corresponding position in sequence 2.
   * The two sequences must be the same, excepted for the gaps.
   * If no sequence contains gaps, or if the gaps are at the same place in both sequences, the translated postion will be the same as the original positions.
   *
   * @param seq1 The sequence to translate.
   * @param seq2 The reference sequence.
   * @return A map with original alignement positions as keys, and translated positions as values.
   * @throw AlphabetMismatchException If the sequences do not share the same alphabet.
   * @throw Exception If the sequence do not match.
   */
  static std::map<size_t, size_t> translateAlignment(
		  const Sequence& seq1,
		  const Sequence& seq2);

  /**
   * @brief Translate sequence positions from a sequence to another in the same alignment.
   *
   * Takes each position (starting at 1) in sequence 1, and look for the corresponding position in sequence 2 at the same site.
   * If no corresponding position is available (i.e. if there is a gap in sequence 2 at the corresponding position), 0 is returned.
   *
   * @param sequences The alignment to use.
   * @param i1 The index of the sequence to translate.
   * @param i2 The index of the reference sequence.
   * @return A map with original sequence positions as keys, and translated positions as values.
   */
  static std::map<size_t, size_t> translateSequence(
		  const SiteContainerInterface& sequences,
		  size_t i1,
		  size_t i2);

  /**
   * @brief Align two sequences using the Needleman-Wunsch dynamic algorithm.
   *
   * If the input sequences contain gaps, they will be ignored.
   *
   * @see BLOSUM50, DefaultNucleotideScore for score matrices.
   *
   * @param seq1 The first sequence.
   * @param seq2 The second sequence.
   * @param s The score matrix to use.
   * @param gap Gap penalty.
   * @return A new SiteContainer instance.
   * @throw AlphabetMismatchException If the sequences and the score matrix do not share the same alphabet.
   */
  static std::unique_ptr<AlignedSequenceContainer> alignNW(
		  const Sequence& seq1,
		  const Sequence& seq2,
		  const AlphabetIndex2& s,
		  double gap);

  /**
   * @brief Align two sequences using the Needleman-Wunsch dynamic algorithm.
   *
   * If the input sequences contain gaps, they will be ignored.
   *
   * @see BLOSUM50, DefaultNucleotideScore for score matrices.
   *
   * @param seq1 The first sequence.
   * @param seq2 The second sequence.
   * @param s The score matrix to use.
   * @param opening Gap opening penalty.
   * @param extending Gap extending penalty.
   * @return A new SiteContainer instance.
   * @throw AlphabetMismatchException If the sequences and the score matrix do not share the same alphabet.
   */
  static std::unique_ptr<AlignedSequenceContainer> alignNW(
		  const Sequence& seq1,
		  const Sequence& seq2,
		  const AlphabetIndex2& s,
		  double opening,
		  double extending);

  /**
   * @brief Sample sites in an alignment.
   *
   * Original site positions will be kept. The resulting container will hence probably have duplicated
   * positions. You may wish to call the reindexSites() method on the returned container.
   *
   * Note: This method will be optimal with a container with vertical storage like VectorSiteContainer.
   *
   * @param sites An input alignment to sample.
   * @param nbSites The size of the resulting container.
   * @param index [out] If non-null the underlying vector will be appended with the original site indices.
   * @param outSites A container where the sample will be added.
   */
  template<class SiteType, class SequenceType, class HashType>
  static void sampleSites(
    const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites,
    size_t nbSites,
    TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& outSites,
    std::shared_ptr< std::vector<size_t> > index = nullptr)
  {
    for (size_t i = 0; i < nbSites; ++i) {
      size_t pos = static_cast<size_t>(RandomTools::giveIntRandomNumberBetweenZeroAndEntry(static_cast<int>(sites.getNumberOfSites())));
      auto s = std::unique_ptr<SiteType>(sites.site(pos).clone());
      outSites.addSite(s, false);

      if (index)
        index->push_back(pos);
    }
  }



  /**
   * @brief Sample sites in an alignment.
   *
   * Original site positions will be kept. The resulting container will hence probably have duplicated
   * positions. You may wish to call the reindexSites() method on the returned container.
   *
   * Note: This method will be optimal with a container with vertical storage like VectorSiteContainer.
   *
   * @param sites An input alignment to sample.
   * @param nbSites The size of the resulting container.
   * @param index [out] If non-null the underlying vector will be appended with the original site indices.
   * @return A container with the sampled sites.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr< TemplateVectorSiteContainer<SiteType, SequenceType> >
  sampleSites(
      const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites,
      size_t nbSites,
      std::shared_ptr< std::vector<size_t> > index = nullptr)
  {
    auto sampledSites = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(sites.getAlphabet());
    sampleSites<SiteType, SequenceType, std::string>(sites, nbSites, *sampledSites, index);
    return sampledSites;   
  }



  /**
   * @brief Bootstrap sites in an alignment.
   *
   * Original site positions will be kept. The resulting container will hence probably have duplicated
   * positions. You may wish to call the reindexSites() method on the returned container.
   *
   * Note: This method will be optimal with a container with vertical storage like VectorSiteContainer.
   *
   * @param sites An input alignment to sample.
   * @param outputSites A container that will contain the sampled alignment.
   */
  template<class SiteType, class SequenceType, class HashType>
  static void bootstrapSites(
		  const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& sites,
		  TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& outputSites)
  {
    sampleSites(sites, sites.getNumberOfSites(), outputSites, nullptr);
  }



  /**
   * @brief Bootstrap sites in an alignment.
   *
   * Original site positions will be kept. The resulting container will hence probably have duplicated
   * positions. You may wish to call the reindexSites() method on the returned container.
   *
   * Note: This method will be optimal with a container with vertical storage like VectorSiteContainer.
   *
   * @param sites An input alignment to sample.
   * @return A container that contains the sampled alignment.
   */
  template<class SiteType, class SequenceType>
  static std::unique_ptr< TemplateVectorSiteContainer<SiteType, SequenceType> > 
  bootstrapSites(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>& sites)
  {
    auto outputSites = std::make_unique< TemplateVectorSiteContainer<SiteType, SequenceType> >(sites.getAlphabet());
    bootstrapSites<SiteType, SequenceType, std::string>(sites, *outputSites);
    return outputSites;
  }



  /**
   * @brief Compute the similarity/distance score between two aligned sequences.
   *
   * The similarity measures are computed as the proportion of identical match.
   * The distance between the two sequences is defined as 1 - similarity.
   * This function can be used with any type of alphabet.
   *
   * @param seq1 The first sequence.
   * @param seq2 The second sequence.
   * @param dist Shall we return a distance instead of similarity?
   * @param gapOption How to deal with gaps:
   * - SIMILARITY_ALL: all positions are used.
   * - SIMILARITY_NODOUBLEGAP: ignore all positions with a gap in the two sequences.
   * - SIMILARITY_NOGAP: ignore all positions with a gap in at least one of the two sequences.
   * @param unresolvedAsGap Tell if unresolved characters must be considered as gaps when counting.
   * If set to yes, the gap option will also apply to unresolved characters.
   * @return The proportion of matches between the two sequences.
   * @throw SequenceNotAlignedException If the two sequences do not have the same length.
   * @throw AlphabetMismatchException If the two sequences do not share the same alphabet type.
   * @throw Exception If an invalid gapOption is passed.
   */
  static double computeSimilarity(
		  const SequenceInterface& seq1,
		  const SequenceInterface& seq2,
		  bool dist = false,
		  const std::string& gapOption = SIMILARITY_NODOUBLEGAP,
		  bool unresolvedAsGap = true);

  /**
   * @brief Compute the similarity matrix of an alignment.
   *
   * The similarity measures are computed as the proportion of identical match.
   * The distance between the two sequences is defined as 1 - similarity.
   * This function can be used with any type of alphabet.
   * Several options concerning gaps and unresolved characters are proposed:
   * - SIMILARITY_ALL: all positions are used.
   * - SIMILARITY_NOFULLGAP: ignore positions with a gap in all the sequences in the alignment.
   * - SIMILARITY_NODOUBLEGAP: ignore all positions with a gap in the two sequences for each pair.
   * - SIMILARITY_NOGAP: ignore all positions with a gap in at least one of the two sequences for each pair.
   *
   *
   * @see computeSimilarityMatrix
   *
   * @param sites The input alignment.
   * @param dist Shall we return a distance instead of similarity?
   * @param gapOption How to deal with gaps.
   * @param unresolvedAsGap Tell if unresolved characters must be considered as gaps when counting.
   * If set to yes, the gap option will also apply to unresolved characters.
   * @return All pairwise similarity measures.
   */
  static std::unique_ptr<DistanceMatrix> computeSimilarityMatrix(
		  const SiteContainerInterface& sites,
		  bool dist = false,
		  const std::string& gapOption = SIMILARITY_NOFULLGAP,
		  bool unresolvedAsGap = true);

  static const std::string SIMILARITY_ALL;
  static const std::string SIMILARITY_NOFULLGAP;
  static const std::string SIMILARITY_NODOUBLEGAP;
  static const std::string SIMILARITY_NOGAP;

  /**
   * @brief Add the content of a site container to an exhisting one.
   *
   * The input containers are supposed to have unique sequence names.
   * If it is not the case, several things can happen:
   * - If the two containers have exactly the same keys in the same order, then the content of the second one will be added as is to the first one.
   * - If the second container does not have exactly the same sequences keys or in a different order, then a reordered selection of the second contianer is created first,
   *   and in that case, only the first sequence with a given name will be used and duplicated.
   * In any case, note that the second container should always contains all the sequence names from the first one,
   * otherwise an exception will be thrown.
   *
   * @author Julien Dutheil
   *
   * @param seqCont1 First container.
   * @param seqCont2 Second container. This container must contain sequences with the same names as in seqcont1.
   * Additional sequences will be ignored.
   * @param leavePositionAsIs Tell is site position should be unchanged. Otherwise (the default) is to add the size of container 1 to the positions in container 2.
   * @throw AlphabetMismatchException If the alphabet in the 2 containers do not match.
   * @throw Exception If sequence names do not match.
   */
  template<class SiteType, class SequenceType, class HashType>
  static void merge(
		  TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& seqCont1, 
		  const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>& seqCont2,
		  bool leavePositionAsIs = false)
  {
    if (seqCont1.getAlphabet()->getAlphabetType() != seqCont2.getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("SiteContainerTools::merge.", seqCont1.getAlphabet(), seqCont2.getAlphabet());

    std::vector<HashType> seqKeys1 = seqCont1.getSequenceKeys();
    std::vector<HashType> seqKeys2 = seqCont2.getSequenceKeys();
    const TemplateSiteContainerInterface<SiteType, SequenceType, HashType>* seqCont2bis = 0;
    bool del = false;
    if (seqKeys1 == seqKeys2) {
      seqCont2bis = &seqCont2;
    } else {
      // We shall reorder sequences first:
      TemplateSiteContainerInterface<SiteType, SequenceType, HashType>* seqCont2ter = seqCont2.createEmptyContainer();
      SequenceContainerTools::getSelectedSequences(seqCont2, seqKeys1, *seqCont2ter);
      seqCont2bis = seqCont2ter;
      del = true;
    }

    if (leavePositionAsIs) {
      for (size_t i = 0; i < seqCont2bis->getNumberOfSites(); ++i) {
        seqCont1.addSite(seqCont2bis->site(i), false);
      }
    } else {
      int offset = static_cast<int>(seqCont1.getNumberOfSites());
      for (size_t i = 0; i < seqCont2bis->getNumberOfSites(); ++i) {
        seqCont1.addSite(seqCont2bis->site(i), offset + seqCont2bis->site(i).getPosition(), false);
      }
    }

    if (del)
      delete seqCont2bis;
  }



  /**
   * @brief Compare an alignment to a reference alignment, and compute the column scores.
   *
   * Calculations are made according to formula for the "CS" score in Thompson et al 1999, Nucleic Acids Research (1999):27(13);2682â2690.
   *
   * @param positions1 Alignment index for the test alignment.
   * @param positions2 Alignment index for the reference alignment.
   * @param na         The score to use if the tested column is full of gap.
   * @return A vector of score, as 0 or 1.
   * @see getSequencePositions for creating the alignment indexes.
   * @warning The indexes for the two alignments must have the sequences in the exact same order!
   * @author Julien Dutheil
   */
  static std::vector<int> getColumnScores(const Matrix<size_t>& positions1, const Matrix<size_t>& positions2, int na = 0);

  /**
   * @brief Compare an alignment to a reference alignment, and compute the sum-of-pairs scores.
   *
   * Calculations are made according to formula for the "SPS" score in Thompson et al 1999, Nucleic Acids Research (1999):27(13);2682â2690.
   *
   * @param positions1 Alignment index for the test alignment.
   * @param positions2 Alignment index for the reference alignment.
   * @param na         The score to use if the tested column is not testable, that is not containing at least to residues.
   * @return A vector of score, between 0 and 1 (+ na value).
   * @see getSequencePositions for creating the alignment indexes.
   * @warning The indexes for the two alignments must have the sequences in the exact same order!
   * @author Julien Dutheil
   */
  static std::vector<double> getSumOfPairsScores(const Matrix<size_t>& positions1, const Matrix<size_t>& positions2, double na = 0);
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SITECONTAINERTOOLS_H
