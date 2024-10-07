// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX1_H
#define BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX1_H

#include <Bpp/Clonable.h>

#include "../Alphabet/Alphabet.h"
#include "../Alphabet/AlphabetTools.h"
#include "../Alphabet/ProteicAlphabet.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief One dimensionnal alphabet index interface.
 *
 * Derivatives of this interface implement properties for a single state.
 */
class AlphabetIndex1 :
  public virtual Clonable
{
public:
  AlphabetIndex1()
  {}
  virtual ~AlphabetIndex1()
  {}

public:
  virtual AlphabetIndex1* clone() const override = 0;

  /**
   * @brief Get the index associated to a state.
   *
   * @param state The state to consider, as a int value.
   * @return The index associated to the state
   */
  virtual double getIndex(int state) const = 0;

  /**
   * @brief Get the index associated to a state.
   *
   * @param state The state to consider, as a string value.
   * @return The index associated to the state
   */
  virtual double getIndex(const std::string& state) const = 0;

  /**
   * @brief Get the alphabet associated to this index.
   *
   * @return Alphabet The alphabet associated to this index.
   */
  virtual std::shared_ptr<const Alphabet> getAlphabet() const = 0;

  /**
   * @brief Get the alphabet associated to this index.
   *
   * @return Alphabet The alphabet associated to this index.
   */
  virtual const Alphabet& alphabet() const = 0;

  /**
   * @return A vector object with all indices.
   */
  virtual const std::vector<double>& indexVector() const = 0;
};

/*
 * @brief Virtual AlphabetIndex1 for proteic alphabet
 *
 */

class ProteicAlphabetIndex1 :
  public virtual AlphabetIndex1
{
private:
  std::shared_ptr<const ProteicAlphabet> alpha_;

public:
  ProteicAlphabetIndex1() : alpha_(AlphabetTools::PROTEIN_ALPHABET)
  {}
  virtual ~ProteicAlphabetIndex1()
  {}

  virtual ProteicAlphabetIndex1* clone() const override = 0;

  ProteicAlphabetIndex1(const ProteicAlphabetIndex1& pt) :
    alpha_(pt.alpha_)
  {}

  ProteicAlphabetIndex1& operator=(const ProteicAlphabetIndex1& pt)
  {
    alpha_ = pt.alpha_;
    return *this;
  }

  std::shared_ptr<const Alphabet> getAlphabet() const override
  {
    return alpha_;
  }

  const Alphabet& alphabet() const override
  {
    return *alpha_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX1_H
