# - Top level CMake script for SNREDBridge

#-----------------------------------------------------------------------
# Copyright (C) 2022 Guillaume Oliviero <oliviero@cenbg.in2p3.fr>
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
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(SNREDBridge VERSION 0.1.0)

# - Dependencies
find_package(SNFrontEndElectronics REQUIRED)
find_package(Falaise REQUIRED)
include_directories(${SNFrontEndElectronics_INCLUDE_DIRS})
include_directories(${Falaise_INCLUDE_DIRS})

# Mandatory variable to use and find external libraries such as Bayeux, Falaise, SNFEE...
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#-----------------------------------------------------------------------
# Build the subdirectories as required
#
message(STATUS "[info] Adding subdirectory 'programs'...")
add_subdirectory(programs)
