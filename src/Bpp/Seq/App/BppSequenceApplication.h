//
// File: BppSequenceApplication.h
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
#include "SequenceApplicationTools.h"
#include "../Container/AlignedValuesContainer.h"

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
  virtual Alphabet* getAlphabet(
    const string& suffix = "",
    bool suffixIsOptional = true,
    bool allowGeneric = false) const;

  /*
   * @brief get the GeneticCode
   */
  virtual GeneticCode* getGeneticCode(
    const Alphabet* alphabet,
    const string& suffix = "",
    bool suffixIsOptional = true) const;

  /*
   * @brief Get the std::map of alignments
   */
  virtual std::map<size_t, AlignedValuesContainer*> getAlignmentsMap(
    const Alphabet* alphabet,
    bool changeGapsToUnknownCharacters = true,
    bool optionalData = false,
    const std::string& prefix = "input.",
    const std::string& suffix = "",
    bool suffixIsOptional = true) const;
};
} // end of namespace bpp;
#endif // BPP_SEQ_APP_BPPSEQUENCEAPPLICATION_H
