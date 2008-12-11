//
// File: Phylip.h
// Created by: Julien Dutheil
// Created on: Mon Oct 27 12:22:56 2003
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

#ifndef _PHYLIP_H_
#define _PHYLIP_H_

#include "AbstractISequence2.h"
#include "AbstractOSequence.h"
#include "Sequence.h"
#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"
#include "AlignedSequenceContainer.h"

// From the STL:
#include <iostream>

using namespace std;

namespace bpp
{

/**
 * @brief The Phylip & co format.
 *
 * An AlignedSequenceContainer is used instead of a VectorSequenceContainer.
 *
 * This format is described on the Phylip package documentation website:
 * http://evolution.genetics.washington.edu/phylip/doc/sequence.html
 */
class Phylip:
  public virtual AbstractISequence2,
  public virtual AbstractOSequence
{
  protected:

    /* this class allows two kind of Phylip format:
     * traditional, with names limited to 10 chars,
     * and 'extended', defined by PAML, with names separated from sequences by at least 6 white spaces.
     */
    bool _extended;
    /* tells if sequences are in the seuqential or the interleave format/
     */
    bool _sequential;

    /**
     * @brief The maximum number of chars to be written on a line.
     */
    unsigned int _charsByLine;

    bool _checkNames;

    string _namesSplit;
  
  public:
    /**
     * @brief Build a new Phylip file reader.
     *
     * @param extended If true, sequences with names longer than 10 characters are allowed.
     * @param sequential If false, sequences are supposed to be interlaved.
     * @param charsByLine The number of base to display in a row.
     * @param checkNames Tell if the names in the file should be checked for unicity (slower, in o(n*n) where n is the number of sequences).
     * @param split The string to use to split sequence name from content (only for 'extended' format). This will typically be "  " (two spaces) or "\t" (a tabulation).
     */
    Phylip(bool extended = true, bool sequential = true, unsigned int charsByLine = 100, bool checkNames = true, const string & split = "  "):
      _extended(extended), _sequential(sequential), _charsByLine(charsByLine), _checkNames(checkNames), _namesSplit(split) {}

    virtual ~Phylip() {}

  public:

    /**
     * @name The AbstractISequence2 interface.
     *
     * @{
     */
    void appendFromStream(istream & input, AlignedSequenceContainer & sc) const throw (Exception);
    /** @} */

    /**
     * @return The number of sequences contained in the specified file.
     *
     * This methods parses the firt line of the phylip file.
     * @param path The path of the file to parse.
     */
    unsigned int getNumberOfSequences(const string & path) const throw (IOException);

    /**
     * @name The OSequence interface.
     *
     * @{
     */
    void write(ostream & output, const SequenceContainer & sc) const throw (Exception);
    void write(const string & path, const SequenceContainer & sc, bool overwrite) const throw (Exception)
    {
      AbstractOSequence::write(path, sc, overwrite);
    }
    /** @} */


    /**
     * @name The IOSequence interface.
     *
     * @{
     */
    const string getFormatName() const;
    const string getFormatDescription() const;
    /** @} */

    /**
     * @return true if the names are to be checked when reading sequences from files.
     */
    bool checkNames() const { return _checkNames; }

    /**
     * @brief Tell whether the sequence names should be checked when reading from files.
     *
     * @param yn whether the sequence names should be checked when reading from files.
     */
    void checkNames(bool yn) { _checkNames = yn; }

    /**
     * @return The string used to split sequence name from content.
     */
    string getSplit() const { return _namesSplit; }

    /**
     * @param split The string to be used to split sequence name from content.
     */
    void setSplit(const string & split) { _namesSplit = split; }
     
  protected:
    //Reading tools:
    const vector<string> splitNameAndSequence(const string & s) const throw (Exception); 
    void readSequential (istream & in, AlignedSequenceContainer & asc) const throw (Exception);
    void readInterleaved(istream & in, AlignedSequenceContainer & asc) const throw (Exception);
    //Writing tools:
    vector<string> getSizedNames(const vector<string> & names) const;
    void writeSequential (ostream & out, const SequenceContainer & sc, int charsByLine) const;
    void writeInterleaved(ostream & out, const SequenceContainer & sc, int charsByLine) const;
};

} //end of namespace bpp.

#endif  //_PHYLIP_H_

