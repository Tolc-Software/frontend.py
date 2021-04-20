include_guard()

include(${PROJECT_SOURCE_DIR}/cmake/GetParser.cmake)

option(BUILD_DEPENDENCIES_FROM_SOURCE "Building dependencies from source instead of downloading an asset" FALSE)
set(parser_source_switch "")
if(BUILD_DEPENDENCIES_FROM_SOURCE)
  set(parser_source_switch "BUILD_FROM_SOURCE")
endif()
set(parser_asset_name "Parser-${CMAKE_SYSTEM_NAME}-master")
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(parser_asset_name "${parser_asset_name}-${CMAKE_BUILD_TYPE}")
endif()
get_parser(ASSET_NAME ${parser_asset_name}.tar.xz ${parser_source_switch})

# If we are not building from source we need to create some variables
# containing the paths to the included system library
if(NOT parser_source_switch)
  # Set the include path for the system library in the variable
  # We are using the standard library shipped
  # with the downloaded llvm for include paths in the parsing
  get_parser_system_include(
    VARIABLE
      SYSTEM_INCLUDE_PATHS
    PARSER_SOURCE_DIR
      ${parser_SOURCE_DIR}
    LIBCPP_ROOT_DIR
      ${parser_SOURCE_DIR}/lib/Parser
    LLVM_VERSION
      ${PARSER_LLVM_VERSION})
endif()

# If Parser was built from source, the file should only be expanded with empty strings
# and therefore use the default config from Parser.
configure_file(
  TestUtil/include/TestUtil/parserConfig.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestUtil/parserConfig.hpp
    @ONLY)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY git@github.com:catchorg/Catch2.git
  GIT_TAG v2.12.1)
FetchContent_MakeAvailable(Catch2)

# These are needed to use create_catch2_test
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

# Everything put in TestUtil/include is available
add_library(TestUtil TestUtil/include/TestUtil/pybindStage.cpp)
target_include_directories(TestUtil PUBLIC TestUtil/include)
target_link_libraries(TestUtil PRIVATE Tolc::Parser Catch2::Catch2)
target_link_libraries(TestUtil PUBLIC IR::IR)
find_package(fmt REQUIRED)

add_warnings(TARGET TestUtil)
add_options(TARGET TestUtil)

# Defines create_catch2_test
include(${PROJECT_SOURCE_DIR}/cmake/SetupTest.cmake)

add_library(TestStage TestUtil/include/TestStage/stage.cpp)
target_include_directories(TestStage PUBLIC TestUtil/include)
# find_package(fmt)
# target_link_libraries(TestStage PRIVATE fmt::fmt)

add_warnings(TARGET TestStage)
add_options(TARGET TestStage)

if(ENABLE_COVERAGE)
  include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

  # Use coverage of frontend
  target_code_coverage(Frontend.py)

  add_code_coverage_all_targets()
endif()

# Create a platform independent way of getting the path to the test stage
set(test_stage ${CMAKE_CURRENT_LIST_DIR}/testStage)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  # This is to normalize strings relative to how they are used in std::filesystem
  string(REPLACE "/" "\\\\" test_stage ${test_stage})
endif()
configure_file(
  TestUtil/include/TestStage/paths.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestStage/paths.hpp @ONLY)

# Create a test fixture for downloading dependencies in the test stage
# In this case pybind11
add_test(
  NAME ConfigureTestStage
  WORKING_DIRECTORY ${test_stage}
  COMMAND ${CMAKE_COMMAND} -S. -Bbuild -GNinja)
set_tests_properties(ConfigureTestStage PROPERTIES FIXTURES_SETUP TestStage)

foreach(
  test
  Builders/classBuilder.cpp
  Builders/functionBuilder.cpp
  Builders/enumBuilder.cpp
  Builders/moduleFileBuilder.cpp
  Helpers/Pybind/extractIncludes.cpp
  Helpers/combine.cpp
  PybindProxy/class.cpp
  PybindProxy/enum.cpp
  PybindProxy/function.cpp
  PybindProxy/module.cpp
  PybindProxy/moduleFile.cpp
  arrays.cpp
  classes.cpp
  enums.cpp
  functions.cpp
  maps.cpp
  memberVariables.cpp
  namespaces.cpp
  namespaceSameAsModule.cpp
  pairs.cpp
  tuples.cpp
  sets.cpp
  unordered_maps.cpp
  unordered_sets.cpp
  userDefined.cpp
  vectors.cpp)
  # Ex: PybindProxy/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${test} NAME_WLE)

  setup_test(
    TEST_NAME
    ${testName}
    SOURCE
    ${test}
    LIBRARIES
    TestUtil
    TestStage
    fmt::fmt)

  string(FIND "${test}" "/" subdirectory)
  if(subdirectory EQUAL -1)
    # The test is in the root => should be an integration test

    # Require that the dependencies are downloaded so they can be copied to a
    # temporary test stage
    set_tests_properties(${testName} PROPERTIES FIXTURES_REQUIRED TestStage)
  endif()
endforeach()
