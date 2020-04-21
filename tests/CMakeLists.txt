include_guard()

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY git@github.com:catchorg/Catch2.git
  GIT_TAG v2.11.0)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
  Parser
  GIT_REPOSITORY git@github.com:srydell/Parser.git
  GIT_TAG master)
FetchContent_MakeAvailable(Parser)

# Everything put in TestUtil/include is available
add_library(TestUtil TestUtil/include/TestUtil/pybindStage.cpp)
target_include_directories(TestUtil PUBLIC TestUtil/include)
target_link_libraries(TestUtil PRIVATE Tolc::Parser frontend.py_options
                                       frontend.py_warnings)
target_link_libraries(TestUtil PUBLIC IR::IR)

# These are needed to use create_catch2_test
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

# Defines create_catch2_test
include(${PROJECT_SOURCE_DIR}/cmake/SetupTest.cmake)

add_library(TestStage TestUtil/include/TestStage/stage.cpp)
target_include_directories(TestStage PUBLIC TestUtil/include)
target_link_libraries(TestStage PRIVATE CONAN_PKG::fmt frontend.py_options
                                        frontend.py_warnings)

# Use coverage of frontend
target_code_coverage(Frontend.py)

add_code_coverage_all_targets()

# Create a platform independent way of getting the path to the test stage
set(test_stage ${CMAKE_CURRENT_LIST_DIR}/testStage)
configure_file(
  TestUtil/include/TestStage/paths.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestStage/paths.hpp @ONLY)

# Create a test fixture for downloading dependencies in the test stage In this
# case pybind11
add_test(
  NAME ConfigureTestStage
  WORKING_DIRECTORY ${test_stage}
  COMMAND ${CMAKE_COMMAND} -S. -Bbuild -GNinja)
set_tests_properties(ConfigureTestStage PROPERTIES FIXTURES_SETUP TestStage)

# Integration tests are located at the root and executed in a temporary test
# stage
foreach(integrationTest functions)
  setup_test(
    TEST_NAME
    ${integrationTest}
    SOURCE
    ${integrationTest}.cpp
    LIBRARIES
    TestUtil
    TestStage)

  # Require that the dependencies are downloaded so they can be copied to a
  # temporary test stage
  set_tests_properties(${integrationTest} PROPERTIES FIXTURES_REQUIRED
                                                     TestStage)
endforeach()

foreach(unittest PybindProxy/moduleFile.cpp PybindProxy/function.cpp)
  # Ex: PybindProxy/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${unittest} NAME_WLE)

  setup_test(
    TEST_NAME
    ${testName}
    SOURCE
    ${unittest}
    LIBRARIES
    TestUtil
    TestStage)
endforeach()
