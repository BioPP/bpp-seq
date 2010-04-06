// 
// File:    SequenceWithQuality.h
// Author:  Sylvain Gaillard
// Created: 19/01/2010 16:01:20
// 

/*
Copyright or © or Copr. CNRS, (January 19, 2010)

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

#ifndef _SEQUENCEWITHQUALITY_H_
#define _SEQUENCEWITHQUALITY_H_

#include "Sequence.h"

// From the STL

#include <string>
#include <vector>

// From Bio++

#include <NumCalc/VectorTools.h>
#include <NumCalc/VectorExceptions.h>

namespace bpp {
  /**
   * @brief The SequenceWithQuality class
   *
   * This is a sequence with quality score associated to each element.
   * The score is a signed int value that can represent the phred score
   * for nucleic sequence.
   *
   * @todo 
   * - addElement with quality score
   *
   * @author Sylvain Gaillard
   */
  class SequenceWithQuality: public Sequence {
    private:
      std::vector<int> qualScores_;

    private:
      static const int DEFAULT_QUALITY_VALUE;

    public:

      /**
       * @name Constructors
       * @{
       */

      /**
       * @brief Build a new SequenceWithQuality
       *
       * Build a new SequenceWithQuality and set the quality scores to
       * the default value DEFAULT_QUALITY_VALUE.
       *
       * @param name The name of the sequence
       * @param sequence The string representing the sequence
       * @param alpha A pointer to an Alphabet
       *
       * @throw BadCharException if a state is not alowed by the Alphabet
       */
      SequenceWithQuality(const std::string& name, const std::string& sequence, const Alphabet* alpha) throw (BadCharException): Sequence(name, sequence, alpha), qualScores_(sequence.size(), DEFAULT_QUALITY_VALUE) {}

      /**
       * @brief Build a new SequenceWithQuality
       *
       * Build a new SequenceWithQuality and assign quality scores from
       * a vector of int.
       *
       * @param name The name of the sequence
       * @param sequence The string representing the sequence
       * @param quality The quality scores
       * @param alpha A pointer to an alphabet
       *
       * @throw BadCharException if a state is not alowed by the Alphabet
       * @throw DimensionException if the number of quality values is not equal to the number of sequence states
       */
      SequenceWithQuality(const std::string& name, const std::string& sequence, const std::vector<int>& quality, const Alphabet* alpha) throw (BadCharException, DimensionException): Sequence(name, sequence, alpha), qualScores_(quality) {
        if (size() != qualScores_.size())
          throw DimensionException("SequenceWithQuality constructor: sequence and quality must have the same length", qualScores_.size(), size());
      }

      /**
       * @brief Build a new SequenceWithQuality
       *
       * Build a new SequenceWithQuality and set the quality scores to
       * the default value DEFAULT_QUALITY_VALUE.
       *
       * @param name The name of the sequence
       * @param sequence The sequence in int
       * @param alpha A pointer to an Alphabet
       *
       * @throw BadIntException if a state is not alowed by the Alphabet
       */
      SequenceWithQuality(const std::string& name, const std::vector<int>& sequence, const Alphabet* alpha) throw (BadIntException): Sequence(name, sequence, alpha), qualScores_(sequence.size(), DEFAULT_QUALITY_VALUE) {}

      /**
       * @brief Build a new SequenceWithQuality
       *
       * Build a new SequenceWithQuality and assign quality scores from
       * a vector of int.
       *
       * @param name The name of the sequence
       * @param sequence The sequence in int
       * @param quality The quality scores
       * @param alpha A pointer to an Alphabet
       *
       * @throw BadIntException if a state is not alowed by the Alphabet
       * @throw DimensionException if the number of quality values is not equal to the number of sequence states
       */
      SequenceWithQuality(const std::string& name, const std::vector<int>& sequence, const std::vector<int>& quality, const Alphabet* alpha) throw (BadIntException, DimensionException): Sequence(name, sequence, alpha), qualScores_(quality) {
        if (size() != qualScores_.size())
          throw DimensionException("SequenceWithQuality constructor: sequence and quality must have the same length", qualScores_.size(), size());
      }

      /**
       * @brief Build a new SequenceWithQuality
       *
       * Build a new SequenceWithQuality from a Sequence object and set the
       * quality scores to the default value DEFAULT_QUALITY_VALUE.
       *
       * @param s The Sequence object
       */
      SequenceWithQuality(const Sequence& s): Sequence(s), qualScores_(s.size(), DEFAULT_QUALITY_VALUE) {}

      /**
       * @brief Build a new SequenceWithQuality
       *
       * Build a new SequenceWithQuality from a Sequence object and set the
       * quality scores from a vector of int.
       *
       * @param s The Sequence object
       * @param sc The quality scores
       *
       * @throw DimensionException if the number of quality values is not equal to the number of sequence states
       */
      SequenceWithQuality(const Sequence& s, const std::vector<int>& sc) throw (DimensionException): Sequence(s), qualScores_(sc) {
        if (s.size() != qualScores_.size()) {
          throw DimensionException("SequenceWithQuality constructor: sequence and qualities must have the same length", qualScores_.size(),  s.size());
        }
      }

      /** @} */

      /**
       * @name Destructor
       * @{
       */
      virtual ~SequenceWithQuality() {}
      /** @} */

      /**
       * @name The Clonable interface
       * @{
       */
#ifdef NO_VIRTUAL_COV
      Clonable*
#else
      SequenceWithQuality*
#endif
      clone() const { return new SequenceWithQuality(*this); }
      /** @} */

      void setToSizeR(unsigned int newSize);

      void setToSizeL(unsigned int newSize);

      void append(const std::vector<int>& content) throw (BadIntException);
      void append(const std::vector<std::string>& content) throw (BadCharException);
      void append(const std::string& content) throw (BadCharException);

      void addElement(const std::string& c) throw (BadCharException);
      void addElement(unsigned int pos, const std::string& c) throw (BadCharException, IndexOutOfBoundsException);
      void addElement(int v) throw (BadIntException);
      void addElement(unsigned int pos, int v) throw (BadIntException, IndexOutOfBoundsException);

      void setContent(const std::string& sequence) throw (BadCharException);
      void setContent(const std::vector<int>& list) throw (BadIntException);
      void setContent(const std::vector<std::string>& list) throw (BadCharException);

      void deleteElement(unsigned int pos) throw (IndexOutOfBoundsException);

      /**
       * @name Dealing with quality
       * @{
       */

      /**
       * @brief Set the quality score
       *
       * @param pos The position where the quality must be set
       * @param quality The quality value
       *
       * @throw IndexOutOfBoundsException if pos is greater than the
       * sequence size
       */
      void setQuality(unsigned int pos, int quality) throw (IndexOutOfBoundsException) {
        if (pos >= qualScores_.size())
          throw IndexOutOfBoundsException("SequenceWithQuality::setQuality: pos out of bounds", pos, 0, qualScores_.size() - 1);
        qualScores_[pos] = quality;
      }
      
      /**
       * @brief Get the quality score
       *
       * @param pos The position where the quality is read
       *
       * @return The quality score
       *
       * @throw IndexOutOfBoundsException if pos is greater than the
       * sequence size
       */
      int getQuality(unsigned int pos) const throw (IndexOutOfBoundsException) {
        if (pos >= qualScores_.size())
          throw IndexOutOfBoundsException("SequenceWithQuality::getQuality: pos out of boundsé", pos, 0, qualScores_.size() - 1);
        return qualScores_[pos];
      }

      /**
       * @brief Set the whole quality scores
       *
       * @param quality The vector of quality scores
       *
       * @throw DimensionException if the quality vector does not feet the
       * sequence size
       */
      void setQualities(const std::vector<int>& quality) throw (DimensionException) {
        if (quality.size() != qualScores_.size())
          throw DimensionException("SequenceWithQuality::setQualities: quality must fit sequence size", quality.size(), qualScores_.size());
        qualScores_ = quality;
      }

      /**
       * @brief Get the whole quality scores
       *
       * @return A reference to the quality vector
       */
      const std::vector<int>& getQualities() const {
        return qualScores_;
      }

      /**
       * @brief Append content with quality
       *
       * @param content A vector of int to append to the sequence
       * @param qualities A vector of int to append to the qualities
       *
       * @throw BadIntException if one of the content int is not in the
       * Alphabet
       * @throw DimensionException if qualities does not have the same size as
       * content
       */
      void append(
          const std::vector<int>& content,
          const std::vector<int>& qualities
          ) throw (BadIntException, DimensionException) {
        if (content.size() != qualities.size())
          throw DimensionException("SequenceWithQuality::append: qualities must fit content size", qualities.size(), content.size());
        Sequence::append(content);
        VectorTools::append(qualScores_, qualities);
      }

      /**
       * @brief Append content with quality
       *
       * @param content A vector of string to append to the sequence
       * @param qualities A vector of int to append to the qualities
       *
       * @throw BadCharException if one of the content string is not in the
       * Alphabet
       * @throw DimensionException if qualities does not have the same size as
       * content
       */
      void append(
          const std::vector<std::string>& content,
          const std::vector<int>& qualities
          ) throw (BadCharException, DimensionException) {
        if (content.size() != qualities.size())
          throw DimensionException("SequenceWithQuality::append: qualities must fit content size", qualities.size(), content.size());
        Sequence::append(content);
        VectorTools::append(qualScores_, qualities);
      }

      /**
       * @brief Append content with quality
       *
       * @param content A string to append to the sequence
       * @param qualities A vector of int to append to the qualities
       *
       * @throw BadCharException if one of the character of the string is not in
       * the Alphabet
       * @throw DimensionException if qualities does not have the same size as
       * content
       */
      void append(
          const std::string& content,
          const std::vector<int> qualities
          ) throw (BadCharException, DimensionException) {
        if (content.size() / this->getAlphabet()->getStateCodingSize()
            != qualities.size())
          throw DimensionException("SequenceWithQuality::append: qualities must fit content size", qualities.size(), content.size() / this->getAlphabet()->getStateCodingSize());
        Sequence::append(content);
        VectorTools::append(qualScores_, qualities);
      }

      /**
       * Add a character to the end of the list with quality
       */
      void addElement(
          const std::string& c, int q
          ) throw (BadCharException) {
        Sequence::addElement(c);
        qualScores_.push_back(q);
      }

      /** @} */

    private:
      void extendQualityScores_() {
        if (qualScores_.size() >= size()) return;
        while (qualScores_.size() < size())
          qualScores_.push_back(DEFAULT_QUALITY_VALUE);
      }
  };
}

#endif // _SEQUENCEWITHQUALITY_H_
