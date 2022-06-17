list(APPEND SNREDBridge_TESTS
#  test-snredbridge.cxx
  )

if (SNREDBRIDGE_ENABLE_TESTING)
  foreach(_testsource ${SNREDBridge_TESTS})
    set(_snredbridge_TEST_ENVIRONMENT "SNREDBRIDGE_RESOURCE_FILES_DIR=${PROJECT_BUILD_DATAROOTDIR}/${SNREDBRIDGE_TAG}/resources")
    get_filename_component(_testname "${_testsource}" NAME_WE)
    string(REPLACE "-" ";" _testname_splitted ${_testname})
    list(APPEND _snredbridge_TEST_ENVIRONMENT
      "SNREDBRIDGE_TESTING_DIR=${CMAKE_CURRENT_SOURCE_DIR}/testing")
    set(_testname "snredbridge-${_testname}")
    add_executable(${_testname} ${_testsource})
    target_link_libraries(${_testname} SNREDBridge)
    add_test(NAME ${_testname} COMMAND ${_testname})
    set_target_properties(${_testname}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snredbridge_tests
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snredbridge_tests
      )
  endforeach()
endif()

# - end
