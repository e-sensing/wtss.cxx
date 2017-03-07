#
#   Copyright (C) 2016 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of WTSS.CXX.
#
#  WTSS.CXX is free software: you can
#  redistribute it and/or modify it under the terms of the
#  GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  WTSS.CXX is distributed in the hope that
#  it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with WTSS.CXX. See LICENSE. If not, write to
#  e-sensing team at <esensing-team@dpi.inpe.br>.
#
#  Description: Main CMake script for WTSS.CXX build system.
#
#  Author: Gilberto Ribeiro de Queiroz
#          Matheus Cavassan Zaglia
#

cmake_minimum_required(VERSION 2.8.12)

project(wtss_cxx)

#
# main variables for the build script
#

# variables that control the system versioning
set(WTSS_CXX_VERSION_MAJOR 0)
set(WTSS_CXX_VERSION_MINOR 4)
set(WTSS_CXX_VERSION_PATCH 0)
set(WTSS_CXX_VERSION_STATUS "")
set(WTSS_CXX_VERSION_STATUS_FULL "")
set(WTSS_CXX_VERSION_STRING "${WTSS_CXX_VERSION_MAJOR}.${WTSS_CXX_VERSION_MINOR}.${WTSS_CXX_VERSION_PATCH}${WTSS_CXX_VERSION_STATUS_FULL}")
set(WTSS_CXX_VERSION "0x000100")

# variables storing absolute paths
get_filename_component(WTSS_CXX_ABSOLUTE_ROOT_DIR "../../" ABSOLUTE)

# variables that control paths
set(WTSS_CXX_BASE_DESTINATION_DIR "")

# variables that control install locations
if(NOT DEFINED WTSS_CXX_DESTINATION_UNITTEST)
  set(WTSS_CXX_DESTINATION_UNITTEST "${WTSS_CXX_BASE_DESTINATION_DIR}share/wtss-cxx/unittest")
endif()

if(NOT DEFINED WTSS_CXX_DESTINATION_LIBRARY)
  set(WTSS_CXX_DESTINATION_LIBRARY "${WTSS_CXX_BASE_DESTINATION_DIR}lib")
endif()

if(NOT DEFINED WTSS_CXX_DESTINATION_RUNTIME)
  set(WTSS_CXX_DESTINATION_RUNTIME "${WTSS_CXX_BASE_DESTINATION_DIR}bin")
endif()

if(NOT DEFINED WTSS_CXX_DESTINATION_ARCHIVE)
  set(WTSS_CXX_DESTINATION_ARCHIVE "${WTSS_CXX_BASE_DESTINATION_DIR}lib")
endif()

if(NOT DEFINED WTSS_CXX_DESTINATION_HEADERS)
  set(WTSS_CXX_DESTINATION_HEADERS "${WTSS_CXX_BASE_DESTINATION_DIR}include")
endif()


#
# global definitions, includes and config
#

# Common definitions to all modules
add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_UBLAS_TYPE_CHECK=0)
add_definitions(-DWTSSCXXDLL)

# Specific definitions per platform
#if(APPLE)
#  add_definitions(-ftemplate-depth-1024)
#endif()

# For each CMake version we need to check some policies
# Note: automatically link executables with qtmain.lib on Windows when it has Qt dependency.
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER  2.8.9)
  cmake_policy(SET CMP0020 NEW)
endif()

if(${CMAKE_MAJOR_VERSION} GREATER 2)
  cmake_policy(SET CMP0026 OLD)
  cmake_policy(SET CMP0042 NEW)
  cmake_policy(SET CMP0043 NEW)
endif()

if(${CMAKE_VERSION} VERSION_GREATER "3.1.2")
  cmake_policy(SET CMP0053 OLD)
endif()

# Build will generate files into a single folder
if((CMAKE_GENERATOR STREQUAL Xcode) OR MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# Where to find modules (find procedures)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# Compiler options: warning and C++ 11
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER MATCHES ".*clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
endif()


# Enable CTest
enable_testing()

# Build tree directory will be needed in all projects.
include_directories(${CMAKE_BINARY_DIR})

#
# first check of libraries
#

# log timer
find_package(Boost 1.54.0 REQUIRED)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "WTSS_CXX: could not find required Boost libraries!")
endif()

find_package(CURL REQUIRED)

#find_package(RapidJSON REQUIRED)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "WTSS_CXX: could not find required RapidJSON libraries!")
endif()


#
# some variables needed in config file
#

#
# add include targets
#
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CURL_INCLUDE_DIRS})
include_directories("${WTSS_CXX_ABSOLUTE_ROOT_DIR}/3rdparty")

#
# build targets
#

# wtss.cxx client library
file(GLOB WTSS_CXX_SRC_FILES ${WTSS_CXX_ABSOLUTE_ROOT_DIR}/src/wtss-cxx/*.cpp)
file(GLOB WTSS_CXX_HDR_FILES ${WTSS_CXX_ABSOLUTE_ROOT_DIR}/src/wtss-cxx/*.hpp)

add_library(wtss_cxx_client SHARED ${WTSS_CXX_SRC_FILES} ${WTSS_CXX_HDR_FILES})

target_link_libraries(wtss_cxx_client ${CURL_LIBRARIES})

# wtss.cxx unit-test
include_directories(${WTSS_CXX_ABSOLUTE_ROOT_DIR}/src)

file(GLOB WTSS_CXX_UTEST_SRC_FILES ${WTSS_CXX_ABSOLUTE_ROOT_DIR}/src/unittest/*.cpp)
file(GLOB WTSS_CXX_UTEST_HDR_FILES ${WTSS_CXX_ABSOLUTE_ROOT_DIR}/src/unittest/*.hpp)

add_executable(wtss_cxx_unittest ${WTSS_CXX_UTEST_HDR_FILES} ${WTSS_CXX_UTEST_SRC_FILES})

target_link_libraries(wtss_cxx_unittest wtss_cxx_client)

# file organization in some IDEs
source_group("wtss_cxx\\Source Files"  FILES ${WTSS_CXX_SRC_FILES})
source_group("wtss_cxx\\Header Files"  FILES ${WTSS_CXX_HDR_FILES})

source_group("wtss_unittest\\Source Files"  FILES ${WTSS_CXX_UTEST_SRC_FILES})
source_group("wtss_unittest\\Header Files"  FILES ${WTSS_CXX_UTEST_HDR_FILES})

#
# process WTSS_CXX configuration files
#

configure_file(${WTSS_CXX_ABSOLUTE_ROOT_DIR}/src/wtss-cxx/version.hpp.in
               ${CMAKE_BINARY_DIR}/wtss_cxx_version.hpp @ONLY)

configure_file(${WTSS_CXX_ABSOLUTE_ROOT_DIR}/src/wtss-cxx/config.hpp.in
              ${CMAKE_BINARY_DIR}/wtss_cxx_config.hpp @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/wtss_cxx-config-version.cmake.in
               ${CMAKE_BINARY_DIR}/wtss_cxx-config-version.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/wtss_cxx-config.cmake.in
               ${CMAKE_BINARY_DIR}/wtss_cxx-config.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/wtss_cxx-config-pkg.cmake.in
               ${CMAKE_BINARY_DIR}/pkg/wtss_cxx-config.cmake @ONLY)

#
# install and targets export
#

install(TARGETS wtss_cxx_client
        EXPORT wtss_cxx-targets
        RUNTIME DESTINATION ${WTSS_CXX_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${WTSS_CXX_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${WTSS_CXX_DESTINATION_ARCHIVE} COMPONENT runtime)

install(EXPORT wtss_cxx-targets
        DESTINATION ${WTSS_CXX_DESTINATION_LIBRARY}/cmake/wtss_cxx-${WTSS_CXX_VERSION_STRING}
        FILE wtss_cxx-exports.cmake
        COMPONENT devel)

install(FILES ${CMAKE_BINARY_DIR}/pkg/wtss_cxx-config.cmake
              ${CMAKE_BINARY_DIR}/wtss_cxx-config-version.cmake
        DESTINATION ${WTSS_CXX_DESTINATION_LIBRARY}/cmake/wtss_cxx-${WTSS_CXX_VERSION_STRING}
        COMPONENT devel)

install(FILES ${CMAKE_BINARY_DIR}/wtss_cxx_version.hpp
              ${CMAKE_BINARY_DIR}/wtss_cxx_config.hpp
              ${WTSS_CXX_HDR_FILES}
        DESTINATION ${WTSS_CXX_DESTINATION_HEADERS}/wtss-cxx COMPONENT devel)


install(FILES ${WTSS_CXX_UTEST_SRC_FILES}
        DESTINATION ${WTSS_CXX_DESTINATION_UNITTEST} COMPONENT devel)

export(TARGETS wtss_cxx_client FILE ${CMAKE_BINARY_DIR}/wtss_cxx-exports.cmake)
