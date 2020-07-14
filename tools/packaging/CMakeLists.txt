include_guard()

include(GNUInstallDirs)

# Export the Frontend.py library
install(TARGETS Frontend.py EXPORT Frontend.pyTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(EXPORT Frontend.pyTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Frontend.py NAMESPACE Tolc::)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/Frontend
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Need to be able to download conan packages
install(FILES ${CMAKE_SOURCE_DIR}/cmake/ConanHelpers.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Frontend.py)

configure_file(${CMAKE_SOURCE_DIR}/cmake/packaging/Frontend.pyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/Frontend.pyConfig.cmake @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Frontend.pyConfig.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Frontend.py)

# Export IR
install(TARGETS IR EXPORT IRTargets PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/IR)
install(EXPORT IRTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IR NAMESPACE IR::)
install(FILES ${CMAKE_SOURCE_DIR}/cmake/packaging/IRConfig.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IR)

set(CPACK_VERBATIM_VARIABLES YES)
# Allow packaging
include(CPack)