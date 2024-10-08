// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>
#include <iostream>
#include <typeinfo>

#include "NumericAlphabet.h" // class's header file

using namespace std;
using namespace bpp;

/****************************************************************************************/

NumericAlphabet::NumericAlphabet(const UniformDiscreteDistribution& pd) :
  AbstractAlphabet(), pdd_(pd.clone()), values_()
{
  // Alphabet size definition
  size_t size = pdd_->getNumberOfCategories();

  Vdouble vd = pdd_->getCategories();

  // gap
  registerState(new AlphabetNumericState(-1, 0, "-", "gap"));

  for (size_t i = 0; i < size; ++i)
  {
    registerState(new AlphabetNumericState(static_cast<int>(i), vd[i], TextTools::toString(vd[i]), TextTools::toString(vd[i])));
  }
}


NumericAlphabet::NumericAlphabet(const NumericAlphabet& na) :
  AbstractAlphabet(na),
  pdd_(na.pdd_->clone()),
  values_(na.values_)
{}

NumericAlphabet& NumericAlphabet::operator=(const NumericAlphabet& na)
{
  AbstractAlphabet::operator=(na);
  pdd_ = na.pdd_->clone();
  return *this;
}

/****************************************************************************************/

void NumericAlphabet::setState(size_t stateIndex, AlphabetState* st)
{
  try
  {
    AbstractAlphabet::setState(stateIndex, st);
    double x = dynamic_cast<AlphabetNumericState*>(st)->getValue();
    if (values_.find(x) == values_.end())
      values_[x] = stateIndex;
  }
  catch (std::bad_cast&)
  {
    throw Exception("NumericAlphabet::setState. Incorrect alphabet type.");
  }
}

void NumericAlphabet::registerState(AlphabetState* st)
{
  try
  {
    AbstractAlphabet::registerState(st);
    double x = dynamic_cast<AlphabetNumericState*>(st)->getValue();
    if (values_.find(x) == values_.end())
      values_[x] = getSize();
  }
  catch (std::bad_cast&)
  {
    throw Exception("NumericAlphabet::registerState. Incorrect alphabet type.");
  }
}

vector<int> NumericAlphabet::getAlias(int state) const
{
  vector<int> v(1);
  v[0] = state;
  return v;
}

vector<string> NumericAlphabet::getAlias(const string& state) const
{
  vector<string> v(1);
  v[0] = state;
  return v;
}

/****************************************************************************************/

bool NumericAlphabet::isUnresolved(const string& state) const
{
  return false;
}

bool NumericAlphabet::isUnresolved(int state) const
{
  return false;
}

unsigned int NumericAlphabet::getSize() const
{
  return static_cast<unsigned int>(values_.size());
}

unsigned int NumericAlphabet::getNumberOfTypes() const
{
  return static_cast<unsigned int>(values_.size());
}

void NumericAlphabet::remap()
{
  AbstractAlphabet::remap();
  values_.clear();
  for (size_t pos = 0; pos < getSize(); pos++)
  {
    double x = getStateAt(pos).getValue();
    if (values_.find(x) == values_.end())
      values_[x] = pos;
  }
}

/************************************************************/

double NumericAlphabet::getDelta() const
{
  return (pdd_->getUpperBound() - pdd_->getLowerBound()) / static_cast<double>(pdd_->getNumberOfCategories());
}

double NumericAlphabet::intToValue(int state) const
{
  return static_cast<const AlphabetNumericState&>(getState(state)).getValue();
}

size_t NumericAlphabet::getValueIndex(double value) const
{
  map<double, size_t>::const_iterator it = values_.find(pdd_->getValueCategory(value));
  return it->second;
}

AlphabetNumericState& NumericAlphabet::getStateAt(size_t stateIndex)
{
  return static_cast<AlphabetNumericState&>(AbstractAlphabet::getStateAt(stateIndex));
}

const AlphabetNumericState& NumericAlphabet::getStateAt(size_t stateIndex) const
{
  return static_cast<const AlphabetNumericState&>(AbstractAlphabet::getStateAt(stateIndex));
}
