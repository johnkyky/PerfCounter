cmake_minimum_required(VERSION 3.27)
project(PerfLib)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(PerfCounter STATIC
            perfCounter.cpp perfCounter.h
            perfCounters.cpp perfCounters.h
)
target_include_directories(PerfCounter PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS PerfCounter
        EXPORT PerfCounterTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES perf.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT PerfCounterTargets
        FILE PerfCounterTargets.cmake
        NAMESPACE Perf::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PerfCounter
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/PerfCounterConfig.cmake"
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PerfCounter
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/PerfCounterConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/PerfCounterConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PerfCounter
)

export(EXPORT PerfCounterTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/PerfCounterTargets.cmake"
       NAMESPACE Perf::
)
