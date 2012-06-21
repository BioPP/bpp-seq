//
// File: SequenceFeature.h
// Created by: Julien Dutheil
// Created on: Mon Nov 21 2011
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

#ifndef _SEQUENCEFEATURE_H_
#define _SEQUENCEFEATURE_H_

//From the STL:
#include <string>
#include <map>
#include <set>
#include <algorithm>

//From bpp-core:
#include <Bpp/Clonable.h>
#include <Bpp/Numeric/Range.h>

namespace bpp
{

/**
 * @brief a coordinate range on a sequence.
 * Stores coordiantes as a Range<unsigned int> object,
 * but also keep the strand information.
 */
class SeqRange:
  public Range<unsigned int>
{
  private:
    char strand_;

  public:
    /**
     * @param a First position
     * @param b Second position
     * @param strand The strand information. Can take one of the four values: '+' for positive strand, '-' for negative, '.' if not stranded or '?' if strandedness is relevant but unknown.
     */
    SeqRange(unsigned int a, unsigned int b, char strand = '.'):
      Range<unsigned int>(a, b), strand_(strand) {
        if (strand != '+' && strand != '-' && strand != '?' && strand != '.')
          strand_ = '.';
    }

    SeqRange* clone() const { return new SeqRange(*this); }

  public:
    virtual char getStrand() const { return strand_; }

    virtual bool isNegativeStrand() const { return strand_ == '-'; }
    virtual bool isStranded() const { return strand_ == '+' || strand_ == '-'; }

};

/**
 * @brief The base interface for sequence features.
 *
 * This interface provides the most generic information common to all features, that is:
 * - sequence reference
 * - procedure used to generate the information
 * - start and end positions. Coordinates are 0-based, of type [a,b[, so that if a==b,
 * the annotation is considered empty. A one-character annotation is then noted as start=12(included),end=13(excluded) for instance.
 *
 * Subclasses inheriting this interface provide specialisations for various types of annotations.
 * Depending on the file format of annotations, not all types of annotations may be supported.
 *
 * @author Julien Dutheil
 */
class SequenceFeature:
  public virtual Clonable
{
  public:
    static const std::string NO_ATTRIBUTE_SET;

  public:
    virtual SequenceFeature* clone() const = 0;

  public:
    /**
     * @return The id of this feature.
     */
    virtual const std::string& getId() const = 0;
    /**
     * @param id A std::string representing the id.
     */
    virtual void setId(const std::string& id) = 0;

    /**
     * @return The id of the sequence on which this feature is based.
     */
    virtual const std::string& getSequenceId() const = 0;
    /**
     * @param id A std::string representing the id of the reference.
     */
    virtual void setSequenceId(const std::string& id) = 0;

    /**
     * @return A text intended to describe the algorithm or procedure used to generate the feature.
     */
    virtual const std::string& getSource() const = 0;
    /**
     * @param source A std::string representing the source of the feature.
     */
    virtual void setSource(const std::string& source) = 0;

    /**
     * @return A text describing the type of feature. Depending on the format, it can be restricted (for example, mRNA), or any text can be supplied (for example TFXX binding site).
     */
    virtual const std::string& getType() const = 0;
    /**
     * @param type A std::string representing the type of this feature.
     */
    virtual void setType(const std::string& type) = 0;

    /**
     * @return The starting position of the feature, 0-based, included.
     */
    virtual const unsigned int getStart() const = 0;

    /**
     * @return The ending position of the feature, 0-based, excluded.
     */
    virtual const unsigned int getEnd() const = 0;

    /**
     * @return True if the feature is stranded.
     */
    virtual bool isStranded() const = 0;

    /**
     * @return True if the sequence is coded on the negative strand. False if it is on the positive one or unknown.
     */
    virtual bool isNegativeStrand() const = 0;

    /**
     * @return Coordinates as a Range object.
     */
    virtual SeqRange getRange() const = 0;
    
    /**
     * @return The score associated to the feature (eg, an E-value or a P-value).
     */
    virtual const double& getScore() const = 0;
    /**
     * @param score A double representing the score of this feature.
     */
    virtual void setScore(double score) = 0;

    /**
     * @param  attribute The name of the attribute to retrieve.
     * @return The attribute with specified name (read only).
     */
    virtual const std::string& getAttribute(const std::string& attribute) const = 0;
    
    /**
     * @param  attribute The name of the attribute to retrieve.
     * @return The attribute with specified name.
     */
    virtual std::string& getAttribute(const std::string& attribute) = 0;

    /**
     * @return The list of all attributes available.
     */
    virtual std::set< std::string > getAttributeList() const = 0;
    
    /**
     * @brief Set the value of an attribute.
     *
     * @param attribute The name of the attribute to set.
     * @param value     The value of the attribute to set.
     */
    virtual void setAttribute(const std::string& attribute, const std::string& value) = 0;

    /**
     * @param attribute The name of the attribute to be removed.
     */
    virtual void removeAttribute(const std::string& attribute) = 0;

};

/**
 * @brief A very simple implementation of the SequenceFeature class.
 *
 * It uses a hash map for storing attributes.
 */
class BasicSequenceFeature:
  public SequenceFeature
{
  protected:
    std::string id_;
    std::string sequenceId_;
    std::string source_;
    std::string type_;
    SeqRange range_;
    double score_;
    mutable std::map<std::string, std::string> attributes_;

  public:
    BasicSequenceFeature(
        const std::string& id,
        const std::string& seqId,
        const std::string& source,
        const std::string& type,
        unsigned int start,
        unsigned int end,
        char strand,
        double score = -1):
      id_(id), sequenceId_(seqId), source_(source),
      type_(type), range_(start, end, strand), score_(score),
      attributes_()
    {}

    virtual BasicSequenceFeature* clone() const { return new BasicSequenceFeature(*this); }

  public:
    const std::string& getId() const { return id_; }
    void setId(const std::string& id) { id_ = id; }
    const std::string& getSequenceId() const { return sequenceId_; }
    void setSequenceId(const std::string& sid) { sequenceId_ = sid; }
    const std::string& getSource() const { return source_; }
    void setSource(const std::string& source) { source_ = source; }
    const std::string& getType() const { return type_; }
    void setType(const std::string& type) { type_ = type; }
    const unsigned int getStart() const { return range_.begin(); }
    const unsigned int getEnd() const { return range_.end(); }
    bool isStranded() const { return range_.isStranded(); }
    bool isNegativeStrand() const { return range_.isNegativeStrand(); }
    const double& getScore() const { return score_; }
    void setScore(double score) { score_ = score; }

    const std::string& getAttribute(const std::string& attribute) const {
      std::map<std::string, std::string>::iterator it = attributes_.find(attribute);
      if (it != attributes_.end())
        return it->second;
      else
        return NO_ATTRIBUTE_SET;
    }
    
    std::string& getAttribute(const std::string& attribute) {
      return attributes_[attribute];
    }
    
    void setAttribute(const std::string& attribute, const std::string& value) {
      attributes_[attribute] = value;
    }

    std::set< std::string > getAttributeList() const {
      std::set< std::string > d;
      for (std::map<std::string, std::string>::iterator it = attributes_.begin() ; it != attributes_.end() ; it++) {
        d.insert(it->first);
      }
      return d;
    }

    void removeAttribute(const std::string& attribute) {
      std::map<std::string, std::string>::iterator it = attributes_.find(attribute);
      if (it != attributes_.end()) {
        attributes_.erase(it);
      }
    }

    SeqRange getRange() const {
      return SeqRange(range_);
    }

};

/**
 * @brief A simple ensemble of sequence features.
 *
 * This class is at a draft stage, and further improvements are expected, notably
 * to allow proper indexation, nested features, etc.
 *
 * For now, it is mostly a vector of feature object, stored as pointers.
 * A few functions are provided for convenience.
 *
 * @author Julien Dutheil
 */
class SequenceFeatureSet
{
  private:
    std::vector<SequenceFeature*> features_;

  public:
    SequenceFeatureSet(): features_() {};

    virtual ~SequenceFeatureSet() { clear(); }

    SequenceFeatureSet(const SequenceFeatureSet& sfs):
      features_()
    {
      for (std::vector<SequenceFeature*>::const_iterator it = sfs.features_.begin();
          it != sfs.features_.end();
          ++it) {
        features_.push_back((**it).clone());
      }
    }
    SequenceFeatureSet& operator=(const SequenceFeatureSet& sfs)
    {
      clear();
      for (std::vector<SequenceFeature*>::const_iterator it = sfs.features_.begin();
          it != sfs.features_.end();
          ++it) {
        features_.push_back((**it).clone());
      }
      return *this;
    }

  public:
    /**
     * @brief Delete all features in this set.
     */
    void clear()
    {
      for (std::vector<SequenceFeature*>::iterator it = features_.begin();
          it != features_.end();
          ++it) {
        delete *it;
      }
      features_.clear();
    }

    /**
     * @param i The index of the feature.
     * @return A reference toward the feature.
     */
    const SequenceFeature& getFeature(unsigned int i) const {
      return *features_[i];
    }

    /**
     * @return The number of features in this set.
     */
    unsigned int getNumberOfFeatures() const { return features_.size(); }

    /**
     * @brief Add a feature to the container. The feature will be copied and the copy owned by the container.
     *
     * @param feature The feature to add to the container.
     */
    void addFeature(const SequenceFeature& feature) {
      features_.push_back(feature.clone());
    }

    /**
     * @return A set containing all sequences ids in this set.
     */
    std::set<std::string> getSequences() const {
      std::set<std::string> seqIds;
      for (std::vector<SequenceFeature*>::const_iterator it = features_.begin();
          it != features_.end();
          ++it) {
        seqIds.insert((**it).getSequenceId());
      }
      return seqIds;
    }

    /**
     * @return A set containing all feature type in this set.
     */
    std::set<std::string> getTypes() const {
      std::set<std::string> types;
      for (std::vector<SequenceFeature*>::const_iterator it = features_.begin();
          it != features_.end();
          ++it) {
        types.insert((**it).getType());
      }
      return types;
    }

    /**
     * @brief Get all coordinates of features for a given source.
     * All ranges are added to a RangeCollection container, as SeqRange objects.
     * @param seqId The name of the sequence id to consider.
     * @param coords [out] a container where to add the coordinates of each feature.
     */
    void fillRangeCollectionForSequence(const std::string& seqId, RangeCollection<unsigned int>& coords) const {
      for (std::vector<SequenceFeature*>::const_iterator it = features_.begin();
          it != features_.end();
          ++it) {
        if ((**it).getSequenceId() == seqId) {
          coords.addRange((**it).getRange());
        }
      }
    }

    /**
     * @param types The feature type.
     * @return A new set with all features of a given type.
     */
    SequenceFeatureSet* getSubsetForType(const std::vector<std::string>& types) const {
      SequenceFeatureSet* subset = new SequenceFeatureSet();
      for (std::vector<SequenceFeature*>::const_iterator it = features_.begin();
          it != features_.end();
          ++it) {
        if (std::find(types.begin(), types.end(), (**it).getType()) != types.end()) {
          subset->addFeature(**it);
        }
      }
      return subset;
    }

};

} //end of namespace bpp

#endif //_SEQUENCEFEATURE_H_

