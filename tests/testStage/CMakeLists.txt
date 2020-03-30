cmake_minimum_required(VERSION 3.15)

project(
  TestStage
  VERSION 0.1
  LANGUAGES CXX)

include(cmake/GetPybind11.cmake)
getpybind11(VERSION 2.4.3)
target_compile_options(pybind11 INTERFACE -fsized-deallocation)

# pybind11_add_module(example src/pybind11_examples.cpp SYSTEM)
pybind11_add_module(myModule src/myModule.cpp SYSTEM)
target_include_directories(myModule PRIVATE src)

# # Integration tests are located at the root
# foreach(integrationTest functions)
#   setup_test(TEST_NAME ${integrationTest} SOURCE ${integrationTest}.cpp
#              LIBRARIES TestUtil)
# endforeach()
