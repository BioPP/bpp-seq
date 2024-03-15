// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCEITERATOR_H
#define BPP_SEQ_SEQUENCEITERATOR_H


namespace bpp
{
/**
 * @brief Generic sequence iterator interface, allowing to loop over sequences.
 */
template<class SequenceType>
class TemplateSequenceIteratorInterface
{
public:
  TemplateSequenceIteratorInterface() {}
  virtual ~TemplateSequenceIteratorInterface() {}

public:
  virtual std::unique_ptr<SequenceType> nextSequence() = 0;
  virtual bool hasMoreSequences() const = 0;
};

} // end of namespace bpp.
#endif // BPP_SEQ_SEQUENCEITERATOR_H
