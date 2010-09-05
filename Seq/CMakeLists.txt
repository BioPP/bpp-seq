# CMake script for Bio++ NumCalc
# Author: Sylvain Gaillard
# Created: 20/08/2009

# File list
set(CPP_FILES
  AbstractAlphabet.cpp
  AbstractSequenceContainer.cpp
  AlignedSequenceContainer.cpp
  AlphabetExceptions.cpp
  AlphabetTools.cpp
  Clustal.cpp
  CodonAlphabet.cpp
  CodonSiteTools.cpp
  DCSE.cpp
  DefaultAlphabet.cpp
  CaseMaskedAlphabet.cpp
  DNA.cpp
  DNAToRNA.cpp
  Fasta.cpp
  GeneticCode.cpp
  GranthamAAChemicalDistance.cpp
  MiyataAAChemicalDistance.cpp
  BLOSUM50.cpp
  DefaultNucleotideScore.cpp
  AAIndex1Entry.cpp
  AAIndex2Entry.cpp
  MapSequenceContainer.cpp
  Mase.cpp
  MaseTools.cpp
  NucleicAcidsReplication.cpp
  Phylip.cpp
  ProteicAlphabet.cpp
  RNA.cpp
  SequenceContainerTools.cpp
  Sequence.cpp
  SequenceWithAnnotation.cpp
  SequenceWithQuality.cpp
  SequenceExceptions.cpp
  SequencePositionIterators.cpp
  SequenceTools.cpp
  SequenceWithQualityTools.cpp
  SiteContainerExceptions.cpp
  SiteContainerTools.cpp
  Site.cpp
  SiteExceptions.cpp
  SiteIterator.cpp
  SiteTools.cpp
  StandardCodonAlphabet.cpp
  StandardGeneticCode.cpp
  VertebrateMitochondrialCodonAlphabet.cpp
  VertebrateMitochondrialGeneticCode.cpp
  InvertebrateMitochondrialCodonAlphabet.cpp
  InvertebrateMitochondrialGeneticCode.cpp
  EchinodermMitochondrialCodonAlphabet.cpp
  EchinodermMitochondrialGeneticCode.cpp
  StringSequenceTools.cpp
  SymbolList.cpp
  SymbolListTools.cpp
  Translator.cpp
  VectorSequenceContainer.cpp
  VectorSiteContainer.cpp
  CompressedVectorSiteContainer.cpp
  SequenceApplicationTools.cpp
  IOSequenceFactory.cpp
  SimpleScore.cpp
  DistanceMatrix.cpp
  GenBank.cpp
  PhredPhd.cpp
  PhredPoly.cpp
  WordAlphabet.cpp
  NexusIOSequence.cpp
  NexusTools.cpp
  Stockholm.cpp
  MafAlignmentParser.cpp
  __GranthamMatrixCode
  __MiyataMatrixCode
  __BLOSUM50MatrixCode
  )

set(H_FILES
  AbstractAlphabet.h
  AbstractISequence.h
  AbstractISequence2.h
  AbstractOSequence.h
  AbstractSequenceContainer.h
  AlignedSequenceContainer.h
  AlphabetExceptions.h
  Alphabet.h
  AlphabetState.h
  AlphabetIndex1.h
  AlphabetIndex2.h
  alphabets
  AlphabetTools.h
  Clustal.h
  CodonAlphabet.h
  CodonSiteTools.h
  containers
  DCSE.h
  DefaultAlphabet.h
  CaseMaskedAlphabet.h
  DNA.h
  DNAToRNA.h
  Fasta.h
  GeneticCode.h
  GranthamAAChemicalDistance.h
  GranthamAAPolarityIndex.h
  GranthamAAVolumeIndex.h
  MiyataAAChemicalDistance.h
  BLOSUM50.h
  SimpleIndexDistance.h
  DefaultNucleotideScore.h
  AAIndex1Entry.h
  AAIndex2Entry.h
  IOSequence.h
  ISequence.h
  ISequenceStream.h
  IOSequenceStream.h
  ioseq
  LetterAlphabet.h
  MapSequenceContainer.h
  Mase.h
  MaseTools.h
  NucleicAlphabet.h
  NucleicAlphabetState.h
  NucleicAcidsReplication.h
  OrderedSequenceContainer.h
  OSequence.h
  OSequenceStream.h
  Phylip.h
  ProteicAlphabet.h
  ProteicAlphabetState.h
  RNA.h
  SequenceContainerExceptions.h
  SequenceContainer.h
  SequenceContainerTools.h
  SequenceExceptions.h
  SequenceFileIndex.h
  SequencePositionIterators.h
  Sequence.h
  SequenceWithAnnotation.h
  SequenceWithQuality.h
  SequenceTools.h
  SequenceWithQualityTools.h
  SiteContainerExceptions.h
  SiteContainer.h
  SiteContainerTools.h
  SiteExceptions.h
  Site.h
  SiteIterator.h
  SiteTools.h
  StandardCodonAlphabet.h
  StandardGeneticCode.h
  VertebrateMitochondrialCodonAlphabet.h
  VertebrateMitochondrialGeneticCode.h
  InvertebrateMitochondrialCodonAlphabet.h
  InvertebrateMitochondrialGeneticCode.h
  EchinodermMitochondrialCodonAlphabet.h
  EchinodermMitochondrialGeneticCode.h
  StringSequenceTools.h
  SymbolList.h
  SymbolListTools.h
  Translator.h
  VectorSequenceContainer.h
  VectorSiteContainer.h
  CompressedVectorSiteContainer.h
  KleinAANetChargeIndex.h
  AAChargeIndex.h
  SequenceApplicationTools.h
  sequences
  IOSequenceFactory.h
  SimpleScore.h
  DistanceMatrix.h
  GenBank.h
  PhredPhd.h
  PhredPoly.h
  WordAlphabet.h
  NexusIOSequence.h
  NexusTools.h
  Stockholm.h
  MafAlignmentParser.h
  geneticcodes
  aadistances
  )

# Build the static lib
add_library(bppseq-static STATIC ${CPP_FILES})
set_target_properties(bppseq-static
  PROPERTIES OUTPUT_NAME bpp-seq
  CLEAN_DIRECT_OUTPUT 1
  )
target_link_libraries(bppseq-static ${LIBS})

# Build the shared lib
add_library(bppseq-shared SHARED ${CPP_FILES})
set_target_properties(bppseq-shared
  PROPERTIES OUTPUT_NAME bpp-seq
  CLEAN_DIRECT_OUTPUT 1
  VERSION ${BPPSEQ_VERSION}
  SOVERSION ${BPPSEQ_VERSION_MAJOR}
  )
target_link_libraries(bppseq-shared ${LIBS})

# Install libs
install(TARGETS bppseq-static bppseq-shared DESTINATION lib)

# Install headers
install(FILES ${H_FILES} DESTINATION include/Seq)
