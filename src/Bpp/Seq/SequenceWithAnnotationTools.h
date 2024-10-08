// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCEWITHANNOTATIONTOOLS_H
#define BPP_SEQ_SEQUENCEWITHANNOTATIONTOOLS_H

#include <Bpp/Numeric/VectorTools.h>
#include <cstddef>

#include "SequenceTools.h"
#include "SequenceWithAnnotation.h"

namespace bpp
{
class SequenceMask :
  public virtual SequenceAnnotation
{
private:
  bool removable_;
  std::vector<bool> mask_;

public:
  static const std::string MASK;

public:
  /**
   * @name Constructors
   * @{
   */

  /**
   * @brief Build a new SequenceMask object
   *
   * Build a new SequenceMask object and set the mask to false.
   *
   * @param size The size of the sequence.
   * @param removable Tell if this listener can be removed by the user.
   */
  SequenceMask(size_t size = 0, bool removable = true) :
    removable_(removable),
    mask_(size, false)
  {}


  /**
   * @brief Build a new SequenceMask object
   *
   * Build a new SequenceMask object and set the mask as a vector of bool.
   *
   * @param mask The boolean mask
   * @param removable Tell if this listener can be removed by the user.
   */
  SequenceMask(const std::vector<bool>& mask, bool removable = true) :
    removable_(removable),
    mask_(mask)
  {}

  /** @} */

  /**
   * @name Destructor
   * @{
   */
  virtual ~SequenceMask()
  {}
  /** @} */

  /**
   * @name The Clonable interface
   * @{
   */
  SequenceMask* clone() const override
  {
    return new SequenceMask(*this);
  }
  /** @} */

public:
  void init(const Sequence& seq) override
  {
    mask_.resize(seq.size());
    std::fill(mask_.begin(), mask_.end(), false);
  }

  const std::string& getType() const override
  {
    return MASK;
  }

  bool isValidWith(const SequenceWithAnnotation& sequence, bool throwException = true) const override
  {
    if (throwException && mask_.size() != sequence.size()) throw Exception("SequenceMask. The mask size must match the sequence size.");
    return mask_.size() == sequence.size();
  }

  bool isRemovable() const override
  {
    return removable_;
  }
  bool isShared() const override
  {
    return false;
  }
  void beforeSequenceChanged(const IntSymbolListEditionEvent& event) override
  {
  }
  void afterSequenceChanged(const IntSymbolListEditionEvent& event) override;
  void beforeSequenceInserted(const IntSymbolListInsertionEvent& event) override
  {
  }
  void afterSequenceInserted(const IntSymbolListInsertionEvent& event) override;
  void beforeSequenceDeleted(const IntSymbolListDeletionEvent& event) override
  {
  }
  void afterSequenceDeleted(const IntSymbolListDeletionEvent& event) override;
  void beforeSequenceSubstituted(const IntSymbolListSubstitutionEvent& event) override
  {
  }
  void afterSequenceSubstituted(const IntSymbolListSubstitutionEvent& event) override
  {
  }

  size_t getSize() const
  {
    return mask_.size();
  }

  const bool operator[](size_t i) const
  {
    return mask_[i];
  }

  void setMask(const std::vector<bool>& mask)
  {
    if (mask.size() != mask_.size())
      throw DimensionException("SequenceMask::setMask. Trying to replace mask by a vector with different length.", mask.size(), mask_.size());
    mask_ = mask;
  }

  /**
   * @return The mask as a vector.
   */
  const std::vector<bool>& getMask() const
  {
    return mask_;
  }

  void setMask(size_t pos, bool mask)
  {
    if (pos >= mask_.size())
      throw Exception("SequenceMask::setMask. Vector overflow. Scores number: " + TextTools::toString(mask_.size()) + ", but trying to insert mask at position " + TextTools::toString(pos) + ".");
    mask_[pos] = mask;
  }

  void setMask(size_t pos, const std::vector<bool>& mask)
  {
    if (pos + mask.size() > mask_.size())
      throw Exception("SequenceMask::setMask. Vector overflow. Scores number: " + TextTools::toString(mask_.size()) + ", but trying to insert " + TextTools::toString(mask.size()) + " scores at position " + TextTools::toString(pos) + ".");
    std::copy(mask.begin(), mask.end(), mask_.begin() + static_cast<ptrdiff_t>(pos));
  }

  bool merge(const SequenceAnnotation& anno) override
  {
    try
    {
      const SequenceMask* mask = &dynamic_cast<const SequenceMask&>(anno);
      VectorTools::append(mask_, mask->getMask());
      return true;
    }
    catch (std::exception& e)
    {
      return false;
    }
  }

  std::unique_ptr<SequenceAnnotation> getPartAnnotation(size_t pos, size_t len) const override
  {
    return std::unique_ptr<SequenceAnnotation>(new SequenceMask(std::vector<bool>(mask_.begin() + static_cast<ptrdiff_t>(pos), mask_.begin() + static_cast<ptrdiff_t>(pos + len)), removable_));
  }
};

/**
 * @brief The SequenceWithAnnotationTools static class
 *
 * Implement methods to manipulate SequencesWithAnnotation
 *
 * @author Julien Dutheil
 */

class SequenceWithAnnotationTools
{
public:
  /**
   * @brief Parse a sequence with a CaseMaskedAlphabet and creates a new SequenceWithAnnotation object with original alphabet and a mask annotation.
   *
   * @param seq The sequence to parse.
   * @return A new SequenceWithAnnotation object.
   * @throw AlphabetException if the input sequence does not have a CaseMaskedAlphabet.
   */
  std::unique_ptr<SequenceWithAnnotation> createMaskAnnotation(const Sequence& seq);
};
}
#endif // BPP_SEQ_SEQUENCEWITHANNOTATIONTOOLS_H
