cmake_minimum_required(VERSION 3.22)

project(
  perfcounters
  VERSION 0.1.1.1
  DESCRIPTION "description"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckCXXCompilerFlag)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/perfcounters)
set(LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)

enable_testing()

add_subdirectory(src)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)

write_basic_package_version_file(
  perfcountersConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion)

install(
  TARGETS perfcounters
  EXPORT perfcountersTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/perfcounters)

install(
  EXPORT perfcountersTargets
  FILE perfcountersTargets.cmake
  NAMESPACE PC::
  DESTINATION lib/cmake/perfcounters)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/perfcountersConfigVersion.cmake
              ${CMAKE_CURRENT_SOURCE_DIR}/cmake/perfcountersConfig.cmake
        DESTINATION lib/cmake/perfcounters)

add_executable(main main.cpp)
target_link_libraries(main perfcounters)
