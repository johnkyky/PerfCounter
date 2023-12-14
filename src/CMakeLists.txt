set(HEADERS
        "${INCLUDE_DIR}/perfCounter_LAPI.hpp"
        "${INCLUDE_DIR}/perfCounters_HAPI.hpp"
        )

add_library(perfcounters STATIC
        perfCounter_LAPI.cpp
        perfCounters_HAPI.cpp
        ${HEADERS}
)

add_library(PC::PerfCounters ALIAS perfcounters)

target_include_directories(perfcounters
        PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include/perfcounters>
)

# target_link_libraries(perfcounters
#         PUBLIC
#         Threads::Threads
#         )

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/perfcounters)
