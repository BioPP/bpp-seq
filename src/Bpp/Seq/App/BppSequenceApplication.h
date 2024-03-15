// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_APP_BPPSEQUENCEAPPLICATION_H
#define BPP_SEQ_APP_BPPSEQUENCEAPPLICATION_H



// From the STL:
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

// From bpp-core:
#include <Bpp/App/BppApplication.h>

#include "../Alphabet/Alphabet.h"
#include "../Alphabet/CodonAlphabet.h"
#include "../Container/AlignmentData.h"
#include "../Container/SiteContainer.h"
#include "SequenceApplicationTools.h"

namespace bpp
{
class BppSequenceApplication :
  public virtual BppApplication
{
public:
  BppSequenceApplication(int argc, char* argv[], const std::string& name) :
    BppApplication(argc, argv, name)
  {}

public:
  /***************************************
   * @brief Methods to build objects
   *
   * @{
   */


  /*
   * @brief get the Alphabet
   */
  virtual std::shared_ptr<Alphabet> getAlphabet(
    const string& suffix = "",
    bool suffixIsOptional = true,
    bool allowGeneric = false) const;

  /*
   * @brief get the GeneticCode
   */
  virtual std::shared_ptr<GeneticCode> getGeneticCode(
    std::shared_ptr<const Alphabet>& alphabet,
    const string& suffix = "",
    bool suffixIsOptional = true) const;

  /*
   * @brief Get the std::map of alignments
   */
  virtual std::map<size_t, std::unique_ptr<AlignmentDataInterface> >
  getAlignmentsMap(
    std::shared_ptr<const Alphabet>& alphabet,
    bool changeGapsToUnknownCharacters = true,
    bool optionalData = false,
    const std::string& prefix = "input.",
    const std::string& suffix = "",
    bool suffixIsOptional = true) const;


  /*
   * @brief Get the std::map of const alignments
   */
  virtual std::map<size_t, std::unique_ptr<const AlignmentDataInterface> >
  getConstAlignmentsMap(
    std::shared_ptr<const Alphabet>& alphabet,
    bool changeGapsToUnknownCharacters = true,
    bool optionalData = false,
    const std::string& prefix = "input.",
    const std::string& suffix = "",
    bool suffixIsOptional = true) const;
};
  
} // end of namespace bpp;
#endif // BPP_SEQ_APP_BPPSEQUENCEAPPLICATION_H
