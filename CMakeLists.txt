# - Top level CMake script for SNREDBridge

#-----------------------------------------------------------------------
# Copyright (C) 2022 Guillaume Oliviero <g.oliviero@ucl.ac.uk>
#
# This file is part of SNREDBridge.
#
# SNREDBridge is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SNREDBridge is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SNREDBridge.  If not, see <http://www.gnu.org/licenses/>.
#-----------------------------------------------------------------------
message (STATUS "[info] Begin of 'CMakeLists.txt'.")

#-----------------------------------------------------------------------
# Enforce an out-of-source build.
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(STATUS "SNREDBridge requires an out-of-source build.")
  message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
  message(STATUS "  CMakeCache.txt")
  message(STATUS "  CMakeFiles")
  message(STATUS "Once these files are removed, create a separate directory")
  message(STATUS "and run CMake from there, pointing it to:")
  message(STATUS "  ${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "in-source build detected")
endif()

#-----------------------------------------------------------------------
# CMake/project requirements and configuration
#
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(SNREDBridge)

# - Load custom modules
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

# - Version number management utilities:
include(BxVersionManager)

# - Versioning only giving major and minor. Patch number is
#   automatically deduced from an external file.
bx_version_set(SNREDBridge 0 1 0)
message(STATUS "[info] Project version = '${PROJECT_VERSION}'")

#-----------------------------------------------------------------------
# Common LPC utilities
#
include(LPCCMakeSettings)

message (STATUS "[info] SNREDBRIDGE_LIBDIR_TO_RESOURCEDIR = '${SNREDBRIDGE_LIBDIR_TO_RESOURCEDIR}'")

#-----------------------------------------------------------------------
# Useful variables
set(SNREDBRIDGE_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(SNREDBRIDGE_BINARY_DIR "${PROJECT_BINARY_DIR}")

#-----------------------------------------------------------------------
# Configure testing if required
#
option(SNREDBRIDGE_ENABLE_TESTING "Build unit testing system for SNREDBridge" ON)
if(SNREDBRIDGE_ENABLE_TESTING)
  enable_testing()
endif()

#-----------------------------------------------------------------------
# Configure/Build utilities as needed
#
option(SNREDBRIDGE_WITH_DEVELOPER_TOOLS "Build/install developer tools" ON)
mark_as_advanced(SNREDBRIDGE_WITH_DEVELOPER_TOOLS)

#-----------------------------------------------------------------------
# Optional build of documentation
#
option(SNREDBRIDGE_WITH_SERVICE "Build service support for SNREDBridge (depends on Bayeux)" OFF)

#-----------------------------------------------------------------------
# Optional build of documentation
#
option(SNREDBRIDGE_WITH_DOCS "Build documentation for SNREDBridge" ON)

#-----------------------------------------------------------------------
# Debug
message( STATUS "[info] SNREDBridge_VERSION              = '${SNREDBridge_VERSION}'")
message( STATUS "[info] SNREDBRIDGE_SOURCE_DIR           = '${SNREDBRIDGE_SOURCE_DIR}'")
message( STATUS "[info] SNREDBRIDGE_BINARY_DIR           = '${SNREDBRIDGE_BINARY_DIR}'")
message( STATUS "[info] SNREDBRIDGE_ENABLE_TESTING       = '${SNREDBRIDGE_ENABLE_TESTING}'")
message( STATUS "[info] SNREDBRIDGE_WITH_DEVELOPER_TOOLS = '${SNREDBRIDGE_WITH_DEVELOPER_TOOLS}'")
message( STATUS "[info] SNREDBRIDGE_WITH_DOCS            = '${SNREDBRIDGE_WITH_DOCS}'")

#-----------------------------------------------------------------------
include(SNREDBridgeDependencies)

#-----------------------------------------------------------------------
# Build the subdirectories as required
#
message( STATUS "[info] Adding subdirectory 'source'...")
add_subdirectory(source)

# message( STATUS "[info] Adding subdirectory 'programs'...")
# add_subdirectory(programs)

if(SNREDBRIDGE_WITH_DOCS)
  add_subdirectory(documentation)
endif()

#-----------------------------------------------------------------------
# - CMake Support files
include(CMakePackageConfigHelpers)

# - Versioning file is the same for the build and install trees
write_basic_package_version_file(
  ${SNREDBRIDGE_BUILD_CMAKEDIR}/${SNREDBRIDGE_TAG}/SNREDBridgeConfigVersion.cmake
  COMPATIBILITY SameMajorVersion
  )

message(STATUS "[debug] CMAKE_INSTALL_CMAKEDIR = '${CMAKE_INSTALL_CMAKEDIR}'")
message(STATUS "[debug] PROJECT_TAG            = '${PROJECT_TAG}'")

# - Config file is also the same in build/install trees as we use same layout
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/SNREDBridgeConfig.cmake.in
  ${SNREDBRIDGE_BUILD_CMAKEDIR}/${SNREDBRIDGE_TAG}/SNREDBridgeConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_TAG}
  PATH_VARS
    CMAKE_INSTALL_BINDIR
    CMAKE_INSTALL_LIBDIR
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_DATAROOTDIR
    )

# - Targets (build tree)
export(EXPORT SNREDBridgeTargets
  NAMESPACE SNREDBridge::
  FILE ${SNREDBRIDGE_BUILD_CMAKEDIR}/${SNREDBRIDGE_TAG}/SNREDBridgeTargets.cmake
  )

# - Targets (install tree)
install(EXPORT SNREDBridgeTargets
  NAMESPACE SNREDBridge::
  DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/${SNREDBRIDGE_TAG}
  )

# - Installation of, well, install tree files
install(
  FILES
    ${PROJECT_BUILD_CMAKEDIR}/${SNREDBRIDGE_TAG}/SNREDBridgeConfigVersion.cmake
    ${PROJECT_BUILD_CMAKEDIR}/${SNREDBRIDGE_TAG}/SNREDBridgeConfig.cmake
  DESTINATION
    ${CMAKE_INSTALL_CMAKEDIR}/${SNREDBRIDGE_TAG}
  )

#-----------------------------------------------------------------------
# - Install published resources and documentation

#-----------------------------------------------------------------------
# Resource installation (uncomment if needed)
# add_subdirectory(resources)

message (STATUS "[info] End of 'CMakeLists.txt'.")

# - end
