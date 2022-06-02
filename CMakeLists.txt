cmake_minimum_required(VERSION 3.16)
project(snrs VERSION 1.1.0)
message("PROJECT_VERSION=${PROJECT_VERSION}")

find_package(Bayeux 3.5 REQUIRED)
message("Bayeux_INCLUDE_DIRS=${Bayeux_INCLUDE_DIRS}")

include(GNUInstallDirs)

###########################################
# - Developer mode:
option(SNRS_DEVELOPER_BUILD "Developer build mode" OFF)
set(snrs_developer_build 0)
if (SNRS_DEVELOPER_BUILD)
  set(snrs_developer_build 1)
endif(SNRS_DEVELOPER_BUILD)

if (snrs_developer_build)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Warning !!! Using developer build mode...       =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snrs_developer_build)

############################################
# - Tests
option(SNRS_ENABLE_TESTING "Build unit testing system for SNRS" OFF)
set(snrs_generate_tests 0)
if (SNRS_ENABLE_TESTING)
  set(snrs_generate_tests 1)
endif(SNRS_ENABLE_TESTING)

###########################################
# - Datasets:
option(SNRS_GENERATE_DATA "Generate datasets at build" OFF)
set(snrs_generate_data 0)
set(snrs_generate_data_with_plots 0)
if (SNRS_GENERATE_DATA)
  set(snrs_generate_data 1)
endif(SNRS_GENERATE_DATA)

###########################################
# - Docs:
option(SNRS_WITH_DOC "Build SNRS documentations" OFF)
set(snrs_generate_doc 0)
if (SNRS_WITH_DOC)
  set(snrs_generate_doc 1)
endif(SNRS_WITH_DOC)

if (snrs_developer_build)
  set(snrs_generate_tests 1)
  set(snrs_generate_doc 1)
endif()

if (snrs_generate_tests)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Generating tests...                             =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snrs_generate_tests)

if (snrs_generate_doc)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Generating docs...                              =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snrs_generate_doc)

if (snrs_generate_doc)
  set(snrs_generate_data 1)
  set(snrs_generate_data_with_plots 1)
endif()

if (snrs_generate_data)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Generating data...                              =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snrs_generate_data)

if (snrs_generate_data_with_plots)
  message(STATUS "====================================================")
  message(STATUS "=                                                  =")
  message(STATUS "=  Generating data with plots...                   =")
  message(STATUS "=                                                  =")
  message(STATUS "====================================================")
endif(snrs_generate_data_with_plots)

set(CONFIG_PATH "${PROJECT_SOURCE_DIR}")
configure_file(
  snrs/config.hpp.in
  ${PROJECT_BINARY_DIR}/snrs/config.hpp
  @ONLY)

configure_file(
  cmake/snrs-config.in
  ${PROJECT_BINARY_DIR}/snrs-config
  @ONLY)

include_directories(${Bayeux_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_BINARY_DIR}/snrs)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/snrs)

set(snrs_HEADERS
  snrs/mesh_pad_vg.hpp
  snrs/mesh_pad_model.hpp
  snrs/pad_tessellation.hpp
  snrs/fsfs.hpp
  snrs/array_smoother.hpp
  snrs/sngeom.hpp
  snrs/ltd.hpp
  snrs/pad.hpp
  snrs/foil_shaping.hpp
  snrs/vertex3d.hpp
  snrs/vertex3d_id.hpp
  snrs/random.hpp
  snrs/snrs.hpp
  )
set(snrs_SOURCES
  snrs/mesh_pad_vg.cpp
  snrs/mesh_pad_model.cpp
  snrs/pad_tessellation.cpp
  snrs/fsfs.cpp
  snrs/array_smoother.cpp
  snrs/sngeom.cpp
  snrs/ltd.cpp
  snrs/pad.cpp
  snrs/foil_shaping.cpp
  snrs/vertex3d.cpp
  snrs/vertex3d_id.cpp
  snrs/initfini.cpp
  snrs/snrs.cpp
  )

add_library(snrs SHARED 
  ${snrs_HEADERS}
  ${snrs_SOURCES}
  )
target_link_libraries(snrs Bayeux::Bayeux)

install(TARGETS snrs
  LIBRARY DESTINATION lib
  )
set_target_properties(snrs PROPERTIES
  SKIP_BUILD_RPATH            FALSE
  BUILD_WITH_INSTALL_RPATH    FALSE
  INSTALL_RPATH_USE_LINK_PATH TRUE)
if(UNIX AND NOT APPLE)
  set_target_properties(snrs
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
endif() 

install(FILES
  ${PROJECT_BINARY_DIR}/snrs/config.hpp
  ${snrs_HEADERS}
  DESTINATION include/snrs
  )

###########################################
# - Programs:

if (snrs_developer_build)
  add_executable(snrs-build-ltd
    programs/snrs-build-ltd.cxx
    )
  target_link_libraries(snrs-build-ltd snrs)
  set_target_properties(snrs-build-ltd PROPERTIES
    SKIP_BUILD_RPATH            FALSE
    BUILD_WITH_INSTALL_RPATH    FALSE
    INSTALL_RPATH_USE_LINK_PATH TRUE)
  if(UNIX AND NOT APPLE)
    set_target_properties(snrs-build-ltd
      PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
      )
  endif()

  add_executable(snrs-build-fsf
    programs/snrs-build-fsf.cxx
    )
  target_link_libraries(snrs-build-fsf snrs)
  set_target_properties(snrs-build-fsf PROPERTIES
    SKIP_BUILD_RPATH            FALSE
    BUILD_WITH_INSTALL_RPATH    FALSE
    INSTALL_RPATH_USE_LINK_PATH TRUE)
  if(UNIX AND NOT APPLE)
    set_target_properties(snrs-build-fsf
      PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
      )
  endif()

  install(TARGETS snrs-build-ltd
    RUNTIME DESTINATION bin)

  install(TARGETS snrs-build-fsf
    RUNTIME DESTINATION bin)

  install(PROGRAMS scripts/build-ltd.bash
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/SNRS/scripts)

  install(FILES scripts/plot_fsf_strip.gp
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/SNRS/scripts)

  install(PROGRAMS scripts/build-fsf.bash
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/SNRS/scripts)

  install(FILES scripts/plot_ltd_strip.gp
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/SNRS/scripts)
  
endif(snrs_developer_build)

install(PROGRAMS ${PROJECT_BINARY_DIR}/snrs-config
  DESTINATION ${CMAKE_INSTALL_BINDIR})


###########################################
# - Checks
if (snrs_generate_data_with_plots)
  message(STATUS "Finding the Gnuplot executable, needed for image generation")
  find_program(GnuplotExecutable gnuplot
    DOC "The Gnuplot executable"
    REQUIRED
    )
endif(snrs_generate_data_with_plots)

# - If testing required, *must* call enable_testing here!!
if(SNRS_ENABLE_TESTING)
  enable_testing()
endif()

###########################################
# - Datasets generation:
if(snrs_generate_data EQUAL 1)
  
  set(_plotOption "--no--plot")
  if (snrs_generate_data_with_plots)
    set(_plotOption "--plot")
  endif(snrs_generate_data_with_plots)

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
  set(SNRS_DATA_DIR "${CMAKE_BINARY_DIR}/snrs_data" )
  message( STATUS "Using SNRS_DATA_DIR='${SNRS_DATA_DIR}'")
  
  add_custom_command(OUTPUT ${SNRS_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
    COMMAND "${CMAKE_SOURCE_DIR}/scripts/build-ltd.bash"
    ARGS "--source-dir" "${CMAKE_SOURCE_DIR}"
    "--build-dir" "${CMAKE_CURRENT_BINARY_DIR}"
    "--raw-ltd-dir" "${RAW_LTD_DATA_DIR}"
    "--snrs-data-dir" "${SNRS_DATA_DIR}" ${_plotOption}
    COMMENT "Build the SNRS Laser Tracker datasets (LTD) associated to individual strips"
    DEPENDS ${CMAKE_SOURCE_DIR}/scripts/build-ltd.bash ${CMAKE_SOURCE_DIR}/scripts/plot_ltd_strip.gp 
    VERBATIM)

  # add_custom_command(OUTPUT ${SNRS_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
  #   COMMAND "touch" ARGS ${SNRS_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
  #   VERBATIM)
    
  add_custom_target(snrsBuildLtdDataset ALL
    DEPENDS ${SNRS_DATA_DIR}/geometry/source_foils/ltd/processed-strips.lis
    COMMENT "Build the LTD dataset"
    VERBATIM
    )

  add_dependencies(snrsBuildLtdDataset snrs-build-ltd)

  # - no need to install intermediate LTD datasets:
  # install(DIRECTORY ${SNRS_DATA_DIR}/geometry/source_foils/ltd
  #   DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRS/resources/data/geometry/source_foils
  #   FILES_MATCHING
  #   PATTERN "processed-strips.lis"
  #   PATTERN "ltd-strip-*.dat"
  #    )
  # install(DIRECTORY ${SNRS_DATA_DIR}/geometry/source_foils/ltd/images
  #   DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRS/resources/data/geometry/source_foils/ltd
  #   FILES_MATCHING PATTERN "ltd-strip-*.png"
  #   )

  # Generate FSF datasets:
  # add_custom_command(OUTPUT ${SNRS_DATA_DIR}/geometry/source_foils/fsf/processed-strips.lis
  #   COMMAND "${CMAKE_SOURCE_DIR}/scripts/build-fsf.bash"
  #   ARGS "--source-dir" "${CMAKE_SOURCE_DIR}" "--build-dir" "${CMAKE_CURRENT_BINARY_DIR}" "--snrs-data-dir" "${SNRS_DATA_DIR}" ${_plotOption}
  #   COMMENT "Build the SNRS Foil Shaping Fit (FSF) datasets associated to individual strips"
  #   DEPENDS ${CMAKE_SOURCE_DIR}/scripts/build-fsf.bash ${CMAKE_SOURCE_DIR}/scripts/plot_fsf_strip.gp
  #   VERBATIM)

  # add_custom_target(snrsBuildFsfDataset ALL
  #   DEPENDS ${SNRS_DATA_DIR}/geometry/source_foils/fsf/processed-strips.lis
  #   COMMENT "Build the FSF dataset"
  #   VERBATIM
  #   )

  add_custom_target(snrsBuildFsfDataset ALL
    "${CMAKE_SOURCE_DIR}/scripts/build-fsf.bash"
    "--source-dir" "${CMAKE_SOURCE_DIR}"
    "--build-dir" "${CMAKE_CURRENT_BINARY_DIR}"
    "--snrs-resource-dir" "${PROJECT_SOURCE_DIR}/resources"
    "--snrs-data-dir" "${SNRS_DATA_DIR}" "${_plotOption}"
    DEPENDS ${CMAKE_SOURCE_DIR}/scripts/build-fsf.bash ${CMAKE_SOURCE_DIR}/scripts/plot_fsf_strip.gp
    BYPRODUCTS ${SNRS_DATA_DIR}/geometry/source_foils/fsf/processed-strips.lis
    COMMENT "Build the SNRS Foil Shaping Fit (FSF) datasets associated to individual strips"
    VERBATIM
    )

  add_dependencies(snrsBuildFsfDataset snrsBuildLtdDataset snrs-build-fsf) 

  message(STATUS "CMAKE_INSTALL_DATADIR='${CMAKE_INSTALL_DATADIR}'")
  message(STATUS "Resource installation dir='${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRS/resources'")

  add_custom_target(snrsSetFsfDataset 
    cp
    ${SNRS_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-shaped.dat
    ${SNRS_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tiles.dat
    ${SNRS_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tiles-back-film.dat
    ${SNRS_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tiles-front-film.dat
    ${SNRS_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tessellated.dat
    ${SNRS_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tessellated-back-film.dat
    ${SNRS_DATA_DIR}/geometry/source_foils/fsf/strip-*-pad-0-tessellated-front-film.dat
    ${PROJECT_SOURCE_DIR}/resources/data/geometry/source_foils/fsf/
    )

  add_dependencies(snrsSetFsfDataset snrsBuildFsfDataset) 
  
  install(DIRECTORY ${SNRS_DATA_DIR}/geometry/source_foils/fsf
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRS/resources/data/geometry/source_foils
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
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRS/resources/data/geometry/source_foils
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
#   DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/SNRS/resources/data/geometry/source_foils/fsf
#   FILES_MATCHING PATTERN "*.png"
#   )
 
###########################################
# - Documentation generation:
if (snrs_generate_doc)
  add_subdirectory(doc)
endif(snrs_generate_doc)

###########################################
# - Documentation generation:
if (snrs_generate_tests)
  add_subdirectory(snrs/test)
endif(snrs_generate_tests)

# - end
