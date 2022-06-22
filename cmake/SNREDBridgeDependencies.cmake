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
    set(SNREDBridge_BAYEUX_VERSION ${Bayeux_VERSION})
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
  # SNFrontEndElectronics
  set(SNFEE_MIN_VERSION "0.2.0")
  message( STATUS "[info] Searching SNFrontEndElectronics ${SNFEE_MIN_VERSION}...")
  find_package(SNFrontEndElectronics ${SNFEE_MIN_VERSION} REQUIRED NO_MODULE)
  if (SNFrontEndElectronics_FOUND)
    message( STATUS "[info] SNFrontEndElectronics ${SNFrontEndElectronics_VERSION} found.")
    set(SNREDBridge_SNFEE_VERSION ${SNFrontEndElectronics_VERSION})
    message( STATUS "[info] SNFrontEndElectronics_DIR = '${SNFrontEndElectronics_DIR}'")
    message( STATUS "[info] SNFrontEndElectronics_INCLUDE_DIRS = '${SNFrontEndElectronics_INCLUDE_DIRS}'")
    find_program(SNFrontEndElectronicsQueryExe snfee-query PATHS "${SNFrontEndElectronics_DIR}/../../.." PATH_SUFFIXES "bin" REQUIRED)
    message( STATUS "[info] SNFrontEndElectronicsQueryExe = '${SNFrontEndElectronicsQueryExe}'")
    if ("${SNFrontEndElectronicsQueryExe}" STREQUAL "SNFrontEndElectronicsQueryExe-NOTFOUND")
      message( FATAL_ERROR "[info] Could not find SNFrontEndElectronics 'sncablingquery' script!")
    endif()
    exec_program(${SNFrontEndElectronicsQueryExe} ARGS "--with-snfee" OUTPUT_VARIABLE _snfrontendelectronics_with_snfee)
    message( STATUS "[info] _snfrontendelectronics_with_snfee = '${_snfrontendelectronics_with_snfee}'")
    if (NOT "${_snfrontendelectronics_with_snfee}" STREQUAL "1")
      message( FATAL_ERROR "[info] Found SNFrontEndElectronics without 'snfee' support!")
    endif()
  endif()

  #-------------------------------------------------------
  # Falaise
  set(FALAISE_MIN_VERSION "4.0.0")
  message( STATUS "[info] Searching Falaise ${FALAISE_MIN_VERSION}...")
  find_package(Falaise ${FALAISE_MIN_VERSION} REQUIRED NO_MODULE)
  if (Falaise_FOUND)
    message( STATUS "[info] Falaise ${Falaise_VERSION} found.")
    set(SNREDBridge_FALAISE_VERSION ${Falaise_VERSION})
    message( STATUS "[info] Falaise_DIR = '${Falaise_DIR}'")
    message( STATUS "[info] Falaise_INCLUDE_DIRS = '${Falaise_INCLUDE_DIRS}'")
  endif()

endif()


# -----------------------------------------------------
message( STATUS "[info] Define SNREDBridge dependencies")

# - end
