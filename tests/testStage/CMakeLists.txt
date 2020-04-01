cmake_minimum_required(VERSION 3.15)

project(
  TestStage
  VERSION 1.0
  LANGUAGES CXX)

include(cmake/GetPybind11.cmake)
get_pybind11(VERSION 2.4.3)

# Get the variable 'module_name'
# NOTE: This will be generated for each test
include(cmake/ModuleName.cmake)

# Create the python module
pybind11_add_module(${module_name} src/${module_name}.cpp SYSTEM)
target_include_directories(myModule PRIVATE src)
