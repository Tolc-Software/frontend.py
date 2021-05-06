cmake_minimum_required(VERSION 3.15)

project(
  TestStage
  VERSION 1.0
  LANGUAGES CXX)

include(cmake/GetPybind11.cmake)
get_pybind11(VERSION 2.6.2)

# Get the variable 'target_name'
# NOTE: This will be generated for each test
include(cmake/TargetName.cmake)

# Get the variable 'sources'
# NOTE: This will be generated for each test
include(cmake/Sources.cmake)

# Create the python module
pybind11_add_module(${target_name} ${sources} SYSTEM)
target_include_directories(${target_name} PRIVATE src)

# Use C++17 since the tests are using std::optional etc.
set_target_properties(${target_name} PROPERTIES CXX_STANDARD_REQUIRED ON
                                                CXX_EXTENSIONS OFF
                                                CXX_STANDARD 17)
