include_guard()
# Requires other scripts
include(${CMAKE_CURRENT_LIST_DIR}/Catch2Helpers.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CodeCoverage.cmake)

# Helper function to create a test
function(setup_test)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TEST_NAME SOURCE)
  set(multiValues LIBRARIES)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  get_filename_component(project_dir ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
  create_catch2_test(
    TARGET
    ${ARG_TEST_NAME}
    SOURCES
    ${ARG_SOURCE}
    INCLUDE
    ${project_dir}/src
    LINK_LIBRARIES
    Frontend.py
    CONAN_PKG::fmt
    frontend.py_options
    frontend.py_warnings
    ${ARG_LIBRARIES})

  # Assumes CodeCoverage.cmake is included Adds code coverage to the test and
  # adds it to the ccov-all target
  target_code_coverage(${ARG_TEST_NAME} ALL)

  add_test(NAME ${ARG_TEST_NAME} COMMAND ${ARG_TEST_NAME})
endfunction()
