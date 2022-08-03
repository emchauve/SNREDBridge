# - Executable:
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx)
foreach(source ${SOURCES})
  file(RELATIVE_PATH cxx_filename ${CMAKE_CURRENT_SOURCE_DIR} ${source})
  string(REPLACE ".cxx" "" exe_filename ${cxx_filename})
  message(STATUS "adding executable ${exe_filename}")
  add_executable(${exe_filename} ${cxx_filename})
  target_link_libraries(${exe_filename}
    Bayeux::Bayeux
    SNFrontEndElectronics::snfee
    Falaise::Falaise)
  message(STATUS "[debug] CMAKE_INSTALL_PREFIX = '${CMAKE_INSTALL_PREFIX}'")
  message(STATUS "[debug] CMAKE_INSTALL_BINDIR = '${CMAKE_INSTALL_BINDIR}'")
  message(STATUS "[debug] CMAKE_CURRENT_BINARY_DIR = '${CMAKE_CURRENT_BINARY_DIR}'")
  message(STATUS "[debug] CMAKE_CURRENT_SOURCE_DIR = '${CMAKE_CURRENT_SOURCE_DIR}'")
  install(FILES
    ${SNREDBRIDGE_BUILD_PREFIX}/bin/${exe_filename}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )
endforeach(source ${SOURCES})
