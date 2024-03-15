// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_IO_IOSEQUENCEFACTORY_H
#define BPP_SEQ_IO_IOSEQUENCEFACTORY_H


#include "../Alphabet/Alphabet.h"
#include "ISequence.h"
#include "OSequence.h"

namespace bpp
{
/**
 * @brief Utilitary class for creating sequence readers and writers.
 */
class IoSequenceFactory
{
public:
  static const std::string FASTA_FORMAT;
  static const std::string MASE_FORMAT;
  static const std::string CLUSTAL_FORMAT;
  static const std::string DCSE_FORMAT;
  static const std::string PHYLIP_FORMAT_INTERLEAVED;
  static const std::string PHYLIP_FORMAT_SEQUENTIAL;
  static const std::string PAML_FORMAT_INTERLEAVED;
  static const std::string PAML_FORMAT_SEQUENTIAL;
  static const std::string GENBANK_FORMAT;
  static const std::string NEXUS_FORMAT;

public:
  /**
   * @brief Creates a new factory object.
   *
   * Example:
   * @code
   * auto AlphabetTools::DNA_ALPHABET;
   * auto seqReader = IoSequenceFactory().createReader(IoSequenceFactory::FASTA_FORMAT);
   * unique_ptr<SequenceContainer> sequences = seqReader->read("file.fasta", alphabet);
   * delete seqReader;
   * @endcode
   */
  IoSequenceFactory() {}
  virtual ~IoSequenceFactory() {}

  /**
   * @brief Get a new dynamically created ISequence object.
   *
   * @param format The input file format.
   * @return A pointer toward a new ISequence object.
   * @throw Exception If the format name do not match any available format.
   */
  virtual std::unique_ptr<ISequence> createReader(const std::string& format);

  /**
   * @brief Get a new dynamically created IAlignment object.
   *
   * @param format The input file format.
   * @return A pointer toward a new IAlignment object.
   * @throw Exception If the format name do not match any available format.
   */
  virtual std::unique_ptr<IAlignment> createAlignmentReader(const std::string& format);

  /**
   * @brief Get a new dynamically created OSequence object.
   *
   * @param format The output file format.
   * @return A pointer toward a new OSequence object.
   * @throw Exception If the format name do not match any available format.
   */
  virtual std::unique_ptr<OSequence> createWriter(const std::string& format);

  /**
   * @brief Get a new dynamically created OAlignment object.
   *
   * @param format The output file format.
   * @return A pointer toward a new OAlignment object.
   * @throw Exception If the format name do not match any available format.
   */
  virtual std::unique_ptr<OAlignment> createAlignmentWriter(const std::string& format);
};
} // end of namespace bpp.
#endif // BPP_SEQ_IO_IOSEQUENCEFACTORY_H
