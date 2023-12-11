add_library(perf 
            perf.cpp perf.h
)
target_include_directories(perf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
