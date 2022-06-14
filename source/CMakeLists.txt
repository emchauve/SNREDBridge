# - CMake script for SNREDBridge Library and Apps
#
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
message (STATUS "[info] Begin of 'source/CMakeLists.txt'.")

#-----------------------------------------------------------------------
# Core configuration
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${SNREDBRIDGE_BUILD_PREFIX}/include)
include_directories(${SNREDBRIDGE_BUILD_PREFIX}/include/sncabling)
#
#-----------------------------------------------------------------------
# Libraries and apps
#

#-----------------------------------------------------------------------
# Main SNREDBridge library:
message( STATUS "[info] Adding subdirectory 'snredbridge'...")
add_subdirectory(snredbridge)

#-----------------------------------------------------------------------
# Manual
#
if(Pandoc_FOUND)
  set(SNREDBRIDGE_MANPAGE_IN  ${CMAKE_CURRENT_SOURCE_DIR}/SNREDBridge.1.md)
  set(SNREDBRIDGE_MANPAGE_OUT ${SNREDBRIDGE_BUILD_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${SNREDBRIDGE_TAG}/man/man1/SNREDBridge.1)

  add_custom_command(OUTPUT ${SNREDBRIDGE_MANPAGE_OUT}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SNREDBRIDGE_BUILD_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${SNREDBRIDGE_TAG}/man/man1
    COMMAND ${Pandoc_EXECUTABLE} -s -w man ${SNREDBRIDGE_MANPAGE_IN} -o ${SNREDBRIDGE_MANPAGE_OUT}
    COMMENT "Generating SNREDBridge.1 man page"
    DEPENDS ${SNREDBRIDGE_MANPAGE_IN}
    )
  add_custom_target(vire_man ALL DEPENDS ${SNREDBRIDGE_MANPAGE_OUT})
endif()

message (STATUS "[info] End of 'source/CMakeLists.txt'.")

#- end
