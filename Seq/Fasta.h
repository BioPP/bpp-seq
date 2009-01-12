//
// File: Fasta.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Tue Aug 21 2003
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

#ifndef _FASTA_H_
#define _FASTA_H_

#include "AbstractISequence.h"
#include "AbstractOSequence.h"
#include "Sequence.h"
#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"

namespace bpp
{

/**
 * @brief The fasta sequence file format.
 *
 * Read and write from/to Fasta files.
 */
class Fasta:
  public virtual AbstractISequence,
  public virtual AbstractOSequence
{
  protected:

    /**
     * @brief The maximum number of chars to be written on a line.
     */
    unsigned int _charsByLine;

    bool _checkNames;

    bool _extended;

  public:
  
    /**
     * @brief Build a new Fasta object.
     *
     * @param charsByLine Number of character per line when writing files.
     * @param checkNames  Tell if the names in the file should be checked for unicity (slower, in o(n*n) where n is the number of sequences).
     * @param extended Tell if we should read general comments and sequence comments in HUPO-PSI format.
     */
    Fasta(unsigned int charsByLine = 100, bool checkNames = true, bool extended = false): _charsByLine(charsByLine), _checkNames(checkNames), _extended(extended) {}

    // Class destructor
    virtual ~Fasta() {}

  public:

    /**
     * @name The AbstractISequence interface.
     *
     * @{
     */
    void appendFromStream(istream & input, VectorSequenceContainer & sc) const throw (Exception);
    /** @} */

    /**
     * @name The OSequence interface.
     *
     * @{
     */
    void write(ostream & output, const SequenceContainer & sc) const throw (Exception);
    void write(const string & path, const SequenceContainer & sc, bool overwrite=true) const throw (Exception)
    {
      AbstractOSequence::write(path, sc, overwrite);
    }
    /** @} */
  
    /**
     * @name The IOSequence interface.
     *
     * @{
     */
    const string getFormatName() const { return "FASTA file"; };
    const string getFormatDescription() const
    {
      return "Sequence name (preceded by >) in one line, sequence content, no comments";
    }
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
};

} //end of namespace bpp.

#endif // _FASTA_H_

