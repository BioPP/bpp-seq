# CMake script for Bio++ SeqLib
# Author: Sylvain Gaillard and Julien Dutheil
# Created: 20/08/2009

# File list
set(CPP_FILES
  Bpp/Seq/DNAToRNA.cpp
  Bpp/Seq/NucleicAcidsReplication.cpp
  Bpp/Seq/Sequence.cpp
  Bpp/Seq/SequenceWithAnnotation.cpp
  Bpp/Seq/SequenceWithAnnotationTools.cpp
  Bpp/Seq/SequenceWithQuality.cpp
  Bpp/Seq/SequenceExceptions.cpp
  Bpp/Seq/SequencePositionIterators.cpp
  Bpp/Seq/SequenceTools.cpp
  Bpp/Seq/SequenceWithQualityTools.cpp
  Bpp/Seq/Site.cpp
  Bpp/Seq/SiteExceptions.cpp
  Bpp/Seq/SiteTools.cpp
  Bpp/Seq/StringSequenceTools.cpp
  Bpp/Seq/SymbolList.cpp
  Bpp/Seq/SymbolListTools.cpp
  Bpp/Seq/Transliterator.cpp
  Bpp/Seq/DistanceMatrix.cpp
  Bpp/Seq/CodonSiteTools.cpp
  Bpp/Seq/Alphabet/AbstractAlphabet.cpp
  Bpp/Seq/Alphabet/AlphabetExceptions.cpp
  Bpp/Seq/Alphabet/AlphabetTools.cpp
  Bpp/Seq/Alphabet/CodonAlphabet.cpp
  Bpp/Seq/Alphabet/DefaultAlphabet.cpp
  Bpp/Seq/Alphabet/CaseMaskedAlphabet.cpp
  Bpp/Seq/Alphabet/DNA.cpp
  Bpp/Seq/Alphabet/ProteicAlphabet.cpp
  Bpp/Seq/Alphabet/RNA.cpp
  Bpp/Seq/Alphabet/StandardCodonAlphabet.cpp
  Bpp/Seq/Alphabet/VertebrateMitochondrialCodonAlphabet.cpp
  Bpp/Seq/Alphabet/InvertebrateMitochondrialCodonAlphabet.cpp
  Bpp/Seq/Alphabet/EchinodermMitochondrialCodonAlphabet.cpp
  Bpp/Seq/Alphabet/YeastMitochondrialCodonAlphabet.cpp
  Bpp/Seq/Alphabet/WordAlphabet.cpp
  Bpp/Seq/Alphabet/RNY.cpp
  Bpp/Seq/GeneticCode/GeneticCode.cpp
  Bpp/Seq/GeneticCode/StandardGeneticCode.cpp
  Bpp/Seq/GeneticCode/VertebrateMitochondrialGeneticCode.cpp
  Bpp/Seq/GeneticCode/InvertebrateMitochondrialGeneticCode.cpp
  Bpp/Seq/GeneticCode/EchinodermMitochondrialGeneticCode.cpp
  Bpp/Seq/GeneticCode/YeastMitochondrialGeneticCode.cpp
  Bpp/Seq/StateProperties/GranthamAAChemicalDistance.cpp
  Bpp/Seq/StateProperties/MiyataAAChemicalDistance.cpp
  Bpp/Seq/StateProperties/BLOSUM50.cpp
  Bpp/Seq/StateProperties/DefaultNucleotideScore.cpp
  Bpp/Seq/StateProperties/AAIndex1Entry.cpp
  Bpp/Seq/StateProperties/AAIndex2Entry.cpp
  Bpp/Seq/StateProperties/SimpleScore.cpp
  Bpp/Seq/StateProperties/__GranthamMatrixCode
  Bpp/Seq/StateProperties/__MiyataMatrixCode
  Bpp/Seq/StateProperties/__BLOSUM50MatrixCode
  Bpp/Seq/Container/AbstractSequenceContainer.cpp
  Bpp/Seq/Container/AlignedSequenceContainer.cpp
  Bpp/Seq/Container/MapSequenceContainer.cpp
  Bpp/Seq/Container/SequenceContainerTools.cpp
  Bpp/Seq/Container/SiteContainerExceptions.cpp
  Bpp/Seq/Container/SiteContainerTools.cpp
  Bpp/Seq/Container/VectorSequenceContainer.cpp
  Bpp/Seq/Container/VectorSiteContainer.cpp
  Bpp/Seq/Container/CompressedVectorSiteContainer.cpp
  Bpp/Seq/Container/SiteIterator.cpp
  Bpp/Seq/Io/Clustal.cpp
  Bpp/Seq/Io/Dcse.cpp
  Bpp/Seq/Io/Fasta.cpp
  Bpp/Seq/Io/Mase.cpp
  Bpp/Seq/Io/MaseTools.cpp
  Bpp/Seq/Io/Phylip.cpp
  Bpp/Seq/Io/IoSequenceFactory.cpp
  Bpp/Seq/Io/GenBank.cpp
  Bpp/Seq/Io/PhredPhd.cpp
  Bpp/Seq/Io/PhredPoly.cpp
  Bpp/Seq/Io/NexusIoSequence.cpp
  Bpp/Seq/Io/NexusTools.cpp
  Bpp/Seq/Io/Stockholm.cpp
  Bpp/Seq/Io/MafAlignmentParser.cpp
  Bpp/Seq/Io/MafIterator.cpp
  Bpp/Seq/App/SequenceApplicationTools.cpp
  )

set(H_FILES
  Bpp/Seq/CodonSiteTools.h
  Bpp/Seq/DNAToRNA.h
  Bpp/Seq/NucleicAcidsReplication.h
  Bpp/Seq/SequenceExceptions.h
  Bpp/Seq/SequencePositionIterators.h
  Bpp/Seq/Sequence.h
  Bpp/Seq/SequenceWithAnnotation.h
  Bpp/Seq/SequenceWithAnnotationTools.h
  Bpp/Seq/SequenceWithQuality.h
  Bpp/Seq/SequenceTools.h
  Bpp/Seq/SequenceWithQualityTools.h
  Bpp/Seq/SiteExceptions.h
  Bpp/Seq/Site.h
  Bpp/Seq/SiteTools.h
  Bpp/Seq/StringSequenceTools.h
  Bpp/Seq/SymbolList.h
  Bpp/Seq/SymbolListTools.h
  Bpp/Seq/Transliterator.h
  Bpp/Seq/DistanceMatrix.h
  Bpp/Seq/Alphabet/AbstractAlphabet.h
  Bpp/Seq/Alphabet/AlphabetExceptions.h
  Bpp/Seq/Alphabet/Alphabet.h
  Bpp/Seq/Alphabet/AlphabetState.h
  Bpp/Seq/Alphabet/AlphabetTools.h
  Bpp/Seq/Alphabet/CodonAlphabet.h
  Bpp/Seq/Alphabet/CaseMaskedAlphabet.h
  Bpp/Seq/Alphabet/DefaultAlphabet.h
  Bpp/Seq/Alphabet/DNA.h
  Bpp/Seq/Alphabet/LetterAlphabet.h
  Bpp/Seq/Alphabet/NucleicAlphabet.h
  Bpp/Seq/Alphabet/NucleicAlphabetState.h
  Bpp/Seq/Alphabet/ProteicAlphabet.h
  Bpp/Seq/Alphabet/ProteicAlphabetState.h
  Bpp/Seq/Alphabet/RNA.h
  Bpp/Seq/Alphabet/StandardCodonAlphabet.h
  Bpp/Seq/Alphabet/VertebrateMitochondrialCodonAlphabet.h
  Bpp/Seq/Alphabet/InvertebrateMitochondrialCodonAlphabet.h
  Bpp/Seq/Alphabet/EchinodermMitochondrialCodonAlphabet.h
  Bpp/Seq/Alphabet/YeastMitochondrialCodonAlphabet.h
  Bpp/Seq/Alphabet/WordAlphabet.h
  Bpp/Seq/Alphabet/RNY.h
  Bpp/Seq/GeneticCode/GeneticCode.h
  Bpp/Seq/GeneticCode/StandardGeneticCode.h
  Bpp/Seq/GeneticCode/VertebrateMitochondrialGeneticCode.h
  Bpp/Seq/GeneticCode/InvertebrateMitochondrialGeneticCode.h
  Bpp/Seq/GeneticCode/EchinodermMitochondrialGeneticCode.h
  Bpp/Seq/GeneticCode/YeastMitochondrialGeneticCode.h
  Bpp/Seq/Io/AbstractISequence.h
  Bpp/Seq/Io/AbstractISequence2.h
  Bpp/Seq/Io/AbstractOSequence.h
  Bpp/Seq/Io/Clustal.h
  Bpp/Seq/Io/Dcse.h
  Bpp/Seq/Io/Fasta.h
  Bpp/Seq/Io/IOSequence.h
  Bpp/Seq/Io/ISequence.h
  Bpp/Seq/Io/ISequenceStream.h
  Bpp/Seq/Io/IoSequenceStream.h
  Bpp/Seq/Io/Mase.h
  Bpp/Seq/Io/MaseTools.h
  Bpp/Seq/Io/OSequence.h
  Bpp/Seq/Io/OSequenceStream.h
  Bpp/Seq/Io/Phylip.h
  Bpp/Seq/Io/GenBank.h
  Bpp/Seq/Io/PhredPhd.h
  Bpp/Seq/Io/PhredPoly.h
  Bpp/Seq/Io/NexusIOSequence.h
  Bpp/Seq/Io/NexusTools.h
  Bpp/Seq/Io/Stockholm.h
  Bpp/Seq/Io/MafAlignmentParser.h
  Bpp/Seq/Io/MafIterator.h
  Bpp/Seq/Io/IoSequenceFactory.h
  Bpp/Seq/Io/SequenceFileIndex.h
  Bpp/Seq/Container/VectorSequenceContainer.h
  Bpp/Seq/Container/VectorSiteContainer.h
  Bpp/Seq/Container/CompressedVectorSiteContainer.h
  Bpp/Seq/Container/MapSequenceContainer.h
  Bpp/Seq/Container/OrderedSequenceContainer.h
  Bpp/Seq/Container/AbstractSequenceContainer.h
  Bpp/Seq/Container/AlignedSequenceContainer.h 
  Bpp/Seq/Container/SequenceContainerExceptions.h
  Bpp/Seq/Container/SequenceContainer.h
  Bpp/Seq/Container/SequenceContainerTools.h
  Bpp/Seq/Container/SiteContainerExceptions.h
  Bpp/Seq/Container/SiteContainer.h
  Bpp/Seq/Container/SiteContainerTools.h
  Bpp/Seq/Container/SiteIterator.h
  Bpp/Seq/StateProperties/AlphabetIndex1.h
  Bpp/Seq/StateProperties/AlphabetIndex2.h
  Bpp/Seq/StateProperties/GranthamAAChemicalDistance.h
  Bpp/Seq/StateProperties/GranthamAAPolarityIndex.h
  Bpp/Seq/StateProperties/GranthamAAVolumeIndex.h
  Bpp/Seq/StateProperties/MiyataAAChemicalDistance.h
  Bpp/Seq/StateProperties/BLOSUM50.h
  Bpp/Seq/StateProperties/SimpleIndexDistance.h
  Bpp/Seq/StateProperties/DefaultNucleotideScore.h
  Bpp/Seq/StateProperties/AAIndex1Entry.h
  Bpp/Seq/StateProperties/AAIndex2Entry.h
  Bpp/Seq/StateProperties/KleinAANetChargeIndex.h
  Bpp/Seq/StateProperties/AAChargeIndex.h
  Bpp/Seq/StateProperties/SimpleScore.h
  Bpp/Seq/StateProperties/AAChouFasmanAHelixIndex.h
  Bpp/Seq/StateProperties/AAChouFasmanBSheetIndex.h
  Bpp/Seq/StateProperties/AAChouFasmanTurnIndex.h
  Bpp/Seq/StateProperties/AAVolumeIndex.h
  Bpp/Seq/StateProperties/AASurfaceIndex.h
  Bpp/Seq/StateProperties/AAMassIndex.h
  Bpp/Seq/StateProperties/AAChenGuHuangHydrophobicityIndex.h
  Bpp/Seq/StateProperties/AASEASup30Index.h
  Bpp/Seq/StateProperties/AASEAInf10Index.h
  Bpp/Seq/StateProperties/AASEA1030Index.h
  Bpp/Seq/App/SequenceApplicationTools.h
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
install(DIRECTORY Bpp/ DESTINATION include/Bpp FILES_MATCHING PATTERN ".svn" EXCLUDE PATTERN "*.h" PATTERN "*.all")
