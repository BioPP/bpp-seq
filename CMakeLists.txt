# CMake script for Bio++ Sequence library
# Authors:
#   Sylvain Gaillard
#   Julien Dutheil
#   Francois Gindraud (2017)
# Created: 20/08/2009

cmake_minimum_required (VERSION 2.8.11)
project (bpp-seq CXX)

# Global parameters
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

SET(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Weffc++ -Wshadow -Wconversion")

IF(NOT NO_DEP_CHECK)
  SET(NO_DEP_CHECK FALSE CACHE BOOL
      "Disable dependencies check for building distribution only."
      FORCE)
ENDIF(NOT NO_DEP_CHECK)

IF(NO_DEP_CHECK)
  MESSAGE("-- Dependencies checking disabled. Only distribution can be built.")
ELSE(NO_DEP_CHECK)

# Libtool-like version number
# CURRENT:REVISION:AGE => file.so.(C-A).A.R
# current:  The most recent interface number that this library implements.
# revision: The implementation number of the current interface.
# age:      The difference between the newest and oldest interfaces that this
#           library implements.
# In other words, the library implements all the interface numbers in the
# range from number current - age to current.
SET(BPPSEQ_VERSION_CURRENT "10")
SET(BPPSEQ_VERSION_REVISION "3")
SET(BPPSEQ_VERSION_AGE "1")

# Effective version number computation
math(EXPR BPPSEQ_VERSION_MAJOR "${BPPSEQ_VERSION_CURRENT} - ${BPPSEQ_VERSION_AGE}")
SET(BPPSEQ_VERSION_MINOR ${BPPSEQ_VERSION_AGE})
SET(BPPSEQ_VERSION_PATCH ${BPPSEQ_VERSION_REVISION})
SET(BPPSEQ_VERSION "${BPPSEQ_VERSION_MAJOR}.${BPPSEQ_VERSION_MINOR}.${BPPSEQ_VERSION_PATCH}")

# Set the CMAKE_PREFIX_PATH for the find_package function (to pull other bpp libs from install dir)
if (CMAKE_INSTALL_PREFIX)
  set (CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}" ${CMAKE_PREFIX_PATH})
endif (CMAKE_INSTALL_PREFIX)

macro (require_bpp_lib name)
  find_package (${name} REQUIRED)
  # Add IMPORTED targets to lists used later
  set (BPP_LIBS_STATIC "${BPP_LIBS_STATIC}" ${name}-static)
  set (BPP_LIBS_SHARED "${BPP_LIBS_SHARED}" ${name}-shared)
  # Show path information
  get_property (${name}-static-location TARGET ${name}-static PROPERTY LOCATION)
  get_property (${name}-shared-location TARGET ${name}-shared PROPERTY LOCATION)
  get_property (${name}-header-location TARGET ${name}-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
  message (STATUS "${name} found:")
  message (STATUS "  static lib: ${${name}-static-location}")
  message (STATUS "  shared lib: ${${name}-shared-location}")
  message (STATUS "  includes: ${${name}-header-location}")
endmacro (require_bpp_lib)

require_bpp_lib (bpp-core)

# Subdirectories
add_subdirectory (src) # Defines targets bpp-seq-static and bpp-seq-shared

# Doxygen
FIND_PACKAGE(Doxygen)
IF (DOXYGEN_FOUND)
  ADD_CUSTOM_TARGET (apidoc cp Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile-build
    COMMAND echo "OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}" >> ${CMAKE_BINARY_DIR}/Doxyfile-build
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile-build
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
  ADD_CUSTOM_TARGET (apidoc-stable cp Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile-stable
    COMMAND echo "OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}" >> ${CMAKE_BINARY_DIR}/Doxyfile-stable
    COMMAND echo "HTML_HEADER=header.html" >> ${CMAKE_BINARY_DIR}/Doxyfile-stable
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile-stable
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
ENDIF (DOXYGEN_FOUND)

ENDIF(NO_DEP_CHECK)

# Packager
SET(CPACK_PACKAGE_NAME "libbpp-seq")
SET(CPACK_PACKAGE_VENDOR "Bio++ Development Team")
SET(CPACK_PACKAGE_VERSION "2.2.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "2")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Bio++ Sequence library")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.txt")
SET(CPACK_RESOURCE_FILE_AUTHORS "${CMAKE_SOURCE_DIR}/AUTHORS.txt")
SET(CPACK_RESOURCE_FILE_INSTALL "${CMAKE_SOURCE_DIR}/INSTALL.txt")
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_SOURCE_IGNORE_FILES
 "CMakeFiles"
 "Makefile"
 "_CPack_Packages"
 "CMakeCache.txt"
 ".*\\\\.cmake"
 ".*\\\\.git"
 ".*\\\\.gz"
 ".*\\\\.deb"
 ".*\\\\.rpm"
 ".*\\\\.dmg"
 ".*\\\\..*\\\\.swp"
 "src/\\\\..*"
 "src/libbpp*"
 "html"
 "Seq.tag"
 "Testing"
 "build-stamp"
 "install_manifest.txt"
 "DartConfiguration.tcl"
 ${CPACK_SOURCE_IGNORE_FILES}
)
IF (MACOS)
  SET(CPACK_GENERATOR "Bundle")
ENDIF()

SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_DEBSOURCE_PACKAGE_FILE_NAME "lib${CMAKE_PROJECT_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}.orig")
INCLUDE(CPack)

#This adds the 'dist' target
ADD_CUSTOM_TARGET(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
# 'clean' is not (yet) a first class target. However, we need to clean the directories before building the sources:
IF("${CMAKE_GENERATOR}" MATCHES "Make")
  ADD_CUSTOM_TARGET(make_clean
  COMMAND ${CMAKE_MAKE_PROGRAM} clean
  WORKING_DIRECTORY ${CMAKE_CURRENT_DIR}
  )
  ADD_DEPENDENCIES(dist make_clean)
ENDIF()

IF(NOT NO_DEP_CHECK)
IF (UNIX)

#This creates rpm packages:
ADD_CUSTOM_TARGET(rpm rpmbuild -ta ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz)
ADD_DEPENDENCIES(rpm dist)

ENDIF()

SET(CTEST_UPDATE_TYPE git)
SET(UPDATE_COMMAND "git")
SET(UPDATE_OPTIONS "")

# Tests
enable_testing ()
include (CTest)
if (BUILD_TESTING)
  add_subdirectory (test)
endif (BUILD_TESTING)

ENDIF(NOT NO_DEP_CHECK)
