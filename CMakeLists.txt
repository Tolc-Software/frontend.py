cmake_minimum_required(VERSION 3.25)

# Temporary workaround for clang 14
# https://github.com/llvm/llvm-project/issues/53950
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(frontend_languages CXX)
else()
  set(frontend_languages C CXX)
endif()

project(
  Frontend.py
  VERSION 0.7.0
  LANGUAGES ${frontend_languages})

configure_file(docs/ReleaseNotes/version.in
               ${CMAKE_CURRENT_LIST_DIR}/docs/ReleaseNotes/version @ONLY)

# Set ccache, ipo, etc.
include(./cmake/ProjectSettings.cmake)

include(./cmake/WarningsAndOptions.cmake)

include(./cmake/Sanitizers.cmake)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY https://github.com/Tolc-Software/IntermediateRepresentation.git
  GIT_TAG v0.15.1)
FetchContent_MakeAvailable(IRSpecification)

add_library(
  Frontend.py
  src/Frontend/Python/frontend.cpp
  src/Pybind/Builders/attributeBuilder.cpp
  src/Pybind/Builders/classBuilder.cpp
  src/Pybind/Builders/enumBuilder.cpp
  src/Pybind/Builders/functionBuilder.cpp
  src/Pybind/Builders/moduleBuilder.cpp
  src/Pybind/Builders/moduleFileBuilder.cpp
  src/Pybind/Builders/typeToStringBuilder.cpp
  src/Pybind/Helpers/getDocumentationParameter.cpp
  src/Pybind/Helpers/operatorNames.cpp
  src/Pybind/Helpers/split.cpp
  src/Pybind/Helpers/string.cpp
  src/Pybind/Helpers/trampolineClass.cpp
  src/Pybind/Helpers/types.cpp
  src/Pybind/Proxy/attribute.cpp
  src/Pybind/Proxy/class.cpp
  src/Pybind/Proxy/enum.cpp
  src/Pybind/Proxy/function.cpp
  src/Pybind/Proxy/module.cpp
  src/Pybind/Proxy/moduleFile.cpp
  src/Pybind/checkType.cpp
  src/Pybind/getOverloadedFunctions.cpp
  src/Pybind/returnValuePolicy.cpp)

add_warnings(TARGET Frontend.py)
add_options(TARGET Frontend.py)

# Set the sanitizer options on project_options
enable_sanitizers(Frontend.py)

target_link_libraries(
  Frontend.py
  PUBLIC IR::IR
  PRIVATE fmt::fmt spdlog::spdlog)

target_include_directories(Frontend.py PRIVATE src)
target_include_directories(
  Frontend.py PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                     $<INSTALL_INTERFACE:include>)

set_target_properties(Frontend.py PROPERTIES CXX_STANDARD_REQUIRED ON
                                             CXX_EXTENSIONS OFF)

# Add library to link against
add_library(Tolc::Frontend.py ALIAS Frontend.py)

# Only build tests/benchmarks/packaging if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" ON)
  if(ENABLE_TESTING)
    option(ENABLE_COVERAGE "Use code coverage tools on the tests" OFF)
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" ON)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" ON)
  if(ENABLE_PACKAGING)
    add_subdirectory(tools/packaging)
  endif()
endif()
