#Copyright or © or Copr. CNRS, (November 16, 2004-2008)
#
#This software is a computer program whose purpose is to provide classes
#for sequences analysis.
#
#This software is governed by the CeCILL  license under French law and
#abiding by the rules of distribution of free software.  You can  use, 
#modify and/ or redistribute the software under the terms of the CeCILL
#license as circulated by CEA, CNRS and INRIA at the following URL
#"http://www.cecill.info". 
#
#As a counterpart to the access to the source code and  rights to copy,
#modify and redistribute granted by the license, users are provided only
#with a limited warranty  and the software's author,  the holder of the
#economic rights,  and the successive licensors  have only  limited
#liability. 
#
#In this respect, the user's attention is drawn to the risks associated
#with loading,  using,  modifying and/or developing or reproducing the
#software by the user in light of its specific status of free software,
#that may mean  that it is complicated to manipulate,  and  that  also
#therefore means  that it is reserved for developers  and  experienced
#professionals having in-depth computer knowledge. Users are therefore
#encouraged to load and test the software's suitability as regards their
#requirements in conditions enabling the security of their systems and/or 
#data to be ensured and,  more generally, to use and operate it in the 
#same conditions as regards security. 
#
#The fact that you are presently reading this means that you have had
#knowledge of the CeCILL license and that you accept its terms.

H_FILES = \
AbstractAlphabet.h\
AbstractISequence.h\
AbstractISequence2.h\
AbstractOSequence.h\
AbstractSequenceContainer.h\
AlignedSequenceContainer.h\
AlphabetExceptions.h\
Alphabet.h\
AlphabetIndex1.h\
AlphabetIndex2.h\
alphabets\
AlphabetTools.h\
Clustal.h\
CodonAlphabet.h\
CodonSiteTools.h\
containers\
DCSE.h\
DefaultAlphabet.h\
DNA.h\
DNAToRNA.h\
Fasta.h\
GeneticCode.h\
GranthamAAChemicalDistance.h\
GranthamAAPolarityIndex.h\
GranthamAAVolumeIndex.h\
MiyataAAChemicalDistance.h\
BLOSUM50.h\
SimpleIndexDistance.h\
DefaultNucleotideScore.h\
AAIndex1Entry.h\
AAIndex2Entry.h\
IOSequence.h\
ISequence.h\
ioseq\
MapSequenceContainer.h\
Mase.h\
MaseTools.h\
NucleicAlphabet.h\
NucleicAcidsReplication.h\
OrderedSequenceContainer.h\
OSequence.h\
Phylip.h\
ProteicAlphabet.h\
RNA.h\
SequenceContainerExceptions.h\
SequenceContainer.h\
SequenceContainerTools.h\
SequenceExceptions.h\
Sequence.h\
SequenceTools.h\
SiteContainerExceptions.h\
SiteContainer.h\
SiteContainerTools.h\
SiteExceptions.h\
Site.h\
SiteIterator.h\
SiteTools.h\
StandardCodonAlphabet.h\
StandardGeneticCode.h\
VertebrateMitochondrialCodonAlphabet.h\
VertebrateMitochondrialGeneticCode.h\
InvertebrateMitochondrialCodonAlphabet.h\
InvertebrateMitochondrialGeneticCode.h\
EchinodermMitochondrialCodonAlphabet.h\
EchinodermMitochondrialGeneticCode.h\
StringSequenceTools.h\
SymbolList.h\
SymbolListTools.h\
Translator.h\
VectorSequenceContainer.h\
VectorSiteContainer.h\
KleinAANetChargeIndex.h\
AAChargeIndex.h\
SequenceApplicationTools.h\
sequences\
IOSequenceFactory.h\
SimpleScore.h \
DistanceMatrix.h \
GenBank.h \
PhredPhd.h\
PhredPoly.h

CPP_FILES = \
AbstractAlphabet.cpp\
AbstractSequenceContainer.cpp\
AlignedSequenceContainer.cpp\
AlphabetExceptions.cpp\
AlphabetTools.cpp\
Clustal.cpp\
CodonAlphabet.cpp\
CodonSiteTools.cpp\
DCSE.cpp\
DefaultAlphabet.cpp\
DNA.cpp\
DNAToRNA.cpp\
Fasta.cpp\
GeneticCode.cpp\
GranthamAAChemicalDistance.cpp\
MiyataAAChemicalDistance.cpp\
BLOSUM50.cpp\
DefaultNucleotideScore.cpp\
AAIndex1Entry.cpp\
AAIndex2Entry.cpp\
MapSequenceContainer.cpp\
Mase.cpp\
MaseTools.cpp\
NucleicAcidsReplication.cpp\
Phylip.cpp\
ProteicAlphabet.cpp\
RNA.cpp\
SequenceContainerTools.cpp\
Sequence.cpp\
SequenceExceptions.cpp\
SequenceTools.cpp\
SiteContainerExceptions.cpp\
SiteContainerTools.cpp\
Site.cpp\
SiteExceptions.cpp\
SiteIterator.cpp\
SiteTools.cpp\
StandardCodonAlphabet.cpp\
StandardGeneticCode.cpp\
VertebrateMitochondrialCodonAlphabet.cpp\
VertebrateMitochondrialGeneticCode.cpp\
InvertebrateMitochondrialCodonAlphabet.cpp\
InvertebrateMitochondrialGeneticCode.cpp\
EchinodermMitochondrialCodonAlphabet.cpp\
EchinodermMitochondrialGeneticCode.cpp\
StringSequenceTools.cpp\
SymbolList.cpp\
SymbolListTools.cpp\
Translator.cpp\
VectorSequenceContainer.cpp\
VectorSiteContainer.cpp\
SequenceApplicationTools.cpp\
IOSequenceFactory.cpp\
SimpleScore.cpp\
DistanceMatrix.cpp\
GenBank.cpp\
PhredPhd.cpp\
PhredPoly.cpp

CPP_EXTRA = \
__GranthamMatrixCode\
__MiyataMatrixCode\
__BLOSUM50MatrixCode

lib_LTLIBRARIES = libbpp-seq.la
libbpp_seq_la_SOURCES = $(H_FILES) $(CPP_FILES)
EXTRA_libbpp_seq_la_SOURCES = $(CPP_EXTRA)
# CURRENT:REVISION:AGE => file.so.(C-A).A.R
# current: The most recent interface number that this library implements.
# revision: The implementation number of the current interface.
# age: The difference between the newest and oldest interfaces that this library implements.
# In other words, the library implements all the interface numbers in the range from number current - age to current.
libbpp_seq_la_LDFLAGS = -version-info 7:0:1

library_includedir=$(includedir)/Seq
library_include_HEADERS = $(H_FILES)

