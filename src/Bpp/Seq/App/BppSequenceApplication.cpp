//
// File: BppSequenceApplication.cpp
// Authors:
//   Laurent GuÃ©guen, Julien Dutheil
// Created: 2021-06-14 09:57:00
//

/*
  Copyright or Â© or Copr. Development Team, (November 17, 2021)
  
  This software is a computer program whose purpose is to provide basal and
  utilitary classes. This file belongs to the Bio++ Project.
  
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

map<size_t, unique_ptr<AlignmentDataInterface<std::string> > >
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

    map<size_t, unique_ptr<AlignmentDataInterface<std::string> > > mSites2;
    for (auto& sites : mSites) {
      mSites2.emplace(sites.first, unique_ptr< AlignmentDataInterface<string> >(sites.second.release()));
    }

    return mSites2;
  }
  catch(Exception& e)
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

    map<size_t, unique_ptr<AlignmentDataInterface<std::string> > > mSites2;
    for (auto& sites : mSites) {
      mSites2.emplace(sites.first, unique_ptr< AlignmentDataInterface<string> >(sites.second.release()));
    }

    return mSites2;
  }
}

map<size_t, unique_ptr<const AlignmentDataInterface<std::string> > >
BppSequenceApplication::getConstAlignmentsMap(
  shared_ptr<const Alphabet>& alphabet,
  bool changeGapsToUnknownCharacters,
  bool optionalData,
  const std::string& prefix,
  const std::string& suffix,
  bool suffixIsOptional) const
{
  auto mSites = getAlignmentsMap(alphabet, changeGapsToUnknownCharacters, optionalData, prefix, suffix, suffixIsOptional);

  map<size_t, unique_ptr<const AlignmentDataInterface<std::string> > > mSitesConst;
  for (auto& sites : mSites) {
    mSitesConst[sites.first] = std::move(sites.second);
  }

  return mSitesConst;
}

