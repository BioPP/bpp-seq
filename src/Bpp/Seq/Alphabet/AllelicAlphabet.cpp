// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "AllelicAlphabet.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

AllelicAlphabet::AllelicAlphabet(std::shared_ptr<const Alphabet> alph, unsigned int nbAlleles) :
  alph_(alph),
  nbAlleles_(nbAlleles),
  nbUnknown_(0)
{
  if (nbAlleles_ <= 1)
    throw BadIntException((int)nbAlleles_, "AllelicAlphabet::AllelicAlphabet : wrong number of alleles", this);

  unsigned int size = alph_->getSize();

  auto sword = alph_->getStateCodingSize();
  auto snb = std::to_string(nbAlleles_).size();

  // Gap is such as "-6-0"

  string gapchar = alph_->getState(alph_->getGapCharacterCode()).getLetter();

  auto gapword =  gapchar + std::string("0", snb);

  registerState(new AlphabetState(-1, gapchar + std::to_string(nbAlleles_) + gapword, "gap"));


  // Monomorphic states are such as "A6-0"

  for (int i = 0; i < static_cast<int>(size); ++i)
  {
    auto desc = alph_->intToChar(i) + std::to_string(nbAlleles_) + gapword;
    registerState(new AlphabetState(i, desc, desc));
  }

  // Polymorphic states are such as "A4C2"

  for (int i = 0; i < static_cast<int>(size) - 1; ++i)
  {
    for (int j = i + 1; j < static_cast<int>(size); ++j)
    {
      auto nbl = (i * static_cast<int>(size) + j) * static_cast<int>(nbAlleles_ - 1) + static_cast<int>(size);
      for (int nba = 1; nba < static_cast<int>(nbAlleles_); ++nba)
      {
        auto sni = std::string("0", snb - std::to_string(static_cast<int>(nbAlleles_) - nba).size()) + std::to_string(static_cast<int>(nbAlleles_) - nba);
        auto snj = std::string("0", snb - std::to_string(nba).size()) + std::to_string(nba);
        auto desc = alph_->intToChar(i) + sni + alph_->intToChar(j) + snj;
        registerState(new AlphabetState(static_cast<int>(nbl) + nba - 1, desc, desc));
      }
    }
  }

  // Unknown

  nbUnknown_ = (int)(size * size * (nbAlleles_ - 1));

  auto desc = std::string("?", sword) + std::to_string(nbAlleles_) + gapword;
  registerState(new AlphabetState(nbUnknown_, desc, desc));
}

/******************************************************************************/

std::string AllelicAlphabet::getAlphabetType() const
{
  return "Allelic(alphabet=" + alph_->getAlphabetType() + ",nbAlleles_=" + std::to_string(nbAlleles_) + ")";
}


// /******************************************************************************/

bool AllelicAlphabet::isResolvedIn(int state1, int state2) const
{
  if (!isIntInAlphabet(state1))
    throw BadIntException(state1, "AllelicAlphabet::isResolvedIn(int, int): Specified base unknown.", this);

  if (!isIntInAlphabet(state2))
    throw BadIntException(state2, "AllelicAlphabet::isResolvedIn(int, int): Specified base unknown.", this);

  if (isUnresolved(state2))
    throw BadIntException(state2, "AllelicAlphabet::isResolvedIn(int, int): Unresolved base.", this);

  auto size = alph_->getSize();
  return (static_cast<unsigned int>(state1) == size * size * (nbAlleles_ - 1)) ? (state2 >= 0) : (state1 == state2);
}

/******************************************************************************/

std::vector<int> AllelicAlphabet::getAlias(int state) const
{
  if (!isIntInAlphabet(state))
    throw BadIntException(state, "AllelicAlphabet::getAlias(int): Specified base unknown.", this);

  if (state == nbUnknown_)
    return getSupportedInts();
  else
    return vector<int>(1, state);
}

/******************************************************************************/

std::vector<std::string> AllelicAlphabet::getAlias(const std::string& state) const
{
  string locstate = TextTools::toUpper(state);
  if (!isCharInAlphabet(locstate))
    throw BadCharException(locstate, "AllelicAlphabet::getAlias(string): Specified base unknown.", this);

  if (state == getState(nbUnknown_).getLetter())
    return getSupportedChars();
  else
    return vector<string>(1, state);
}

/******************************************************************************/

std::unique_ptr<ProbabilisticSequence> AllelicAlphabet::convertFromStateAlphabet(const CoreSequenceInterface& sequence) const
{
  auto seq = dynamic_cast<const SequenceInterface*>(&sequence);
  auto probseq = dynamic_cast<const ProbabilisticSequenceInterface*>(&sequence);

  if (!seq && !probseq)
    throw Exception("AllelicAlphabet::convertFromStateAlphabet: unknown type for sequence: " + sequence.getName());

  auto alphabet = seq ? seq->getAlphabet() : probseq->getAlphabet();

  if (alphabet->getAlphabetType() != getStateAlphabet()->getAlphabetType())
    throw AlphabetMismatchException("AllelicAlphabet::convertFromStateAlphabet", getStateAlphabet().get(), alphabet.get());

  auto alphaPtr = shared_from_this();
  auto tSeq = make_unique<ProbabilisticSequence>(alphaPtr);
  tSeq->setName(sequence.getName());
  tSeq->setComments(sequence.getComments());

  auto tt = tSeq->getTable();

  auto size = seq ? seq->size() : probseq->size();

  auto alphasize = alphabet->getSize();

  int gap = alphabet->getGapCharacterCode();

  Vdouble tstate(alphasize);
  Vdouble likelihood(getSize());

  for (unsigned int pos = 0; pos < size; ++pos)
  {
    // first observed values
    if (seq)
    {
      int state = seq->getValue(pos);
      if (state == gap)
      {
        for (size_t a = 0; a < alphasize; ++a)
        {
          tstate[a] = 1;
        }
        tSeq->addElement(tstate);
        continue; // no binomial calculation
      }
      else
      {
        for (size_t a = 0; a < alphasize; a++)
        {
          tstate[a] = 0;
        }
        tstate[(size_t)state] = 1;
      }
    }
    else
      tstate = probseq->getValue(pos);

    computeLikelihoods(tstate, likelihood);
    tSeq->addElement(likelihood);
  }

  return tSeq;
}


void AllelicAlphabet::computeLikelihoods(const Vdouble& counts, Vdouble& likelihoods) const
{
  auto alphasize = getStateAlphabet()->getSize();
  if (counts.size() != alphasize)
    throw BadSizeException("AllelicAlphabet::computeLikelihoods: bad vector size for counts.", alphasize, counts.size());

  likelihoods.resize(getSize(), 0);

  // vector of bool if strictly positive counts
  std::vector<bool> presence(alphasize);
  for (size_t state = 0; state < alphasize; ++state)
  {
    presence[state] = (counts[state] > NumConstants::TINY());
  }

  // Monomorphic states (likelihood = 1 or 0)
  for (size_t state = 0; state < alphasize; ++state)
  {
    if (presence[state])
    {
      likelihoods[state] = 1;
      for (size_t ns = 0; ns < alphasize; ns++)
      {
        if (ns == state)
          continue;

        if (presence[ns]) // because polymorphic state
        {
          likelihoods[state] = 0;
          break;
        }
      }
    }
    else
      likelihoods[state] = 0;
  }

  // Polymorphic states are such as "A4C2"

  auto numetat = alphasize;
  for (unsigned int i = 0; i < alphasize - 1; ++i) // A
  {
    for (unsigned int j = i + 1; j < alphasize; ++j) // C
    {
      bool todo = true;
      for (unsigned int ns = 0; ns < alphasize; ++ns)
      {
        if ((ns == i) || (ns == j))
          continue;
        if (presence[ns]) // non null mismatch count
        {
          for (unsigned int nba = 1; nba < nbAlleles_; ++nba)
          {
            likelihoods[numetat] = 0;
            numetat++;
          }
          todo = false;
          break;
        }
      }

      if (todo)
      {
        auto lnorm = std::lgamma(counts[size_t(i)] + counts[size_t(j)] + 1) - std::lgamma(counts[size_t(i)] + 1) - std::lgamma(counts[size_t(j)] + 1);
        for (size_t nba = 1; nba < nbAlleles_; nba++)
        {
          auto lprob = std::log((double)(nbAlleles_ - nba) / nbAlleles_) * counts[size_t(i)] + std::log((double)nba / nbAlleles_) * counts[size_t(j)];
          likelihoods[numetat] = std::exp(lprob + lnorm);
          if (likelihoods[numetat] < NumConstants::TINY())
            likelihoods[numetat] = 0;
          numetat++;
        }
      }
    }
  }
}
