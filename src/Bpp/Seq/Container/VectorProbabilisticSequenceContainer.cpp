//
// File: VectorProbabilisticSequenceContainer.cpp
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
// Last modified: 2003-07-30 00:00:00
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

#include <Bpp/Text/TextTools.h>

#include "VectorProbabilisticSequenceContainer.h"

using namespace bpp;
using namespace std;

/** Class constructors: *******************************************************/

VectorProbabilisticSequenceContainer::VectorProbabilisticSequenceContainer(
  const std::vector<std::shared_ptr<ProbabilisticSequence>>& vs,
  const Alphabet* alpha) :
  VectorMappedContainer<ProbabilisticSequence>(),
  AbstractValuesContainer(alpha)
{
  for (auto i = vs.begin(); i < vs.end(); i++)
    addSequence(*i);
}

/** Copy constructors: ********************************************************/

VectorProbabilisticSequenceContainer::VectorProbabilisticSequenceContainer(
  const VectorProbabilisticSequenceContainer& vsc) :
  VectorMappedContainer<ProbabilisticSequence>(),
  AbstractValuesContainer(vsc)
{
  size_t max = vsc.getNumberOfSequences();
  for (size_t i = 0; i < max; i++)
    addSequence(vsc.getSequence(i), false);

}

/** Assignation operator: *****************************************************/

VectorProbabilisticSequenceContainer& VectorProbabilisticSequenceContainer::operator=(
  const VectorProbabilisticSequenceContainer& vsc)
{
  clear();
  AbstractValuesContainer::operator=(vsc);
  VectorMappedContainer::operator=(vsc);

  return *this;
}

/******************************************************************************/

void VectorProbabilisticSequenceContainer::setSequenceNames(
  const std::vector<std::string>& names,
  bool checkNames)
{
  if (names.size() != getNumberOfSequences())
    throw IndexOutOfBoundsException("VectorProbabilisticSequenceContainer::setSequenceNames : bad number of names", names.size(), getNumberOfSequences(), getNumberOfSequences());
  if (checkNames)
  {
    for (size_t i = 0; i < names.size(); i++)
    {
      // For all names in vector : throw exception if name already exists
      for (size_t j = 0; j < i; j++)
      {
        if (names[j] == names[i])
          throw Exception("VectorSiteContainer::setSequenceNames : Sequence's name already exists in container");
      }
    }
  }
  for (size_t i = 0; i < names.size(); i++)
  {
    getSequence_(i).setName(names[i]);
  }

  setObjectNames(names);
}

/******************************************************************************/

VectorProbabilisticSequenceContainer* VectorProbabilisticSequenceContainer::createEmptyContainer() const
{
  VectorProbabilisticSequenceContainer* vsc = new VectorProbabilisticSequenceContainer(getAlphabet());
  vsc->setGeneralComments(getGeneralComments());
  return vsc;
}

/******************************************************************************/
