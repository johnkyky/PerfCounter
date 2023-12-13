cmake_minimum_required(VERSION 3.27)
project(PerfLib)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(foo STATIC
            perfCounters.cpp perfCounters.h
            perfCounter.cpp perfCounter.h
)
include_directories($(FOO_SOURCE_DIR))
target_include_directories(foo PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS foo
        EXPORT aPerfCounterTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES perfCounters.h perfCounter.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES perfCounter.h perfCounters.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES perfCounter.h perfCounter.cpp perfCounters.h perfCounters.cpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT aPerfCounterTargets
        FILE aPerfCounterTargets.cmake
        NAMESPACE Perf::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/foo
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfig.cmake"
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/foo
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/PerfCountersConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/foo
)

export(EXPORT aPerfCounterTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/PerfCountersTargets.cmake"
       NAMESPACE Perf::
)
