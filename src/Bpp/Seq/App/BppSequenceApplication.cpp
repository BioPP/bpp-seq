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

Alphabet* BppSequenceApplication::getAlphabet(
  const string& suffix,
  bool suffixIsOptional,
  bool allowGeneric) const
{
  return SequenceApplicationTools::getAlphabet(params_, suffix, suffixIsOptional, verbose_, allowGeneric, warn_);
}

GeneticCode* BppSequenceApplication::getGeneticCode(
  const Alphabet* alphabet,
  const string& suffix,
  bool suffixIsOptional) const
{
  const CodonAlphabet* codonAlphabet;

  codonAlphabet = dynamic_cast<const CodonAlphabet*>(alphabet);
  if (!codonAlphabet)
  {
    const AllelicAlphabet* allAlp = dynamic_cast<const AllelicAlphabet*>(alphabet);
    if (allAlp)
      codonAlphabet = dynamic_cast<const CodonAlphabet*>(&allAlp->getStateAlphabet());
  }
  
  if (codonAlphabet)
  {
    string codeDesc = ApplicationTools::getStringParameter("genetic_code", params_, "Standard", suffix, suffixIsOptional, warn_);
    if (verbose_)
      ApplicationTools::displayResult("Genetic Code", codeDesc);
    return SequenceApplicationTools::getGeneticCode(codonAlphabet->shareNucleicAlphabet(), codeDesc);
  }
  else
    return nullptr;
}

map<size_t, AlignedValuesContainer*> BppSequenceApplication::getAlignmentsMap(
  const Alphabet* alphabet,
  bool changeGapsToUnknownCharacters,
  bool optionalData,
  const std::string& prefix,
  const std::string& suffix,
  bool suffixIsOptional) const
{
  auto mSites = SequenceApplicationTools::getAlignedContainers(alphabet, params_, prefix, suffix, suffixIsOptional, verbose_, warn_);

  if (!optionalData && mSites.size() == 0)
    throw Exception("Missing data input.sequence.file option");

  if (changeGapsToUnknownCharacters)
    for (auto itc : mSites)
    {
      SiteContainerTools::changeGapsToUnknownCharacters(*itc.second);
    }

  for (auto& sites:mSites)
  {
    if (sites.second->getNumberOfSites() == 0)
      throw Exception("Empty alignment number " + TextTools::toString(sites.first));
  }

  return mSites;
}

map<size_t, const AlignedValuesContainer*> BppSequenceApplication::getConstAlignmentsMap(
  const Alphabet* alphabet,
  bool changeGapsToUnknownCharacters,
  bool optionalData,
  const std::string& prefix,
  const std::string& suffix,
  bool suffixIsOptional) const
{
  map<size_t, AlignedValuesContainer*>  mSites;
  
  mSites = getAlignmentsMap(alphabet, changeGapsToUnknownCharacters, optionalData, prefix, suffix, suffixIsOptional);

  map<size_t, const AlignedValuesContainer*> mSitesconst;
  for (auto it:mSites)
    mSitesconst[it.first]=it.second;

  return mSitesconst;
}
