//
// File VectorSequenceContainer.cpp
// Author : Guillaume Deuchst
//          Julien Dutheil
// Last modification : Wednesday July 30 2003
//

/*
   Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#include "VectorSequenceContainer.h"
#include <Bpp/Text/TextTools.h>

using namespace bpp;
using namespace std;

/** Class constructors: *******************************************************/

VectorSequenceContainer::VectorSequenceContainer(
  const std::vector<const Sequence*>& vs,
  const Alphabet* alpha) :
  AbstractSequenceContainer(alpha),
  VectorMappedContainer<Sequence>()
{
  for (auto i = vs.begin(); i < vs.end(); i++)
    addSequence(**i);
}

/** Copy constructors: ********************************************************/

VectorSequenceContainer::VectorSequenceContainer(
  const VectorSequenceContainer& vsc) :
  AbstractSequenceContainer(vsc),
  VectorMappedContainer<Sequence>()
{
  size_t max = vsc.getNumberOfSequences();
  for (size_t i = 0; i < max; i++)
  {
    addSequence(vsc.getSequence(i), false);
  }
}

VectorSequenceContainer::VectorSequenceContainer(
  const OrderedSequenceContainer& osc) :
  AbstractSequenceContainer(osc.getAlphabet()),
  VectorMappedContainer<Sequence>()
{
  // Sequences insertion
  for (unsigned int i = 0; i < osc.getNumberOfSequences(); i++)
  {
    addSequence(osc.getSequence(i), false);
  }
}

VectorSequenceContainer::VectorSequenceContainer(
  const SequenceContainer& sc) :
  AbstractSequenceContainer(sc.getAlphabet()),
  VectorMappedContainer<Sequence>()
{
  // Sequences insertion
  std::vector<std::string> names = sc.getSequencesNames();
  for (unsigned int i = 0; i < names.size(); i++)
  {
    addSequence(sc.getSequence(names[i]), false);
  }

  setGeneralComments(sc.getGeneralComments());
}

/** Assignation operator: *****************************************************/

VectorSequenceContainer& VectorSequenceContainer::operator=(
  const VectorSequenceContainer& vsc)
{
  clear();
  AbstractSequenceContainer::operator=(vsc);
  VectorMappedContainer::operator=(vsc);

  return *this;
}

VectorSequenceContainer& VectorSequenceContainer::operator=(
  const OrderedSequenceContainer& osc)
{
  clear();
  AbstractSequenceContainer::operator=(osc);

  // Sequences insertion
  size_t max = osc.getNumberOfSequences();
  for (unsigned int i = 0; i < max; i++)
  {
    addSequence(osc.getSequence(i), false);
  }

  return *this;
}

/******************************************************************************/

VectorSequenceContainer& VectorSequenceContainer::operator=(
  const SequenceContainer& sc)
{
  clear();
  AbstractSequenceContainer::operator=(sc);

  // Seq names:
  std::vector<std::string> names = sc.getSequencesNames();

  for (unsigned int i = 0; i < names.size(); i++)
  {
    addSequence(sc.getSequence(names[i]), false);
  }

  return *this;
}

/******************************************************************************/

void VectorSequenceContainer::setSequencesNames(
  const std::vector<std::string>& names,
  bool checkNames)
{
  if (names.size() != getNumberOfSequences())
    throw IndexOutOfBoundsException("VectorSequenceContainer::setSequenceNames : bad number of names", names.size(), getNumberOfSequences(), getNumberOfSequences());
  if (checkNames)
  {
    for (size_t i = 0; i < names.size(); i++)
    {
      // For all names in vector : throw exception if name already exists
      for (size_t j = 0; j < i; j++)
      {
        if (names[j] == names[i])
          throw Exception("VectorSiteContainer::setSequencesNames : Sequence's name already exists in container");
      }
    }
  }
  for (size_t i = 0; i < names.size(); i++)
    getSequence_(i).setName(names[i]);

  setObjectsNames(names);
}

/******************************************************************************/

VectorSequenceContainer* VectorSequenceContainer::createEmptyContainer() const
{
  VectorSequenceContainer* vsc = new VectorSequenceContainer(getAlphabet());
  vsc->setGeneralComments(getGeneralComments());
  return vsc;
}

/******************************************************************************/

