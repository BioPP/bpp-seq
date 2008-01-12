//
// File: EchinodermMitochondrialCodonAlphabet.h
// Created by: Eric Bazin
// Created on: 14 11:31:27 CET 2005
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

#ifndef _ECHINODERMMITOCHONDRIALCODONALPHABET_H_
#define _ECHINODERMMITOCHONDRIALCODONALPHABET_H_


#include "CodonAlphabet.h"

namespace bpp
{

/**
 * @brief This class implements the Echinoderm and Faltworms
 * Mitochondrial codon alphabet as describe on the NCBI website:
 * http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi?mode=t#SG9
 */

class EchinodermMitochondrialCodonAlphabet : public CodonAlphabet
{
	public:
		EchinodermMitochondrialCodonAlphabet(const NucleicAlphabet * alpha);
		virtual ~EchinodermMitochondrialCodonAlphabet();
	
	public:
		unsigned int getSize() const;
		unsigned int getNumberOfTypes() const;
	
		string getAlphabetType() const;

		bool isStop(      int      codon) const;
		bool isStop(const string & codon) const;
};

} //end of namespace bpp.

#endif	//_ECHINODERMMITOCHONDRIALCODONALPHABET_H_
