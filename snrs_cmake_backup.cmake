cmake_minimum_required(VERSION 3.16)
project(snred_bridge VERSION 1.1.0)
message("PROJECT_VERSION=${PROJECT_VERSION}")

find_package(Bayeux 3.5 REQUIRED)
message("Bayeux_INCLUDE_DIRS=${Bayeux_INCLUDE_DIRS}")

include(GNUInstallDirs)

###########################################
# - Developer mode:
option(SNRED_BRIDGE_DEVELOPER_BUILD "Developer build mode" OFF)
set(snred_bridge_developer_build 0)
if (SNRED_BRIDGE_DEVELOPER_BUILD)
  set(snred_bridge_developer_build 1)
endif(SNRED_BRIDGE_DEVELOPER_BUILD)

if (snred_bridge_developer_build)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Warning !!! Using developer build mode...       =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snred_bridge_developer_build)

############################################
# - Tests
option(SNRED_BRIDGE_ENABLE_TESTING "Build unit testing system for SNRED_BRIDGE" OFF)
set(snred_bridge_generate_tests 0)
if (SNRED_BRIDGE_ENABLE_TESTING)
  set(snred_bridge_generate_tests 1)
endif(SNRED_BRIDGE_ENABLE_TESTING)

###########################################
# - Datasets:
option(SNRED_BRIDGE_GENERATE_DATA "Generate datasets at build" OFF)
set(snred_bridge_generate_data 0)
set(snred_bridge_generate_data_with_plots 0)
if (SNRED_BRIDGE_GENERATE_DATA)
  set(snred_bridge_generate_data 1)
endif(SNRED_BRIDGE_GENERATE_DATA)

###########################################
# - Docs:
option(SNRED_BRIDGE_WITH_DOC "Build SNRED_BRIDGE documentations" OFF)
set(snred_bridge_generate_doc 0)
if (SNRED_BRIDGE_WITH_DOC)
  set(snred_bridge_generate_doc 1)
endif(SNRED_BRIDGE_WITH_DOC)

if (snred_bridge_developer_build)
  set(snred_bridge_generate_tests 1)
  set(snred_bridge_generate_doc 1)
endif()

if (snred_bridge_generate_tests)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Generating tests...                             =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snred_bridge_generate_tests)

if (snred_bridge_generate_doc)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Generating docs...                              =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snred_bridge_generate_doc)

if (snred_bridge_generate_doc)
  set(snred_bridge_generate_data 1)
  set(snred_bridge_generate_data_with_plots 1)
endif()

if (snred_bridge_generate_data)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Generating data...                              =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snred_bridge_generate_data)

if (snred_bridge_generate_data_with_plots)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Generating data with plots...                   =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snred_bridge_generate_data_with_plots)

set(CONFIG_PATH "${PROJECT_SOURCE_DIR}")
configure_file(
  snred_bridge/config.hpp.in
  ${PROJECT_BINARY_DIR}/snred_bridge/config.hpp
  @ONLY)

configure_file(
  cmake/snred_bridge-config.in
  ${PROJECT_BINARY_DIR}/snred_bridge-config
  @ONLY)

include_directories(${Bayeux_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_BINARY_DIR}/snred_bridge)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/snred_bridge)

set(snred_bridge_HEADERS
  snred_bridge/mesh_pad_vg.hpp
  snred_bridge/mesh_pad_model.hpp
  snred_bridge/pad_tessellation.hpp
  snred_bridge/fsfs.hpp
  snred_bridge/array_smoother.hpp
  snred_bridge/sngeom.hpp
  snred_bridge/ltd.hpp
  snred_bridge/pad.hpp
  snred_bridge/foil_shaping.hpp
  snred_bridge/vertex3d.hpp
  snred_bridge/vertex3d_id.hpp
  snred_bridge/random.hpp
  snred_bridge/snred_bridge.hpp
  )
set(snred_bridge_SOURCES
  snred_bridge/mesh_pad_vg.cpp
  snred_bridge/mesh_pad_model.cpp
  snred_bridge/pad_tessellation.cpp
  snred_bridge/fsfs.cpp
  snred_bridge/array_smoother.cpp
  snred_bridge/sngeom.cpp
  snred_bridge/ltd.cpp
  snred_bridge/pad.cpp
  snred_bridge/foil_shaping.cpp
  snred_bridge/vertex3d.cpp
  snred_bridge/vertex3d_id.cpp
  snred_bridge/initfini.cpp
  snred_bridge/snred_bridge.cpp
  )

add_library(snred_bridge SHARED
  ${snred_bridge_HEADERS}
  ${snred_bridge_SOURCES}
  )
target_link_libraries(snred_bridge Bayeux::Bayeux)

install(TARGETS snred_bridge
  LIBRARY DESTINATION lib
  )
set_target_properties(snred_bridge PROPERTIES
  SKIP_BUILD_RPATH            FALSE
  BUILD_WITH_INSTALL_RPATH    FALSE
  INSTALL_RPATH_USE_LINK_PATH TRUE)
if(UNIX AND NOT APPLE)
  set_target_properties(snred_bridge
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
endif()

install(FILES
  ${PROJECT_BINARY_DIR}/snred_bridge/config.hpp
  ${snred_bridge_HEADERS}
  DESTINATION include/snred_bridge
  )

###########################################
# - Programs:

if (snred_bridge_developer_build)
  add_executable(snred_bridge-build-ltd
    programs/snred_bridge-build-ltd.cxx
    )
  target_link_libraries(snred_bridge-build-ltd snred_bridge)
  set_target_properties(snred_bridge-build-ltd PROPERTIES
    SKIP_BUILD_RPATH            FALSE
    BUILD_WITH_INSTALL_RPATH    FALSE
    INSTALL_RPATH_USE_LINK_PATH TRUE)
  if(UNIX AND NOT APPLE)
    set_target_properties(snred_bridge-build-ltd
      PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
      )
  endif()

  add_executable(snred_bridge-build-fsf
    programs/snred_bridge-build-fsf.cxx
    )
  target_link_libraries(snred_bridge-build-fsf snred_bridge)
  set_target_properties(snred_bridge-build-fsf PROPERTIES
    SKIP_BUILD_RPATH            FALSE
    BUILD_WITH_INSTALL_RPATH    FALSE
    INSTALL_RPATH_USE_LINK_PATH TRUE)
  if(UNIX AND NOT APPLE)
    set_target_properties(snred_bridge-build-fsf
      PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
      )
  endif()

  install(TARGETS snred_bridge-build-ltd
    RUNTIME DESTINATION bin)

  install(TARGETS snred_bridge-build-fsf
    RUNTIME DESTINATION bin)

  install(PROGRAMS scripts/build-ltd.bash
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/SNRED_BRIDGE/scripts)

  install(FILES scripts/plot_fsf_strip.gp
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/SNRED_BRIDGE/scripts)

  install(PROGRAMS scripts/build-fsf.bash
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/SNRED_BRIDGE/scripts)

  install(FILES scripts/plot_ltd_strip.gp
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/SNRED_BRIDGE/scripts)

endif(snred_bridge_developer_build)

install(PROGRAMS ${PROJECT_BINARY_DIR}/snred_bridge-config
  DESTINATION ${CMAKE_INSTALL_BINDIR})


###########################################
# - Checks
if (snred_bridge_generate_data_with_plots)
  message(STATUS "Finding the Gnuplot executable, needed for image generation")
  find_program(GnuplotExecutable gnuplot
    DOC "The Gnuplot executable"
    REQUIRED
    )
endif(snred_bridge_generate_data_with_plots)

# - If testing required, *must* call enable_testing here!!
if(SNRED_BRIDGE_ENABLE_TESTING)
  enable_testing()
endif()

###########################################
# - Datasets generation:
if(snred_bridge_generate_data EQUAL 1)

  set(_plotOption "--no--plot")
  if (snred_bridge_generate_data_with_plots)
    set(_plotOption "--plot")
  endif(snred_bridge_generate_data_with_plots)

  # Generate LTD datasets:
  if ("${RAW_LTD_DATA_DIR}" STREQUAL "")
    message( STATUS "RAW_LTD_DATA_DIR is not set... trying to guess...")
    set(RAW_LTD_DATA_DIR "$ENV{RAW_LTD_DATA_DIR}")
    if ("${RAW_LTD_DATA_DIR}" STREQUAL "")
      message( SEND_ERROR "The 'RAW_LTD_DATA_DIR' environment variable is not set")
      message( SEND_ERROR "Please set 'RAW_LTD_DATA_DIR' CMake variable at configure step")
      message( SEND_ERROR "(ex: 'cmake ... -DRAW_LTD_DATA_DIR:PATH=/path/to/SNLTD_3D_measurements ...')")
      message( SEND_ERROR "or provide the 'RAW_LTD_DATA_DIR' environment variable!")
      message( SEND_ERROR "(ex: 'export RAW_LTD_DATA_DIR=\"/path/to/SNLTD_3D_measurements\")")
      message( FATAL_ERROR "Abort.")
    else()
      message( STATUS "Found the 'RAW_LTD_DATA_DIR' environment variable set at '${RAW_LTD_DATA_DIR}'")
    endif()
  endif()
  message( STATUS "Using RAW_LTD_DATA_DIR='${RAW_LTD_DATA_DIR}'")
  set(SNRED_BRIDGE_DATA_DIR "${CMAKE_BINARY_DIR}/snred_bridge_data" )
  message( STATUS "Using SNRED_BRIDGE_DATA_DIR='${SNRED_BRIDGE_DATA_DIR}'")

  add_custom_command(OUTPUT ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
    COMMAND "${CMAKE_SOURCE_DIR}/scripts/build-ltd.bash"
    ARGS "--source-dir" "${CMAKE_SOURCE_DIR}"
    "--build-dir" "${CMAKE_CURRENT_BINARY_DIR}"
    "--raw-ltd-dir" "${RAW_LTD_DATA_DIR}"
    "--snred_bridge-data-dir" "${SNRED_BRIDGE_DATA_DIR}" ${_plotOption}
    COMMENT "Build the SNRED_BRIDGE Laser Tracker datasets (LTD) associated to individual strips"
    DEPENDS ${CMAKE_SOURCE_DIR}/scripts/build-ltd.bash ${CMAKE_SOURCE_DIR}/scripts/plot_ltd_strip.gp
    VERBATIM)

  # add_custom_command(OUTPUT ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
  #   COMMAND "touch" ARGS ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
  #   VERBATIM)

  add_custom_target(snred_bridgeBuildLtdDataset ALL
    DEPENDS ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
    COMMENT "Build the LTD dataset"
    VERBATIM
    )

  add_dependencies(snred_bridgeBuildLtdDataset snred_bridge-build-ltd)

  # - no need to install intermediate LTD datasets:
  # install(DIRECTORY ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/ltd
  #   DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRED_BRIDGE/resources/data/geometry/source_foils
  #   FILES_MATCHING
  #   PATTERN "processed-strips.lis"
  #   PATTERN "ltd-strip-*.dat"
  #    )
  # install(DIRECTORY ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/ltd/images
  #   DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRED_BRIDGE/resources/data/geometry/source_foils/ltd
  #   FILES_MATCHING PATTERN "ltd-strip-*.png"
  #   )

  # Generate FSF datasets:
  # add_custom_command(OUTPUT ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/processed-strips.lis
  #   COMMAND "${CMAKE_SOURCE_DIR}/scripts/build-fsf.bash"
  #   ARGS "--source-dir" "${CMAKE_SOURCE_DIR}" "--build-dir" "${CMAKE_CURRENT_BINARY_DIR}" "--snred_bridge-data-dir" "${SNRED_BRIDGE_DATA_DIR}" ${_plotOption}
  #   COMMENT "Build the SNRED_BRIDGE Foil Shaping Fit (FSF) datasets associated to individual strips"
  #   DEPENDS ${CMAKE_SOURCE_DIR}/scripts/build-fsf.bash ${CMAKE_SOURCE_DIR}/scripts/plot_fsf_strip.gp
  #   VERBATIM)

  # add_custom_target(snred_bridgeBuildFsfDataset ALL
  #   DEPENDS ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/processed-strips.lis
  #   COMMENT "Build the FSF dataset"
  #   VERBATIM
  #   )

  add_custom_target(snred_bridgeBuildFsfDataset ALL
    "${CMAKE_SOURCE_DIR}/scripts/build-fsf.bash"
    "--source-dir" "${CMAKE_SOURCE_DIR}"
    "--build-dir" "${CMAKE_CURRENT_BINARY_DIR}"
    "--snred_bridge-resource-dir" "${PROJECT_SOURCE_DIR}/resources"
    "--snred_bridge-data-dir" "${SNRED_BRIDGE_DATA_DIR}" "${_plotOption}"
    DEPENDS ${CMAKE_SOURCE_DIR}/scripts/build-fsf.bash ${CMAKE_SOURCE_DIR}/scripts/plot_fsf_strip.gp
    BYPRODUCTS ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/processed-strips.lis
    COMMENT "Build the SNRED_BRIDGE Foil Shaping Fit (FSF) datasets associated to individual strips"
    VERBATIM
    )

  add_dependencies(snred_bridgeBuildFsfDataset snred_bridgeBuildLtdDataset snred_bridge-build-fsf)

  message(STATUS "CMAKE_INSTALL_DATADIR='${CMAKE_INSTALL_DATADIR}'")
  message(STATUS "Resource installation dir='${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRED_BRIDGE/resources'")

  add_custom_target(snred_bridgeSetFsfDataset
    cp
    ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-shaped.dat
    ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tiles.dat
    ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tiles-back-film.dat
    ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tiles-front-film.dat
    ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tessellated.dat
    ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tessellated-back-film.dat
    ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tessellated-front-film.dat
    ${PROJECT_SOURCE_DIR}/resources/data/geometry/source_foils/fsf/
    )

  add_dependencies(snred_bridgeSetFsfDataset snred_bridgeBuildFsfDataset)

  install(DIRECTORY ${SNRED_BRIDGE_DATA_DIR}/geometry/source_foils/fsf
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRED_BRIDGE/resources/data/geometry/source_foils
    FILES_MATCHING
    PATTERN "processed-strips.lis"
    PATTERN "strip-*-pad-?-shaped.dat"
    PATTERN "strip-*-pad-?-shaping.report"
    PATTERN "strip-*-pad-?-tessellated.dat"
    PATTERN "strip-*-pad-?-tiles.dat"
    PATTERN "strip-*-pad-?-tessellated-back-film.dat"
    PATTERN "strip-*-pad-?-tessellated-front-film.dat"
    PATTERN "strip-*-pad-?-tiles-back-film.dat"
    PATTERN "strip-*-pad-?-tiles-front-film.dat"
    )

endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/resources/data/geometry/source_foils/fsf
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRED_BRIDGE/resources/data/geometry/source_foils
  FILES_MATCHING
  # PATTERN "processed-strips.lis"
  PATTERN "strip-*-pad-?-shaped.dat"
  # PATTERN "strip-*-pad-?-shaping.report"
  PATTERN "strip-*-pad-?-tiles.dat"
  PATTERN "strip-*-pad-?-tiles-back-film.dat"
  PATTERN "strip-*-pad-?-tiles-front-film.dat"
  PATTERN "strip-*-pad-?-tessellated.dat"
  PATTERN "strip-*-pad-?-tessellated-back-film.dat"
  PATTERN "strip-*-pad-?-tessellated-front-film.dat"
  )

# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources
#   DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRED_BRIDGE/resources/data/geometry/source_foils/fsf
#   FILES_MATCHING PATTERN "*.png"
#   )

###########################################
# - Documentation generation:
if (snred_bridge_generate_doc)
  add_subdirectory(doc)
endif(snred_bridge_generate_doc)

###########################################
# - Documentation generation:
if (snred_bridge_generate_tests)
  add_subdirectory(snred_bridge/test)
endif(snred_bridge_generate_tests)

# - end
