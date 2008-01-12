//
// File IOSequenceFactory.cpp
// Created by: Julien Dutheil
// Created on: Tue 18/04/06 10:24
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

#include "IOSequenceFactory.h"

using namespace bpp;

const string IOSequenceFactory::FASTA_FORMAT              = "Fasta";
const string IOSequenceFactory::MASE_FORMAT               = "Mase";  
const string IOSequenceFactory::CLUSTAL_FORMAT            = "Clustal";
const string IOSequenceFactory::DCSE_FORMAT               = "DCSE";  
const string IOSequenceFactory::PHYLIP_FORMAT_INTERLEAVED = "Phylip I";  
const string IOSequenceFactory::PHYLIP_FORMAT_SEQUENTIAL  = "Phylip S";  
const string IOSequenceFactory::PAML_FORMAT_INTERLEAVED   = "PAML I";  
const string IOSequenceFactory::PAML_FORMAT_SEQUENTIAL    = "PAML S";  

ISequence * IOSequenceFactory::createReader(const string & format) throw (Exception)
{
       if(format == FASTA_FORMAT) return new Fasta();
  else if(format == MASE_FORMAT) return new Mase();
  else if(format == CLUSTAL_FORMAT) return new Clustal();
  else if(format == DCSE_FORMAT) return new DCSE();
  else if(format == PHYLIP_FORMAT_INTERLEAVED) return new Phylip(false, false);
  else if(format == PHYLIP_FORMAT_SEQUENTIAL) return new Phylip(false, true);
  else if(format == PAML_FORMAT_INTERLEAVED) return new Phylip(true, false);
  else if(format == PAML_FORMAT_SEQUENTIAL) return new Phylip(true, true);
  else throw Exception("Format " + format + " is not supported for input.");
}
  
OSequence * IOSequenceFactory::createWriter(const string & format) throw (Exception)
{
       if(format == FASTA_FORMAT) return new Fasta();
  else if(format == MASE_FORMAT) return new Mase();
  else if(format == PHYLIP_FORMAT_INTERLEAVED) return new Phylip(false, false);
  else if(format == PHYLIP_FORMAT_SEQUENTIAL) return new Phylip(false, true);
  else if(format == PAML_FORMAT_INTERLEAVED) return new Phylip(true, false);
  else if(format == PAML_FORMAT_SEQUENTIAL) return new Phylip(true, true);
  else throw Exception("Format " + format + " is not supported for output.");
}

