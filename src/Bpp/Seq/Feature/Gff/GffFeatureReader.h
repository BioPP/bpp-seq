//
// File: GffFeatureReader.h
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

#ifndef _GFFFEATUREREADER_H_
#define _GFFFEATUREREADER_H_

#include "../SequenceFeature.h"

//From bpp-core:
#include <Bpp/Exceptions.h>

//From the STL:
#include <string>
#include <vector>

namespace bpp {

/**
 * @brief A simple reader implementing the Gene Finding Feature format.
 *
 * The reference norm in use is the one of GFF3 http://www.sequenceontology.org/gff3.shtml .
 * This class is a "beta" class, and may undeavour interface changes in the future.
 *
 * Note that in GFF, coordinates are [a, b] 1-based. They will therefore be converted to [a, b[ 0-based,
 * as specified for the SequenceFeature object.
 *
 * @author Julien Dutheil
 */
class GffFeatureReader
{
  public:
    static const std::string GFF_STRAND;
    static const std::string GFF_PHASE;
    static const std::string GFF_NAME;
    static const std::string GFF_ALIAS;
    static const std::string GFF_PARENT;
    static const std::string GFF_TARGET;
    static const std::string GFF_GAP;
    static const std::string GFF_DERIVES_FROM;
    static const std::string GFF_NOTE;
    static const std::string GFF_DBXREF;
    static const std::string GFF_ONTOLOGY_TERM;
    static const std::string GFF_IS_CIRCULAR;

  private:
    std::istream* input_;
    std::string nextLine_;

  public:
    GffFeatureReader(std::istream& input):
      input_(&input), nextLine_()
    {
      getNextLine_();
    }

  private:
    GffFeatureReader(const GffFeatureReader& reader): input_(0), nextLine_() {}
    GffFeatureReader& operator=(const GffFeatureReader& reader) { return *this; }

  public:
    bool hasMoreFeature() const { return nextLine_ != ""; }
    const BasicSequenceFeature nextFeature() throw (Exception);

    void getAllFeatures(SequenceFeatureSet& features) {
      while (hasMoreFeature()) {
        features.addFeature(nextFeature());
      }
    }
    void getFeaturesOfType(const std::string& type, SequenceFeatureSet& features) {
      while (hasMoreFeature()) {
        BasicSequenceFeature feature = nextFeature();
        if (feature.getType() == type)
          features.addFeature(feature);
      }
    }
    void getFeaturesOfSequence(const std::string& seqId, SequenceFeatureSet& features) {
      while (hasMoreFeature()) {
        BasicSequenceFeature feature = nextFeature();
        if (feature.getSequenceId() == seqId)
          features.addFeature(feature);
      }
    }


  private:
    void getNextLine_();

};

} //end of namespace bpp

#endif //_GFFFEATUREREADER_H_

