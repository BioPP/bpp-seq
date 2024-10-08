// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "NucleicAcidsReplication.h"

using namespace bpp;

using namespace std;

NucleicAcidsReplication::NucleicAcidsReplication(
    std::shared_ptr<const NucleicAlphabet> nuc1,
    std::shared_ptr<const NucleicAlphabet> nuc2) :
  nuc1_(nuc1), nuc2_(nuc2), trans_()
{
  trans_[-1] = -1;
  trans_[0] = 3;
  trans_[1] = 2;
  trans_[2] = 1;
  trans_[3] = 0;

  trans_[4] = 9;
  trans_[5] = 8;
  trans_[6] = 6;
  trans_[7] = 7;
  trans_[8] = 5;
  trans_[9] = 4;

  trans_[10] = 13;
  trans_[11] = 12;
  trans_[12] = 11;
  trans_[13] = 10;

  trans_[14] = 14;
}

int NucleicAcidsReplication::translate(int state) const
{
  nuc1_->intToChar(state);
  return trans_[state];
}

std::string NucleicAcidsReplication::translate(const std::string& state) const
{
  int i = nuc1_->charToInt(state);
  return nuc2_->intToChar(trans_[i]);
}

unique_ptr<Sequence> NucleicAcidsReplication::translate(const SequenceInterface& sequence) const
{
  if (sequence.getAlphabet()->getAlphabetType() != getSourceAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("NucleicAcidsReplication::translate", getSourceAlphabet(), getTargetAlphabet());
  auto alphaPtr = getTargetAlphabet();
  auto tSeq = make_unique<Sequence>(sequence.getName(), "", sequence.getComments(), alphaPtr);
  for (size_t i = 0; i < sequence.size(); ++i)
  {
    tSeq->addElement(translate(sequence.getValue(i)));
  }
  return tSeq;
}


int NucleicAcidsReplication::reverse(int state) const
{
  nuc2_->intToChar(state);
  return trans_[state];
}

std::string NucleicAcidsReplication::reverse(const std::string& state) const
{
  int i = nuc2_->charToInt(state);
  return nuc1_->intToChar(trans_[i]);
}

unique_ptr<Sequence> NucleicAcidsReplication::reverse(const SequenceInterface& sequence) const
{
  if (sequence.getAlphabet()->getAlphabetType() != getTargetAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("NucleicAcidsReplication::reverse", getSourceAlphabet(), getTargetAlphabet());
  auto alphaPtr = getSourceAlphabet();
  auto rSeq = make_unique<Sequence>(sequence.getName(), "", sequence.getComments(), alphaPtr);
  for (size_t i = 0; i < sequence.size(); ++i)
  {
    rSeq->addElement(reverse(sequence.getValue(i)));
  }
  return rSeq;
}
