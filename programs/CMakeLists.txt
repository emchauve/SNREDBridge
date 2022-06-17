add_executable(snredbridge-red_bridge red_bridge.cxx)

target_link_libraries(snredbridge-red_bridge
  snredbridge
  Bayeux::Bayeux
  SNCabling::SNCabling
  SNFrontEndElectronics::snfee
  Falaise::Falaise
  )
set_target_properties(snredbridge-red_bridge
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  )
install(FILES
    ${CMAKE_BINARY_DIR}/snredbridge-red_bridge
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
    )
