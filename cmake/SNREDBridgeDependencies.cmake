message( STATUS "[info] Define SNREDBridge dependencies")

set(needBayeux 1)
if (SNREDBRIDGE_WITH_SERVICE)
  set(SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY 1)
endif()

if(needBayeux)
  #-------------------------------------------------------
  # Bayeux
  set(SNREDBridge_BAYEUX_MIN_VERSION "3.5.2")
  message( STATUS "[info] Searching Bayeux ${SNREDBridge_BAYEUX_MIN_VERSION}...")
  find_package(Bayeux ${SNREDBridge_BAYEUX_MIN_VERSION} REQUIRED NO_MODULE)
  if (Bayeux_FOUND)
    message( STATUS "[info] Bayeux ${Bayeux_VERSION} found.")
    message( STATUS "[info] BAYEUX_BOOST_VERSION    = '${BAYEUX_BOOST_VERSION}'")
    # Trick because BayeuxConfig 3.2 does not provide the BAYEUX_BOOST_COMPONENTS variable:
    set(SNREDBridge_BAYEUX_VERSION ${Bayeux_VERSION})
    # # Trick because BayeuxConfig 3.2 does not provide the BAYEUX_BOOST_COMPONENTS variable:
    # set(BAYEUX_BOOST_COMPONENTS
    #   "filesystem;system;serialization;iostreams;program_options;regex;thread"
    #   )
    message( STATUS "[info] BAYEUX_BOOST_COMPONENTS = '${BAYEUX_BOOST_COMPONENTS}'")
    message( STATUS "[info] Bayeux_CMAKE_CONFIG_DIR = '${Bayeux_CMAKE_CONFIG_DIR}'")
  endif()

  #-------------------------------------------------------
  # SNCabling
  set(SNCABLING_MIN_VERSION "1.1.1")
  message( STATUS "[info] Searching SNCabling ${SNCABLING_MIN_VERSION}...")
  find_package(SNCabling ${SNCABLING_MIN_VERSION} REQUIRED NO_MODULE)
  if (SNCabling_FOUND)
    message( STATUS "[info] SNCabling ${SNCabling_VERSION} found.")
    set(SNREDBridge_SNCABLING_VERSION ${SNCabling_VERSION})
    message( STATUS "[info] SNCabling_DIR = '${SNCabling_DIR}'")
    message( STATUS "[info] SNCabling_INCLUDE_DIRS = '${SNCabling_INCLUDE_DIRS}'")
    find_program(SNCablingQueryExe sncablingquery PATHS "${SNCabling_DIR}/../../.." PATH_SUFFIXES "bin" REQUIRED)
    message( STATUS "[info] SNCablingQueryExe = '${SNCablingQueryExe}'")
    if ("${SNCablingQueryExe}" STREQUAL "SNCablingQueryExe-NOTFOUND")
      message( FATAL_ERROR "[info] Could not find SNCabling 'sncablingquery' script!")
    endif()
    exec_program(${SNCablingQueryExe} ARGS "--with-service" OUTPUT_VARIABLE _sncabling_with_service)
    message( STATUS "[info] _sncabling_with_service = '${_sncabling_with_service}'")
    if (NOT "${_sncabling_with_service}" STREQUAL "1")
      message( FATAL_ERROR "[info] Found SNCabling without 'service' support!")
    endif()
  endif()

  #-------------------------------------------------------
  # SNFEE



  #-------------------------------------------------------
  # Falaise

endif()


# -----------------------------------------------------
message( STATUS "[info] Define SNREDBridge dependencies")

# - end
