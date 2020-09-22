include_guard()

# Downloads the Parser project either from source or as a prebuilt asset
# If BUILD_FROM_SOURCE is set to true, then an ssh key is needed
# If it is set to false, the environment variable GITHUB_AUTH_TOKEN will be read and ASSET_NAME will need to be set
# Relies on the GithubHelpers.cmake module
# GITHUB_AUTH_TOKEN is a token generated by github to query things about github
# ASSET_NAME will be the name of a release asset (e.g. Parser-Linux-master.tar.gz)
# Variables exported:
#   PARSER_LLVM_VERSION - version of llvm used
#   parser_SOURCE_DIR - source directory of downloaded parser
function(get_parser)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues BUILD_FROM_SOURCE)
  set(singleValues ASSET_NAME)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix}
                        "${noValues}"
                        "${singleValues}"
                        "${multiValues}"
                        ${ARGN})

  if(ARG_BUILD_FROM_SOURCE)
    # Download source
    FetchContent_Declare(
      Parser
      GIT_REPOSITORY git@github.com:srydell/Parser.git
      GIT_TAG master)
    FetchContent_MakeAvailable(Parser)
  else()
    # Download prebuilt version
    if(NOT DEFINED ENV{GITHUB_AUTH_TOKEN})
      message(FATAL_ERROR "GITHUB_AUTH_TOKEN not defined as a environment variable. Cannot download assets from Github.")
    endif()
    include(${PROJECT_SOURCE_DIR}/cmake/GithubHelpers.cmake)
    fetch_asset_from_github(
      FETCH_VARIABLE
      parser
      GITHUB_TOKEN
      $ENV{GITHUB_AUTH_TOKEN}
      USER
      srydell
      REPOSITORY
      Parser
      TAG
      master-release
      ASSET_NAME
      ${ARG_ASSET_NAME})

    set(Parser_ROOT ${parser_SOURCE_DIR})
    find_package(
      Parser
      REQUIRED
      CONFIG
      PATHS
      ${Parser_ROOT}
      REQUIRED)
  endif()

  # Export the variables
  set(parser_SOURCE_DIR ${parser_SOURCE_DIR} PARENT_SCOPE)
  set(PARSER_LLVM_VERSION ${PARSER_LLVM_VERSION} PARENT_SCOPE)
endfunction()

function(get_parser_system_include)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues LIBCPP_ROOT_DIR PARSER_SOURCE_DIR LLVM_VERSION VARIABLE)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix}
                        "${noValues}"
                        "${singleValues}"
                        "${multiValues}"
                        ${ARGN})

  # Defines get_system_include and format_includes
  include(${ARG_PARSER_SOURCE_DIR}/lib/cmake/Parser/IncludePathHelpers.cmake)

  # Set the include path for the system library in the variable
  # We are using the standard library shipped
  # with the downloaded llvm for include paths in the parsing
  get_system_include(
    VARIABLE
      platform_include
    LLVM_DIRECTORY
      ${ARG_LIBCPP_ROOT_DIR}
    LLVM_VERSION
      ${ARG_LLVM_VERSION})

  format_includes(VARIABLE ${ARG_VARIABLE} INCLUDES
                  ${platform_include} SYSTEM)

  set(${ARG_VARIABLE} ${${ARG_VARIABLE}} PARENT_SCOPE)
endfunction()
