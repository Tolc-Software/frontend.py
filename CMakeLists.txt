cmake_minimum_required(VERSION 3.15)

project(
  Frontend.py
  VERSION 0.2.0
  LANGUAGES CXX)

configure_file(docs/ReleaseNotes/version.in
               ${CMAKE_CURRENT_LIST_DIR}/docs/ReleaseNotes/version @ONLY)

include(FetchContent)
FetchContent_Declare(
  cmake_modules
  GIT_REPOSITORY https://github.com/Tolc-Software/cmake-modules.git
  GIT_TAG main)
FetchContent_MakeAvailable(cmake_modules)
# Basis of all shared CMake modules
set(modules ${cmake_modules_SOURCE_DIR}/cmake)

# Set ccache, ipo, etc.
include(${modules}/ProjectSettings.cmake)

include(${modules}/WarningsAndOptions.cmake)

include(${modules}/Sanitizers.cmake)

# Define run_conan
include(${modules}/ConanHelpers.cmake)
# Create the profile from variables known during configure
setup_conan_profile(VARIABLE frontend_conan_profile)
# This helps with packaging
set(FRONTEND.PY_CONAN_REQUIRES_PACKAGES spdlog/1.9.2 fmt/8.1.1)
find_conan_packages(REQUIRES ${FRONTEND.PY_CONAN_REQUIRES_PACKAGES} PROFILE
                    ${frontend_conan_profile})

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY https://github.com/Tolc-Software/IntermediateRepresentation.git
  GIT_TAG main)

FetchContent_MakeAvailable(IRSpecification)

add_library(
  Frontend.py
  src/Pybind/Builders/attributeBuilder.cpp
  src/Pybind/Builders/classBuilder.cpp
  src/Pybind/Builders/enumBuilder.cpp
  src/Pybind/Builders/functionBuilder.cpp
  src/Pybind/Builders/moduleBuilder.cpp
  src/Pybind/Builders/moduleFileBuilder.cpp
  src/Pybind/Builders/typeToStringBuilder.cpp
  src/Frontend/Python/frontend.cpp
  src/Pybind/checkType.cpp
  src/Pybind/returnValuePolicy.cpp
  src/Pybind/getOverloadedFunctions.cpp
  src/Pybind/Helpers/split.cpp
  src/Pybind/Helpers/string.cpp
  src/Pybind/Helpers/types.cpp
  src/Pybind/Proxy/attribute.cpp
  src/Pybind/Proxy/class.cpp
  src/Pybind/Proxy/enum.cpp
  src/Pybind/Proxy/function.cpp
  src/Pybind/Proxy/module.cpp
  src/Pybind/Proxy/moduleFile.cpp)

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

set_target_properties(
  Frontend.py
  PROPERTIES CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF)

# Add library to link against
add_library(Tolc::Frontend.py ALIAS Frontend.py)

# Only build tests/benchmarks/packaging if building this project directly
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
    add_subdirectory(tools/packaging)
  endif()
endif()
