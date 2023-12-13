cmake_minimum_required(VERSION 3.27)
project(PerfLib)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(libb SHARED
            perfCounters.cpp perfCounters.h
            # perfCounter.cpp perfCounter.h
)
target_include_directories(libb PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS libb
        EXPORT libbTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES perfCounters.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES perfCounter.h perfCounters.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES perfCounter.h perfCounter.cpp perfCounters.h perfCounters.cpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT libbTargets
        FILE libbTargets.cmake
        NAMESPACE Perf::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libb
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/libbConfig.cmake"
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libb
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/libbConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/libbConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libb
)

export(EXPORT libbTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/libbTargets.cmake"
       NAMESPACE Perf::
)
