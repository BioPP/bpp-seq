// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Alphabet/AlphabetTools.h"
#include "../SequenceTools.h"
#include "GeneticCode.h"

using namespace bpp;
using namespace std;

/**********************************************************************************************/

StopCodonException::StopCodonException(const std::string& text, const std::string& codon) :
  Exception("StopCodonException: " + text + "(" + codon + ")"),
  codon_(codon) {}

/**********************************************************************************************/

int GeneticCode::translate(int state) const
{
  if (isStop(state))
    throw StopCodonException("GeneticCode::translate().", codonAlphabet_->intToChar(state));

  map<int, int>::const_iterator it = tlnTable_.find(state);
  if (it == tlnTable_.end())
    throw BadIntException(state, "GeneticCode::translate().", codonAlphabet_.get());

  return it->second;
}

/**********************************************************************************************/

std::string GeneticCode::translate(const std::string& state) const
{
  int x = codonAlphabet_->charToInt(state);
  return proteicAlphabet_->intToChar(translate(x));
}

/**********************************************************************************************/

vector<int> GeneticCode::getSynonymous(int aminoacid) const
{
  // test:
  proteicAlphabet_->intToChar(aminoacid);

  vector<int> synonyms;
  for (int i = 0; i < static_cast<int>(codonAlphabet_->getSize()); ++i)
  {
    try
    {
      if (translate(i) == aminoacid)
        synonyms.push_back(i);
    }
    catch (StopCodonException&)
    {}
  }
  return synonyms;
}

/**********************************************************************************************/

std::vector<std::string> GeneticCode::getSynonymous(const std::string& aminoacid) const
{
  // test:
  int aa = proteicAlphabet_->charToInt(aminoacid);

  vector<string> synonyms;
  for (int i = 0; i < static_cast<int>(codonAlphabet_->getSize()); ++i)
  {
    try
    {
      if (translate(i) == aa)
        synonyms.push_back(codonAlphabet_->intToChar(i));
    }
    catch (StopCodonException&)
    {}
  }
  return synonyms;
}

/**********************************************************************************************/

bool GeneticCode::isFourFoldDegenerated(int val) const
{
  if (isStop(val))
    return false;

  vector<int> codon = codonAlphabet_->getPositions(val);
  int acid = translate(val);

  // test all the substitution on third codon position
  for (int an = 0; an < 4; an++)
  {
    if (an == codon[2])
      continue;
    vector<int> mutcodon = codon;
    mutcodon[2] = an;
    int intcodon = codonAlphabet_->getCodon(mutcodon[0], mutcodon[1], mutcodon[2]);
    if (isStop(intcodon))
      return false;
    int altacid = translate(intcodon);
    if (altacid != acid)   // if non-synonymous
    {
      return false;
    }
  }

  return true;
}

/**********************************************************************************************/

unique_ptr<Sequence> GeneticCode::getCodingSequence(
    const SequenceInterface& sequence,
    bool lookForInitCodon,
    bool includeInitCodon) const
{
  size_t initPos = 0;
  size_t stopPos = sequence.size();
  if (AlphabetTools::isCodonAlphabet(sequence.alphabet()))
  {
    // Look for AUG(or ATG) codon:
    if (lookForInitCodon)
    {
      for (size_t i = 0; i < sequence.size(); i++)
      {
        vector<int> pos = codonAlphabet_->getPositions(sequence[i]);
        if (pos[0] == 0 && pos[1] == 3 && pos[2] == 2)
        {
          initPos = includeInitCodon ? i : i + 1;
          break;
        }
      }
    }
    // Look for stop codon:
    for (size_t i = initPos; i < sequence.size(); i++)
    {
      if (isStop(sequence[i]))
      {
        stopPos = i;
        break;
      }
    }
  }
  else if (AlphabetTools::isNucleicAlphabet(sequence.alphabet()))
  {
    // Look for AUG(or ATG) codon:
    if (lookForInitCodon)
    {
      for (size_t i = 0; i < sequence.size() - 2; i++)
      {
        if (sequence[i] == 0 && sequence[i + 1] == 3 && sequence[i + 2] == 2)
        {
          initPos = includeInitCodon ? i : i + 3;
          break;
        }
      }
    }
    // Look for stop codon:
    shared_ptr<const NucleicAlphabet> nucAlpha = codonAlphabet_->getNucleicAlphabet();
    for (size_t i = initPos; i < sequence.size() - 2; i += 3)
    {
      string codon = nucAlpha->intToChar(sequence[i])
          + nucAlpha->intToChar(sequence[i + 1])
          + nucAlpha->intToChar(sequence[i + 2]);
      if (isStop(codon))
      {
        stopPos = i;
        break;
      }
    }
  }
  else
    throw AlphabetMismatchException("Sequence must have alphabet of type nucleic or codon in GeneticCode::getCodingSequence.", 0, &sequence.alphabet());

  return SequenceTools::subseq<Sequence>(sequence, initPos, stopPos - 1);
}

/**********************************************************************************************/
