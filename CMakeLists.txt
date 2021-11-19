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

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

include(cmake/Sanitizers.cmake)

# Define run_conan
include(cmake/ConanHelpers.cmake)
# Create the profile from variables known during configure
setup_conan_profile(VARIABLE frontend_conan_profile)
# This helps with packaging
set(FRONTEND.PY_CONAN_REQUIRES_PACKAGES spdlog/1.8.0 fmt/7.1.3)
find_conan_packages(
  REQUIRES ${FRONTEND.PY_CONAN_REQUIRES_PACKAGES} PROFILE
  ${frontend_conan_profile})

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY git@github.com:srydell/IntermediateRepresentation.git
  GIT_TAG master)

FetchContent_MakeAvailable(IRSpecification)

add_library(
  Frontend.py
  src/Builders/classBuilder.cpp
  src/Builders/enumBuilder.cpp
  src/Builders/functionBuilder.cpp
  src/Builders/moduleBuilder.cpp
  src/Builders/moduleFileBuilder.cpp
  src/Builders/typeToStringBuilder.cpp
  src/Frontend/Python/frontend.cpp
  src/Helpers/Pybind/extractIncludes.cpp
  src/Helpers/enumsToString.cpp
  src/Helpers/getOverloadedFunctions.cpp
  src/Helpers/string.cpp
  src/Helpers/types.cpp
  src/PybindProxy/class.cpp
  src/PybindProxy/enum.cpp
  src/PybindProxy/function.cpp
  src/PybindProxy/module.cpp
  src/PybindProxy/moduleFile.cpp)

add_warnings(TARGET Frontend.py)
add_options(TARGET Frontend.py)

# Set the sanitizer options on project_options
enable_sanitizers(Frontend.py)

target_link_libraries(
  Frontend.py
  PUBLIC IR::IR
  PRIVATE fmt::fmt)

target_include_directories(Frontend.py PRIVATE src)
target_include_directories(
  Frontend.py PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                     $<INSTALL_INTERFACE:include>)

set_target_properties(Frontend.py PROPERTIES CXX_STANDARD_REQUIRED ON
                                             CXX_EXTENSIONS OFF CXX_STANDARD 17)

# Add library to link against
add_library(Tolc::Frontend.py ALIAS Frontend.py)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    option(ENABLE_COVERAGE "Use code coverage tools on the tests" OFF)
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" OFF)
  if(ENABLE_PACKAGING)
    if(BUILD_DEPENDENCIES_FROM_SOURCE)
      message(
        FATAL_ERROR
          "It is not allowed to build a release package and build dependencies from source. Use a prebuilt library from CI."
      )
    endif()

    add_subdirectory(tools/packaging)
  endif()
endif()
