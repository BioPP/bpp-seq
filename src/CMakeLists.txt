# CMake script for Bio++ Sequence Library
# Authors:
#   Sylvain Gaillard
#   Julien Dutheil
#   Francois Gindraud (2017)
# Created: 20/08/2009

# File list
SET(CPP_FILES
  Bpp/Seq/Alphabet/AbstractAlphabet.cpp
  Bpp/Seq/Alphabet/AlphabetExceptions.cpp
  Bpp/Seq/Alphabet/AlphabetTools.cpp
  Bpp/Seq/Alphabet/BinaryAlphabet.cpp
  Bpp/Seq/Alphabet/CaseMaskedAlphabet.cpp
  Bpp/Seq/Alphabet/CodonAlphabet.cpp
  Bpp/Seq/Alphabet/DNA.cpp
  Bpp/Seq/Alphabet/DefaultAlphabet.cpp
  Bpp/Seq/Alphabet/IntegerAlphabet.cpp
  Bpp/Seq/Alphabet/LetterAlphabet.cpp
  Bpp/Seq/Alphabet/LexicalAlphabet.cpp
  Bpp/Seq/Alphabet/NumericAlphabet.cpp
  Bpp/Seq/Alphabet/ProteicAlphabet.cpp
  Bpp/Seq/Alphabet/RNA.cpp
  Bpp/Seq/Alphabet/RNY.cpp
  Bpp/Seq/Alphabet/WordAlphabet.cpp
  Bpp/Seq/AlphabetIndex/AAIndex1Entry.cpp
  Bpp/Seq/AlphabetIndex/AAIndex2Entry.cpp
  Bpp/Seq/AlphabetIndex/BLOSUM50.cpp
  Bpp/Seq/AlphabetIndex/DefaultNucleotideScore.cpp
  Bpp/Seq/AlphabetIndex/GranthamAAChemicalDistance.cpp
  Bpp/Seq/AlphabetIndex/MiyataAAChemicalDistance.cpp
  Bpp/Seq/AlphabetIndex/SimpleScore.cpp
  Bpp/Seq/AlphabetIndex/__BLOSUM50MatrixCode
  Bpp/Seq/AlphabetIndex/__GranthamMatrixCode
  Bpp/Seq/AlphabetIndex/__MiyataMatrixCode
  Bpp/Seq/App/SequenceApplicationTools.cpp
  Bpp/Seq/CodonSiteTools.cpp
  Bpp/Seq/Container/AbstractSequenceContainer.cpp
  Bpp/Seq/Container/AlignedSequenceContainer.cpp
  Bpp/Seq/Container/CompressedVectorSiteContainer.cpp
  Bpp/Seq/Container/MapSequenceContainer.cpp
  Bpp/Seq/Container/SequenceContainerIterator.cpp
  Bpp/Seq/Container/SequenceContainerTools.cpp
  Bpp/Seq/Container/SiteContainerExceptions.cpp
  Bpp/Seq/Container/SiteContainerIterator.cpp
  Bpp/Seq/Container/SiteContainerTools.cpp
  Bpp/Seq/Container/VectorSequenceContainer.cpp
  Bpp/Seq/Container/VectorSiteContainer.cpp
  Bpp/Seq/DNAToRNA.cpp
  Bpp/Seq/DistanceMatrix.cpp
  Bpp/Seq/GeneticCode/AscidianMitochondrialGeneticCode.cpp
  Bpp/Seq/GeneticCode/EchinodermMitochondrialGeneticCode.cpp
  Bpp/Seq/GeneticCode/GeneticCode.cpp
  Bpp/Seq/GeneticCode/InvertebrateMitochondrialGeneticCode.cpp
  Bpp/Seq/GeneticCode/MoldMitochondrialGeneticCode.cpp
  Bpp/Seq/GeneticCode/StandardGeneticCode.cpp
  Bpp/Seq/GeneticCode/VertebrateMitochondrialGeneticCode.cpp
  Bpp/Seq/GeneticCode/YeastMitochondrialGeneticCode.cpp
  Bpp/Seq/Io/BppOAlignmentReaderFormat.cpp
  Bpp/Seq/Io/BppOAlignmentWriterFormat.cpp
  Bpp/Seq/Io/BppOAlphabetIndex1Format.cpp
  Bpp/Seq/Io/BppOAlphabetIndex2Format.cpp
  Bpp/Seq/Io/BppOSequenceReaderFormat.cpp
  Bpp/Seq/Io/BppOSequenceStreamReaderFormat.cpp
  Bpp/Seq/Io/BppOSequenceWriterFormat.cpp
  Bpp/Seq/Io/Clustal.cpp
  Bpp/Seq/Io/Dcse.cpp
  Bpp/Seq/Io/Fasta.cpp
  Bpp/Seq/Io/GenBank.cpp
  Bpp/Seq/Io/IoSequenceFactory.cpp
  Bpp/Seq/Io/Mase.cpp
  Bpp/Seq/Io/MaseTools.cpp
  Bpp/Seq/Io/NexusIoSequence.cpp
  Bpp/Seq/Io/NexusTools.cpp
  Bpp/Seq/Io/PhredPhd.cpp
  Bpp/Seq/Io/PhredPoly.cpp
  Bpp/Seq/Io/Phylip.cpp
  Bpp/Seq/Io/Stockholm.cpp
  Bpp/Seq/Io/StreamSequenceIterator.cpp
  Bpp/Seq/NucleicAcidsReplication.cpp
  Bpp/Seq/Sequence.cpp
  Bpp/Seq/SequenceExceptions.cpp
  Bpp/Seq/SequencePositionIterators.cpp
  Bpp/Seq/SequenceTools.cpp
  Bpp/Seq/SequenceWalker.cpp
  Bpp/Seq/SequenceWithAnnotation.cpp
  Bpp/Seq/SequenceWithAnnotationTools.cpp
  Bpp/Seq/SequenceWithQuality.cpp
  Bpp/Seq/SequenceWithQualityTools.cpp
  Bpp/Seq/Site.cpp
  Bpp/Seq/SiteExceptions.cpp
  Bpp/Seq/SiteTools.cpp
  Bpp/Seq/StringSequenceTools.cpp
  Bpp/Seq/SymbolList.cpp
  Bpp/Seq/SymbolListTools.cpp
  Bpp/Seq/Transliterator.cpp
  )

# Build the static lib
add_library (bpp-seq-static STATIC ${CPP_FILES})
target_include_directories (bpp-seq-static PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
  )
set_target_properties (bpp-seq-static PROPERTIES OUTPUT_NAME bpp-seq)
target_link_libraries (bpp-seq-static ${BPP_LIBS_STATIC})

# Build the shared lib
add_library (bpp-seq-shared SHARED ${CPP_FILES})
target_include_directories (bpp-seq-shared PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
  )
set_target_properties (bpp-seq-shared
  PROPERTIES OUTPUT_NAME bpp-seq
  MACOSX_RPATH 1
  VERSION ${BPPSEQ_VERSION}
  SOVERSION ${BPPSEQ_VERSION_MAJOR}
  )
target_link_libraries (bpp-seq-shared ${BPP_LIBS_SHARED})

# Install libs and headers
install (
  TARGETS bpp-seq-static bpp-seq-shared
  EXPORT bpp-seq-config
  LIBRARY DESTINATION lib${LIB_SUFFIX}
  ARCHIVE DESTINATION lib${LIB_SUFFIX}
  )
install (
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
  )
install (EXPORT bpp-seq-config DESTINATION lib/cmake/bpp-seq)

