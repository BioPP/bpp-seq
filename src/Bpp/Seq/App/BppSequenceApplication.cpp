// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

// From the STL:
#include <iostream>
#include <iomanip>
#include <limits>

#include "BppSequenceApplication.h"

#include "../Alphabet/AlphabetTools.h"
#include "../Container/SiteContainerTools.h"

using namespace std;
using namespace bpp;

/******************************************************************************/

shared_ptr<Alphabet> BppSequenceApplication::getAlphabet(
  const string& suffix,
  bool suffixIsOptional,
  bool allowGeneric) const
{
  return SequenceApplicationTools::getAlphabet(params_, suffix, suffixIsOptional, verbose_, allowGeneric, warn_);
}

shared_ptr<GeneticCode> BppSequenceApplication::getGeneticCode(
  shared_ptr<const Alphabet>& alphabet,
  const string& suffix,
  bool suffixIsOptional) const
{
  shared_ptr<const CodonAlphabet> codonAlphabet = dynamic_pointer_cast<const CodonAlphabet>(alphabet);

  if (!codonAlphabet)
  {
    auto allAlp = dynamic_pointer_cast<const AllelicAlphabet>(alphabet);
    if (allAlp)
      codonAlphabet = dynamic_pointer_cast<const CodonAlphabet>(allAlp->getStateAlphabet());
  }

  if (codonAlphabet)
  {
    string codeDesc = ApplicationTools::getStringParameter("genetic_code", params_, "Standard", suffix, suffixIsOptional, warn_);
    if (verbose_)
      ApplicationTools::displayResult("Genetic Code", codeDesc);
    auto alphaPtr = codonAlphabet->getNucleicAlphabet();
    return SequenceApplicationTools::getGeneticCode(alphaPtr, codeDesc);
  }
  else
    return nullptr;
}

map<size_t, unique_ptr<AlignmentDataInterface> >
BppSequenceApplication::getAlignmentsMap(
  shared_ptr<const Alphabet>& alphabet,
  bool changeGapsToUnknownCharacters,
  bool optionalData,
  const std::string& prefix,
  const std::string& suffix,
  bool suffixIsOptional) const
{
  try
  {
    auto mSites = SequenceApplicationTools::getProbabilisticSiteContainers(alphabet, params_, prefix, suffix, suffixIsOptional, verbose_, warn_);

    if (!optionalData && mSites.size() == 0)
      throw Exception("Missing data input.sequence.file option");

    if (changeGapsToUnknownCharacters) {
      for (auto& sites : mSites) {
        SiteContainerTools::changeGapsToUnknownCharacters(*sites.second);
      }
    }

    for (auto& sites : mSites) {
      if (sites.second->getNumberOfSites() == 0)
        throw Exception("Empty alignment number " + TextTools::toString(sites.first));
    }

    map<size_t, unique_ptr<AlignmentDataInterface> > mSites2;
    for (auto& sites : mSites) {
      mSites2.emplace(sites.first, unique_ptr<AlignmentDataInterface>(sites.second.release()));
    }

    return mSites2;
  }
  catch(IOException& e)
  {
    auto mSites = SequenceApplicationTools::getSiteContainers(alphabet, params_, prefix, suffix, suffixIsOptional, verbose_, warn_);

    if (!optionalData && mSites.size() == 0)
      throw Exception("Missing data input.sequence.file option");

    if (changeGapsToUnknownCharacters) {
      for (auto& itc : mSites) {
        SiteContainerTools::changeGapsToUnknownCharacters(*itc.second);
      }
    }

    for (auto& sites:mSites) {
      if (sites.second->getNumberOfSites() == 0)
        throw Exception("Empty alignment number " + TextTools::toString(sites.first));
    }

    map<size_t, unique_ptr<AlignmentDataInterface> > mSites2;
    for (auto& sites : mSites) {
      mSites2.emplace(sites.first, unique_ptr<AlignmentDataInterface>(sites.second.release()));
    }

    return mSites2;
  }
}

map<size_t, unique_ptr<const AlignmentDataInterface> >
BppSequenceApplication::getConstAlignmentsMap(
  shared_ptr<const Alphabet>& alphabet,
  bool changeGapsToUnknownCharacters,
  bool optionalData,
  const std::string& prefix,
  const std::string& suffix,
  bool suffixIsOptional) const
{
  auto mSites = getAlignmentsMap(alphabet, changeGapsToUnknownCharacters, optionalData, prefix, suffix, suffixIsOptional);

  map<size_t, unique_ptr<const AlignmentDataInterface> > mSitesConst;
  for (auto& sites : mSites) {
    mSitesConst[sites.first] = std::move(sites.second);
  }

  return mSitesConst;
}

