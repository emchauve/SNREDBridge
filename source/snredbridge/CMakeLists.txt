message(STATUS "[info] Begin of 'source/snredbridge/CMakeLists.txt'")


#-----------------------------------------------------------------------
# Configure SNREDBridge library
#

message(STATUS "[debug] SNREDBRIDGE_LIBDIR_TO_RESOURCEDIR = '${SNREDBRIDGE_LIBDIR_TO_RESOURCEDIR}'")
# set(SNREDBRIDGE_DATAROOTDIR ${SNREDBRIDGE_BUILD_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${SNREDBRIDGE_TAG})
# set(SNREDBRIDGE_DATAROOTDIR ${PROJECT_BUILD_DATAROOTDIR}/${SNREDBRIDGE_TAG})
# message(STATUS "[debug] SNREDBRIDGE_DATAROOTDIR           = '${SNREDBRIDGE_DATAROOTDIR}'")


# -- RPATH settings --
# From: https://cmake.org/Wiki/CMake_RPATH_handling
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_FULL_LIBDIR}" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
endif()

set(SNREDBridgeLibrary_DEFINITIONS
  -std=c++11
  )

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS ${SNREDBridgeLibrary_DEFINITIONS})
endif()

set(SNREDBridgeLibrary_HEADERS
  snredbridge_config.h.in
  resource_files.h
  version.h.in
  snredbridge_library.h
  exception.h
  snredbridge.h
  )

#-----------------------------------------------------------------------
# Declare sources
#
configure_file(snredbridge.cc.in      snredbridge.cc      @ONLY)
configure_file(resource_files.cc.in resource_files.cc @ONLY)

string(RANDOM LENGTH 18 MANGLE_BINRELOC_MAIN)
set(MANGLE_BINRELOC "SNREDBRIDGEMB${MANGLE_BINRELOC_MAIN}")
configure_file(snredbridge_binreloc.h.in snredbridge_binreloc.h @ONLY)

set(SNREDBridgeLibrary_SOURCES
  version.cc
  ${CMAKE_CURRENT_BINARY_DIR}/snredbridge.cc
  ${CMAKE_CURRENT_BINARY_DIR}/resource_files.cc
  ${CMAKE_CURRENT_BINARY_DIR}/snredbridge_binreloc.h
  snredbridge.h
  snredbridge_library.cc
  )

if (SNREDBRIDGE_WITH_SERVICE)

  list(APPEND SNREDBridgeLibrary_HEADERS
    service.h
    )
  list(APPEND SNREDBridgeLibrary_SOURCES
    service.cc
    )

endif()

# - Publish headers
foreach(_hdrin ${SNREDBridgeLibrary_HEADERS})
  string(REGEX REPLACE "\\.in$" "" _hdrout "${_hdrin}")
  configure_file(${_hdrin} ${SNREDBRIDGE_BUILD_PREFIX}/include/snredbridge/${_hdrout} @ONLY)
endforeach()

#-----------------------------------------------------------------------
# Build/Link Library
#
include_directories(${SNREDBRIDGE_BUILD_PREFIX}/include)

#-----------------------------------------------------------------------
# Build/Link the library
#
add_library(SNREDBridge SHARED
  ${SNREDBridgeLibrary_HEADERS}
  ${SNREDBridgeLibrary_PRIVATE_HEADERS}
  ${SNREDBridgeLibrary_SOURCES}
  ${_SNREDBridgeProtobufLibrary_cpp_HEADERS}
  ${_SNREDBridgeProtobufLibrary_cpp_SOURCES}
  )
# Doc: https://cmake.org/Wiki/CMake_FAQ#How_can_I_add_a_dependency_to_a_source_file_which_is_generated_in_a_subdirectory.3F
# Tell CMake the source won't be available until build time.
set_source_files_properties(${_SNREDBridgeProtobufLibrary_cpp_HEADERS} PROPERTIES GENERATED 1)
set_source_files_properties(${_SNREDBridgeProtobufLibrary_cpp_SOURCES} PROPERTIES GENERATED 1)
set_target_properties(SNREDBridge PROPERTIES LINKER_LANGUAGE CXX)
foreach(_pb_target ${_SNREDBridgeProtobufLibrary_cpp_TARGETS})
  message( STATUS "[info] ##### Adding new dependency from target='${_pb_target}'")
  add_dependencies(SNREDBridge ${_pb_target})
endforeach()

target_compile_definitions(SNREDBridge PRIVATE ENABLE_BINRELOC)
message( STATUS "[debug] SNREDBRIDGE_CXX_COMPILE_FEATURES = '${SNREDBRIDGE_CXX_COMPILE_FEATURES}'")
target_compile_features(SNREDBridge PUBLIC ${SNREDBRIDGE_CXX_COMPILE_FEATURES})
# set_property(TARGET SNREDBridge APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include/snredbridge>  # <prefix>/include/snredbridge
#   )
message( STATUS "[debug] SNREDBRIDGE_BUILD_PREFIX   = '${SNREDBRIDGE_BUILD_PREFIX}'")
message( STATUS "[debug] CMAKE_CURRENT_BINARY_DIR = '${CMAKE_CURRENT_BINARY_DIR}'")
message( STATUS "[debug] CMAKE_INSTALL_INCLUDEDIR = '${CMAKE_INSTALL_INCLUDEDIR}'")
# - Use BinReloc
set_target_properties(SNREDBridge PROPERTIES COMPILE_DEFINITIONS ENABLE_BINRELOC)
# target_compile_definitions(SNREDBridge PRIVATE ENABLE_BINRELOC)

target_include_directories(SNREDBridge
  PUBLIC
  $<BUILD_INTERFACE:${SNREDBRIDGE_BUILD_PREFIX}/include>
  $<BUILD_INTERFACE:${SNREDBRIDGE_BUILD_PREFIX}/include/snredbridge>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/snredbridge>
  )

message( STATUS "[debug] SNREDBridge_Boost_LIBRARIES    = '${SNREDBridge_Boost_LIBRARIES}'")

set(_SNREDBridge_SPECIAL_LINKS)
if (SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY)
  set(_SNREDBridge_SPECIAL_LINKS Bayeux::Bayeux)
endif()

target_link_libraries(SNREDBridge PUBLIC
  ${_SNREDBridge_SPECIAL_LINKS}
  ${SNREDBridge_Boost_LIBRARIES}
  )

# - Install it
install(TARGETS SNREDBridge
  EXPORT  SNREDBridgeTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

# - Install public headers (preliminary)
install(DIRECTORY ${SNREDBRIDGE_BUILD_PREFIX}/include/snredbridge
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
  FILES_MATCHING PATTERN "*.h"
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )

add_subdirectory(testing)

message(STATUS "[info] End of 'source/snredbridge/CMakeLists.txt'.")

# - end
