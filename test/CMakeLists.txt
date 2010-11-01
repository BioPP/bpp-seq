# CMake script for bpp-seq unit tests
# Author: Julien Dutheil
# Created: 30/10/2010

MACRO(IMPROVED_FIND_STATIC_LIBRARY OUTPUT_LIBS lib_name)
  #start:
  UNSET(${lib_name}_STATIC_LIBRARY CACHE) #Reset the cache.

  SET(${lib_name}_STATIC_NAMES lib${lib_name}.a)
  FIND_LIBRARY(${lib_name}_STATIC_LIBRARY NAMES ${${lib_name}_STATIC_NAMES})

  IF(${lib_name}_STATIC_LIBRARY)
    MESSAGE("-- Library ${lib_name} found here:")
    MESSAGE("   static libraries: ${${lib_name}_STATIC_LIBRARY}")
  ELSE()
    MESSAGE(FATAL_ERROR "${lib_name} required but not found.")
  ENDIF()
  
  #add the dependency:
  SET(${OUTPUT_LIBS} ${${OUTPUT_LIBS}} ${${lib_name}_STATIC_LIBRARY})
ENDMACRO(IMPROVED_FIND_STATIC_LIBRARY)

IMPROVED_FIND_STATIC_LIBRARY(STATIC_LIBS bpp-core)

MACRO(TEST_FIND_LIBRARY OUTPUT_LIBS lib_name include_to_find)
  #start:
  FIND_PATH(${lib_name}_INCLUDE_DIR ${include_to_find})
  SET(${lib_name}_STATIC_NAMES ${lib_name}.a)
  FIND_LIBRARY(${lib_name}_STATIC_LIBRARY NAMES ${${lib_name}_STATIC_NAMES})

  IF(${lib_name}_STATIC_LIBRARY)
    MESSAGE(WARNING "Library ${lib_name} is already installed in the system tree. Test will be built against it. This may lead to unexpected results. You may want to do 'make install' before 'make test', or remove the installed version.")
    #add the dependency:
    INCLUDE_DIRECTORIES(${${lib_name}_INCLUDE_DIR})
    SET(${OUTPUT_LIBS} ${${OUTPUT_LIBS}} ${${lib_name}_STATIC_LIBRARY})
  ELSE()
    INCLUDE_DIRECTORIES("../src/")
    SET(${OUTPUT_LIBS} "--static -L../src -lbpp-seq" ${${OUTPUT_LIBS}})
  ENDIF()  
ENDMACRO(TEST_FIND_LIBRARY)

#Find the bpp-seq library library:
TEST_FIND_LIBRARY(STATIC_LIBS bpp-seq Bpp/Seq/Alphabet/Alphabet.h)

ADD_EXECUTABLE(test_alphabets test_alphabets.cpp)
TARGET_LINK_LIBRARIES(test_alphabets ${STATIC_LIBS})
ADD_TEST(test_alphabets "test_alphabets")

ADD_EXECUTABLE(test_io test_io.cpp)
TARGET_LINK_LIBRARIES(test_io ${STATIC_LIBS})
ADD_TEST(test_io "test_io")

ADD_EXECUTABLE(test_containers test_containers.cpp)
TARGET_LINK_LIBRARIES(test_containers ${STATIC_LIBS})
ADD_TEST(test_containers "test_containers")

