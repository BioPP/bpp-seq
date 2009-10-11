# CMake script for Bio++ SeqLib
# Author: Sylvain Gaillard and Julien Dutheil
# Created: 20/08/2009

# Global parameters
cmake_minimum_required(VERSION 2.6)
project(bppseq CXX)
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT DEFINED CMAKE_BUILD_TYPE)
set(CMAKE_CXX_FLAGS "-Wall")

# Libtool-like version number
# CURRENT:REVISION:AGE => file.so.(C-A).A.R
# current:  The most recent interface number that this library implements.
# revision: The implementation number of the current interface.
# age:      The difference between the newest and oldest interfaces that this
#           library implements.
# In other words, the library implements all the interface numbers in the
# range from number current - age to current.
set(BPPSEQ_VERSION_CURRENT "8")
set(BPPSEQ_VERSION_REVISION "0")
set(BPPSEQ_VERSION_AGE "0")

# Effective version number computation
math(EXPR BPPSEQ_VERSION_MAJOR "${BPPSEQ_VERSION_CURRENT} - ${BPPSEQ_VERSION_AGE}")
set(BPPSEQ_VERSION_MINOR ${BPPSEQ_VERSION_AGE})
set(BPPSEQ_VERSION_PATCH ${BPPSEQ_VERSION_REVISION})
set(BPPSEQ_VERSION "${BPPSEQ_VERSION_MAJOR}.${BPPSEQ_VERSION_MINOR}.${BPPSEQ_VERSION_PATCH}")

# Set the CMAKE_PREFIX_PATH for the find_library fonction when using non
# standard install location
if(CMAKE_INSTALL_PREFIX)
  set(CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}" ${CMAKE_PREFIX_PATH})
endif(CMAKE_INSTALL_PREFIX)

# Find the bpp-utils library
find_path(BPPUTILS_INCLUDE_DIR Utils/Clonable.h)
set(BPPUTILS_NAMES bpp-utils bpp-utilslib bpp-utilsdll)
find_library(BPPUTILS_LIBRARY NAMES ${BPPUTILS_NAMES})
set( BPPUTILS_LIBRARIES ${BPPUTILS_LIBRARY} )
if(BPPUTILS_LIBRARY)
  message("-- bpp-utils found here:")
  message("   includes : ${BPPUTILS_INCLUDE_DIR}")
  message("   libraries: ${BPPUTILS_LIBRARIES}")
else(BPPUTILS_LIBRARY)
  message(FATAL_ERROR "bpp-utils required but not found")
endif(BPPUTILS_LIBRARY)

# Include the bpp-utils dependency
include_directories(${BPPUTILS_INCLUDE_DIR})
set(LIBS ${LIBS} ${BPPUTILS_LIBRARIES})

# Find the bpp-numcalc library
find_path(BPPNUMCALC_INCLUDE_DIR NumCalc/NumTools.h)
set(BPPNUMCALC_NAMES bpp-numcalc bpp-numcalclib bpp-numcalcdll)
find_library(BPPNUMCALC_LIBRARY NAMES ${BPPNUMCALC_NAMES})
set( BPPNUMCALC_LIBRARIES ${BPPNUMCALC_LIBRARY} )
if(BPPNUMCALC_LIBRARY)
  message("-- bpp-numcalc found here:")
  message("   includes : ${BPPNUMCALC_INCLUDE_DIR}")
  message("   libraries: ${BPPNUMCALC_LIBRARIES}")
else(BPPNUMCALC_LIBRARY)
  message(FATAL_ERROR "bpp-numcalc required but not found")
endif(BPPNUMCALC_LIBRARY)

# Include the bpp-numcalc dependency
include_directories(${BPPNUMCALC_INCLUDE_DIR})
set(LIBS ${LIBS} ${BPPNUMCALC_LIBRARIES})

# Subdirectories
add_subdirectory(Seq)

# Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
  add_custom_target (apidoc ${DOXYGEN_EXECUTABLE} Doxyfile)
endif (DOXYGEN_FOUND)

# Packager
set(CPACK_PACKAGE_NAME "libbpp-seq")
set(CPACK_PACKAGE_VENDOR "Bio++ Development Team")
set(CPACK_PACKAGE_VERSION "1.6.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "6")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Bio++ Sequence library")
set(CPACK_RESOURCE_FILE_LICENSE "COPYING")
set(CPACK_RESOURCE_FILE_AUTHORS "AUTHORS")
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SYSTEM_NAME x86_64)
set(CPACK_TOPLEVEL_TAG x86_64)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6, libbpp-utils (= 1.4.0), libbpp-numcalc (= 1.7.0)")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Julien Dutheil <jdutheil@birc.au.dk>")
set(CPACK_SOURCE_IGNORE_FILES
 ".*CMakeFiles"
 ".*Makefile"
 ".*_CPack_Packages"
 ".*CMakeCache.txt"
 "debian/"
 "Seq.spec"
 ".*\\\\.cmake"
 ".*CVS/"
 ".*.gz"
 ".*.deb"
 ".*.rpm"
 ".*~"
 "Seq/\\\\..*"
 "html"
 "Seq.tag"
 "autogen.sh"
 "autoclean.sh"
 ${CPACK_SOURCE_IGNORE_FILES}
)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
include(CPack)

#This adds the 'dist' target
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

