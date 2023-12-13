cmake_minimum_required(VERSION 3.27)
project(PerfLib)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_FIND_DEBUG_MODE True)

add_library(PerfCounters SHARED
            perfCounters.cpp perfCounters.h
            # perfCounter.cpp perfCounter.h
)
target_include_directories(PerfCounters PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS PerfCounters
        EXPORT PerfCounterTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES perfCounters.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES perfCounter.h perfCounters.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES perfCounter.h perfCounter.cpp perfCounters.h perfCounters.cpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT PerfCounterTargets
        FILE PerfCounterTargets.cmake
        NAMESPACE Perf::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PerfCounters
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfig.cmake"
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PerfCounters
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PerfCounters
)

export(EXPORT PerfCounterTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/PerfCountersTargets.cmake"
       NAMESPACE Perf::
)
