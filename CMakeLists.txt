cmake_minimum_required(VERSION 3.15)

project(
  Frontend.py
  VERSION 0.1
  LANGUAGES CXX)

include(cmake/Doxygen.cmake)
enable_doxygen()

# Set ccache, ipo, etc.
include(cmake/ProjectSettings.cmake)

include(cmake/WarningsAndOptions.cmake)
add_warnings(TARGET frontend.py_warnings)
add_options(TARGET frontend.py_options)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

include(cmake/Sanitizers.cmake)
# Set the sanitizer options on project_options
enable_sanitizers(frontend.py_options)

# include(FetchContent)
# FetchContent_Declare(
#   Parser
#   GIT_REPOSITORY git@github.com:srydell/Parser.git
#   GIT_TAG master)
# FetchContent_MakeAvailable(Parser)

include(cmake/GetPybind11.cmake)
getpybind11(VERSION 2.4.3)

pybind11_add_module(frontend src/Frontend.py.cpp SYSTEM)
target_compile_options(pybind11 INTERFACE -fsized-deallocation)

set(PYBIND11_CPP_STANDARD -std=c++1z)
# target_link_libraries(frontend PRIVATE Parser frontend.py_options
                                          # frontend.py_warnings)

target_include_directories(frontend PRIVATE src)
target_include_directories(frontend PUBLIC include)
# set_target_properties(frontend PROPERTIES CXX_STANDARD_REQUIRED ON
#                                              CXX_EXTENSIONS OFF CXX_STANDARD 17)

# Add library to link against
add_library(Tolc::Frontend.py ALIAS frontend)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()
endif()
