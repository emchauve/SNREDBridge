# - Executable:
add_executable(SNREDBridge-red-bridge
  red_bridge.cxx
)

target_link_libraries(SNREDBridge-red-bridge PRIVATE
  Bayeux::Bayeux
  SNFrontEndElectronics::snfee
  Falaise::Falaise
)

# - Executable:
add_executable(SNREDBridge-red-bridge-validation
  red_bridge_validation.cxx
)

target_link_libraries(SNREDBridge-red-bridge-validation PRIVATE
  Bayeux::Bayeux
  SNFrontEndElectronics::snfee
  Falaise::Falaise
)

# - Install if required - change install path with option DCMAKE_INSTALL_PREFIX:PATH=""
install(TARGETS SNREDBridge-red-bridge SNREDBridge-red-bridge-validation
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
)
