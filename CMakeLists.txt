cmake_minimum_required(VERSION 3.27)

project(perfcounters VERSION 0.1.1.1
        DESCRIPTION "description"
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckCXXCompilerFlag)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/perfcounters)
set(LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

enable_testing()

add_subdirectory(src)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)

write_basic_package_version_file(
        perfcountersConfigVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS perfcounters
        EXPORT perfcountersTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/perfcounters
        )

install(EXPORT perfcountersTargets 
        FILE perfcountersTargets.cmake 
        NAMESPACE PC:: 
        DESTINATION lib/cmake/perfcounters
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/perfcountersConfigVersion.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/perfcountersConfig.cmake
        DESTINATION lib/cmake/perfcounters
)






# cmake_minimum_required(VERSION 3.27)
# project(PerfLib)
#
# include(GNUInstallDirs)
# include(CMakePackageConfigHelpers)
#
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#
# add_library(foo STATIC
#             perfCounters.cpp
#             perfCounter.cpp
# )
# target_include_directories(foo PUBLIC
#                            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
#                            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
# )
#
# install(TARGETS foo
#         EXPORT aPerfCounterTargets
#         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )
#
# install(FILES perfCounters.h perfCounter.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# # install(FILES perfCounter.h perfCounters.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# # install(FILES perfCounter.h perfCounter.cpp perfCounters.h perfCounters.cpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#
# install(EXPORT aPerfCounterTargets
#         FILE aPerfCounterTargets.cmake
#         NAMESPACE Perf::
#         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/foo
# )
#
# configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
#                               "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfig.cmake"
#                               INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/foo
# )
#
# install(FILES
#         "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfig.cmake"
#         "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfigVersion.cmake"
#         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/foo
# )
#
# export(EXPORT aPerfCounterTargets
#        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/PerfCountersTargets.cmake"
#        NAMESPACE Perf::
# )
