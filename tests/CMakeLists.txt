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
add_library(TestUtil INTERFACE)
target_include_directories(TestUtil INTERFACE TestUtil/include)

# These are needed to use create_catch2_test
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

# Defines create_catch2_test
include(${PROJECT_SOURCE_DIR}/cmake/SetupTest.cmake)

# Use coverage of frontend
target_code_coverage(Frontend.py)

add_code_coverage_all_targets()

# Integration tests are located at the root
foreach(integrationTest functions)
  setup_test(TEST_NAME ${integrationTest} SOURCE ${integrationTest}.cpp
    LIBRARIES TestUtil Tolc::Parser)
endforeach()

# setup_test(TEST_NAME split SOURCE Helpers/Utils/split.cpp)
# setup_test(TEST_NAME includes SOURCE Helpers/includes.cpp)
